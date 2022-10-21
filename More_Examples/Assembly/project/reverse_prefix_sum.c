#include <stdio.h>
#include <stdint.h>

/*
  Given an array arr terminated by entry -1, transform the array by adding
  to each entry the sum of the remainder of the array after that entry.
  That is, if initially arr == [a_0, a_1, ..., a_n, -1], then finally
  arr == [b_0, b_1, ..., b_n, -1], where b_j = sum(a_i for i in j..n).
 */

uint32_t reverse_prefix_sum(uint32_t *arr) {
  uint32_t r;

  if(*arr == -1) return 0;
  r = reverse_prefix_sum(arr+1) + (uint32_t)*arr;
  *arr = r;        /* may discard MSB */
  return(r);
}
