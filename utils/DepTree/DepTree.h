#ifndef MYDEPTREE_DEPTREE_H
#define MYDEPTREE_DEPTREE_H

#include <string>
#include <filesystem>
#include <queue>
#include "PkgNode.h"


namespace fs = std::filesystem;

class DepTree {
public:
    private:

private:
    PkgNode* root_pkg;
public:
    std::string GetPlantUml();
    bool GenerateGraphImage(std::string plantuml);

    explicit DepTree(const fs::path& root_pkg_path, int depth);
    ~DepTree();





};


#endif //MYDEPTREE_DEPTREE_H
