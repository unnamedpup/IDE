#pragma once

#include "INCLUDE.cpp"
#include "SETTINGS.cpp"
#include <filesystem>

std::string wstringToString(std::wstring fileName);
std::wstring stringToWstring(std::string fileName);


bool fileExists(std::string fileName);

std::wstring fileToText(std::string fileName);
void textToFile(std::wstring text, std::string fileName);

std::wstring getLine(std::wstring text, int pos);

void initColors();