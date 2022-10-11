#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include <stdlib.h>
#include <ctype.h>
#include "document.h"

#define MAX_CHARACTERS 1024


static int run_command(Document *doc, char *line) {
  char target[MAX_CHARACTERS + 1], replacement[MAX_CHARACTERS + 1], 
    extra[MAX_CHARACTERS + 1], filename[MAX_CHARACTERS + 1];
  char command[MAX_CHARACTERS + 1], line1[MAX_CHARACTERS + 1];
  /* Value represents number of arguments in the line */
  int value, paragraph_number, line_number;
  /* Sequential order of quotation for replace_text */
  char *first, *second;

  /* Scan the first string in the line and set is as the command */
  sscanf(line, " %s", command);

  /* Add paragraph after if user inputs add_paragraph_after */
  if (strcmp(command, "add_paragraph_after") == 0) {
  value = sscanf(line, "%s%d%s", command, &paragraph_number, extra);
    /* Make sure the paragraph has the correct data */
    if (paragraph_number >= 0 && value == 2) {
      if (add_paragraph_after(doc, paragraph_number) == FAILURE) {
	printf("add_paragraph_after failed\n");
      }
      /* If not, it is an invalid command */
    } else {
      printf("Invalid Command \n");
    }

      /* Add line after if user inputs add_line_after */
  } else if (strcmp(command, "add_line_after") == 0) {

    value = sscanf(line, " %s %d %d %s", command, &paragraph_number, 
           &line_number, line1);
    /* If the paragraph number and line number is positive. Input needs to 
       have four arguments and must contain '*' */
    if (paragraph_number >= 0 && line_number >= 0 && value == 4 && 
	strchr(line, '*') != NULL) {
      strcpy(line1, strchr(line, '*') + 1);
      if(add_line_after(doc, paragraph_number, line_number, line1) 
	 == FAILURE) {
	printf("add_line_after failed\n");
      }
      /* Otherwise, print invalid command */
    } else {
      printf("Invalid Command \n"); 
    }

    /* Print the document if user inputs print_document */
  } else if (strcmp(command, "print_document") == 0) {

    value = sscanf(line, " %s %s", command, extra);
    /* If nothing comes after the command*/
    if (value == 1) {
      print_document(doc);
    } else {
      printf("Invalid Command\n");
    }
    /* Exit the document if user inputs quit or exit */
  } else if (strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0) {

    value = sscanf(line, " %s %s", command, extra);
    /* If no data appears after quit or exit, the document is printed */
    if (value != 1) {
      printf("Invalid Command\n");
    } else {
      return SUCCESS;
    }

    /* Append line if user inputs append_line */
  } else if (strcmp(command, "append_line") == 0) {
    value = sscanf(line, " %s%d%s", command, &paragraph_number, line1);
      /* If there aren't zero or negative paragraphs, if the line doesn't
       contain a '*' or if the value isn't 3, print invalid command */

    if (!(paragraph_number > 0) || value != 3 || strchr(line, '*') == NULL) {
      printf("Invalid Command\n");
      
      /* Otherwise copy the statement after '*' and append it to the line */
    } else {
      /* Copy part after * onto line1*/
      strcpy(line1, strchr(line, '*') + 1);
      if (append_line(doc, paragraph_number, line1) == FAILURE) {
	printf("append_line failed\n");
      }
    }

    /* Remove line if user inputs remove_line*/
  } else if (strcmp(command, "remove_line") == 0) {

    value = sscanf(line, " %s%d%d%s", command, &paragraph_number, 
           &line_number, extra);

    if (value != 3 || paragraph_number <= 0 || line_number <=0) {
      printf("Invalid Command\n");
    } else {
      if (remove_line(doc, paragraph_number, line_number) == FAILURE) {
    printf("remove_line failed\n");
      }
    }
      
    /* Load file if user inputs load_file */
  } else if(strcmp(command, "load_file") == 0) {
      value = sscanf(line, " %s %s %s", command, filename, extra);
    if (value != 2) {
      printf("Invalid Command\n");
    } else {
      if (load_file(doc, filename) == FAILURE) {
      }
    }
      
    /* Replace text if user inputs replace text */
  } else if (strcmp(command, "replace_text") == 0) {
    value = sscanf(line, " %s %s %s %s", command, target, replacement, extra);
    printf("%s\n", target);


    /*  strncpy(target, first + 1, second - first);
     target[second - (first + 1)] = '\0';
      strncpy(replacement, third + 1, fourth - third);
      replacement[fourth - (third + 1)] = '\0'; */

    if (value !=3) {
      printf("Invalid Command\n");
    } else {
      if(replace_text(doc, target, replacement) == FAILURE) {
    printf("replace_text failed\n");
      }
    }

    /* Highlight the text if user inputs highlight text*/
  } else if (strcmp(command, "highlight_text") == 0) {
    value = sscanf(line, " %s %s", command, line);

    first = strstr(line, "\"");
    second = strstr(line, "\"");

    if (first != NULL && value == 2) {
      strncpy(line, first, second-first);

      if (highlight_text(doc, first) == FAILURE) {
	printf("highlight_text failed\n");
      }
    } else {
      printf("Invalid Command\n");      
    }    

    /* Remove text if the user inputs remove_text */
  } else if (strcmp(command, "remove_text") == 0) {
    value = sscanf(line, " %s %s", command, line);
    first = strstr(line, "\"");
    second = strstr(first + 1, "\"");
    if (first != NULL && second != NULL && value == 2) {
      if(replace_text(doc, target, replacement) == FAILURE) {
    printf("replace_text failed\n");
      }
    } else {
      printf("Invalid Command\n");
    }

    /* Save document if the user inputs save document */
 } else if (strcmp(command, "save_document") == 0) {
    value = sscanf(line, " %s %s %s", command, filename, extra);
    if (value != 2) {
      printf("Invalid Command\n");
    } else {
      if (save_document(doc, filename) == FAILURE ) {
    printf("save_document failed\n");
      }
    }

    /* Reset the document if the user inputs reset document */
  } else if (strcmp(command, "reset_document") == 0) {
    value = sscanf(line, " %s", command);
    if (value != 1) {
      printf("Invalid Command");
    } else {
      if (reset_document(doc) == FAILURE) {
        printf("reset_document failed\n");
      }
    }

    /* If none of the commands */
  } else {
    printf("%s\n", command);
    printf("Invalid Command final \n");
  }
  return SUCCESS;
}

