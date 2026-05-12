#include "QuestionGenerator.h"
#include "Operations.h"
#include <cstdlib>

QuestionGenerator::QuestionGenerator() : selectedOperationIndex(0) {
    operations.emplace_back(std::make_unique<Addition>());
    operations.emplace_back(std::make_unique<Subtraction>());
    operations.emplace_back(std::make_unique<Multiplication>());
    operations.emplace_back(std::make_unique<Division>());
}

void QuestionGenerator::setOperationIndex(int idx) {
    if (idx >= 0 && idx < static_cast<int>(operations.size())) {
        selectedOperationIndex = idx;
    }
}

int QuestionGenerator::getOperationCount() const {
    return static_cast<int>(operations.size());
}

const Operation* QuestionGenerator::getOperation(int idx) const {
    if (idx >= 0 && idx < static_cast<int>(operations.size())) {
        return operations[idx].get();
    }
    return nullptr;
}

Question QuestionGenerator::generate() const {
    const Operation* op = operations[selectedOperationIndex].get();

    // Let each operation generate operands that are valid for it.
    // Division overrides this to guarantee integer results.
    auto [a, b] = op->generateOperands(1, 20);

    Question q;
    q.a = a;
    q.b = b;
    q.op = op->getSymbol();
    q.correctAnswer = op->compute(a, b);

    q.correctIndex = rand() % 3;

    for (int i = 0; i < 3; i++) {
        if (i == q.correctIndex) {
            q.options[i] = q.correctAnswer;
        } else {
            int wrong;
            do {
                wrong = q.correctAnswer + (rand() % 11) - 5;
            } while (wrong == q.correctAnswer || wrong < 0);
            q.options[i] = wrong;
        }
    }

    return q;
}
