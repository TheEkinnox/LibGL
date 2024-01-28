#pragma once
#include "IContext.h"

namespace LibGL::Application
{
    class IApplication
    {
    public:
        /**
         * \brief Disable the Application's copy constructor
         */
        IApplication(const IApplication&) = delete;

        /**
         * \brief Disable the Application's move constructor
         */
        IApplication(IApplication&&) = delete;

        /**
         * \brief Virtual destructor
         */
        virtual ~IApplication() = default;

        /**
         * \brief Disable the Application's copy assignment operator
         */
        IApplication& operator=(const IApplication&) = delete;

        /**
         * \brief Disable the Application's move assignment operator
         */
        IApplication& operator=(IApplication&&) = delete;

        /**
         * \brief Starts the application's main loop
         */
        void run();

        /**
         * \brief Whether the application is currently running or not.
         * \return True if the application is running. False otherwise.
         */
        bool isRunning() const;

    protected:
        std::unique_ptr<IContext> m_context = nullptr;

        /**
         * \brief Creates an application with the given context
         * \param context A pointer to the application's context
         */
        explicit IApplication(std::unique_ptr<IContext> context);

        /**
         * \brief The function to call when the app starts
         */
        virtual void onStart() = 0;

        /**
         * \brief The function to call on each main loop cycle
         */
        virtual void onUpdate() = 0;

        /**
         * \brief The function to call when the app stops
         */
        virtual void onStop() = 0;
    };
}
