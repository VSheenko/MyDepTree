#include "JsonManager.h"


std::optional<JsonManager> JsonManager::Open(const fs::path& path_to_json) {
    std::ifstream ifs(path_to_json);
    JsonManager json_manager;

    if (json_manager.ParseJson(path_to_json)) {
        return json_manager;
    }

    return std::nullopt;
}

bool JsonManager::ParseJson(const fs::path &path_to_json) {
    std::ifstream ifs(path_to_json);

    try {
        data = json::parse(ifs);
        return true;
    } catch (json::parse_error& e) {
        return false;
    }
}

std::string JsonManager::Get(const std::string &key, bool& SUCCESS) {
    if (data.find(key) == data.end()) {
        SUCCESS = false;
        return "";
    }

    SUCCESS = true;
    return data[key];
}
