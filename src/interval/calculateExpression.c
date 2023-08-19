#include <stdlib.h>

#include "../../include/interval.h"

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
