#ifndef _DLTRACE_FUNCMSG_H_
#define _DLTRACE_FUNCMSG_H_

#include <string>
#include <unordered_map>
#include "breakpoint.hpp"

namespace dltrace {

    class FuncMsgIndexHash;
    class FuncMsgIndex {
        friend class FuncMsgIndexHash;
    private:    
        unsigned long m_addr;
        std::string m_funcName;
    CONSTRUCTOR:
        FuncMsgIndex() = default;
        explicit FuncMsgIndex(unsigned long, std::string);

    public:
        bool operator==(const FuncMsgIndex&) const;
        void setAddr(unsigned long);
        unsigned long getAddr() const;
        void setFuncName(std::string);
        std::string getFuncName() const;
    };

    class FuncMsgIndexHash {
    public:
        size_t operator()(const FuncMsgIndex&) const;
    };
  
    class FuncMsg {
    public:
        enum class FUNCMSGTYPE {
            FUNCIN,
            FUNCOUT,
        };
    private:
        BreakPointSptr m_pBreakPoint;
        std::string m_funcName;
        FUNCMSGTYPE m_type;

    CONSTRUCTOR:
        FuncMsg() = default;
        explicit FuncMsg(const BreakPointSptr&);
        explicit FuncMsg(const BreakPointSptr&, const std::string&);
        explicit FuncMsg(const BreakPointSptr&, const std::string&, FUNCMSGTYPE);
    DESTRUCTOR:

    public:
        FuncMsgIndex makeIndex() const;
        void setBreakPoint(const BreakPointSptr&);
        BreakPointSptr getBreakPoint() const;
        void setFuncName(const std::string&);
        std::string getFuncName() const;
        void setType(FUNCMSGTYPE);
        FUNCMSGTYPE getType() const;
        std::string demangledFunc();
    };

    typedef std::unordered_map<FuncMsgIndex ,FuncMsg, FuncMsgIndexHash> FuncMsgMap;
    typedef std::pair<FuncMsgIndex, FuncMsg> FuncMsgMapPair;

}

#endif