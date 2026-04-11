#pragma once
#include "logSystem.h"
#include <cstdint>
#include <cstdio>
#include <cstring>

struct Style {
    uint8_t r, g, b;
    uint8_t bgR, bgG, bgB;
    bool bg = false;
};

class terminalLog : public logSystem {
    char _ansi[64];
    char _final_msg[512];

    Style _styles[5] = {
        {150, 150, 150},
        {100, 200, 100},
        {255, 200,   0},
        {255, 100, 100},
        {255, 255, 255, 180, 0, 0, true},
    };

    void buildAnsi(const Style& s) {
        char tmp[32];

        strcpy(_ansi, "\033[");
        snprintf(tmp, sizeof(tmp), "38;2;%d;%d;%d;", s.r, s.g, s.b);
        strcat(_ansi, tmp);

        if (s.bg) {
            snprintf(tmp, sizeof(tmp), "48;2;%d;%d;%d;", s.bgR, s.bgG, s.bgB);
            strcat(_ansi, tmp);
        }

        _ansi[strlen(_ansi) - 1] = 'm'; 
    }

public:
    void setStyle(LogLevel level, const Style& style) {
        _styles[level] = style;
    }

    void addLog(const char* msg, const LogLevel level) override {
        buildAnsi(_styles[level]);
        snprintf(_final_msg, sizeof(_final_msg), "%s%s\033[0m\n", _ansi, msg);
        printf("%s", _final_msg);
        fflush(stdout);
    }
};