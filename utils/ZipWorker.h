#ifndef MYDEPTREE_ZIPWORKER_H
#define MYDEPTREE_ZIPWORKER_H

#include <libzippp/libzippp.h>
#include <filesystem>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

bool Unzip(const fs::path& zip_path, const fs::path& out_path) {
    libzippp::ZipArchive archive(zip_path.string());
    archive.open(libzippp::ZipArchive::ReadOnly);

    if (!archive.isOpen()) {
        std::cerr << "Error: Failed to open archive: " << zip_path << std::endl;
        return false;
    }

    for (auto entry : archive.getEntries()) {
        if (!entry.isFile()) continue;

        fs::path outputPath = out_path / entry.getName();
        fs::create_directories(outputPath.parent_path());

        std::ofstream outputFile(outputPath, std::ios::binary);

        if (outputFile.is_open()) {
            std::string data = entry.readAsText();
            outputFile.write(data.c_str(), data.size());
            outputFile.close();
        } else {
            std::cerr << "Error: Failed to create file: " << outputPath << std::endl;
            return false;
        }
    }

    archive.close();
    return true;
}


#endif //MYDEPTREE_ZIPWORKER_H
