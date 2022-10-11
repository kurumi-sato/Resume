#include <stdio.h>

/* Print the prompt and scan the option of which shape*/
int prompt() {
  int value;
  printf("Enter 1(rectangle), 2(triangle), 3(other), 0(quit): ");
  scanf("%d", &value);
  return value;
}

/* Return true or false whether the character is valid*/
int valid_character(char character) {
  /* if input is a not a valid character,  return false true*/
  if (character == '*' || character == '%' || character == '#') {
    return 1;

  /* If it is not a valid character, print invalid return false */
  } else {
    printf("Invalid data provided.\n");
    return 0;
  }
}

/* Return true or false whether the character is valid*/
int valid_number(int number) {

  /* if a valid number, return true */
  if (number > 0){
    return 1;

    /* if not a valid number, print invalid and return false */
  } else { 
    printf("Invalid data provided.\n");
    return 0;
  }
}

/* Prints rectangle given by character, width and length*/
int draw_rectangle(char character, int width, int length) {
  int row, column;

  /* If any of the inputs are invalid, return false*/
  if (valid_character(character) && valid_number(width) && valid_number(length)) {
  
    /* Draw the rectangle*/
    for (row = 0; row < width; row++) {
      for (column = 0; column < length; column++) {
	printf("%c", character);
      }
      printf("\n");
    }
    return 1;

  } else {
    return 0;
  }
}

/* Prints the triangle given the character and size */
int draw_triangle(char character, int size) {

  int i,j,k;

   /* If inputs are invalid, return 0 */
  if (!valid_character(character) || size <= 0) {
    return 0;
  }

  /*print the triangle */
  for (i = 1; i <= size; i++) {
    /* print the space */
    for (j = 1; j <= size - i; j++) {
      printf(" ");
    }
    /* print the character */
    for (k = 1; k <= (i*2) - 1; k++) {
      printf("%c", character);
    }
    printf("\n");
  }
  return 1;
}


int main () {
  
  int value = 1;

  /* while the inputted value is not 0(quit) */
  while (value != 0) {
    /* Prints the prompt and scans the option */
    value = prompt();
    
    /* If the input value is 0(quit)*/
    if (value == 0) {
      /* Do not print anything*/

      /* If value is one, draw rectangle*/
    } else if (value == 1) {
      char character;
      int width, length;

      /* Intake information needed to make rectangle */
      printf("Enter character, width and length: ");
      scanf(" %c %d %d", &character, &width, &length);
      
      /* Draw the rectangle*/
      draw_rectangle(character, width, length);

      /* If value is two, draw triangle*/
    } else if (value == 2) {
      char character;
      int size;
      
      printf("Enter character and size: ");
      scanf(" %c %d", &character, &size);

      draw_triangle(character, size);

      /* If value is three, do nothing*/
    } else if (value == 3) {
      printf("Do nothing\n");

      /* If the value is anything but 0, 1, 2, 3 print invalid choice*/
    } else if (value != 1 && value != 2 && value !=3 && value != 0) {
      printf("Invalid choice.\n");
    } else {
      printf("what is happening");
    }
  }  
  printf("Bye Bye.\n");
  return 0;
}
