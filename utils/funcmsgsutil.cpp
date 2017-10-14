#include "funcmsgsutil.hpp"
#include <gelf.h>
#include <iostream>
#include <unistd.h>

using namespace std;

namespace dltrace {

    bool FuncMsgsUtil::loadEntryFuncMsgs(const std::string &fileName, FuncMsgMap &maps,std::unordered_map<unsigned long, BreakPointSptr>& bps, size_t offset) {
        cout << "load entry breakpoints." << endl;
        int fd;
        Elf *elf;
        bool ret = false;
        size_t i, nr_sym = 0;
        Elf_Scn *sym_sec, *sec;
        Elf_Data *sym_data;
        size_t shstr_idx, symstr_idx = 0;
        unsigned long prev_sym_value = -1;
    
        fd = open(fileName.c_str(), O_RDONLY);
        if (fd < 0) {
            cout <<ERROR_START  << "error during open symbol file: " << fileName.c_str()<<ERROR_END  << endl;
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
    
        elf_version(EV_CURRENT);
    
        elf = elf_begin(fd, ELF_C_READ_MMAP, NULL);
        if (elf == NULL)
            goto elf_error;
    
        if (elf_getshdrstrndx(elf, &shstr_idx) < 0)
            goto elf_error;
    
        sec = sym_sec = NULL;
        while ((sec = elf_nextscn(elf, sec)) != NULL) {
            string shstr;
            GElf_Shdr shdr;
    
            if (gelf_getshdr(sec, &shdr) == NULL)
                goto elf_error;
    
            shstr = elf_strptr(elf, shstr_idx, shdr.sh_name);

            if (shstr == ".symtab") {
                sym_sec = sec;
                nr_sym = shdr.sh_size / shdr.sh_entsize;
                symstr_idx = shdr.sh_link;
                break;
            } else if (shstr == ".dynsym") {
                sym_sec = sec;
                nr_sym = shdr.sh_size / shdr.sh_entsize;
                symstr_idx = shdr.sh_link;
            }

        }
    
        if (sym_sec == NULL) {
            cout <<ERROR_START <<"no symbol table is found" <<ERROR_END<< endl;
            goto out;
        }
    
        sym_data = elf_getdata(sym_sec, NULL);
        if (sym_data == NULL)
            goto elf_error;

        for (i = 0; i < nr_sym; i++) {
            GElf_Sym elf_sym;
    
            if (gelf_getsym(sym_data, i, &elf_sym) == NULL)
                goto elf_error;
    
            if (elf_sym.st_shndx == STN_UNDEF)
                continue;
    
            if (elf_sym.st_size == 0)
                continue;
    
            if (GELF_ST_TYPE(elf_sym.st_info) != STT_FUNC)
                continue;
    
            /* skip aliases */
            if (prev_sym_value == elf_sym.st_value)
                continue;
            prev_sym_value = elf_sym.st_value;


            unsigned long addr = elf_sym.st_value + offset;
            string name = elf_strptr(elf, symstr_idx, elf_sym.st_name);
            FuncMsg funcMsg;
            auto bp=make_shared<BreakPoint>(addr);

            funcMsg.setBreakPoint(bp);
            funcMsg.setFuncName(name);
            cout<<name<<":"<<hex<<addr<<dec<<endl;
            funcMsg.setType(FuncMsg::FUNCMSGTYPE::FUNCIN);

            auto bpOfHashMap=bps.find(addr);
            if(bpOfHashMap==bps.end())
            {
                //ensure there are no duplicated breakpoints
                bps[addr]=bp;
                maps.insert(FuncMsgMapPair(funcMsg.makeIndex(), funcMsg));
            }else
            {

            }

        }

    out:
        ret = 1;
        elf_end(elf);
        close(fd);
        return ret;
    
    elf_error:
        cout <<ERROR_START<< "ELF error during symbol loading: " <<
               elf_errmsg(elf_errno())<<ERROR_END    << endl;
        goto out;
    }

    bool FuncMsgsUtil::loadLibEntryFuncMsgs(const std::string &fileName, FuncMsgMap &maps,std::unordered_map<unsigned long, BreakPointSptr>& bps, size_t offset) {
        cout << "load entry breakpoints." << endl;
        int fd;
        Elf *elf;
        bool ret = false;
        size_t i, nr_sym = 0;
        Elf_Scn *sym_sec, *sec;
        Elf_Data *sym_data;
        size_t shstr_idx, symstr_idx = 0;
        unsigned long prev_sym_value = -1;

        fd = open(fileName.c_str(), O_RDONLY);
        if (fd < 0) {
            cout <<ERROR_START  << "error during open symbol file: " << fileName.c_str()<<ERROR_END  << endl;
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

        elf_version(EV_CURRENT);

        elf = elf_begin(fd, ELF_C_READ_MMAP, NULL);
        if (elf == NULL)
            goto elf_error;

        if (elf_getshdrstrndx(elf, &shstr_idx) < 0)
            goto elf_error;

        sec = sym_sec = NULL;
        while ((sec = elf_nextscn(elf, sec)) != NULL) {
            string shstr;
            GElf_Shdr shdr;

            if (gelf_getshdr(sec, &shdr) == NULL)
                goto elf_error;

            shstr = elf_strptr(elf, shstr_idx, shdr.sh_name);


            if(shstr == ".dynsym") {
                sym_sec = sec;
                nr_sym = shdr.sh_size / shdr.sh_entsize;
                symstr_idx = shdr.sh_link;
                break;
            }

        }

        if (sym_sec == NULL) {
            cout <<ERROR_START <<"no symbol table is found" <<ERROR_END<< endl;
            goto out;
        }

        sym_data = elf_getdata(sym_sec, NULL);
        if (sym_data == NULL)
            goto elf_error;

        for (i = 0; i < nr_sym; i++) {
            GElf_Sym elf_sym;

            if (gelf_getsym(sym_data, i, &elf_sym) == NULL)
                goto elf_error;

            if (elf_sym.st_shndx == STN_UNDEF)
                continue;

            if (elf_sym.st_size == 0)
                continue;

            if (GELF_ST_TYPE(elf_sym.st_info) != STT_FUNC)
                continue;

            /* skip aliases */
            if (prev_sym_value == elf_sym.st_value)
                continue;
            prev_sym_value = elf_sym.st_value;


            unsigned long addr = elf_sym.st_value + offset;
            string name = elf_strptr(elf, symstr_idx, elf_sym.st_name);
            FuncMsg funcMsg;
            auto bp=make_shared<BreakPoint>(addr);

            funcMsg.setBreakPoint(bp);
            funcMsg.setFuncName(name);
            cout<<name<<":"<<hex<<addr<<dec<<endl;
            funcMsg.setType(FuncMsg::FUNCMSGTYPE::FUNCIN);

            auto bpOfHashMap=bps.find(addr);
            if(bpOfHashMap==bps.end())
            {
                //ensure there are no duplicated breakpoints
                bps[addr]=bp;
                maps.insert(FuncMsgMapPair(funcMsg.makeIndex(), funcMsg));
            }else
            {

            }

        }

        out:
        ret = 1;
        elf_end(elf);
        close(fd);
        return ret;

        elf_error:
        cout <<ERROR_START<< "ELF error during symbol loading: " <<
             elf_errmsg(elf_errno())<<ERROR_END    << endl;
        goto out;
    }
}   