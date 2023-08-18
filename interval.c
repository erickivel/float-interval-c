#include <fenv.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "interval.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

struct Expression *readExpression() {
  struct Expression *expr = malloc(sizeof(struct Expression));

  float x1, x2, x3, x4, x5;
  char o1, o2, o3, o4;

  scanf("%f %c %f %c %f %c %f %c %f", &x1, &o1, &x2, &o2, &x3, &o3, &x4, &o4,
        &x5);

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

struct Interval *makeInterval(float num) {
  struct Interval *inter = malloc(sizeof(struct Interval));

  fesetround(FE_DOWNWARD);

  inter->first = nextafter(num, -INFINITY);
  inter->second = nextafter(num, INFINITY);

  return inter;
}

struct Interval **makeIntervalVector(struct Expression *expr) {
  struct Interval **intervalVectors =
      malloc(sizeof(struct Interval *) * (OPS + 1));

  for (int i = 0; i < OPS + 1; i++) {
    intervalVectors[i] = makeInterval(expr->nums[i]);
  }

  return intervalVectors;
}

void printInterval(struct Interval interval) {
  printf("[%1.8e,%1.8e]", interval.first, interval.second);
}

float absError(struct Interval interval) {
  return interval.second - interval.first;
}

float relError(struct Interval interval) {
  return (interval.second - interval.first) / interval.first;
}

int calcULPs(struct Interval interval) {
  // float aux = interval.first;
  // long ulps = 0;

  return (int)interval.second - (int)interval.first;
  // while (nextafter(double x, double y)) {
  //   aux = nextafter(aux, INFINITY);
  //   ulps++;
  // }
  //
  // return ulps;
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
  aux.first = 1 / b.second;
  aux.second = 1 / b.first;

  return intervalMult(a, aux);
}

struct Interval **calculate(struct Expression expression,
                            struct Interval **intervalVector) {

  struct Interval **ansIntervalVector =
      malloc(sizeof(struct Interval *) * (OPS + 1));

  if (expression.ops[0] == '+') {
    ansIntervalVector[0] = intervalSum(*intervalVector[0], *intervalVector[1]);
  } else if (expression.ops[0] == '-') {
    ansIntervalVector[0] = intervalSub(*intervalVector[0], *intervalVector[1]);
  } else if (expression.ops[0] == '*') {
    ansIntervalVector[0] = intervalMult(*intervalVector[0], *intervalVector[1]);
  } else if (expression.ops[0] == '/') {
    ansIntervalVector[0] = intervalDiv(*intervalVector[0], *intervalVector[1]);
  }

  for (int i = 1; i < OPS; i++) {
    if (expression.ops[i] == '+') {
      ansIntervalVector[i] =
          intervalSum(*ansIntervalVector[i - 1], *intervalVector[i + 1]);
    } else if (expression.ops[i] == '-') {
      ansIntervalVector[i] =
          intervalSub(*ansIntervalVector[i - 1], *intervalVector[i + 1]);
    } else if (expression.ops[i] == '*') {
      ansIntervalVector[i] =
          intervalMult(*ansIntervalVector[i - 1], *intervalVector[i + 1]);
    } else if (expression.ops[i] == '/') {
      ansIntervalVector[i] =
          intervalDiv(*ansIntervalVector[i - 1], *intervalVector[i + 1]);
    }
  }

  return ansIntervalVector;
}

void printVector(struct Expression expression,
                 struct Interval **intervalVector) {
  for (int i = 0; i < 4; i++) {
    printInterval(*intervalVector[i]);
  }
}

void printAnswer(struct Expression expression, struct Interval **intervalVector,
                 struct Interval **ansIntervalVector) {
  printf("1:\n");
  printInterval(*intervalVector[0]);
  printf(" %c ", expression.ops[0]);
  printInterval(*intervalVector[1]);
  printf(" = ");
  printInterval(*ansIntervalVector[0]);
  printf("\n");
  printf("EA: %1.8e; ", absError(*ansIntervalVector[0]));
  printf("ER: %1.8e; ", relError(*ansIntervalVector[0]));
  printf("ULPs: %d;", calcULPs(*ansIntervalVector[0]));
  printf("\n\n");

  for (int i = 1; i < OPS; i++) {
    printf("%d:\n", i + 1);
    printInterval(*ansIntervalVector[i - 1]);
    printf(" %c ", expression.ops[i]);
    printInterval(*intervalVector[i + 1]);
    printf(" = ");
    printInterval(*ansIntervalVector[i]);
    printf("\n");

    printf("EA: %1.8e; ", absError(*ansIntervalVector[i]));
    printf("ER: %1.8e; ", relError(*ansIntervalVector[i]));
    printf("ULPs: %d;", calcULPs(*ansIntervalVector[i]));
    printf("\n\n");
  }
}

void freeExpression(struct Expression expression);
void freeIntervalVector(struct Interval **interval);
