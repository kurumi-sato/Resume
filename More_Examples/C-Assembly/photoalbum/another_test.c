#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "photo_album.h"
#include "my_memory_checker_216.h"

int main() {
   Photo *photo;

   start_memory_check(); /* Start memory check */


   photo = create_photo(1, NULL);
   print_photo(photo);
   destroy_photo(photo);


   stop_memory_check(); /* End memory check */

   return EXIT_SUCCESS;
}
