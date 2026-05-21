#pragma once
#include "question/Operation.h"

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

// Division always produces exact integer results.
// Operand generation picks a quotient then multiplies by the divisor so
// that a / b is always a whole number with no remainder.
class Division : public Operation {
    public:
        char getSymbol() const override { return '/'; }
        int compute(int a, int b) const override { return b != 0 ? a / b : 0; }

        std::pair<int, int> generateOperands(int minVal, int maxVal) const override {
            // b is the divisor (2..9 keeps mental-math manageable)
            int b = 2 + rand() % 8;
            // quotient stays in [max(1,minVal), maxVal]
            int lo = (minVal < 1) ? 1 : minVal;
            int quotient = lo + rand() % (maxVal - lo + 1);
            return {quotient * b, b};  // a = quotient*b so a/b == quotient exactly
        }
};
