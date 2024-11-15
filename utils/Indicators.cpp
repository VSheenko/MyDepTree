#include "Indicators.h"

std::vector<std::string> Indicators::units_speed_measurement = {"B/s", "kB/s", "MB/s", "Gb/s"};

Indicators::Indicators(bool is_progressbar) {
    this->is_progressbar = is_progressbar;
    this->start_pos = GetCursorPosition();

    HideCursor();

    if (this->is_progressbar) {
        Update(0);
    }
}

COORD Indicators::GetCursorPosition() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    return csbi.dwCursorPosition;
}

void Indicators::SetCursorPosition(COORD pos) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsole, pos);
}

void Indicators::Update(double progress, double speed) {
    COORD cur_pos = GetCursorPosition();
    SetCursorPosition(start_pos);

    // LOAD ERROR
    if (progress == -1) {
        std::cout << std::setw(50) << "Download error";
        SetCursorPosition(cur_pos);
        return;
    }

    if (!is_progressbar) {
        std::cout << "Loading" << std::left << std::setw(3) << std::string((int)progress, '.');
        SetCursorPosition(cur_pos);
        return;
    }

    std::cout << "|" << std::setw(BAR_LENGTH) <<  std::string((int)(progress / (1.0 / BAR_LENGTH)), '#') << "|" <<
              std::right << std::setw(7) << std::setprecision(2) << std::fixed << progress * 100 << "%" << std::left;

    if (speed != -1) {
        int k = 0;
        while (speed > 500) {
            speed /= 1024;
            k++;
        }

        if (k >= units_speed_measurement.size())
            std::cout << " | ----";
        else
            std::cout << " | " << std::right << std::setw(6) << std::setprecision(2) << std::fixed <<
                      (speed >= 0 ? speed : 0) << " " << units_speed_measurement[k] << std::left;
    }

    SetCursorPosition(cur_pos);
}

Indicators::~Indicators() {
    if (!is_progressbar) {
        SetCursorPosition(start_pos);
        std::cout << std::setw(25) << "Complete";
    }

    SetCursorPosition({0, (short)(start_pos.Y + 1)});
    ShowCursor();
}

void Indicators::HideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

void Indicators::ShowCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

