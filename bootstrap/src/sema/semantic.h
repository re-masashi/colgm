#pragma once
#include "ast/ast.h"
#include "ast/decl.h"
#include "ast/expr.h"
#include "ast/stmt.h"
#include "sir/sir.h"
#include "ast/visitor.h"
#include "sema/symbol.h"
#include "sema/func.h"
#include "sema/struct.h"
#include "package/package.h"
#include "sema/context.h"

#include <unordered_map>
#include <cstring>
#include <sstream>
#include <vector>

namespace colgm {

using namespace ast;

class semantic {
private:
    error& err;
    semantic_context ctx;
    i64 in_loop_level = 0;
    std::string impl_struct_name;

private:
    void report(node* n, const std::string& info) {
        err.err("semantic", n->get_location(), info);
    }
    void warning(node* n, const std::string& info) {
        err.warn("semantic", n->get_location(), info);
    }
    void unimplemented(node* n) {
        err.err("semantic",
            n->get_location(),
            "unimplemented, please report a bug."
        );
    }
    void unreachable(node* n) {
        err.err("semantic",
            n->get_location(),
            "unreachable, please report a bug."
        );
    }
    void report_unreachable_statements(code_block*);
    void report_top_level_block_has_no_return(code_block*, const colgm_func&);

private:
    colgm_func builtin_struct_size(const span&);
    colgm_func builtin_struct_alloc(const span&, const type&);

private:
    void regist_struct(struct_decl*);
    void analyse_single_struct(struct_decl*);
    void analyse_structs(root*);
    void check_self_reference();
    void regist_enum(enum_decl*);
    void analyse_single_enum(enum_decl*);
    void analyse_enums(root*);
    void analyse_parameter(param*, colgm_func&);
    void analyse_method_parameter_list(param_list*,
                                       colgm_func&,
                                       const colgm_struct&);
    void analyse_func_parameter_list(param_list*, colgm_func&);
    void analyse_return_type(type_def*, colgm_func&);
    colgm_func analyse_single_method(func_decl*, const colgm_struct&);
    colgm_func analyse_single_func(func_decl*);
    void regist_function(func_decl*);
    void analyse_functions(root*);
    void analyse_single_impl(impl_struct*);
    void analyse_impls(root*);

private:
    type struct_static_method_infer(const std::string&,
                                    const std::string&,
                                    const std::string&);
    type struct_method_infer(const std::string&,
                             const std::string&,
                             const std::string&);

private:
    type resolve_logical_operator(binary_operator*);
    type resolve_comparison_operator(binary_operator*);
    type resolve_arithmetic_operator(binary_operator*);
    type resolve_bitwise_operator(binary_operator*);
    type resolve_binary_operator(binary_operator*);
    type resolve_unary_neg(unary_operator*);
    type resolve_unary_bnot(unary_operator*);
    type resolve_unary_lnot(unary_operator*);
    type resolve_unary_operator(unary_operator*);
    type resolve_type_convert(type_convert*);
    type resolve_nil_literal(nil_literal*);
    type resolve_number_literal(number_literal*);
    type resolve_string_literal(string_literal*);
    type resolve_char_literal(char_literal*);
    type resolve_bool_literal(bool_literal*);
    type resolve_array_literal(array_literal*);
    type resolve_identifier(identifier*);
    void check_pub_method(node*, const std::string&, const colgm_struct&);
    void check_pub_static_method(node*, const std::string&, const colgm_struct&);
    type resolve_get_field(const type&, get_field*);
    void check_static_call_args(const colgm_func&, call_func_args*);
    void check_method_call_args(const colgm_func&, const type&, call_func_args*);
    type resolve_call_id(call_id*);
    type resolve_call_func_args(const type&, call_func_args*);
    type resolve_call_index(const type&, call_index*);
    type resolve_initializer(const type&, initializer*);
    type resolve_call_path(const type&, call_path*);
    type resolve_ptr_get_field(const type&, ptr_get_field*);
    type resolve_call(call*);
    bool check_valid_left_value(expr*);
    void check_mutable_left_value(expr*, const type&);
    type resolve_assignment(assignment*);
    type resolve_expression(expr*);
    type resolve_type_def(type_def*);
    void resolve_definition(definition*, const colgm_func&);
    void check_defined_variable_is_void(definition*, const type&);
    void resolve_if_stmt(if_stmt*, const colgm_func&);
    bool check_is_enum_literal(expr*);
    void resolve_cond_stmt(cond_stmt*, const colgm_func&);
    void resolve_match_stmt(match_stmt*, const colgm_func&);
    void resolve_while_stmt(while_stmt*, const colgm_func&);
    void resolve_for_stmt(for_stmt*, const colgm_func&);
    void resolve_in_stmt_expr(in_stmt_expr*, const colgm_func&);
    void resolve_ret_stmt(ret_stmt*, const colgm_func&);
    void resolve_statement(stmt*, const colgm_func&);
    void resolve_code_block(code_block*, const colgm_func&);
    void resolve_global_func(func_decl*);
    void resolve_method(func_decl*, const colgm_struct&);
    void resolve_impl(impl_struct*);
    void resolve_function_block(root*);

public:
    semantic(error& e): err(e), impl_struct_name("") {}
    const error& analyse(root*);
    void dump();
    const auto& get_context() const { return ctx; }
};

}