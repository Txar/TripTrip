#pragma once

float capAt(float x, float max) {
    return (x > max) ? max : x;
}

int capAt(int x, int max) {
    return (x > max) ? max : x;
}