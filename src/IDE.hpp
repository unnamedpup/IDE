#pragma once

#include "INCLUDE.cpp"
#include "SETTINGS.cpp"
#include "editor.hpp"
#include "toolbar.hpp"
#include "inputbar.hpp"
#include "utils.hpp"

#define MAIN_EDITOR 0
#define DEFAULT_TITLE L"Untitled.json"

class IDE {
public: 
    IDE();

    void initialization();
    void start();
    void finalization();
    
private:
    void resetMainScr_();

    std::vector <Editor*> editors_;
    ToolBar *toolBar_;
    InputBar *inputBar_;
    uint8_t windowX_, windowY_;
    int8_t editorPointer_;
    bool running_;
};