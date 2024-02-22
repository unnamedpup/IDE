#pragma once

#include "INCLUDE.cpp"


class ToolBar {
public: 
    ToolBar(int width, int height, int curX, int curY);

    WINDOW *getBuffer();
    void draw();
    void input();
    void setChoices(std::vector <std::wstring> choices);
    int8_t getChoice();

    bool inFocus;
private:
    WINDOW *buffer_;
    std::vector <std::wstring> choices_;
    int windowX_, windowY_, cursorX_, cursorY_;
    uint8_t highlight_;
    int8_t currentChoice_;
};



