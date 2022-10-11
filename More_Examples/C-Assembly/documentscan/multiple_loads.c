#include <stdio.h>
#include <string.h>
#include "document.h"

int main() {
   Document doc;
   const char *doc_name = "Loading Document)";
   int data_lines = 8;
   char data[20][MAX_STR_SIZE + 1] = {
        "The first course you need to take",
    "is cmsc131.  This course will be",
    "followed by cmsc132 (which is also based on Java).",
    "",
    "The next course you will take is cmsc216.",
    "This course relies on C.",
    "",
    "Ruby and Ocaml will be covered in cmsc330"
   };
   char input2[10][MAX_STR_SIZE + 1] = {
     "hello",
     "this is the second data"
   };

   init_document(&doc, doc_name);
   load_document(&doc, data, data_lines);
   print_document(&doc);
   load_document(&doc, input2, 2);
   print_document(&doc);
   return 0;
}
