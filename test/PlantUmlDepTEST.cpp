#include "gtest/gtest.h"
#include <filesystem>
#include "../DepTree/DepTree.h"

namespace fs = std::filesystem;

TEST(DepTest, aiogramm) {
    fs::path path = fs::current_path() / ".." / "venv" / "Lib" / "site-packages" / "aiogram";

    DepTree* tree = new DepTree(path, 0);
    std::string exp = "@startuml\n"
                      "[aiogram; 3.13.1]\n"
                      "@enduml\n";
    ASSERT_EQ(exp, tree->GetPlantUml());
    delete tree;

    tree = new DepTree(path, 3);
    exp = "@startuml\n"
          "[aiogram; 3.13.1] --> [magic_filter; 1.0.12]\n"
          "[aiogram; 3.13.1] --> [pydantic; 2.9.2]\n"
          "[aiogram; 3.13.1] --> [certifi; 2024.8.30]\n"
          "[aiogram; 3.13.1] --> [aiofiles; 24.1.0]\n"
          "[aiogram; 3.13.1] --> [aiohttp; 3.10.10]\n"
          "[aiogram; 3.13.1] --> [typing_extensions; 4.12.2]\n"
          "[pydantic; 2.9.2] --> [annotated_types; 0.7.0]\n"
          "[pydantic; 2.9.2] --> [pydantic_core; 2.23.4]\n"
          "[pydantic; 2.9.2] --> [typing_extensions; 4.12.2]\n"
          "[aiohttp; 3.10.10] --> [yarl; 1.16.0]\n"
          "[aiohttp; 3.10.10] --> [aiosignal; 1.3.1]\n"
          "[aiohttp; 3.10.10] --> [frozenlist; 1.4.1]\n"
          "[aiohttp; 3.10.10] --> [aiohappyeyeballs; 2.4.3]\n"
          "[aiohttp; 3.10.10] --> [attrs; 24.2.0]\n"
          "[aiohttp; 3.10.10] --> [multidict; 6.1.0]\n"
          "[annotated_types; 0.7.0] --> [typing_extensions; 4.12.2]\n"
          "[pydantic_core; 2.23.4] --> [typing_extensions; 4.12.2]\n"
          "[yarl; 1.16.0] --> [propcache; 0.2.0]\n"
          "[yarl; 1.16.0] --> [idna; 3.10]\n"
          "[yarl; 1.16.0] --> [multidict; 6.1.0]\n"
          "[aiosignal; 1.3.1] --> [frozenlist; 1.4.1]\n"
          "[multidict; 6.1.0] --> [typing_extensions; 4.12.2]\n"
          "@enduml\n";
    ASSERT_EQ(exp, tree->GetPlantUml());
}


TEST(DepTest, qt6) {
    fs::path path = fs::current_path() / ".." / "venv" / "Lib" / "site-packages" / "PyQt6";

    DepTree* tree = new DepTree(path, 1);
    std::string exp = "@startuml\n"
                      "[PyQt6; 6.7.1] --> [PyQt6_sip; 13.8.0]\n"
                      "[PyQt6; 6.7.1] --> [PyQt6_Qt6; 6.7.3]\n"
                      "@enduml\n";

    ASSERT_EQ(exp, tree->GetPlantUml());
    delete tree;

    tree = new DepTree(path, 0);
    exp = "@startuml\n"
          "[PyQt6; 6.7.1]\n"
          "@enduml\n";

    ASSERT_EQ(exp, tree->GetPlantUml());
    delete tree;
}


TEST(DepTest, numpy) {
    fs::path path = fs::current_path() / ".." / "venv" / "Lib" / "site-packages" / "numpy";
    DepTree tree(path, 3);
    std::string exp = "@startuml\n"
                      "[numpy; 2.1.2]\n"
                      "@enduml\n";
    ASSERT_EQ(exp, tree.GetPlantUml());
}


TEST(DepTest, django) {
    fs::path path = fs::current_path() / ".." / "venv" / "Lib" / "site-packages" / "django";
    DepTree tree(path, 3);
    std::string exp = "@startuml\n"
                      "[django; 5.1.2] --> [sqlparse; 0.5.1]\n"
                      "[django; 5.1.2] --> [asgiref; 3.8.1]\n"
                      "[asgiref; 3.8.1] --> [typing_extensions; 4.12.2]\n"
                      "@enduml\n";

    ASSERT_EQ(exp, tree.GetPlantUml());
}
