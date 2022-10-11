#include <stdio.h>
#include <stdint.h>

/* 
   fibonacci(0) = 0
   fibonacci(1) = 1
   for n > 1: fibonacci(n) = fibonacci(n-1) + fibonacci(n-2)
*/

uint32_t fibonacci(uint32_t n) {
   if (n == 0)
      return 0;

   if (n == 1)
      return 1;

   return fibonacci(n - 1) + fibonacci(n - 2);
}
