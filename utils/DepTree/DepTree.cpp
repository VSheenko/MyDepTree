#include "DepTree.h"

DepTree::DepTree(const fs::path &root_pkg_path, const int depth) {
    root_pkg = PkgNode::CreatePkgNode(root_pkg_path);
    root_pkg->GetDependencies(depth);
}

DepTree::~DepTree() {

}
