#pragma once
#include "Operation.h"

class Addition : public Operation {
    public:
        char getSymbol() const override { return '+'; }
        int compute(int a, int b) const override { return a + b; }
};

class Subtraction : public Operation {
    public:
        char getSymbol() const override { return '-'; }
        int compute(int a, int b) const override { return a - b; }
};

class Multiplication : public Operation {
    public:
        char getSymbol() const override { return 'x'; }
        int compute(int a, int b) const override { return a * b; }
};
