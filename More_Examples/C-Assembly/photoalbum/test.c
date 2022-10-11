#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "photo_album.h"
#include "my_memory_checker_216.h"

int main() {
  Album album, fire;
  start_memory_check(); /* Start memory check */
  initialize_album(&album);
  initialize_album(&fire);
  add_photo_to_album(&album, 1, "In the park"); 
  add_photo_to_album(&album, 2, "At home"); 
  add_photo_to_album(&album, 3, "hi");
  add_photo_to_album(&album, 4, "hello");
  add_photo_to_album(&album, 5, "hello");
  add_photo_to_album(&album, 6, "hello");
  add_photo_to_album(&album, 7, "hello");
  add_photo_to_album(&album, 8, "hello");
  add_photo_to_album(&album, 9, "hello");

  print_album(&album);
  destroy_album(&album);
  stop_memory_check(); /* End memory check */
  return EXIT_SUCCESS;
}
