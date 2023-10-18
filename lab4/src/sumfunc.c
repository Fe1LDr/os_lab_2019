#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "sumfunc.h"

struct SumArgs {
  int *array;
  int begin;
  int end;
};

int Sum(struct SumArgs *args) {
  int sum = 0;
  // TODO: your code here 
  for(int i=args->begin;i<args->end;i++ ){
    sum+=args->array[i];
  }
  printf("%d\n", sum);
  return sum;
}

void *ThreadSum(void *args) {
  struct SumArgs *sum_args = (struct SumArgs *)args;
  return (void *)(size_t)Sum(sum_args);
}
