#include <fenv.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "interval.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

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

struct Interval *intervalSum(struct Interval a, struct Interval b) {
  struct Interval *result = malloc(sizeof(struct Interval));

  result->first = a.first + b.first;
  result->second = a.second + b.second;

  return result;
}

struct Interval *intervalSub(struct Interval a, struct Interval b) {
  struct Interval *result = malloc(sizeof(struct Interval));

  result->first = a.first - b.second;
  result->second = a.second - b.first;

  return result;
}

// X * Y = [a,b] * [c,d]  =  [min{a*c,a*d,b*c,b*d}, max{a*c,a*d,b*c,b*d}]
struct Interval *intervalMult(struct Interval a, struct Interval b) {
  struct Interval *result = malloc(sizeof(struct Interval));

  result->first = MIN(MIN(a.first * b.first, a.first * b.second),
                      MIN(a.second * b.first, a.second * b.second));
  result->second = MAX(MAX(a.first * b.first, a.first * b.second),
                      MAX(a.second * b.first, a.second * b.second));

  return result;
}

struct Interval *intervalDiv(struct Interval a, struct Interval b) {
  struct Interval aux;
  aux.second = 1/b.first;
  aux.first = 1/a.second;

  return intervalMult(a, aux);
}

struct Interval **calculate(struct Expression expression,
                            struct Interval **intervalVector) {

  struct Interval **ansIntervalVector =
      malloc(sizeof(struct Interval *) * OPS + 1);

  if (expression.ops[0] == '+') {
    ansIntervalVector[0] = intervalSum(*intervalVector[0], *intervalVector[1]);
  } else if (expression.ops[0] == '-') {
    ansIntervalVector[0] = intervalSum(*intervalVector[0], *intervalVector[1]);
  } else if (expression.ops[0] == '*') {
    ansIntervalVector[0] = intervalSum(*intervalVector[0], *intervalVector[1]);
  } else if (expression.ops[0] == '/') {
    ansIntervalVector[0] = intervalSum(*intervalVector[0], *intervalVector[1]);
  }

  for (int i = 2; i < OPS + 1; i++) {
    if (expression.ops[i] == '+') {
      ansIntervalVector[i - 1] =
          intervalSum(*ansIntervalVector[i - 2], *intervalVector[i]);
    } else if (expression.ops[i] == '-') {

    } else if (expression.ops[i] == '*') {

    } else if (expression.ops[i] == '/') {
    }
  }

  return intervalVectors;
}

void printAnswer(struct Interval **intervalVector) {
  for (int i = 0; i < OPS; i++) {
    printInterval(*intervalVector[i]);
    printf("EA: %1.8e; ", absError(*intervalVector[i]));
    printf("ER: %1.8e; ", relError(*intervalVector[i]));
    printf("ULPs: %d;", calcULPs(*intervalVector[i]));
  }
}

void freeExpression(struct Expression expression);
void freeIntervalVector(struct Interval **interval);
