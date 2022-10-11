#if !defined(PHOTOALBUM_H)
#define PHOTOALBUM_H

#define MAX_ALBUM_SIZE 8
#define SUCCESS 0
#define FAILURE -1

typedef struct photo {
   int id;
   char *description; /* dynamically-allocated string */
} Photo;

typedef struct album {
   int size;
   Photo *all_photos[MAX_ALBUM_SIZE];
} Album;

Photo *create_photo(int id, const char *description);
void print_photo(Photo *photo);
void destroy_photo(Photo *photo);
void initialize_album(Album *album);
void print_album(const Album *album);
void destroy_album(Album *album);
void add_photo_to_album(Album *album, int id, const char *description);

#endif
