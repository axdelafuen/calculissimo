#pragma once

class Operation {
    public:
        virtual ~Operation() = default;
        virtual char getSymbol() const = 0;
        virtual int compute(int a, int b) const = 0;
};
