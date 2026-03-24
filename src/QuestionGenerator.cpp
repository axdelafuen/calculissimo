#include "QuestionGenerator.h"
#include <cstdlib>

Question QuestionGenerator::generate() const {
    Question q;
    q.a = rand() % 20 + 1;
    q.b = rand() % 20 + 1;

    int opType = rand() % 3;
    switch (opType) {
        case 0: q.op = '+'; q.correctAnswer = q.a + q.b; break;
        case 1: q.op = '-'; q.correctAnswer = q.a - q.b; break;
        case 2: q.op = 'x'; q.correctAnswer = q.a * q.b; break;
    }

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
