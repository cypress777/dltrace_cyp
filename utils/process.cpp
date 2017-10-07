#include "process.hpp"
#include <cstring>
#include <string>
#include <asm-generic/unistd.h>
#include <cassert>
#include <climits>
#include <stdio.h>
#include <sys/ptrace.h>
#include <signal.h>
#include <iostream>
#include <sstream>
#include <unistd.h>

using namespace std;

namespace dltrace {

    const int MAXBUF = 100;

    Process::Process(pid_t pid)
        :m_pid(pid), m_leader(*this), m_isSingleStep(false), m_singleStepbreakPoint(nullptr) {}

    Process::Process(pid_t pid, Process& leader)
        :m_pid(pid), m_leader(leader), m_isSingleStep(false), m_singleStepbreakPoint(nullptr) {}

    bool Process::operator<(const Process &process) const {
        return m_pid < process.m_pid;
    }
    
    bool Process::operator==(const Process &process) const {
        return m_pid == process.m_pid;
    }

    pid_t Process::getPid() const {
        return m_pid;
    }

    stack<FuncMsgIndex>& Process::getCallStack() {
        return m_callStack;
    }

    void Process::traceMe() {
        ptrace(PTRACE_TRACEME, 0, 0, 0);
    }    

    FILE* Process::getProcessFile(string fileName) {
        stringstream ss;
        ss << "/proc/" << m_pid << "/" << fileName;
        ss >> fileName;
        return fopen(fileName.c_str(), "r");
    }

    string Process::getState() {
        FILE* fp = getProcessFile("status");
        if(fp == nullptr) 
            return "";
        char *line = nullptr;
        size_t length = 0;
        const char *prefix = "State:\t";
        while(!feof(fp)) {
            getline(&line, &length, fp);
            if(strncmp(prefix, line, strlen(prefix)) == 0)
                break;
            free(line);
            line = nullptr;
        }
        fclose(fp);        
        assert(line != nullptr);
        string ret = line+strlen(prefix);
        free(line);
        return ret;
    }   
    
    size_t Process::getLibOffset(string libName) {
        string fileName;
        char buf[PATH_MAX];
        size_t ret = -1;

        auto libname = libName.c_str();

        FILE * fp = getProcessFile("maps");
        if (fp == nullptr) return ret;

        while (fgets(buf, sizeof(buf), fp) != nullptr) {
          unsigned long start, end;
          unsigned dev, sdev;
          unsigned long inode;
          unsigned long long offset;
          char prot[5];
          char path[PATH_MAX];
          int len;
      
//       format defined in /proc/pid/maps is constructed as below in fs/proc/task_mmu.c
            if (sscanf(buf, "%lx-%lx %s %llx %x:%x %lu %s",
                     &start, &end, 
                     prot, 
                     &offset, 
                     &dev, &sdev, 
                     &inode, 
                     path) != 8)
            continue;

            if (!inode)
                continue;

            if (!strstr(path, libname))
                continue;
      
            if (prot[2] != 'x')
                continue;
      
            ret = start;

            break;
        }
        
        fclose(fp);        
        return ret;
      }

    bool Process::isStopped() {
        string stateStr = getState();
        if(stateStr == "")
            return false;
        if(stateStr[0] == 't' || stateStr[0] == 'T')
            return true;
        return false; 
    }

    bool Process::isSigStopped() {
        return m_isSigStopped;
    }

    bool Process::isDelivered() {
        return m_isDelivered;
    }

    bool Process::isSingleStep() {
        return m_isSingleStep;
    }

    void Process::stopProcess() {
        string stateStr = getState();
        if(stateStr == "")
            return;
        if(stateStr[0] == 'Z')
            return;
        if(stateStr[0] == 't')
            return;
        if(stateStr == "T (tracing stop)")
            return;
        if(syscall(__NR_tgkill, m_leader.m_pid, m_pid, SIGSTOP) > 0)
            m_isSigStopped = true;
    }

    void Process::setRegister(int id, long data) {
        ptrace(PTRACE_POKEUSER, m_pid, sizeof(long) * id, (void*)data);
    }    

    long Process::getRegister(int id) {
        return ptrace(PTRACE_PEEKUSER, m_pid, sizeof(long) * id, NULL);
    }

    long Process::getData(unsigned long addr) {
        return ptrace(PTRACE_PEEKDATA, m_pid, (void*)addr, 0);
    }    
    
    void Process::continueToRun() {
        ptrace(PTRACE_CONT, m_pid, 0, 0);        
    }

    void Process::singleStep(BreakPointSptr &ptr) {
        cout << dec << m_pid << " singlestep" << endl;
        if(ptrace(PTRACE_SINGLESTEP, m_pid, 0, 0) != -1) {
            m_singleStepbreakPoint = ptr;
            m_isSingleStep = true;
        }
        else
            cout << ERROR_START << "single step failed :" << dec << m_pid << ERROR_END << endl;
    }

    void Process::afterSingleStep() {
        assert(m_isSingleStep == true);
        m_isSingleStep = false;
        m_singleStepbreakPoint->enable(m_pid);
        m_singleStepbreakPoint.reset();
    }    

}