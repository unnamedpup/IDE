#include "utils.hpp"
#include <filesystem>

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
