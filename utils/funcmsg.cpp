#include "funcmsg.hpp"

using namespace std;

namespace dltrace {

    FuncMsgIndex::FuncMsgIndex(unsigned long addr, std::string str):m_addr(addr), m_funcName(str) {}
    
    bool FuncMsgIndex::operator==(const FuncMsgIndex &x) const{
        return (m_addr == x.m_addr && m_funcName == x.m_funcName);
    }

    void FuncMsgIndex::setAddr(unsigned long addr) {
        m_addr = addr;
    }

    unsigned long FuncMsgIndex::getAddr() const {
        return m_addr;
    }

    void FuncMsgIndex::setFuncName(std::string str) {
        m_funcName = str;
    }

    std::string FuncMsgIndex::getFuncName() const {
        return m_funcName;
    }

    /*------------------------------------------------------------*/

    size_t FuncMsgIndexHash::operator()(const FuncMsgIndex &x) const {
        return hash<unsigned long>()(x.m_addr) ^ hash<string>()(x.m_funcName);
    }

    /*------------------------------------------------------------*/

    FuncMsg::FuncMsg(const BreakPointSptr &pbp):m_pBreakPoint(pbp) {}

    FuncMsg::FuncMsg(const BreakPointSptr &pbp, const string& str) :m_pBreakPoint(pbp), m_funcName(str) {}
    
    FuncMsg::FuncMsg(const BreakPointSptr &pbp, const string& str, FuncMsg::FUNCMSGTYPE type) :m_pBreakPoint(pbp), m_funcName(str), m_type(type) {}

    FuncMsgIndex FuncMsg::makeIndex() const {
        return FuncMsgIndex(m_pBreakPoint->getAddr(), m_funcName);        
    }

    void FuncMsg::setBreakPoint(const BreakPointSptr &pbp) {
        m_pBreakPoint = pbp;
    }

    BreakPointSptr FuncMsg::getBreakPoint() const {
        return m_pBreakPoint;
    }

    void FuncMsg::setFuncName(const string& str) {
        m_funcName = str;
    }    

    string FuncMsg::getFuncName() const {
        return m_funcName;
    }

    void FuncMsg::setType(FuncMsg::FUNCMSGTYPE type) {
        m_type = type;
    }

    FuncMsg::FUNCMSGTYPE FuncMsg::getType() const {
        return m_type;
    }

}