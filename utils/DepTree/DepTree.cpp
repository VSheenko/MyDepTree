#include "DepTree.h"

DepTree::DepTree(const fs::path &root_pkg_path, const int depth) {
    root_pkg = PkgNode::CreatePkgNode(root_pkg_path);
    root_pkg->GetDependencies(depth);
}

DepTree::~DepTree() {
    delete root_pkg;
}

std::string DepTree::GetPlantUml() {
    std::string plantuml = "@startuml\n";
    std::queue<PkgNode*> q;
    q.push(root_pkg);

    while (!q.empty()) {
        PkgNode* temp = q.front();
        q.pop();

        for (const auto& dep : temp->dependencies) {
            plantuml += "[" + temp->pkg_name + "; " + temp->version + "]" +
                    " --> [" + dep->pkg_name + "; " + dep->version + "]" + '\n';
            q.push(dep);
        }
    }

    plantuml += "@enduml\n";
    return plantuml;
}
