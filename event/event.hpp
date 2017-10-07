#ifndef _DLTRACE_EVENT_H_
#define _DLTRACE_EVENT_H_


#include <sys/time.h>
#include <stack>
#include "../utils/process.hpp"
#include "../utils/timeex.hpp"

namespace dltrace {

    class Event {
    public:
        enum class EVENTTYPE {
            COMPLETE,
            BREAKPOINT,
            EXITED,
            EXITE_SIGNALED,
            SIGNALED,
            CLONE,
            PROCESSSTOP,
            SINGLESTEP,
            NONE,
            WTHP,
        };

    private:
        TimeEx m_createTime;
        EVENTTYPE m_type;
        Process &m_process;
        std::stack<FuncMsg> m_funcMsgs;
        pid_t m_newPid;

    CONSTRUCTOR:
        explicit Event(TimeEx&, Process&, EVENTTYPE);
    DESTRUCTOR:

    private:
        void doBreakPointEvent();
        void doExitedEvent();
        void doExitSignaledEvent();
        void doSignaledEvent();
        void doCloneEvent();
        void doProcessStopEvent();        
        void doSingleStepEvent();

    public:
        EVENTTYPE getType() const;
        void setCreateTime(struct timeval);
        TimeEx getCreateTime() const;
        void setFuncMsgs(const std::stack<FuncMsg>&);
        void setFuncMsgs(std::stack<FuncMsg>&&);
        const std::stack<FuncMsg>& getFuncMsgs() const;
        void setNewPid(pid_t);
        pid_t getNewPid() const;
        void doEvent();
        
    };

}


#endif /* _DLTRACE_EVENT_H_ */