#pragma once

#include "Question.h"
#include "Operation.h"
#include "Difficulty.h"
#include <vector>
#include <memory>

class QuestionGenerator {
public:
    QuestionGenerator();
    void setOperationIndex(int idx);
    // Generate a question respecting the operand ranges from the given config.
    Question generate(const DifficultyConfig& config) const;
    int getOperationCount() const;
    const Operation* getOperation(int idx) const;
private:
    std::vector<std::unique_ptr<Operation>> operations;
    int selectedOperationIndex;
};
