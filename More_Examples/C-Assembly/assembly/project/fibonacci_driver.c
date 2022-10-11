#include <stdio.h>
#include <stdint.h>

extern uint32_t fib(uint32_t n);

int main() {
  uint32_t inputs[] = { 1, 2, 3, 4, 10, 11, 12, 13, 14, 16, 18, 20, 24, 0 };
  uint32_t i;

  for(i = 0; inputs[i]; i++) {
    /*  printf("%d -> %d\n", inputs[i], fibonacci(inputs[i])); */
  printf("%d\n", fibonacci(inputs[i]));
  }
}

