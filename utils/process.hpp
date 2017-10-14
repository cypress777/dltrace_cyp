#ifndef _DLTRACE_PROCESS_H_
#define _DLTRACE_PROCESS_H_


#include <fcntl.h>
#include <string>
#include <stack>
#include "util.hpp"
#include "funcmsg.hpp"

namespace dltrace {

    class Process {
    public:
        enum class MEMOPSTATE {
            NONE,
            DISABLING_BREAKPOINT,
            DISABLED_BREAKPOINT,
            SINGLESTEPPING,
            SINGLESTEPPED,
        };
    public:
        bool m_isSigStopped = false;
        bool m_waitingForSigStop = false;
        bool m_isNew = false;
        MEMOPSTATE m_memOpState = MEMOPSTATE::NONE;
    private:
        pid_t m_pid;
        Process* m_leader;
        Process *m_memOperatingProcess;
        BreakPointSptr m_singleStepBreakPoint;
        std::stack<FuncMsgIndex> m_callStack;        

    CONSTRUCTOR:
        Process(pid_t);
        explicit Process(pid_t, Process*);
    DESTRUCTOR:

    public:
        bool operator<(const Process&) const;
        bool operator==(const Process&) const;
        pid_t getPid() const;
        std::stack<FuncMsgIndex>& getCallStack();
        void traceMe();
        FILE* getProcessFile(std::string);
        size_t getLibOffset(std::string);
        std::string getState();
        bool isStopped();
        void stopProcess();
        void setRegister(int, long);
        long getRegister(int);
        long getData(unsigned long);
        void getPtraceEventMessage(long*);
        size_t getExecOffset(std::string execName);
        void setMemOperatingProcess(Process*);
        Process* getMemOperatingProcess() const;
        void setLeader(Process*);
        Process* getLeader() const;
        void setSingleStepBreakPoint(BreakPointSptr&);
        BreakPointSptr getSingleStepBreakPoint() const;     
        void continueToRun(int signum = 0);
        bool singleStep();
        void afterSingleStep();
    };
}

#endif /* _DLTRACE_PROCESS_H_ */