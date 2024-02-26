#include "IDE.hpp"

IDE::IDE() {};

void IDE::initialization() {
    initscr();

    cbreak();
    noecho();
    keypad(stdscr, true);

    getmaxyx(stdscr, windowY_, windowX_);

    initColors();
    attron(COLOR_PAIR(COLOR_MAIN));
    bkgd(COLOR_PAIR(COLOR_MAIN));
    resetMainScr_();

    toolBar_ = new ToolBar(windowX_, 1, 0, 0);
    toolBar_->setChoices({
        L" Выход |", L" Закрыть |", L" Создать |", L" Открыть |", L" Сохранить как |"
    }); 

    #define EXIT 0
    #define CLOSE 1
    #define NEW 2
    #define OPEN 3
    #define SAVE 4

    inputBar_ = new InputBar((windowX_ - 10), 3, windowX_ / 2 - (windowX_ - 10) / 2, windowY_ / 2 - 1.5, L"Ввод");

    editors_.push_back(new Editor(windowX_, windowY_ - 1, 0, 1, DEFAULT_TITLE));
    editorPointer_ = -1;
};

void IDE::finalization() {
    endwin();

    for (int i = 0; i < (int)editors_.size(); ++ i) delete editors_[i];
};

void IDE::start() {
    running_ = true;

    refresh();
    toolBar_->draw();

    for (int i = 0; i < (int)editors_.size(); ++ i) editors_[i]->inFocus = false;
 
    toolBar_->inFocus = true;
    inputBar_->inFocus = false;

    uint8_t inputMode = OPEN;

    while (running_) {
        if (toolBar_->inFocus) {
            toolBar_->draw();
            toolBar_->input();

            if (!toolBar_->inFocus) {
                if (editorPointer_ == -1) toolBar_->inFocus = true;
                else {
                    editors_[editorPointer_]->inFocus = true;
                    toolBar_->draw();
                };
            };

            switch(toolBar_->getChoice()) {
                case EXIT: {
                    running_ = false;

                    break;
                };

                case NEW: {
                    editorPointer_ = MAIN_EDITOR;
                    editors_[editorPointer_]->clear();
                    editors_[editorPointer_]->retitle(DEFAULT_TITLE);
                    editors_[editorPointer_]->drawFrame();
                    editors_[editorPointer_]->draw();

                    break;
                };

                case CLOSE: {
                    if (editorPointer_ == -1) break;
                    editors_[editorPointer_]->inFocus = false;
                    resetMainScr_();
                    editorPointer_ = -1;

                    break;
                };

                case OPEN: case SAVE: {
                    inputBar_->inFocus = true;
                    inputBar_->retitle(L"Введите название файла");
                    inputMode = toolBar_->getChoice();

                    toolBar_->inFocus = false;

                    break;
                };
            };
        } else if (inputBar_->inFocus) {
            inputBar_->draw();
            inputBar_->input();
            std::string name = wstringToString(inputBar_->getContents());
            if (!inputBar_->inFocus) {
                if (inputMode == OPEN) {
                    if (inputBar_->getContents() != L"" && inputBar_->getContents() != L" ") {
                        toolBar_->inFocus = true;
                        if (fileExists(name)) {
                            editorPointer_ = MAIN_EDITOR;
                            editors_[editorPointer_]->retitle(inputBar_->getContents());
                            inputBar_->toEmpty(); 
                            editors_[editorPointer_]->setContents(fileToText(name));

                            editors_[editorPointer_]->drawFrame();
                            editors_[editorPointer_]->draw();
                        } else {
                            toolBar_->inFocus = true;
                            inputBar_->toEmpty(); 

                            if (editorPointer_ == -1) resetMainScr_();
                            else editors_[editorPointer_]->draw();
                        };
                    } else {
                        toolBar_->inFocus = true;

                        if (editorPointer_ == -1) resetMainScr_();
                        else editors_[editorPointer_]->draw();
                    };
                } else {
                    if (inputBar_->getContents() != L"" && editorPointer_ != -1) {
                        editors_[editorPointer_]->retitle(inputBar_->getContents());
                        inputBar_->toEmpty(); 
                        textToFile(editors_[editorPointer_]->getContents(), name);

                        editors_[editorPointer_]->drawFrame();
                    } else {
                        toolBar_->inFocus = true;

                        if (editorPointer_ == -1) resetMainScr_();
                        else editors_[editorPointer_]->draw();
                    };
                };
            };
        } else {
            editors_[editorPointer_]->draw();
            editors_[editorPointer_]->input();

            if (!editors_[editorPointer_]->inFocus) toolBar_->inFocus = true;
        };
    };
};


void IDE::resetMainScr_() {
    clear();

    mvprintw(windowY_ / 2 - 3, 0, ((std::string) 
    "\nДля навигации в окнах используйте стрелки.\n" +
    "ESC - переключение между окнами (панель инструментов и зона редактирования).\n" +
    "CTRL + H - сохраняет совершенные изменения в файле.\n" + 
    "ENTER - выбор в панели инструментов.\n")
    .c_str());

    refresh();
};