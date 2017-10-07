#ifndef _DLTRACE_PROCESS_H_
#define _DLTRACE_PROCESS_H_


#include <fcntl.h>
#include <string>
#include <stack>
#include "util.hpp"
#include "funcmsg.hpp"

namespace dltrace {

    class Process {
    private:
        pid_t m_pid;
        Process& m_leader;
        std::stack<FuncMsgIndex> m_callStack;
        bool m_isSingleStep;
        bool m_isSigStopped;
        bool m_isDelivered;
        BreakPointSptr m_singleStepbreakPoint;

    CONSTRUCTOR:
        Process(pid_t);
        explicit Process(pid_t, Process&);
    DESTRUCTOR:

    private:

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
        bool isSigStopped();
        bool isDelivered();
        bool isSingleStep();
        void stopProcess();
        void setRegister(int, long);
        long getRegister(int);
        long getData(unsigned long);
        void continueToRun();
        void singleStep(BreakPointSptr&);
        void afterSingleStep();
    };
}

#endif /* _DLTRACE_PROCESS_H_ */