int main(int argc, char *argv[]) {
  Document doc;
  char command[MAX_CHARACTERS + 1], character, line[MAX_CHARACTERS + 1];
    FILE *file;

  /* Initalize main document*/
  init_document(&doc, "main_document");

  /* If there are no arguments, use standard input*/
  if (argc == 1) {
    file = stdin;
    fgets(line, MAX_CHARACTERS + 1, stdin);
  

    /* Scan input until the command quit or exit is inputted*/
    while (!feof(file)) {
      printf("> ");
      sscanf(command, "%c%s", &character, line);
      
      if (character != '#' && *command != '#') {
	/* If the code was saying quit or exit file is closed and program
	   ends*/
	if (run_command(&doc, command)) {
	  fclose(file);
	  exit(EXIT_SUCCESS);
	}
      }

      command[strlen(command) - 1] = '\0'; /* Remove previous line */
      /* Get input char *fgets(char *str, int n, FILE *stream) */
      fgets(command, MAX_CHARACTERS + 1, stdin);
    }
  }

/* If there are too many arguments, exit with EX_USAGE */
  if (argc > 2) {
    fprintf(stderr, "Usage: user_interface\n"); 
    fprintf(stderr, "Usage: user_interface <filename>\n");
    exit(EX_USAGE);
  }

  /* If there there is only one argument, process the input */
  if (argc == 2) {
    file = fopen(argv[1], "r");

    /* If the file is unable to be opened, use EX_OSERR exit code */
    if (file == NULL) {
      fprintf(stderr, " %s cannot be opened.\n", argv[1]);
      exit(EX_OSERR);
    }
    /*Go through file and run it */
    /* fgets(char *str, int n, FILE *stream);*/
    /* While the current line still has strings */
    while (fgets(command, MAX_CHARACTERS + 1, file)) {

      sscanf(command, " %c %s", &character, line);
      strtok(command, "\n");

      if (character == '#' || strcmp(command, "\n") == 0) {
	/* Do nothing since its a highlight or a blank space*/
      } else {
	/*FAILURE means quit or exit was called*/
	if (run_command(&doc, command)) {
	  break; /* Get out of loop to end*/
	}
      }
    } /* Exit while loop */
  }
  fclose(file);
  exit(EXIT_SUCCESS);
}