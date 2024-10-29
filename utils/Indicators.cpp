#include "Indicators.h"

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

void Indicators::Update(double progress) {
    COORD cur_pos = GetCursorPosition();
    SetCursorPosition(start_pos);

    std::cout << "|" << std::setw(20) <<  std::string((int)(progress / (1.0 / BAR_LENGTH)), '#') << "|" <<
              std::right << std::setw(7) << std::setprecision(2) << std::fixed << progress * 100 << "%" << std::left;


    SetCursorPosition(cur_pos);
}

Indicators::~Indicators() {
    std::cout << "deleted" << '\n';
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

