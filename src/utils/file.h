#ifndef FILE_H
#define FILE_H

struct image;

// todo: add sensor/image data param
// todo: better function name
int file_write(struct image *image);

#endif