#include <stddef.h>
void *malloc(size_t);
void *realloc(void*,size_t);
void free(void*);
void abort(void);
void exit(int);
int rand(void);
void srand(unsigned int);
double strtod(const char*,char**);

#define RAND_MAX 2147483647
#define EXIT_FAILURE 1
