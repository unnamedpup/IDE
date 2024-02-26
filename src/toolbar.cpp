#include "toolbar.hpp"

ToolBar::ToolBar(int width, int height, int curX, int curY):
windowX_(width),
windowY_(height),
cursorX_(curX),
cursorY_(curY),
highlight_(0),
currentChoice_(-1) {
    buffer_ = newwin(windowY_, windowX_, cursorY_, cursorX_);
    keypad(buffer_, true);
    wmove(buffer_, 1, 1);
    
    wbkgd(buffer_, COLOR_PAIR(COLOR_TOOLBAR));
    wattron(buffer_, COLOR_PAIR(COLOR_TOOLBAR));
};

WINDOW *ToolBar::getBuffer() {
    return buffer_;
};

void ToolBar::draw() {
    wmove(buffer_, 0, 0);
    for (int i = 0; i < (int)choices_.size(); ++i) {
        wattron(buffer_, COLOR_PAIR(inFocus && highlight_ == i ? COLOR_HIGHLIGHT : COLOR_TOOLBAR));
        waddwstr(buffer_, choices_[i].c_str());
    };

    wmove(buffer_, 0, windowX_ - 1);
    wattron(buffer_, COLOR_PAIR(COLOR_STOP)); 
    wprintw(buffer_, "|");
    wrefresh(buffer_);
};

void ToolBar::input() {
    int choice = wgetch(buffer_);
    currentChoice_ = -1;

    switch(choice) {
        // ESC
        case 27: {
            inFocus = false;
            break;
        };

        case KEY_LEFT: {
            if (highlight_ == 0) highlight_ = (int)choices_.size();
            --highlight_;
            break;
        };

        case KEY_RIGHT: {
            if (highlight_ == (int)choices_.size() - 1) highlight_ = -1;
            ++highlight_;
            break;
        };

        // ENTER
        case 10: {
            currentChoice_ = highlight_;
            break;
        };
    };
};

void ToolBar::setChoices(std::vector <std::wstring> choices) {
    choices_ = choices;
};

int8_t ToolBar::getChoice() {
    return currentChoice_;
};