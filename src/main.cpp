#include <locale.h>

#include "INCLUDE.cpp"
#include "IDE.hpp"



int main(int argc, char* argv[]) {
    std::locale::global(std::locale(""));

    IDE app;
    app.initialization();
    app.start();
    app.finalization();

    return 0;
}