#include <fstream>
#include "PkgNode.h"

PkgNode *PkgNode::CreatePkgNode(const fs::path &pkg_path, int depth) {
    if (!fs::exists(pkg_path) || !fs::is_directory(pkg_path))
        return nullptr;

    PkgNode* temp = new PkgNode();
    temp->pkg_path = pkg_path;
    temp->pkg_name = temp->GetPkgName();

    temp->FindMetadata();

    if (!fs::exists(temp->METADATA_path)) {
        return nullptr;
    }

    std::ifstream file(temp->METADATA_path);
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("Version:") != std::string::npos && line.find("Metadata-Version:") == std::string::npos) {
            auto n = line.find(':');
            temp->version = line.substr(n + 2);
            break;
        }
    }


    temp->GetDependencies(depth);


    return temp;
}

bool PkgNode::isDistInfo(const std::string &s) {
    std::regex pattern(".*\\.dist-info");
    return std::regex_match(s, pattern);
}

void PkgNode::FindMetadata() {
    if (isDistInfo(this->pkg_path.filename().string()))
        this->METADATA_path = this->pkg_path / "METADATA";
    else
        this->METADATA_path = GetMetadataDir() / "METADATA";
}

fs::path PkgNode::GetMetadataDir() {
    if (isDistInfo(this->pkg_path.filename().string())) {
        return this->pkg_path;
    }

    std::string name = this->pkg_name;
    transform(name.begin(), name.end(), name.begin(), ::tolower);
    std::string pattern = "^(" + name + ").*\\.dist-info$";

    try {
        for (auto entry : fs::directory_iterator(this->pkg_path.parent_path())) {
            fs::path path = entry.path();
            std::string dir_name = path.filename().string();
            transform(dir_name.begin(), dir_name.end(), dir_name.begin(), ::tolower);

            if (entry.is_directory() && std::regex_match(dir_name, std::regex(pattern))) {
                return path;
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    return "";
}

std::vector<std::string> PkgNode::GetDependenciesName() const {
    if (fs::exists(this->METADATA_path)) {
        std::ifstream file(this->METADATA_path);
        std::vector<std::string> deps_name;
        std::regex package_regex(R"((\w[\w-]*)[<>=])");
        std::string line;


        while (std::getline(file, line)) {
            if (line.find("Requires-Dist:") != std::string::npos) {
                auto n = line.find(':');
                std::string dep_name = line.substr(n + 2);

                while (dep_name.find('-') != std::string::npos) {
                    dep_name.replace(dep_name.find('-'), 1, "_");
                }

                std::regex pattern("[ \\(\\)]");
                dep_name = std::regex_replace(dep_name, pattern, "");

                size_t stop_pos = dep_name.find_first_of("<>=");
                dep_name = (stop_pos != std::string::npos) ? dep_name.substr(0, stop_pos) : dep_name;
                deps_name.push_back(dep_name);

            } else if (line.find("Provides-Extra:") != std::string::npos) {
                break;
            }
        }

        return deps_name;
    }

    return {};
}

void PkgNode::GetDependenciesPkg(fs::path& libs_dir, std::vector<std::string> deps_name, int depth) {
    for (auto dep_name: deps_name) {
        transform(dep_name.begin(), dep_name.end(), dep_name.begin(), ::tolower);
        std::regex pattern(dep_name + R"(-[\d\.]+\.dist-info)");

        for (const auto &entry: fs::directory_iterator(libs_dir)) {
            if (entry.is_directory()) {
                std::string dir_name = entry.path().filename().string();
                transform(dir_name.begin(), dir_name.end(), dir_name.begin(), ::tolower);

                if (std::regex_match(dir_name, pattern)) {
                    fs::path dep_path = entry.path();
                    PkgNode *temp = PkgNode::CreatePkgNode(dep_path, depth - 1);
                    this->dependencies.insert(temp);
                }
            }
        }
    }
}


void PkgNode::GetDependencies(int depth) {
    if (depth == 0)
        return;

    fs::path libs_dir = this->pkg_path.parent_path();
    GetDependenciesPkg(libs_dir, GetDependenciesName(), depth);
}

std::string PkgNode::GetPkgName() {
    std::string res = this->pkg_path.filename().string();
    return res.substr(0, res.find('-'));
}

PkgNode::~PkgNode() {
    for (auto& dep : this->dependencies) {
        delete dep;
    }
}
