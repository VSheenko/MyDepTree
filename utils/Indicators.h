#ifndef MYDEPTREE_INDICATORS_H
#define MYDEPTREE_INDICATORS_H

// |########            | 40.00% для загрузки файлов или того, что можно процентно измерить
// или
// |   #####            |

#include <iostream>
#include <iomanip>
#include <windows.h>

#define BAR_LENGTH 20
#define BAR_UPDATE_THRESHOLD 0.01

class Indicators {
private:
    COORD start_pos;
    bool is_progressbar;

private:
    COORD GetCursorPosition();
    void SetCursorPosition(COORD pos);
    void HideCursor();
    void ShowCursor();

public:
    Indicators(bool is_progressbar);
    ~Indicators();
    void Update(double progress);

};


#endif //MYDEPTREE_INDICATORS_H
