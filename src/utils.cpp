#include "utils.hpp"

std::string wstringToString(std::wstring fileName) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string name = converter.to_bytes(fileName);
    return name;
};


bool fileExists(std::string fileName) {
    return (std::filesystem::exists(fileName));
};

std::wstring fileToText(std::string fileName) {
    std::wstring text = L"";
    std::wifstream wif(fileName);

    if (wif.is_open()) {
        std::wstring wLine = L"";

        while (getline(wif, wLine)) {
            text += wLine + L'\n';
        };
    };
    wif.close();

    return text;
};

void textToFile(std::wstring text, std::string fileName) {
    std::wofstream wof(fileName);
    wof << text;
};

std::wstring getLine(std::wstring text, int pos) {
    text = L"\n" + text;
    ++pos;
    
    int i;
    for (i = pos; i > 0; --i) {
        if (text[i] == L'\n') break;
    };

    int LineStart = i;
    for (i = pos; i < (int)text.length(); ++i) {
        if (text[i] == L'\n') break;
    };

    int LineEnd = i - LineStart;
    return text.substr(LineStart, LineEnd);
};

void initColors() {
    start_color();
    use_default_colors();

    if (can_change_color() && COLORS >= 16) {
        init_color(COLOR_GRAY, 128, 128, 128);
    };

    init_pair(COLOR_EDITOR, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_TOOLBAR, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_HIGHLIGHT_TOOLBAR, COLOR_WHITE, COLOR_GRAY);
    init_pair(COLOR_MAIN, COLOR_WHITE, COLOR_BLACK);

};
