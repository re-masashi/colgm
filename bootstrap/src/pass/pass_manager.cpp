#include "pass/pass.h"
#include "pass/pass_manager.h"

#include "pass/add_default_main.h"

namespace colgm {

void pass_manager::run(ir_context& c) {
    ordered_passes.clear();
    ordered_passes.push_back(new add_default_main(c));
    for(auto i : ordered_passes) {
        if (!i->run()) {
            break;
        }
    }
}

}