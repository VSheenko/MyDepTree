#include <iostream>
#include <filesystem>
#include "./utils/InternetWorker.h"
#include "./utils/JsonManager.h"

namespace fs = std::filesystem;

bool GetJsonManager(std::optional<JsonManager>& json_manager, const fs::path& path_to_appsettings) {
    if (!fs::exists(path_to_appsettings)) {
        fs::create_directories(path_to_appsettings.parent_path());
        std::ofstream ofs(path_to_appsettings);
        ofs << "{\n"
               "  \"URL_JDK\": \"url_to_jdk.zip\",\n"
               "  \"URL_PLANTUML\": \"url_to_plantuml.jar\"\n"
               "}";
        ofs.close();

        std::cerr << "Error: appsettings.json not found" << std::endl;
        std::cerr << "Please, complete new appsettings.json in the root directory" << std::endl;
        std::cerr << "{\n"
                     "  \"URL_JDK\": \"url_to_jdk.zip\",\n"
                     "  \"URL_PLANTUML\": \"url_to_plantuml.jar\"\n"
                     "}" << std::endl;

        return false;
    }

    json_manager = JsonManager::Open(path_to_appsettings);

    if (!json_manager) {
        std::cerr << "Error: appsettings.json is not valid" << std::endl;
        return false;
    }

    return true;
}


bool GetNecessaryFiles(short& code) {
    fs::path path_to_appsettings = fs::current_path() / "appsettings.json";
    std::optional<JsonManager> json_manager;

    if (!GetJsonManager(json_manager, path_to_appsettings))
        return false;

    JsonManager manager = json_manager.value();

    // TODO: Реализовать загрузку файлов JDK и plantuml.jar

    return false;
}

void CheckNecessaryFiles(short& code) {
    // JDK in ./PlantUml (bin/java.exe)
    // plantuml.jar in ./PlantUml

    // OK - 0
    // Not found jdk code - 1
    // not found plantuml.jar - 2
    // not found jdk and plantuml - 3

    code = 0;

    fs::path jdk_path = fs::current_path() / "PlantUml" / "bin" / "java.exe";
    fs::path plantuml_path = fs::current_path() / "PlantUml" / "plantuml.jar";

    if (!fs::exists(jdk_path))
        code += 1;

    if (!fs::exists(plantuml_path))
        code += 2;
}


int main() {
}
