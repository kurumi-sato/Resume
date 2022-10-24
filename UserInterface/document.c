#include <stdio.h>
#include <string.h>
#include "document.h"

int load_file (Document *doc, const char *filename) {
  FILE *file;
  char line[MAX_STR_SIZE + 1], line1[MAX_STR_SIZE + 1];
  int paragraph_number = 1;
 
  if (doc != NULL || filename != NULL) {
    file = fopen(filename, "r");
    if (file != NULL) {
      add_paragraph_after(doc, 0);

      /* As long as it is not the end of the file */
      while(fgets(line, MAX_STR_SIZE + 1, file)) {
	line[strlen(line) - 1] = '\0';
	sscanf(line, "%s", line1);
	/* If the string is empty, add a paragraph */
	if (strcmp(line1, "") == 0) {
	  add_paragraph_after(doc, paragraph_number);
	  paragraph_number++;
	} else {
	  append_line(doc, paragraph_number, line);
	}
      }
    }
    fclose(file);
    return SUCCESS;
  } else {
    return FAILURE;
  }
}

int save_document(Document *doc, const char *filename) {
  FILE *file;
  int curr_paragraph, curr_line;
  file = fopen(filename, "w");
  if (doc != NULL || filename != NULL || file != NULL) {
    if(doc->number_of_paragraphs == 0) {
      printf("number of paragraphs is null");
    }
    printf("Document name: \"%s\"\n", doc->name);
    printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);
    /* Iterate through every paragraph*/
    for (curr_paragraph = 0; curr_paragraph < doc->number_of_paragraphs;
	 curr_paragraph++) {
      /* Iterate through each line*/
      for (curr_line = 0; curr_line < 
	     doc->paragraphs[curr_paragraph].number_of_lines; curr_line++) {
	/* Print current line*/
	printf("%s\n", doc->paragraphs[curr_paragraph].lines[curr_line]);
      }
      /* Before the new paragraph, add an extra blank line */
      if (curr_paragraph < doc->number_of_paragraphs - 1) {
	printf("\n");
      }
    }
  } else {
    return FAILURE;
  }
  fclose(file);
  return SUCCESS;
}

/* Initializes *doc to have 0 paragraphs and sets the document?s name based on 
   the provided parameter value. The function returns FAILURE if doc is NULL, 
   name is NULL, or if the length of the name is not between 1 and 
   MAX_STR_SIZE; otherwise the function returns SUCCESS. */
int init_document(Document *doc, const char *name) {
  if (doc == NULL || name == NULL || strlen(name) < 1 || 
      strlen(name) > MAX_STR_SIZE) {
    return FAILURE;
  } else {
    doc->number_of_paragraphs = 0;
    strcpy(doc->name, name);
    return SUCCESS;
  }
}

/* Sets the number of paragraphs to 0. The function returns FAILURE if doc is 
   NULL; otherwise the function returns SUCCESS */
int reset_document(Document *doc) {
  if (doc == NULL) {
    return FAILURE;
  } else {
    doc->number_of_paragraphs = 0;
    return SUCCESS;
  }
}

/* Prints the document?s name, number of paragraphs, followed by the paragraphs
   Exactly one empty line (with zero characters before the newline) is inserted 
   between every two successive paragraphs. The function returns FAILURE if doc 
   is NULL; otherwise the function returns SUCCESS..*/
int print_document(Document *doc) {
  int curr_paragraph, curr_line;
  if (doc != NULL) {
    printf("Document name: \"%s\"\n", doc->name);
    printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);

    /* Iterate through every paragraph*/
    for (curr_paragraph = 0; curr_paragraph < doc->number_of_paragraphs; 
     curr_paragraph++) {
      /* Iterate through each line*/
      for (curr_line = 0; curr_line < 
         doc->paragraphs[curr_paragraph].number_of_lines; curr_line++) {
    /* Print current line*/
    printf("%s\n", doc->paragraphs[curr_paragraph].lines[curr_line]);
      }
      /* Before the new paragraph, add an extra blank line */
      if (curr_paragraph < doc->number_of_paragraphs - 1) {
    printf("\n");
      }
    }
    return SUCCESS;
  } else {
    return FAILURE;
  }
}

/* Adds a paragraph after the specified paragraph number. Paragraph numbers in 
   the document start at 1. If paragraph number is 0 the paragraph is added at 
   the beginning of the document. The function returns FAILURE if doc is NULL, 
   the document already has the maximum number of paragraphs (MAX PARAGRAPHS), 
   or if paragraph number is negative or higher than the number of paragraphs 
   in the document; otherwise, the function returns SUCCESS. */
