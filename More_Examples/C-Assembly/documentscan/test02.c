#include <stdio.h>
#include <string.h>
#include "document.h"

int main() {
   Document doc;
   const char *doc_name = "Testing Document";
   int paragraph_number, line_number;

   init_document(&doc, doc_name);

   /* Adding first paragraph */
   paragraph_number = 0;
   add_paragraph_after(&doc, paragraph_number);

   line_number = 0;
   paragraph_number = 1;
   add_line_after(&doc, paragraph_number, line_number, "First Paragraph, First line");
   add_line_after(&doc, paragraph_number, line_number + 1, "First Paragraph, Second line");
   add_line_after(&doc, paragraph_number, line_number + 2, "First Paragraph, Third line");
   add_line_after(&doc, paragraph_number, line_number + 3, "First Paragraph, Fourth line");
   add_line_after(&doc, paragraph_number, line_number + 4, "First Paragraph, Fifth line");

   print_document(&doc);

   line_number = 3;
   remove_line(&doc, paragraph_number, line_number); 

   printf("\nAfter removing line # %d\n", line_number); 
   print_document(&doc);

   return 0;
}
