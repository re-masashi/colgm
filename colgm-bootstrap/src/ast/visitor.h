#pragma once

#include "ast/ast.h"
#include "ast/decl.h"
#include "ast/expr.h"
#include "ast/stmt.h"

namespace colgm {

class visitor {
public:
    virtual bool visit_node(node*);
    virtual bool visit_root(root*);
    virtual bool visit_decl(decl*);
    virtual bool visit_type_def(type_def*);
    virtual bool visit_struct_field(struct_field*);
    virtual bool visit_struct_decl(struct_decl*);
    virtual bool visit_param(param*);
    virtual bool visit_param_list(param_list*);
    virtual bool visit_func_decl(func_decl*);
    virtual bool visit_impl_struct(impl_struct*);
    virtual bool visit_expr(expr*);
    virtual bool visit_binary_operator(binary_operator*);
    virtual bool visit_identifier(identifier*);
    virtual bool visit_number_literal(number_literal*);
    virtual bool visit_string_literal(string_literal*);
    virtual bool visit_call_index(call_index*);
    virtual bool visit_call_func_args(call_func_args*);
    virtual bool visit_call_field(call_field*);
    virtual bool visit_ptr_call_field(ptr_call_field*);
    virtual bool visit_call(call*);
    virtual bool visit_assignment(assignment*);
    virtual bool visit_stmt(stmt*);
    virtual bool visit_definition(definition*);
    virtual bool visit_cond_stmt(cond_stmt*);
    virtual bool visit_if_stmt(if_stmt*);
    virtual bool visit_while_stmt(while_stmt*);
    virtual bool visit_in_stmt_expr(in_stmt_expr*);
    virtual bool visit_ret_stmt(ret_stmt*);
    virtual bool visit_code_block(code_block*);
};

}