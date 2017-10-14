#include "breakpoint.hpp"
#include <sys/ptrace.h>
#include <iostream>
using namespace std;

namespace dltrace {

    #define CLEAR_LAST_BYTE(DATA) ((DATA) & ~0xFF)
    #define DATA_TO_TRAP(DATA)	(CLEAR_LAST_BYTE(DATA) | 0xCC)
    #define CHANGE_LAST_BYTE(DATA, BYTE) (CLEAR_LAST_BYTE(DATA) | BYTE) 
    #define GET_LAST_BYTE(DATA) ((DATA) & 0xFF)

    BreakPoint::BreakPoint():m_addr(-1), m_data(-1) {}    

    BreakPoint::BreakPoint(unsigned long addr):m_addr(addr), m_data(0){}
        
    unsigned long BreakPoint::getAddr() const {
        return m_addr;
    }

    void BreakPoint::enable(pid_t pid)
    {
        if(m_data) return;
        long data;
        data = ptrace(PTRACE_PEEKDATA, pid, m_addr, 0);
        m_data = GET_LAST_BYTE(data);
        cout << "enable: addr:" << hex << m_addr << "\tdata:" << hex << data << endl;
        ptrace(PTRACE_POKEDATA, pid, m_addr, DATA_TO_TRAP(data));
    }

    void BreakPoint::disable(pid_t pid)
    {
        if(!m_data) return;
        long data;
        data = ptrace(PTRACE_PEEKDATA, pid, m_addr, 0);
        //cout << "disable: addr:" << hex << m_addr << "\tdata:" << hex << data << endl;        
        ptrace(PTRACE_POKEDATA, pid, m_addr, CHANGE_LAST_BYTE(data,m_data));
        m_data = 0;
    }

}