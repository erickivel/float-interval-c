#include "interval.h"
#include <stdio.h>

int main(void) {
  struct Expression *expr = readExpression();

  struct Interval **intervalVector = makeIntervalVector(expr);

  struct Interval **answer = calculate(*expr, intervalVector);

  // printVector(*expr, answer);

  printAnswer(*expr, intervalVector, answer);

  return 0;
}
