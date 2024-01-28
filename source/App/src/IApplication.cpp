#include "IApplication.h"

namespace LibGL::Application
{
    using namespace Exceptions;

    void IApplication::run()
    {
        onStart();

        // Run main loop
        while (isRunning())
        {
            m_context->update();
            onUpdate();
        }

        onStop();
    }

    bool IApplication::isRunning() const
    {
        return !m_context->m_window->shouldClose();
    }

    IApplication::IApplication(std::unique_ptr<IContext> context)
        : m_context(std::move(context))
    {
    }
}
