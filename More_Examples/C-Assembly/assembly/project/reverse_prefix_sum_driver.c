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
