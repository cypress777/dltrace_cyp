#ifndef _DLTRACE_EVENT_H_
#define _DLTRACE_EVENT_H_

#include "../utils/trace.hpp"

namespace dltrace {

    class Trace;

    class Event {
    public:
        enum class TYPE {
            EV_BREAKPOINT,
            EV_EXITED,
            EV_SIGNAL_EXITED,
            EV_SIGNALED,
            EV_CLONE,
            EV_NEWPROCESS,
            EV_SINGLESTEPPED,
            EV_IGNORE,
        };

    protected:
        TimeEx m_createTime;
        Event::TYPE m_type;
        Process *m_process;
        Trace *m_trace;

    CONSTRUCTOR:
        Event(TimeEx, Process*, Trace*, TYPE);
    DESTRUCTOR:
        virtual ~Event();

    public:
        virtual Event* doEvent() = 0;
    public:
        TimeEx getCreateTime() const;
        Event::TYPE getType() const;
        Process* getProcess() const;
        Trace* getTrace() const;

    };

}


#endif /* _DLTRACE_EVENT_H_ */