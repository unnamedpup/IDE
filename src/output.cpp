#include "output.hpp"
#include "utils.hpp"

Output::Output(int width, int height, int posX, int posY, std::wstring title):
windowX_(width),
windowY_(height),
posX_(posX),
posY_(posY),
curPos_(0),
curY_(0),
curX_(0),
start_(0),
end_(1),
title_(title),
selected_(false),
selectPos_(0) {
    frame_ = newwin(windowY_, windowX_, posY_, posX_);

    box(frame_, 0, 0);
    mvwaddwstr(frame_, 0, 1, title.c_str());
    wmove(frame_, 1, 1);

    buffer_ = newwin(windowY_ - 2, windowX_ - 2, posY_ + 1, posX_ + 1);
    keypad(buffer_, true);

    position_ = newwin(1, 8, windowY_, 1);

    wbkgd(frame_, COLOR_PAIR(COLOR_EDITOR));
    wattron(frame_, COLOR_PAIR(COLOR_EDITOR));
    wbkgd(buffer_, COLOR_PAIR(COLOR_EDITOR));
    wattron(buffer_, COLOR_PAIR(COLOR_EDITOR));
    wbkgd(position_, COLOR_PAIR(COLOR_EDITOR));
    wattron(position_, COLOR_PAIR(COLOR_EDITOR));

    contents_ = L"";
};

WINDOW *Output::getBuffer() {
    return buffer_;
};

void Output::resize(int newX, int newY) {
    windowX_ = newX;
    windowY_ = newY;

    wresize(buffer_, windowY_, windowX_);
};

void Output::reposition(int newX, int newY) {
    posX_ = newX;
    posY_ = newY;

    wmove(buffer_, posY_, posX_);
};

void Output::retitle(std::wstring newTitle) {
    title_ = newTitle;

    box(frame_, 0, 0);
    mvwaddwstr(frame_, 0, 1, title_.c_str());
    wmove(frame_, 1, 1);
};

std::wstring Output::getTitle() {
    return title_;
};

void Output::draw() {
    wclear(buffer_);
    wattron(buffer_, COLOR_PAIR(COLOR_EDITOR));

    bool end = false;
    int line = 0, y = start_ > 1? -1 : 0;
    end_ = start_ + windowY_ - (start_ > 0? 3 : 2);
    for (int i = 0; i < (int)contents_.length(); ++i) {
        wchar_t sym[] = { contents_[i], L'\0' };
        switch (contents_[i]) {
            case L'\n': {
                ++ line;

                if (line >= end_) end = true;

                break;
            };
        };

        if (end) break;
 
        if (line >= start_ - 1) switch (contents_[i]) {
            case 9: {
                wprintw(buffer_, "    ");

                break;
            };

            case L'\n': {
                ++y;
                wmove(buffer_, y, 0);

                break;
            };

            default: {
                waddwstr(buffer_, sym);

                break;
            };
        };
    };

    wclear(position_);
    wprintw(position_, "--------");
    mvwprintw(position_, 0, 0, "%i:%i", curY_ + 1, curX_ + 1);
    wrefresh(position_);

    wmove(buffer_, curY_ - start_ + (start_ >= 1? 1 : 0), curX_);
    wrefresh(buffer_);
};

void Output::drawFrame() {
    wrefresh(frame_);
};

