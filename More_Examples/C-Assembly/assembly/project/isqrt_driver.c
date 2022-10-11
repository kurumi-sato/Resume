#include <stdio.h>
#include <stdint.h>

extern uint32_t isqrt(uint32_t n);

int main() {
  uint32_t inputs[] = { 1, 12, 225, 169, 16, 25, 100, 81, 99, 121, 144, 0 };
  uint32_t i;
  for(i = 0; inputs[i]; i++) {
    printf("%d\n", isqrt(inputs[i]));
  }
}
