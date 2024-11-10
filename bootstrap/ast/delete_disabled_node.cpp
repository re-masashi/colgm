#include "ast/delete_disabled_node.h"
#include "colgm.h"

#include <vector>

namespace colgm::ast {

bool delete_disabled_node::check_enable_if(condition_comment* cc) {
    if (cc->get_condition_name() != "enable_if") {
        return false;
    }
    for(const auto& i : cc->get_comments()) {
        if (i.first != "target_os" && i.first != "arch") {
            return false;
        }
    }
    if (!cc->get_comments().count("target_os") ||
        !cc->get_comments().count("arch")) {
        return false;
    }
    const auto& target_os = cc->get_comments().at("target_os");
    const auto& arch = cc->get_comments().at("arch");
    if (target_os == get_platform() && arch == get_arch()) {
        return true;
    }
    return false;
}

void delete_disabled_node::scan(root* node) {
    std::vector<decl*> new_root_decls;
    for(auto i : node->get_decls()) {
        if (i->get_ast_type() != ast_type::ast_condition_comment) {
            new_root_decls.push_back(i);
            continue;
        }
        auto cc = static_cast<condition_comment*>(i);
        // should be enabled
        if (check_enable_if(cc)) {
            new_root_decls.push_back(cc->get_enabled_decl());
            cc->set_enabled_decl(nullptr);
            delete cc;
        } else {
            delete cc;
        }
    }
    node->reset_decls(new_root_decls);
}

}