#pragma once
#include <utility>
#include <cstdlib>

// Abstract base class for arithmetic operations.
// Each concrete operation defines its symbol, how to compute the result,
// and optionally how to generate valid operands (e.g. Division enforces
// integer-only results by overriding generateOperands).
class Operation {
    public:
        virtual ~Operation() = default;
        virtual char getSymbol() const = 0;
        virtual int compute(int a, int b) const = 0;

        // Returns a pair (a, b) of operands valid for this operation.
        // The default picks two random values in [minVal, maxVal].
        // Operations with constraints (e.g. Division) override this.
        virtual std::pair<int, int> generateOperands(int minVal, int maxVal) const {
            int range = maxVal - minVal + 1;
            return {minVal + rand() % range, minVal + rand() % range};
        }
};
