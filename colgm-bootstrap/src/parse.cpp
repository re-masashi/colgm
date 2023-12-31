#include "parse.h"

namespace colgm {

void parse::match(tok type) {
    if (type==toks[ptr].type) {
        next();
        return;
    }
    switch(type) {
        case tok::id:
            err.err("parse",
                toks[ptr].loc,
                "expected identifier but get \"" + toks[ptr].str + "\"."
            );
            break;
        case tok::num:
            err.err("parse",
                toks[ptr].loc,
                "expected number but get \"" + toks[ptr].str + "\"."
            );
            break;
        case tok::str:
            err.err("parse",
                toks[ptr].loc,
                "expected string but get \"" + toks[ptr].str + "\"."
            );
            break;
        default:
            err.err("parse",
                toks[ptr].loc,
                "expected \"" + tokname.at(type) +
                "\" but get \"" + toks[ptr].str + "\"."
            );
            break;
    }
    return;
}

bool parse::look_ahead(tok type) {
    return type==toks[ptr].type;
}

identifier* parse::identifier_gen() {
    auto result = new identifier(toks[ptr].loc, toks[ptr].str);
    match(tok::id);
    return result;
}

call* parse::call_gen() {
    auto result = new call(toks[ptr].loc);
    result->set_head(identifier_gen());
    while(look_ahead(tok::lcurve) || look_ahead(tok::lbracket) || look_ahead(tok::dot)) {
        if (look_ahead(tok::lcurve)) {
            // TODO
        }
        if (look_ahead(tok::lbracket)) {
            match(tok::lbracket);
            auto new_call_index = new call_index(toks[ptr].loc);
            new_call_index->set_index(calculation_gen());
            match(tok::rbracket);
            result->add_chain(new_call_index);
        }
        if (look_ahead(tok::dot)) {
            match(tok::dot);
            auto new_call_field = new call_field(toks[ptr].loc, toks[ptr].str);
            match(tok::id);
            result->add_chain(new_call_field);
        }
    }
    return result;
}

number_literal* parse::number_gen() {
    auto result = new number_literal(toks[ptr].loc, toks[ptr].str);
    match(tok::num);
    return result;
}

string_literal* parse::string_gen() {
    auto result = new string_literal(toks[ptr].loc, toks[ptr].str);
    match(tok::str);
    return result;
}

expr* parse::scalar_gen() {
    if (look_ahead(tok::lcurve)) {
        match(tok::lcurve);
        auto result = calculation_gen();
        match(tok::rcurve);
        return result;
    }
    if (look_ahead(tok::num)) {
        return number_gen();
    }
    if (look_ahead(tok::str)) {
        return string_gen();
    }
    if (look_ahead(tok::id)) {
        return call_gen();
    }
    err.err("parse", toks[ptr].loc, "expected scalar here.");
    next();
    return nullptr;
}

expr* parse::multive_gen() {
    auto result = scalar_gen();
    while(look_ahead(tok::mult) || look_ahead(tok::div) || look_ahead(tok::mult)) {
        auto binary = new binary_operator(toks[ptr].loc);
        binary->set_left(result);
        switch(toks[ptr].type) {
            case tok::mult: binary->set_opr(binary_operator::kind::mult); break;
            case tok::div: binary->set_opr(binary_operator::kind::div); break;
            case tok::mod: binary->set_opr(binary_operator::kind::mod); break;
            default: break;
        }
        match(toks[ptr].type);
        binary->set_right(scalar_gen());
        result = binary;
    }
    return result;
}

expr* parse::additive_gen() {
    auto result = multive_gen();
    while(look_ahead(tok::add) || look_ahead(tok::sub)) {
        auto binary = new binary_operator(toks[ptr].loc);
        binary->set_left(result);
        switch(toks[ptr].type) {
            case tok::add: binary->set_opr(binary_operator::kind::add); break;
            case tok::sub: binary->set_opr(binary_operator::kind::sub); break;
            default: break;
        }
        match(toks[ptr].type);
        binary->set_right(multive_gen());
        result = binary;
    }
    return result;
}

expr* parse::calculation_gen() {
    return additive_gen();
}

type_def* parse::type_gen() {
    auto result = new type_def(toks[ptr].loc);
    result->set_name(identifier_gen());
    while(look_ahead(tok::mult)) {
        result->add_pointer_level();
        match(tok::mult);
    }
    return result;
}

struct_field* parse::struct_field_gen() {
    auto result = new struct_field(toks[ptr].loc);
    result->set_name(identifier_gen());
    match(tok::colon);
    result->set_type(type_gen());
    return result;
}

struct_decl* parse::struct_gen() {
    auto result = new struct_decl(toks[ptr].loc);
    match(tok::stct);
    result->set_name(toks[ptr].str);
    match(tok::id);
    match(tok::lbrace);
    while(!look_ahead(tok::rbrace)) {
        result->add_field(struct_field_gen());
        if (look_ahead(tok::comma)) {
            match(tok::comma);
        } else {
            break;
        }
    }
    match(tok::rbrace);
    return result;
}

param* parse::param_gen() {
    auto result = new param(toks[ptr].loc);
    result->set_name(identifier_gen());
    match(tok::colon);
    result->set_type(type_gen());
    return result;
}

param_list* parse::param_list_gen() {
    auto result = new param_list(toks[ptr].loc);
    match(tok::lcurve);
    while(!look_ahead(tok::rcurve)) {
        result->add_param(param_gen());
        if (look_ahead(tok::comma)) {
            match(tok::comma);
        } else {
            break;
        }
    }
    match(tok::rcurve);
    return result;
}

func_decl* parse::function_gen() {
    auto result = new func_decl(toks[ptr].loc);
    match(tok::func);
    result->set_name(toks[ptr].str);
    match(tok::id);
    result->set_params(param_list_gen());
    if (!look_ahead(tok::arrow)) {
        auto ret = new type_def(toks[ptr].loc);
        ret->set_name(new identifier(toks[ptr].loc, "void"));
        result->set_return_type(ret);
    } else {
        match(tok::arrow);
        result->set_return_type(type_gen());
    }
    result->set_code_block(block_gen());
    return result;
}

code_block* parse::block_gen() {
    auto result = new code_block(toks[ptr].loc);
    match(tok::lbrace);
    while(!look_ahead(tok::rbrace)) {
        switch(toks[ptr].type) {
            case tok::lcurve:
            case tok::num:
            case tok::str:
            case tok::id: result->add_stmt(in_stmt_expr_gen()); break;
            case tok::ret: result->add_stmt(return_gen()); break;
            default: match(toks[ptr].type); break;
        }
    }
    match(tok::rbrace);
    return result;
}

in_stmt_expr* parse::in_stmt_expr_gen() {
    auto result = new in_stmt_expr(toks[ptr].loc);
    result->set_expr(calculation_gen());
    return result;
}

ret_stmt* parse::return_gen() {
    auto result = new ret_stmt(toks[ptr].loc);
    match(tok::ret);
    result->set_value(calculation_gen());
    match(tok::semi);
    return result;
}

const error& parse::analyse(const std::vector<token>& token_list) {
    ptr = 0;
    toks = token_list.data();
    result = new root(token_list[0].loc);
    if (look_ahead(tok::eof)) {
        return err;
    }
    while (!look_ahead(tok::eof)) {
        switch(toks[ptr].type) {
            case tok::func: result->add_decl(function_gen()); break;
            case tok::stct: result->add_decl(struct_gen()); break;
            default: match(toks[ptr].type); break;
        }    
    }
    return err;
}

}