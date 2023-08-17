#include <fenv.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "interval.h"

struct Expression *readExpression() {
  struct Expression *expr = malloc(sizeof(struct Expression));

  double x1, x2, x3, x4, x5;
  char o1, o2, o3, o4;

  scanf("%lf %c %lf %c %lf %c %lf %c %lf", &x1, &o1, &x2, &o2, &x3, &o3, &x4,
        &o4, &x5);

  expr->nums[0] = x1;
  expr->nums[1] = x2;
  expr->nums[2] = x3;
  expr->nums[3] = x4;
  expr->nums[4] = x5;
  expr->ops[0] = o1;
  expr->ops[1] = o2;
  expr->ops[2] = o3;
  expr->ops[3] = o4;

  return expr;
}

struct Interval *makeInterval(double num) {
  struct Interval *inter = malloc(sizeof(struct Interval));

  fesetround(FE_DOWNWARD);

  inter->first = nextafter(num, -INFINITY);
  inter->second = nextafter(num, INFINITY);

  return inter;
}

struct Interval **makeIntervalVector(struct Expression *expr) {
  struct Interval **intervalVectors =
      malloc(sizeof(struct Interval *) * OPS + 1);

  for (int i = 0; i < OPS + 1; i++) {
    intervalVectors[0] = makeInterval(expr->nums[i]);
  }

  return intervalVectors;
}

void printInterval(struct Interval interval) {
  printf("[%1.8e,%1.8e]", interval.first, interval.second);
}

double absError(struct Interval interval) {
  return interval.second - interval.first;
}

double relError(struct Interval interval) {
  return (interval.second - interval.first) / interval.first;
}

int calcULPs(struct Interval interval) {
  return (interval.second - interval.first) / interval.first;
}

void printOps(struct Expression expr) {
  for (int i = 0; i < OPS; i++) {
    printInterval(*makeInterval(expr.nums[i]));
    printf(" %c ", expr.ops[i]);
  }
  printInterval(*makeInterval(expr.nums[4]));
}

struct Interval *calculate(struct Expression expression,
                           struct Interval **intervalVector) {}

void printAnswer(struct Interval **intervalVector) {

  for (int i = 0; i < OPS; i++) {
    printInterval(*interval[i]);
    printf("EA: %1.8e; ", absError(interval));
    printf("ER: %1.8e; ", relError(interval));
    printf("ULPs: %d;", calcULPs(interval));
  }
}

void freeExpression(struct Expression expression);
void freeIntervalVector(struct Interval **interval);
