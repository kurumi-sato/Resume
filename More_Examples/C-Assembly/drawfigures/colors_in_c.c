#include <stdio.h>

#define RED   "\033[0;31m"
#define GREEN "\033[0;32m"
#define BLUE  "\033[0;34m"
#define COLOR_END "\33[0m"

int main() {
   char symbol = 'A';

   printf("%s%c%s\n", RED, symbol, COLOR_END);
   printf("%s%s%s\n", BLUE, "Fear the turtle", COLOR_END);

   return 0;
}
