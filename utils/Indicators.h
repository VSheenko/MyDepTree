#ifndef MYDEPTREE_INDICATORS_H
#define MYDEPTREE_INDICATORS_H

// |########            | 40.00% для загрузки файлов или того, что можно процентно измерить
// или
// Loading...

#include <iostream>
#include <iomanip>
#include <vector>
#include <windows.h>

#define BAR_LENGTH 20

class Indicators {
private:
    COORD start_pos;
    bool is_progressbar;
    static std::vector<std::string> units_speed_measurement;

private:
    COORD GetCursorPosition();
    void SetCursorPosition(COORD pos);
    static void HideCursor();
    static void ShowCursor();

public:
    explicit Indicators(bool is_progressbar);
    ~Indicators();
    void Update(double progress, double speed = -1);

};


#endif //MYDEPTREE_INDICATORS_H
