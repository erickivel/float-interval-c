#include <fenv.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "interval.h"

struct Expression *readExpression() {
  struct Expression *expr = malloc(sizeof(struct Expression));

  float x1, x2, x3, x4, x5;
  char o1, o2, o3, o4;

  scanf("%f %c %f %c %f %c %f %c %f", &x1, &o1, &x2, &o2, &x3, &o3, &x4, &o4,
        &x5);

  expr->nums[0].f = x1;
  expr->nums[1].f = x2;
  expr->nums[2].f = x3;
  expr->nums[3].f = x4;
  expr->nums[4].f = x5;
  expr->ops[0] = o1;
  expr->ops[1] = o2;
  expr->ops[2] = o3;
  expr->ops[3] = o4;

  return expr;
}

struct Interval *makeInterval(float num) {
  struct Interval *inter = malloc(sizeof(struct Interval));

  fesetround(FE_DOWNWARD);

  inter->first.f = nextafterf(num, -INFINITY);
  inter->second.f = nextafterf(num, INFINITY);

  return inter;
}

struct Interval **makeIntervalVector(struct Expression *expr) {
  struct Interval **intervalVectors =
      malloc(sizeof(struct Interval *) * (OPS + 1));

  for (int i = 0; i < OPS + 1; i++) {
    intervalVectors[i] = makeInterval(expr->nums[i].f);
  }

  return intervalVectors;
}

void printInterval(struct Interval interval) {
  printf("[%1.8e,%1.8e]", interval.first.f, interval.second.f);
}

float absError(struct Interval interval) {
  return interval.second.f - interval.first.f;
}

float relError(struct Interval interval) {
  return (interval.second.f - interval.first.f) / interval.first.f;
}

int calcULPs(struct Interval interval) {
  return abs(interval.second.i - interval.first.i);
}

void printOps(struct Expression expr) {
  for (int i = 0; i < OPS; i++) {
    printInterval(*makeInterval(expr.nums[i].f));
    printf(" %c ", expr.ops[i]);
  }
  printInterval(*makeInterval(expr.nums[4].f));
}

struct Interval *intervalSum(struct Interval a, struct Interval b) {
  fesetround(FE_DOWNWARD);

  struct Interval *result = malloc(sizeof(struct Interval));

  result->first.f = a.first.f + b.first.f;
  result->second.f = a.second.f + b.second.f;

  result->first.f = nextafterf(result->first.f, -INFINITY);
  result->second.f = nextafterf(result->second.f, INFINITY);

  return result;
}

struct Interval *intervalSub(struct Interval a, struct Interval b) {
  fesetround(FE_DOWNWARD);

  struct Interval *result = malloc(sizeof(struct Interval));

  result->first.f = a.first.f - b.second.f;
  result->second.f = a.second.f - b.first.f;

  result->first.f = nextafterf(result->first.f, -INFINITY);
  result->second.f = nextafterf(result->second.f, INFINITY);

  return result;
}

struct Interval *intervalMult(struct Interval a, struct Interval b) {
  fesetround(FE_DOWNWARD);

  struct Interval *result = malloc(sizeof(struct Interval));

  result->first.f = fmin(fmin(a.first.f * b.first.f, a.first.f * b.second.f),
                         fmin(a.second.f * b.first.f, a.second.f * b.second.f));
  result->second.f =
      fmax(fmax(a.first.f * b.first.f, a.first.f * b.second.f),
           fmax(a.second.f * b.first.f, a.second.f * b.second.f));

  result->first.f = nextafterf(result->first.f, -INFINITY);
  result->second.f = nextafterf(result->second.f, INFINITY);

  return result;
}

struct Interval *intervalDiv(struct Interval a, struct Interval b) {
  fesetround(FE_DOWNWARD);

  struct Interval aux;
  aux.first.f = 1 / b.second.f;
  aux.second.f = 1 / b.first.f;

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

void freeExpression(struct Expression *expression) { free(expression); }

void freeIntervalVector(struct Interval **interval) {
  for (int i = 0; i < OPS + 1; i++) {
    free(interval[i]);
  }

  free(interval);
}

void freeAnsIntervalVector(struct Interval **interval) {
  for (int i = 0; i < OPS; i++) {
    free(interval[i]);
  }

  free(interval);
}
