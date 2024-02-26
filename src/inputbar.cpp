#include "inputbar.hpp"

InputBar::InputBar(int width, int height, int curX, int curY, std::wstring title):
windowX_(width),
windowY_(height),
title_(title) {
    buffer_ = newwin(windowY_, windowX_, curY, curX);

    keypad(buffer_, true);
    wbkgd(buffer_, COLOR_PAIR(COLOR_INPUTBAR));
    wattron(buffer_, COLOR_PAIR(COLOR_INPUTBAR));

    box(buffer_, 0, 0);
    mvwaddwstr(buffer_, 0, 1, title.c_str());
    wmove(buffer_, 1, 1);
};

void InputBar::draw() {
    wmove(buffer_, 1, 1);
    mvwaddwstr(buffer_, 1, 1, std::wstring(windowX_ - 2, L' ').c_str());
    mvwaddwstr(buffer_, 1, 1, contents_.c_str());

    wrefresh(buffer_);
};

void InputBar::input() {
    wint_t key;
    wget_wch(buffer_, &key);
    switch(key) {
        case KEY_RIGHT: case KEY_LEFT: case KEY_UP: case KEY_DOWN: {
            break;
        };

        case KEY_BACKSPACE: {
            if ((int)contents_.length() > 0) contents_.erase((int)contents_.length() - 1, 1);

            break;
        };
        // ESC
        case 27: {
            inFocus = false;
            contents_ = L"";

            break;
        };
        // ENTER
        case 10: {
            inFocus = false;

            break;
        }

        default: {
            if (((int)contents_.length() < windowX_ - 2)) contents_ += wchar_t(key);
        };
    };
};

void InputBar::retitle(std::wstring newTitle) {
    title_ = newTitle;

    box(buffer_, 0, 0);
    mvwaddwstr(buffer_, 0, 1, title_.c_str());
};

std::wstring InputBar::getContents() {
    return contents_;
};

void InputBar::toEmpty() {
    contents_ = L"";
};