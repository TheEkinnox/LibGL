#include "Resources/MeshMulti.h"

#include "Arithmetic.h"
#include "Utility/ServiceLocator.h"
#include "Utility/ThreadPool.h"
#include "Utility/utility.h"

#include <cstring>
#include <ranges>

#define MIN_BATCH_SIZE static_cast<size_t>(0)
#define MAX_BATCH_SIZE ~static_cast<size_t>(0)

using namespace LibMath;
using namespace LibGL::Utility;

namespace LibGL::Rendering::Resources
{
    bool MeshMulti::load(const char* fileName)
    {
        const std::vector<std::string> lines = readFile(fileName);

        if (lines.empty())
            return false;

        m_vertices.clear();
        m_indices.clear();

        std::mutex posMutex, normalMutex, uvMutex;

        std::vector<Vector3> positions, normals;
        std::vector<Vector2> uvs;

        BatchData batchData{ &posMutex, &normalMutex, &uvMutex, &lines, {}, &positions, &normals, &uvs, 0, 0, 0 };

        std::vector<std::future<void>> tasks;
        std::vector<size_t>            faceLines;
        size_t                         posCount = 0, uvCount = 0, normalCount = 0, linesCount = 0;

        const double tmpBatchSize = ceil(static_cast<double>(lines.size()) / LGL_SERVICE(ThreadPool).getWorkersCount());
        const size_t batchSize    = clamp(static_cast<size_t>(tmpBatchSize), MIN_BATCH_SIZE, MAX_BATCH_SIZE);
        batchData.m_lines.reserve(batchSize);

        for (size_t i = 0; i < lines.size(); ++i)
        {
            const std::string& line = lines[i];

            if (!line.empty() && !line.starts_with('#'))
            {
                if (line.starts_with("f "))
                {
                    faceLines.emplace_back(i);
                }
                else
                {
                    if (line.starts_with("v "))
                        ++posCount;
                    else if (line.starts_with("vt "))
                        ++uvCount;
                    else if (line.starts_with("vn "))
                        ++normalCount;

                    batchData.m_lines.emplace_back(i);
                    ++linesCount;
                }
            }

            if (batchData.m_lines.size() < batchSize && i + 1 < lines.size())
                continue;

            {
                std::lock_guard lock(*batchData.m_posMutex);
                batchData.m_positions->resize(posCount);
            }

            {
                std::lock_guard lock(*batchData.m_uvMutex);
                batchData.m_uvs->resize(uvCount);
            }

            {
                std::lock_guard lock(*batchData.m_normalMutex);
                batchData.m_normals->resize(normalCount);
            }

            tasks.emplace_back(addBatch(batchData));

            batchData.m_lines.clear();
            batchData.m_posOffset    = posCount;
            batchData.m_uvOffset     = uvCount;
            batchData.m_normalOffset = normalCount;
        }

        for (auto& task : tasks)
            task.wait();

        for (const size_t faceLine : faceLines)
            parseFace(lines[faceLine].substr(2), positions, normals, uvs);

        return true;
    }

    std::future<void> MeshMulti::addBatch(const BatchData& batchData)
    {
        auto& threadPool = LGL_SERVICE(ThreadPool);

        return threadPool.enqueue([batchData]
        {
            parseLines(batchData);
        });
    }

    void MeshMulti::parseLines(const BatchData& batchData)
    {
        if (batchData.m_lines.empty())
            return;

        std::vector<Vector3> positions, normals;
        std::vector<Vector2> uvs;

        positions.reserve(batchData.m_lines.size());
        normals.reserve(batchData.m_lines.size());
        uvs.reserve(batchData.m_lines.size());

        for (const size_t lineIndex : batchData.m_lines)
        {
            const std::string& line = (*batchData.m_fileLines)[lineIndex];

            if (line.starts_with("v "))
                positions.emplace_back(parseVector3(line.substr(2)));
            else if (line.starts_with("vt"))
                uvs.emplace_back(parseVector2(line.substr(3)));
            else if (line.starts_with("vn"))
                normals.emplace_back(parseVector3(line.substr(3)));
        }

        {
            std::lock_guard lock(*batchData.m_posMutex);
            memcpy(batchData.m_positions->data() + batchData.m_posOffset,
                positions.data(), positions.size() * sizeof(Vector3));
        }

        {
            std::lock_guard lock(*batchData.m_normalMutex);
            memcpy(batchData.m_normals->data() + batchData.m_normalOffset,
                normals.data(), normals.size() * sizeof(Vector3));
        }

        {
            std::lock_guard lock(*batchData.m_uvMutex);
            memcpy(batchData.m_uvs->data() + batchData.m_uvOffset,
                uvs.data(), uvs.size() * sizeof(Vector2));
        }
    }
}
