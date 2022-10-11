#include <string.h>
#include "document.h"

int main() {
  Document doc;
  const char *doc_name = "Exericise Description";
  int paragraph_number, line_number;

  init_document(&doc, doc_name);
  /* Adding paragraph */
  paragraph_number = 0;
  add_paragraph_after(&doc, paragraph_number);

  printf("Number of Paragraphs: %d\n", paragraph_number);

  /* Adding lines to paragraph */
  paragraph_number = 1;
  printf("Number of Paragraphs: %d\n", paragraph_number);

  /* First line */
  line_number = 0;
  add_line_after(&doc, paragraph_number, line_number, "First Paragraph, First line");
  printf("Number of Lines: %d\n", line_number);


  /* Additional lines */
  add_line_after(&doc, paragraph_number, line_number + 1, "First Paragraph, Second line");
  add_line_after(&doc, paragraph_number, line_number + 2, "First Paragraph, Third line");

  print_document(&doc);
  return 0;
}