int add_paragraph_after(Document *doc, int paragraph_number) {
  int i;
  if (doc == NULL || doc->number_of_paragraphs == MAX_PARAGRAPHS || 
      paragraph_number < 0 || paragraph_number > doc->number_of_paragraphs) {
    return FAILURE;
  } else {
    /* If the paragraph number is zero, the paragraph is added at the 
       beginning */
    if (paragraph_number == 0) {
      doc->paragraphs[0].number_of_lines = 0;
      /* If the paragraph number is the same as the number of paragrahps it is 
         added to the end */
    } else if (paragraph_number == doc->number_of_paragraphs) {
      doc->paragraphs[paragraph_number].number_of_lines = 0;
      /* If the paragraph number is in the middle */
    } else {
      /* Go through the paragraphs to copy the paragraph */
      for (i = 0; i < doc->number_of_paragraphs; i++) {
	doc->paragraphs[i + 1] = doc->paragraphs[i];
      }
      doc->paragraphs[paragraph_number].number_of_lines = 0;
    }
    /* Increment the number of paragraphs by one then return success */
    (doc->number_of_paragraphs)++;
    return SUCCESS;
  }
}

/* Adds a new line after the line with the specified line number. Line numbers 
   start at 1. If line number is 0, the new line is added at the beginning of 
   the paragraph. The function returns FAILURE if doc is NULL, paragraph number
   does not refer to an existing paragraph, the paragraph already has the 
   maximum number of lines allowed, line number is negative or higher than the 
   available number of lines, or new line is NULL; otherwise, the function 
   returns SUCCESS */
int add_line_after(Document *doc, int paragraph_number, int line_number, 
		   const char *new_line) {
  int i, number_of_lines;
  number_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;

  if (doc != NULL && doc->number_of_paragraphs <= MAX_PARAGRAPHS && 
        number_of_lines < MAX_PARAGRAPH_LINES && line_number >= 0 && 
        line_number <= number_of_lines && new_line != NULL && 
        paragraph_number <= doc->number_of_paragraphs) {
    /* If the number of lines is not zero, and inserted line is at the 
       beginning */
    /* Or if the inserted line needs to be at the end */
    if (number_of_lines == line_number || (number_of_lines != 0 && line_number 
					   == 0)) {
      strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number], 
	       new_line);
    /* If the paragraph number is in the middle */
    } else {
	/* Go through lines to copy following line by starting at the 
	   inserted line */
	for (i = line_number; i < number_of_lines; i++) {
	  strcpy(doc->paragraphs[paragraph_number - 1]. lines[i+1], 
		 doc->paragraphs[paragraph_number - 1].lines[i]);
	}
	/* Insert the new line*/
	strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number], 
	       new_line);
    }
    /* Increment the number of paragraphs by one then return success */
    (doc->paragraphs[paragraph_number - 1].number_of_lines)++;
      return SUCCESS;
  } else {
    return FAILURE;
  }
}

/* Returns the number of lines in a paragraph using the number of lines out 
   parameter. The function returns FAILURE if doc or number of lines is NULL or 
   if paragraph number does not refer to an existing paragraph; otherwise, the 
   function returns SUCCESS. */
int get_number_lines_paragraph(Document *doc, int paragraph_number, 
			       int *number_of_lines) {
  int lines;
  /* If the doc is not NULL and the paragraph number exists return SUCCESS */
  if (doc != NULL && paragraph_number > doc->number_of_paragraphs) {
    /* Set the pointer to the number of lines */
    lines = doc->paragraphs[paragraph_number].number_of_lines;
    *number_of_lines = lines;
    return SUCCESS;
  } else {
    return FAILURE;
  }
}

/* Appends a new line to the specified paragraph. The conditions that make add 
   line after fail apply to this function as well. The function returns SUCCESS
   if the line is appended. Assume that a non-NULL new line is a nul-terminated
   string of length between 1 and MAX_STR_SIZE. */
int append_line(Document *doc, int paragraph_number, const char *new_line) {
  int number_of_lines;
  number_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;
  /* As long as the doc and the new line is not NULL, return SUCCESS */
  if (doc != NULL && new_line != NULL) {
    /* Add the line after*/
    add_line_after(doc, paragraph_number, number_of_lines, new_line);
    return SUCCESS;
    /* Otherwise, return FAILURE */
  } else {
    return FAILURE;
  }
}

