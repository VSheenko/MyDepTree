#ifndef MYDEPTREE_JSONMANAGER_H
#define MYDEPTREE_JSONMANAGER_H

#include <string>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>


#define JSON_FILE "../appsettings.json"

using json = nlohmann::json;
namespace fs = std::filesystem;


class JsonManager {
private:
    json data;

    JsonManager() = default;
    bool ParseJson(const fs::path& path_to_json);

public:
    static std::optional<JsonManager> Open (const fs::path& path_to_json);
    std::string Get(const std::string& key, bool& SUCCESS);


};


#endif //MYDEPTREE_JSONMANAGER_H
