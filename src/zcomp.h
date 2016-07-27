#ifndef ZCOMP_H_
#define ZCOMP_H_

#include <zlib.h>

int zdef(FILE *source, FILE *dest, int level);
int zinf(FILE *source, FILE *dest);
void zerr(int ret);

#endif
