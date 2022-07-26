#pragma once
#include <Windows.h>
#include <string>
#include <vector>

#define BLACK           0
#define BLUE            1
#define GREEN           2
#define BABY_BLUE       3
#define RED             4
#define MAGENTA         5
#define GOLD            6
#define WHITE           7
#define GRAY            8
#define LIGHT_BLUE      9
#define LIGHT_GREEN     10
#define SKY_BLUE        11
#define LIGHT_RED       12
#define PINK            13
#define YELLOW          14
#define BRIGHT_WHITE    15


#define DEFAULT         WHITE + BLACK * 16
#define GREEN_TEXT      LIGHT_GREEN + BLACK * 16

void setColor(int n) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), n); }

void clear() {
    COORD topLeft = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);

    COORD bottomLeft = { 0, 30 };

    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, bottomLeft);
}


std::vector<std::string> split(std::string str)
{
    std::vector<int> del;
    del.push_back(-1);
    std::vector<std::string> rtn;
    bool full = false;
    int count = 0;
    for (int i=0;i<str.length();i++)
    {
        if (str[i] == '\"')
        {
            count++;
            full = true;
        } else if (str[i] == ' ' && full == false) del.push_back(i);
        if (full && count == 2)
        {
            del.push_back(i + 1);
            full = false;
        }
    }

    for (int i = 0; i < del.size() - 1; i++)
    {
        rtn.push_back(str.substr(del[i] + 1, del[i + 1] - (del[i] + 1)));
    }
    if(del[del.size() - 1] + 1 <= str.length())
        rtn.push_back(str.substr(del[del.size() - 1] + 1));

    if(rtn.size() > 2) rtn.resize(rtn.size() - 1);

    return rtn;
}

std::string strip(std::string str, char del)
{
    std::string rtn;
    for (char c : str)
    {
        if (c != del) rtn += c;
    }
    return rtn;
}

void changePath(std::filesystem::path* source, std::filesystem::path* prev, std::string dest)
{
    *prev = *source;
    *source = dest;
}

bool contains(std::string str, char token)
{
    for (char c : str) if (c == token) return true;
    return false;
}

std::string toLower(std::string str)
{
    std::string rtn;
    for (char c : str) rtn += (char)std::tolower(c);
    return rtn;
}