#include "./DepTree/DepTree.h"
#include "./utils/Indicators.h"
#include "./utils/AdditionalFileLoader.h"
#include <thread>

std::atomic<bool> is_running(true);

void GenerateGraph(DepTree* tree) {
    tree->GenerateGraphImage(tree->GetPlantUml());
    is_running = false;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Error: Invalid number of arguments" << '\n';
        return 1;
    }

    fs::path path = argv[1];
    if (path.is_relative())
        path = fs::current_path() / path;

    if (!fs::exists(path) || !fs::is_directory(path)) {
        std::cerr << "Error: Invalid path" << '\n';
        return 1;
    }


    int depth;
    try {
        depth = std::stoi(argv[2]);
    } catch (std::invalid_argument& e) {
        std::cerr << "Error: Invalid depth" << '\n';
        return 1;
    }

    if (depth == 0) {
        std::cerr << "Error: Invalid depth" << '\n';
        return 1;
    }

    short code = 0;
    CheckNecessaryFiles(code);

    DepTree* tree = new DepTree(argv[1], std::stoi(argv[2]) - 1);

    Indicators* indicators = new Indicators(false);
    std::thread t(GenerateGraph, tree);

    short i = 1;
    while (is_running) {
        if (i > 3)
            i = 1;

        indicators->Update(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        i++;
    }
    t.join();

    delete tree;
    delete indicators;
}
