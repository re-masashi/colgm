#pragma once

#include "report.h"
#include "ast/ast.h"
#include "ast/decl.h"
#include "sema/context.h"
#include "sema/symbol.h"
#include "sema/reporter.h"

namespace colgm {

class type_resolver {
private:
    error& err;
    const sema_context& ctx;
    reporter rp;

private:
    bool is_generic(const type&);
    u64 get_array_length(ast::number_literal*);

public:
    type_resolver(error& e, const sema_context& c):
        err(e), ctx(c), rp(err) {}
    type resolve(ast::type_def*);
};

}