#include "process.hpp"
#include <cstring>
#include <string>
#include <asm-generic/unistd.h>
#include <cassert>
#include <climits>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/syscall.h>
#include <signal.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/reg.h>
#include "gelf.h"

using namespace std;

namespace dltrace {

    const int MAXBUF = 100;

    Process::Process(pid_t pid)
        :m_pid(pid), m_leader(nullptr), m_memOperatingProcess(nullptr), m_singleStepBreakPoint(nullptr) {}

    Process::Process(pid_t pid, Process *leader)
        :m_pid(pid), m_leader(leader), m_memOperatingProcess(nullptr), m_singleStepBreakPoint(nullptr) {}

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
        ss << "/proc/" << dec << m_pid << "/" << fileName;
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

    size_t Process::getExecOffset(string execName) {
        int fd;
        Elf *elf;
        size_t ret = 0;
        fd = open(execName.c_str(), O_RDONLY);
        if (fd < 0) {
            cout <<ERROR_START  << "error during open symbol file: " << execName.c_str()<<ERROR_END  << endl;
            return ret;
        }

        unsigned char elf_ident[EI_NIDENT];
        uint16_t e_type;

        if(read(fd, elf_ident, sizeof(elf_ident)) < 0)
            goto elf_error;

        if(read(fd, &e_type, sizeof(e_type)) < 0)
            goto elf_error;

        if(e_type != ET_EXEC && e_type != ET_DYN)
            goto elf_error;

        if(e_type == ET_EXEC)
            goto out;

        ret = getLibOffset(execName);

        if (ret < 0) goto elf_error;

        out:
        close(fd);
        return ret;

        elf_error:
        cout <<ERROR_START<< "ELF error during symbol loading: " <<
             elf_errmsg(elf_errno())<<ERROR_END    << endl;
        goto out;
    }

    size_t Process::getLibOffset(string libName) {
        char buf[PATH_MAX];
        size_t ret = -1;

        size_t pos = libName.rfind('/');
        auto libname = libName.substr(pos + 1);
        //auto libname = libName.c_str();

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

            std::string pathName{path};
            pos = pathName.rfind('/');
            auto pathname = pathName.substr(pos + 1);

            //if (!strstr(path, libname))
            if (pathname != libname)
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

    void Process::stopProcess() {
        if(m_isSigStopped || m_isNew) //if process is a new process create by leader, it should be stopped by kernel.
            return;
        string stateStr = getState();
        if(stateStr == "")
            return;
        if(stateStr[0] == 'Z')
            return;
        if(stateStr[0] == 't')
            return;
        if(stateStr == "T (tracing stop)\n")
            return;
        int last = errno;
        errno = 0;
        int ret = syscall(__NR_tgkill, m_leader->m_pid, m_pid, SIGSTOP);
        if(ret >= 0) {
            cout << ERROR_START << "leader:" << dec << m_leader->m_pid << " " << m_pid << "tgkill sucess." << ERROR_END << endl;
            m_isSigStopped = true;
            m_waitingForSigStop = false;
        }
        else {
            stateStr = getState();
            cout << ERROR_START << dec << SIGSTOP << " leader:" << dec << m_leader->m_pid << " " << m_pid << " tgkill failed:"
                << "stateStr:" << stateStr << " errorMSG:"
                << strerror(errno) << " " << dec << errno << ret << ERROR_END << endl;
        }
        errno = last; 
    }

    void Process::setRegister(int id, long data) {
        if(id == RIP) {
            cout << ERROR_START << "pid: " << dec << m_pid << "set rip:" << hex << data << ERROR_END << endl; 
        }
        ptrace(PTRACE_POKEUSER, m_pid, sizeof(long) * id, (void*)data);
    }    

    long Process::getRegister(int id) {
        return ptrace(PTRACE_PEEKUSER, m_pid, sizeof(long) * id, 0);
    }

    long Process::getData(unsigned long addr) {
        return ptrace(PTRACE_PEEKDATA, m_pid, (void*)addr, 0);
    }    

    void Process::getPtraceEventMessage(long* pdata) {
        ptrace(PTRACE_GETEVENTMSG, m_pid, NULL, pdata);
    }    

    void Process::setMemOperatingProcess(Process *process) {
        m_memOperatingProcess = process;
    }

    Process* Process::getMemOperatingProcess() const {
        return m_memOperatingProcess;
    }

    void Process::setLeader(Process *process) {
        m_leader = process;
    }    

    Process* Process::getLeader() const {
        return m_leader;
    }

    void Process::setSingleStepBreakPoint(BreakPointSptr &bp) {
        m_singleStepBreakPoint = bp;
    }
    
    BreakPointSptr Process::getSingleStepBreakPoint() const { 
        return m_singleStepBreakPoint;
    }    
    
    void Process::continueToRun(int signum) {
        cout << ERROR_START << "pid :" << dec << m_pid << " before cont." << hex << getRegister(RIP) << " data:" << getData(getRegister(RIP)) << " sig:" << dec << signum << ERROR_END << endl;
        ptrace(PTRACE_CONT, m_pid, 0, (void *)(uintptr_t)signum);    
    }

    bool Process::singleStep() {
        if(ptrace(PTRACE_SINGLESTEP, m_pid, 0, (void *)(uintptr_t)0) == 0) {
            return true;
        }
        else {
            cout << ERROR_START << dec << m_pid << " single step failed :" 
                << strerror(errno) << ERROR_END << endl;
        }
        return false;
    }   

}