#pragma once 

#include "colgm.h"
#include "code/hir.h"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstring>
#include <sstream>

namespace colgm {

struct ir_context {
    std::vector<hir_struct> struct_decls;
    std::vector<hir_func*> func_decls;
    std::vector<hir_func*> func_impls;
    std::unordered_map<std::string, u64> const_strings;

private:
    void dump_raw_string(std::ostream&, const std::string&) const;
    void dump_const_string(std::ostream&) const;
    void check_and_dump_default_main(std::ostream&) const;

public:
    ~ir_context() {
        for(auto i : func_decls) {
            delete i;
        }
        for(auto i : func_impls) {
            delete i;
        }
    }
    void dump_code(std::ostream&) const;
};

}