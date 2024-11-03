#ifndef MYDEPTREE_ADDITIONALFILELOADER_H
#define MYDEPTREE_ADDITIONALFILELOADER_H

#include "InternetWorker.h"
#include "JsonManager.h"
#include "ZipWorker.h"
#include <iostream>
#include <filesystem>

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

bool ValidateJsonManager(JsonManager& manager) {
    bool success = false;
    std::string jdk_url = manager.Get("URL_JDK", success);
    if (!success) {
        std::cerr << "Error: URL_JDK not found in appsettings.json" << std::endl;
        return false;
    }

    std::string plantuml_url = manager.Get("URL_PLANTUML", success);
    if (!success) {
        std::cerr << "Error: URL_PLANTUML not found in appsettings.json" << std::endl;
        return false;
    }

    return true;
}

bool GetJDK (JsonManager& manager) {
    fs::path path_jdk_zip = fs::current_path() / "PlantUml" / "jdk.zip";
    fs::create_directories(path_jdk_zip.parent_path());

    bool success = false;

    std::string jdk_url = manager.Get("URL_JDK", success);
    if (!success) {
        std::cerr << "Error: URL_JDK not found in appsettings.json" << std::endl;
        return false;
    }

    int res = InternetWorker::DownloadFile(jdk_url, path_jdk_zip.string());
    if (res != 0) {
        std::cerr << "Error: Failed to download JDK" << std::endl;
        return false;
    }

    success = Unzip(path_jdk_zip, fs::current_path() / "PlantUml" / "JDK");
    if (!success) {
        std::cerr << "Error: Failed to unzip JDK" << std::endl;
        return false;
    }

    fs::remove(path_jdk_zip);
    return true;
}

bool GetPlantUml(JsonManager& manager) {
    fs::path path_plantuml = fs::current_path() / "PlantUml" / "plantuml.jar";
    fs::create_directories(path_plantuml.parent_path());

    bool success = false;
    std::string plantuml_url = manager.Get("URL_PLANTUML", success);

    if (!success) {
        std::cerr << "Error: URL_PLANTUML not found in appsettings.json" << std::endl;
        return false;
    }

    int res = InternetWorker::DownloadFile(plantuml_url, path_plantuml.string());
    if (res != 0) {
        std::cerr << "Error: Failed to download PlantUml" << std::endl;
        return false;
    }

    return true;
}

bool GetNecessaryFiles(short& code, JsonManager& manager) {
    if (code == 0)
        return true;

    bool code_return = true;

    if (code >= 2) {
        bool success = GetPlantUml(manager);
        code_return &= success;
        code -= 2;
    }

    if (code >= 1) {
        bool success = GetJDK(manager);
        code_return &= success;
        code -= 1;
    }

    return code_return;
}

void CheckNecessaryFiles(short& code) {
    // JDK in ./PlantUml/JDK/jdk-version/ (bin/java.exe)
    // plantuml.jar in ./PlantUml/plantuml.jar

    fs::path path_to_appsettings = fs::current_path() / "appsettings.json";
    std::optional<JsonManager> json_manager;

    if (!GetJsonManager(json_manager, path_to_appsettings))
        return;

    JsonManager manager = json_manager.value();

    if (!ValidateJsonManager(manager))
        return;

    bool jdk_flag = false;
    bool plantuml_flag = false;

    fs::path path_jdk = manager.Get("PATH_JAVA", jdk_flag);
    fs::path path_plantuml = manager.Get("PATH_PLANTUML", plantuml_flag);

    jdk_flag &= fs::exists(path_jdk);
    plantuml_flag &= fs::exists(path_plantuml);

    code = 0;
    if (!jdk_flag) {
        fs::path jdk_path = fs::current_path() / "PlantUml" / "JDK";
        bool flag = false;
        if (fs::exists(jdk_path))
            for (const auto& entry : fs::directory_iterator(jdk_path))
                jdk_flag |= (entry.is_directory() && fs::exists(entry.path() / "bin" / "java.exe"));

        if (!jdk_flag) {
            code += 1;
        }
    }


    if (!plantuml_flag) {
        fs::path plantuml_path = fs::current_path() / "PlantUml" / "plantuml.jar";
        if (!fs::exists(plantuml_path)) {
            code += 2;
        }
    }

    if (code == 0) {
        std::cout << "All necessary files are found" << std::endl;
        return;
    }

    if (!GetNecessaryFiles(code, manager)) {
        std::cerr << "Error: Failed to get necessary files" << std::endl;
        return;
    }
}

#endif //MYDEPTREE_ADDITIONALFILELOADER_H
