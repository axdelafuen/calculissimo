#pragma once

#include "Question.h"
#include "Operation.h"
#include <vector>
#include <memory>

class QuestionGenerator {
public:
    QuestionGenerator();
    void setOperationIndex(int idx);
    Question generate() const;
    int getOperationCount() const;
    const Operation* getOperation(int idx) const;
private:
    std::vector<std::unique_ptr<Operation>> operations;
    int selectedOperationIndex;
};