void Output::input() {
    wint_t key;
    wget_wch(buffer_, &key);

    switch (key) {
        case CTRL('u'): {

            textToFile(contents_, wstringToString(title_));
            
            break;
        };

        case 27: {
            inFocus = false;

            break;
        };

        case KEY_UP: {
            if (curY_ == 0) break;

            --curY_;
            int LineLength = (int)getLine(contents_, curPos_ - 2).length();
            curPos_ -= curX_;

            LineLength = (int)getLine(contents_, curPos_ - 2).length();
            if (curX_ >= LineLength) {
                curX_ = LineLength > 0? LineLength - 1 : 0;
                --curPos_;
            } else {
                curPos_ -= LineLength - curX_;
            };

            if (curY_ < start_) -- start_;

            break;
        };

        case KEY_DOWN: {
            int prevX = curX_, prevY = curY_, prevPos = curPos_;

            int LineLength = (int)getLine(contents_, curX_ > 0? curPos_ - 1 : curPos_).length();
            LineLength -= LineLength == 0? 0 : 1;

            curPos_ += LineLength - curX_;
            ++curY_;

            LineLength = (int)getLine(contents_, curPos_ + 1).length();
            LineLength -= LineLength == 0? 0 : 1;
            if (curX_ >= LineLength) {
                curPos_ += LineLength + 1;
                curX_ = LineLength;
            } else {
                curPos_ += curX_ + 1;
            };

            if (curPos_ > (int)contents_.length()) {
                curX_ = prevX;
                curY_ = prevY;
                curPos_ = prevPos;
                
                break;
            };

            if (curY_ - start_ + (start_ >= 1? 1 : 0) > windowY_ - 4) ++ start_;

            break;
        };

        case KEY_LEFT: {
            if (curPos_ < 1) break;

            if (curX_ < 1) {
                --curY_;
                int LineLength = (int)getLine(contents_, curPos_ - 2).length();
                curX_ = LineLength == 0? 1 : LineLength;
            };
            
            --curPos_;
            --curX_;

            if (curY_ < start_) --start_;

            break;
        };

        case KEY_RIGHT: {
            if (curPos_ + 1 > (int)contents_.length()) break;
            if (curX_ + 1 > (int)getLine(contents_, curPos_).length()) {
                ++curPos_;
                ++curY_;
                curX_ = 0;

                if (curY_ - start_ + (start_ >= 1? 1 : 0) > windowY_ - 4) ++start_;

                break;
            };

            ++curPos_;
            ++curX_;

            break;
        };

        case KEY_BACKSPACE: {
            int oldSelectPos = selectPos_, 
                oldCurPos = curPos_,
                targX = 0,
                targY = 0,
                targPos = 0;

            if (selected_) {
                if (oldCurPos >= oldSelectPos) {
                    targPos = oldSelectPos;
                    targX = selectX_;
                    targY = selectY_;
                } else {
                    targPos = oldCurPos;
                    curPos_ = oldSelectPos;
                    targX = curX_;
                    targY = curY_;
                };
            };

            while (curPos_ > 0) {
                if (curX_ < 1) {
                    --curY_;
                    int LineLength = (int)getLine(contents_, curPos_ - 2).length();
                    curX_ = LineLength == 0? 1 : LineLength;
                };
                
                contents_.erase(curPos_ - 1, 1);
                --curPos_; 
                --curX_;

                if (curY_ < start_) --start_;

                if (!selected_) break;

                if (curPos_ <= targPos) {
                    curPos_ = targPos;
                    curX_ = targX;
                    curY_ = targY;
                    selected_ = false;

                    break;
                };
            };

            break;
        };

        case 10: {
            contents_.insert(curPos_, L"\n");
            ++curPos_;
            ++curY_;
            curX_ = 0;

            if (curY_ - start_ + (start_ >= 1? 1 : 0) > windowY_ - 4) ++start_;

            break;
        };

        default: {
            if (curX_ > windowX_ - 4) {
                contents_.insert(curPos_, L"\n");
                ++curPos_;
                ++curY_;
                curX_ = 0;

                if (curY_ - start_ + (start_ >= 1? 1 : 0) > windowY_ - 4) ++start_;

                break;
            };

            std::wstring c = L"";
            c += wchar_t(key);
            contents_.insert(curPos_, c);
            ++curPos_;
            ++curX_;

            break;
        };
    };
};

void Output::clear() {
    contents_ = L"";
    curPos_ = curY_ = curX_ = start_ = 0;
    end_ = 1;
    selected_ = false;
};


std::wstring Output::getContents() {
    return contents_;
};

void Output::setContents(std::wstring newContents) {
    clear();

    contents_ = newContents;
};