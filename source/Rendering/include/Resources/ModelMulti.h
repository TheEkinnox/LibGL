#pragma once
#include "Resources/Model.h"

#include "Utility/ThreadPool.h"

namespace LibGL::Rendering::Resources
{
    class ModelMulti : public Model
    {
    public:
        struct BatchData
        {
            std::mutex* m_posMutex;
            std::mutex* m_normalMutex;
            std::mutex* m_uvMutex;

            const std::vector<std::string>* m_fileLines;
            std::vector<size_t>             m_lines;

            std::vector<LibMath::Vector3>* m_positions;
            std::vector<LibMath::Vector3>* m_normals;
            std::vector<LibMath::Vector2>* m_uvs;

            size_t m_posOffset;
            size_t m_uvOffset;
            size_t m_normalOffset;
        };

        using IResource::load;

        /**
         * \brief Loads the model from the given file using multi-threading
         * \param fileName The path of the model to load
         * \return True if the model was successfully loaded. False otherwise.
         */
        bool load(const char* fileName) override;

    private:
        std::mutex m_verticesMutex;
        std::mutex m_indicesMutex;

        /**
         * \brief Adds the action corresponding to the given batch data to the thread pool.
         * \param batchData The data for which a batch action should be created.
         * \return A future containing the action's result.
         */
        static std::future<void> addBatch(const BatchData& batchData);

        /**
         * \brief Parses the given batch's lines and adds the resulting data back in the batch data.
         * \param batchData The batch's information.
         * \return True if the batch's lines were successfully parsed. False otherwise.
         */
        static void parseLines(const BatchData& batchData);
    };
}
