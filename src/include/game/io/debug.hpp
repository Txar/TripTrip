#pragma once

#include <iostream>

class debug {
    public:
        std::string console = "";

        void append(std::string text) {
            console.append(text);
        }

        void line(std::string line) {
            append(line + "\n");
        };

        debug() {

        }
};

debug de;

void print(std::string line) {
    de.line(line);
}