#pragma once
#include "Eventing/Event.h"

#include <memory> // unique_ptr

namespace LibGL
{
    class EventManager
    {
    public:
        using EventPtr = std::unique_ptr<IEvent>;
        using EventMap = std::unordered_map<size_t, EventPtr>;

        template <typename EventType>
        IEvent::ListenerId subscribe(typename EventType::Action action);

        template <typename EventType>
        void unsubscribe(IEvent::ListenerId listener);

        template <typename EventType, typename... Args>
        void broadcast(Args... args);

        void clear();

    private:
        EventMap m_events;
    };
}

#include "Eventing/EventManager.inl"
