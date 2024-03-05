#include "code/ir.h"
#include "colgm.h"

namespace colgm {

void hir_struct::dump(std::ostream& out) const {
    out << "%struct." << name << " = type {";
    for(usize i = 0; i<field_type.size(); ++i) {
        out << field_type[i];
        if (i!=field_type.size()-1) {
            out << ", ";
        }
    }
    out << "}\n";
}

hir_func::~hir_func() {
    delete cb;
}

void hir_func::dump(std::ostream& out) const {
    out << (cb? "define ":"declare ");
    out << return_type << " " << name << "(";
    for(const auto& i : params) {
        out << i.second << " %" << i.first;
        if (i.first!=params.back().first) {
            out << ", ";
        }
    }
    out << ")";
    if (!cb) {
        out << "\n";
        return;
    }
    out << " {\n";
    cb->dump(out);
    out << "}\n";
}

ir_code_block::~ir_code_block() {
    for(auto i : stmts) {
        delete i;
    }
}

void ir_code_block::dump(std::ostream& out) const {
    for(auto i : stmts) {
        out << "  ; ";
        i->dump(out);
    }
}

void ir_ret::dump(std::ostream& out) const {
    out << "ret";
    if (!has_return_value) {
        out << " void";
    }
    out << "\n";
}

void ir_def::dump(std::ostream& out) const {
    out << "define_local " << name << " = alloca " << type << "\n";
}

void ir_number::dump(std::ostream& out) const {
    out << "push_number  " << literal << "\n";
}

void ir_string::dump(std::ostream& out) const {
    out << "push_string  \"" << rawstr(literal) << "\"\n";
}

void ir_call_index::dump(std::ostream& out) const {
    out << "call_index\n";
}

void ir_call_field::dump(std::ostream& out) const {
    out << "call_field   " << field_name << "\n";
}

void ir_ptr_call_field::dump(std::ostream& out) const {
    out << "ptr_callfld  " << field_name << "\n";
}

void ir_call_path::dump(std::ostream& out) const {
    out << "call_path    " << field_name << "\n";
}

void ir_call_func::dump(std::ostream& out) const {
    out << "call_funct   " << argc << "\n";
}

void ir_get_var::dump(std::ostream& out) const {
    out << "get_variable " << name << "\n";
}

void ir_binary::dump(std::ostream& out) const {
    out << "calculation  " << opr << "\n";
}

void ir_label::dump(std::ostream& out) const {
    out << "label " << label_count << ":\n";
}

void ir_assign::dump(std::ostream& out) const {
    out << "store_value  " << opr << "\n";
}

void ir_br_direct::dump(std::ostream& out) const {
    out << "br_direct    label " << destination << "\n";
}

void ir_br_cond::dump(std::ostream& out) const {
    out << "br_cond      ";
    out << "label_true " << destination_true << ", ";
    out << "label_false " << destination_false << "\n";
}

}