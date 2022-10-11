#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "photo_album.h"


/* Returns a dynamically-allocated Photo struct initialized based on the 
   provided parameters. In this case, the function dynamically 
   allocates memory to hold description’s string and sets the Photo struct’s 
   description field to point to this allocated memory. If parameter description
   is NULL, no memory allocation takes place and the Photo struct’s description
   field is initialized to NULL. The function returns NULL if a memory 
   allocation fails. You don’t have to worry about freeing memory if any memory
   allocation fails (e.g., one memory allocation is successful, but a second 
   one fails).*/
Photo *create_photo(int id, const char *description) {
  Photo *photo = malloc(sizeof(Photo));
  char *temp;
  
  if (photo == NULL || description == NULL) {
    return NULL;
  } else {
    /*Allocating space for name */
    temp = (char *)malloc(strlen(description) + 1);
    strcpy(temp, description);
  }
  photo->description = temp;
  photo->id = id;

  return photo;
}

/* Prints the photo’s id and description. If the description is NULL, the 
   message description message is ”None”. The function does nothing if the 
   photo parameter is NULL. .*/
void print_photo(Photo *photo) {
  if (photo->description == NULL) {
    printf("Photo Id: %d, Description: None\n", photo->id);
  } else {
    printf("Photo Id: %d, Description: %s\n", photo->id, photo->description);
  }
}

/* Deallocates all dynamically-allocated memory associated with parameter 
   photo. The function does nothing if parameter photo is NULL.*/
void destroy_photo(Photo *photo) {
  if (photo == NULL) {
    /* If the photo is null, do nothing */
  } else {
    if(photo->description != NULL) {
      free(photo->description);
    }
    photo->id = 0;
    free(photo);
  }
}

/* Initializes the album size to 0. Assume this function is not called on an 
   album that has already been initialized. The function does nothing if 
   parameter album is NULL. */
void initialize_album(Album *album) {
  if (album == NULL) {
    /* Do nothing*/
  } else {
    album->size = 0;
  }
}

/* Prints the contents of the album. If the album has no photos, the message 
   ”Album has no photos.” is printed. The function does nothing if parameter 
   album is NULL. */
void print_album(const Album *album) {
  int i;

  if (album == NULL) {
    /* Do nothing*/

    /* If the size is empty print that the album has no photos */
  } else if (album->size == 0) {
    printf("Album has no photos.");
    /* Print photos */
  } else {
    for (i = 0; i < album->size; i++) {
      print_photo(album->all_photos[i]);
    }
  }
}

/* Deallocates all dynamically-allocated memory associated with parameter 
   album and sets the album’s size to 0. (The Album struct itself is not 
   deallocated, so “clear album” may be a better name for this function.) 
   The function does nothing if parameter album is NULL.*/
void destroy_album(Album *album) {
  int i;
  for (i = 0; i < album->size; i++) {
    destroy_photo(album->all_photos[i]);
  }
  album->size = 0;
}

/* Appends (to the end of the array) a photo if there is space (if the album 
   size is less than MAX ALBUM SIZE). photo is added if a photo cannot be 
   created. The function does nothing if parameter album is NULL or the album 
   has no space for the photo. */
void add_photo_to_album(Album *album, int id, const char *description) {
  int album_size = album->size;
  Photo *photo = create_photo(id, description);
  if (album == NULL || album_size >= MAX_ALBUM_SIZE) {
    /* Do nothing */
  } else {
    if (album->size < MAX_ALBUM_SIZE) {
      album->size++;
      album->all_photos[album_size] = photo;
    }
  }
}