/* Removes the specified line from the paragraph. The function returns FAILURE 
   if doc is NULL, paragraph number does not refer to an existing paragraph, or
   line number does not refer to an existing line; otherwise the function 
   returns SUCCESS. */
int remove_line(Document *doc, int paragraph_number, int line_number) {
  int i, j;
  int number_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;
  if (doc == NULL || paragraph_number > doc->number_of_paragraphs || 
      line_number > number_of_lines) {
    return FAILURE;
    /* Otherwise, line number is removed */
  } else {
    /* Iterate through every paragraph */
    for (i = 0; i < doc->number_of_paragraphs; i++) {
      /* Iterate through every line */
      for (j = line_number; j < doc->paragraphs[i].number_of_lines; j++) {
	/* Copy through next line to remove one*/
	strcpy(doc->paragraphs[i].lines[j - 1], doc->paragraphs[i].lines[j]);
      }
    }
    (doc->paragraphs[paragraph_number - 1].number_of_lines)--;
    return SUCCESS;
  }
}

/* Adds the first data lines number of rows from the data array to the document,
   starting a new paragraph at the beginning of the document. A row with an 
   empty string starts a new paragraph.  */
int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], 
		  int data_lines) {
  int i;
  /* Returns FAILURE if doc is NULL, data is NULL or data lines is 0. */
  if (doc == NULL || data == NULL || data_lines == 0) {
    
    /* If adding data lines number of rows would violate the max number of 
       allowed paragraphs or the max number of lines in a paragraph; in this 
       case, either leave the document unchanged or grow the document to the 
       maximum allowed this is done during the two functions append and add */

    return FAILURE;
    /* Otherwise, go through function and return SUCCESS*/
  } else {
    /* Starting a new paragraph at the beginning of the document */
    add_paragraph_after(doc, 0);
    /* Go through all of the data lines*/
    for (i = 0; i < data_lines; i++) {
      /* A row with an empty string starts a new paragraph. Otherwise, add the 
	 line*/
      if (strcmp(data[i], "") == 0) {
        add_paragraph_after(doc, doc->number_of_paragraphs);
      } else {
        append_line(doc, doc->number_of_paragraphs, data[i]);
      }
    }
    return SUCCESS;
  }
}

/* Replaces every appearance of the text target in the document with the text 
   replacement. Assume that the target is not the empty string. Assume that 
   the replacement does not generate a line that exceeds the maximum allowed 
   line length. */
int replace_text(Document *doc, const char *target, const char *replacement) {
  int i, j, characters;
  char *curr_line, temp[MAX_STR_SIZE + 1];

  characters = strlen(replacement);

  /* The function returns FAILURE if doc, target or replacement is NULL */
  if (doc == NULL || target == NULL || replacement == NULL) {
    return FAILURE;
    /* Otherwise the function returns SUCCESS */
  } else {
    /* First occurance of the target */
    curr_line = strstr(doc->paragraphs[0].lines[0], target);
    /* Go through every paragraph */
    for (i = 0; i < doc->number_of_paragraphs; i++) {
      /* Go through every line */
      for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
	/* Start at the new line */
	curr_line = strstr(doc->paragraphs[i].lines[j], target);
	/* As long as the string does not contain the null byte */
	while (curr_line != '\0') {
	  strcpy(temp, curr_line);
	  strcpy(curr_line, replacement);
	  strcat(curr_line + characters, temp + strlen(target));
	  /* Move current to the next instance of target */
	  curr_line = strstr(curr_line + strlen(replacement), target);
	}
      }
    }
    return SUCCESS;
  }
}

/* Highlights every appearance of the text target in the document, by 
   surrounding the text with the strings HIGHLIGHT START STR and HIGHLIGHT END 
   STR. */
int highlight_text(Document *doc, const char *target) {
  char highlighted_text[MAX_STR_SIZE + 1];
  /* If the document or target is NULL return FAILURE */
  if (doc == NULL || target == NULL) {
    return FAILURE;
    /* Otherwise return success */
  } else {
    /* Crete highlighted text*/
    strcat(highlighted_text, HIGHLIGHT_START_STR);
    strcat(highlighted_text, target);
    strcat(highlighted_text, HIGHLIGHT_END_STR);

    /* Replace the target string with the highlighted one*/
    replace_text(doc, target, highlighted_text);
    return SUCCESS;
  }
}

/* Removes every appearance of the text target in the document. Assume the 
   target is not the empty string */
int remove_text(Document *doc, const char *target) {
  if (doc == NULL || target == NULL) {
    return FAILURE;
  } else {
    replace_text(doc, target, "");
    return SUCCESS;
  }
}