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
#include <stdio.h>
#include <stdint.h>

uint32_t reverse_prefix_sum(uint32_t *a);

uint32_t data1[] = { 1, 2, 3, 4, -1 };
uint32_t data2[] = { 2, 3, 4, 5, -1 };
uint32_t data3[] = { 5, 4, 3, 2,  -1 };
uint32_t data4[] = { 200456, 3345056, 1, 2, 1, 2, -1 };
uint32_t overflow[] = { 1, 1, 1, 1, 2147483646, -1 };

void print_array(uint32_t *v) {
  uint32_t n;
  while (-1 != (n = *v++)) {
    printf("%d ", n);
  }
  printf("\n");
}

int main() {
  uint32_t ret;
  print_array(data1);
  ret = reverse_prefix_sum(data1);
  print_array(data1);

  print_array(data2);
  ret = reverse_prefix_sum(data2);
  print_array(data2);

  print_array(data3);
  ret = reverse_prefix_sum(data3);
  print_array(data3);

  print_array(data4);
  ret = reverse_prefix_sum(data4);
  print_array(data4);

  print_array(overflow);
  ret = reverse_prefix_sum(overflow);
  print_array(overflow);
}
