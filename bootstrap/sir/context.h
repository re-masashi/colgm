#pragma once 

#include "colgm.h"
#include "sema/symbol.h"
#include "sir/sir.h"
#include "sir/debug_info.h"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstring>
#include <sstream>

namespace colgm {

class sir_struct {
private:
    std::string name;
    span location;
    std::vector<std::string> field_type;

public:
    sir_struct(const std::string& n, const span& loc): name(n), location(loc) {}
    const auto get_mangled_name() const;
    void dump(std::ostream&) const;
    const auto& get_name() const { return name; }
    const auto& get_location() const { return location; }
    const auto& get_file() const { return location.file; }

    void add_field_type(const std::string& type) { field_type.push_back(type); }
    const auto& get_field_type() const { return field_type; }
};

class sir_func {
private:
    std::string name;
    std::vector<std::pair<std::string, std::string>> params;
    std::vector<std::string> attributes;
    u64 debug_info_index;
    std::string return_type;
    sir_block* block;

private:
    void dump_attributes(std::ostream&) const;

public:
    sir_func(const std::string& n):
        name(n), debug_info_index(DI_node::DI_ERROR_INDEX), block(nullptr) {}
    ~sir_func() { delete block; }
    void set_debug_info_index(u64 i) { debug_info_index = i; }
    const auto get_mangled_name() const { return quoted_name(name); }
    void dump(std::ostream&) const;
    const auto& get_name() const { return name; }

    void add_param(const std::string& pname, const std::string& ptype) {
        params.push_back({pname, ptype});
    }
    void set_code_block(sir_block* b) { block = b; }
    auto get_code_block() { return block; }
    void set_attributes(const std::vector<std::string>& a) { attributes = a; }
    void set_return_type(const std::string& rtype) { return_type = rtype; }
};

struct sir_context {
public:
    std::vector<sir_struct*> struct_decls;
    std::vector<sir_func*> func_decls;
    std::vector<sir_func*> func_impls;
    std::unordered_map<std::string, u64> const_strings;

public:
    std::vector<DI_named_metadata*> named_metadata;
    std::vector<DI_node*> debug_info;
    std::unordered_map<std::string, u64> DI_file_map;
    std::unordered_map<std::string, u64> DI_basic_type_map;

private:
    void dump_target_tripple(std::ostream&) const;
    void dump_const_string(std::ostream&) const;
    void dump_struct_size_method(std::ostream&) const;
    void dump_struct_alloc_method(std::ostream&) const;

public:
    ~sir_context() {
        for(auto i : struct_decls) {
            delete i;
        }
        for(auto i : func_decls) {
            delete i;
        }
        for(auto i : func_impls) {
            delete i;
        }
        for(auto i : named_metadata) {
            delete i;
        }
        for(auto i : debug_info) {
            delete i;
        }
    }
    void dump_code(std::ostream&);
};

}