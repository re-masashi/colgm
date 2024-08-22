#pragma once

#include "mir/mir.h"
#include "mir/ast2mir.h"
#include "mir/visitor.h"

#include <vector>

namespace colgm::mir {

class pass: public visitor {
public:
    virtual std::string name() = 0;
    virtual bool run(mir_context*) = 0;
};

class pass_manager {
private:
    std::vector<pass*> work_list;

public:
    void execute(mir_context*);
};

}
