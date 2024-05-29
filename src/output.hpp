#pragma once

#define CTRL(c) ((c) & 037)
#include "INCLUDE.cpp"

class Output {
public: 
    Output(int width, int height, int posX, int posY, std::wstring title);

    WINDOW *getBuffer();

    void resize(int newX, int newY);
    void reposition(int newX, int newY);
    void retitle(std::wstring newTitle);
    std::wstring getTitle();
    void draw();
    void drawFrame();
    void input();
    void clear();
    std::wstring getContents();
    void setContents(std::wstring newContents);
    void changePos(int newPos);
    void moveCursor(int newX, int newY);

    bool inFocus;
    bool run = true;

private:
    WINDOW *buffer_, *position_, *frame_;
    std::wstring contents_, title_;
    int windowX_, windowY_, posX_, posY_, curPos_, curY_, curX_, start_, end_, selectPos_, selectY_, selectX_;
    bool selected_;
};