#ifndef INTERVAL_H
#define INTERVAL_H

#define OPS 4

struct Interval {
  double first;
  double second;
};

struct Expression {
  double nums[OPS + 1];
  char ops[OPS];
};

struct Expression *readExpression();

struct Interval **makeIntervalVector(struct Expression *expr);

struct Interval **calculate(struct Expression expression,
                            struct Interval **intervalVector);

void printAnswer(struct Interval **intervalVector);

void freeExpression(struct Expression expression);
void freeIntervalVector(struct Interval **interval);

#endif /* INTERVAL_H */
