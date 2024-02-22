// #define _XOPEN_SOURCE_EXTENDED
#include <locale.h>

#include "INCLUDE.cpp"
#include "IDE.hpp"



int main(int argc, char* argv[]) {
    // setlocale(LC_ALL, "");
    std::locale::global(std::locale(""));

    IDE app;
    app.initialization();
    app.start();
    app.finalization();

    return 0;
}