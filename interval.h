#ifndef INTERVAL_H
#define INTERVAL_H

#define OPS 4

struct Interval {
  float first;
  float second;
};

struct Expression {
  float nums[OPS + 1];
  char ops[OPS];
};

struct Expression *readExpression();

struct Interval **makeIntervalVector(struct Expression *expr);

struct Interval **calculate(struct Expression expression,
                            struct Interval **intervalVector);

void printAnswer(struct Expression expression, struct Interval **intervalVector,
                 struct Interval **ansIntervalVector);

void freeExpression(struct Expression expression);
void freeIntervalVector(struct Interval **interval);

void printVector(struct Expression expression,
                 struct Interval **intervalVector);

#endif /* INTERVAL_H */
