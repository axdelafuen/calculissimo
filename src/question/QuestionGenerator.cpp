#include "question/QuestionGenerator.h"
#include "question/Operations.h"
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

Question QuestionGenerator::generate(const DifficultyConfig& config) const {
    const Operation* op = operations[selectedOperationIndex].get();

    auto [a, b] = op->generateOperands(config.minValue, config.maxValue);

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
            int attempts = 0;
            do {
                int offset = (rand() % (2 * config.wrongAnswerRange + 1)) - config.wrongAnswerRange;
                if (offset == 0) offset = 1;
                wrong = q.correctAnswer + offset;
                ++attempts;
            } while ((wrong == q.correctAnswer || wrong < 0) && attempts < 50);
            q.options[i] = wrong;
        }
    }

    return q;
}
