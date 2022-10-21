#include <string.h>
#include <stdio.h>
#include <stdint.h>

/* A palindrome is a phrase that is the same forward as backwards */

uint32_t is_palindrome(const char *string) {
  uint32_t i;
  uint32_t len = strlen(string);

  for (i = 0; i < len / 2; i++) {
    if (string[i] != string[len - i - 1]) {
      return 0;
    }
  }
  return 1;
}
