#pragma once

#include "INCLUDE.cpp"

class InputBar {
public: 
    InputBar(int width, int height, int curX, int curY, std::wstring title);

    void draw();
    void input();   
    void retitle(std::wstring newTitle);
    std::wstring getContents();
    void toEmpty();

    bool inFocus;

private:
    WINDOW *buffer_;
    std::wstring title_, contents_;
    int windowX_, windowY_;
};