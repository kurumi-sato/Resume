#include <stdio.h>
#include <stdint.h>

extern uint32_t is_palindrome(const char *s);

int main() {
  const char *inputs[] = {
    "abba",
    "racecar",
    "swap paws",
    "not a palindrome",
    "another non palindrome",
    "almost but tsomla",
    NULL,
  };
  uint32_t i;
  for(i = 0; inputs[i]; i++) {
    printf("%s --> %c\n", inputs[i], is_palindrome(inputs[i]) ? 'Y' : 'N');
  }
  return 0;
}

