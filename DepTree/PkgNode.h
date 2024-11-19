#ifndef MYDEPTREE_PKGNODE_H
#define MYDEPTREE_PKGNODE_H

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <regex>
#include <set>
#include <cctype>

namespace fs = std::filesystem;


class PkgNode;

class PkgNode {
public:
    std::string pkg_name;
    std::string version;
    fs::path pkg_path;
    fs::path METADATA_path;

private:
    void FindMetadata();
    fs::path GetMetadataDir();
    std::string GetPkgName();

    std::vector<std::string> GetDependenciesName() const;
    void GetDependenciesPkg(fs::path& libs_dir, std::vector<std::string> deps_name, int depth);
    static bool isDistInfo(const std::string& s);

    struct PkgNodeCompare {
        bool operator()(const PkgNode* lhs, const PkgNode* rhs) const {
            if (lhs->pkg_name != lhs->pkg_name)
                return lhs->pkg_name < rhs->pkg_name;

            return lhs->version < rhs->version;
        }
    };

public:
    std::set<PkgNode*, PkgNodeCompare> dependencies;

    PkgNode() = default;
    ~PkgNode();
    static PkgNode* CreatePkgNode(const fs::path& pkg_path, int depth = 0);
    void GetDependencies(int depth);
};




#endif //MYDEPTREE_PKGNODE_H
