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

bool DepTree::GenerateGraphImage(std::string plantuml) {
    time_t now_time = time(NULL);
    std::tm* local_time = localtime(&now_time);

    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", local_time);
    std::string file_name = ".\\PlantUml\\" +  std::string(buffer) + ".plum";

    std::ofstream temp_file(file_name);

    if (!temp_file.is_open())
        return false;

    temp_file << plantuml;
    temp_file.close();

    std::string command = ".\\PlantUml\\start.bat " + file_name;
    system(command.c_str());
    fs::remove_all(file_name);

    std::cout << "\nPlantUml file: " << plantuml << std::endl;
    ShellExecute(NULL, "open", (".\\PlantUml\\" +  std::string(buffer) + ".png").c_str(), NULL, NULL, SW_SHOWNORMAL);
    return true;
}

