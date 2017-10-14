#ifndef _DLTRACE_EVENTHANDLER_H_
#define _DLTRACE_EVENTHANDLER_H_

#include "event.hpp"
#include <queue>

namespace dltrace {

    _interface EventHandler {
    protected:
        std::queue<Event*> m_eventQueue;

    CONSTRUCTOR:
    DESTRUCTOR:
        virtual ~EventHandler() {}

    protected:
        virtual void submitEvent(Event*) = 0;
        virtual void handleEvent(Event*) = 0;
        virtual void handleEventInQueue() = 0;
    };

}

#endif /* _DLTRACE_EVENT_H_ */