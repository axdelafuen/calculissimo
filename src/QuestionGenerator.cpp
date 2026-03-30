#include "QuestionGenerator.h"
#include "Operations.h"
#include <cstdlib>

QuestionGenerator::QuestionGenerator() : selectedOperationIndex(0) {
    operations.emplace_back(std::make_unique<Addition>());
    operations.emplace_back(std::make_unique<Subtraction>());
    operations.emplace_back(std::make_unique<Multiplication>());
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
    Question q;
    q.a = rand() % 20 + 1;
    q.b = rand() % 20 + 1;

    const Operation* op = operations[selectedOperationIndex].get();
    q.op = op->getSymbol();
    q.correctAnswer = op->compute(q.a, q.b);

    q.correctIndex = rand() % 3;

    for (int i = 0; i < 3; i++) {
        if (i == q.correctIndex) {
            q.options[i] = q.correctAnswer;
        } else {
            int wrong;
            do {
                wrong = q.correctAnswer + (rand() % 11) - 5;
            } while (wrong == q.correctAnswer);
            q.options[i] = wrong;
        }
    }

    return q;
}
