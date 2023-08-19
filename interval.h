#include <stdint.h>

#ifndef INTERVAL_H
#define INTERVAL_H

#define OPS 4

typedef union {
  int32_t i;
  float f;
} FloatT;

struct Interval {
  FloatT first;
  FloatT second;
};

struct Expression {
  FloatT nums[OPS + 1];
  char ops[OPS];
};

struct Expression *readExpression();

struct Interval **makeIntervalVector(struct Expression *expr);

struct Interval **calculate(struct Expression expression,
                            struct Interval **intervalVector);

void printAnswer(struct Expression expression, struct Interval **intervalVector,
                 struct Interval **ansIntervalVector);

void freeExpression(struct Expression *expression);
void freeIntervalVector(struct Interval **interval);

void freeAnsIntervalVector(struct Interval **interval);

#endif /* INTERVAL_H */
