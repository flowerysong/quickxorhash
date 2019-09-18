#ifndef QUICKXORHASH_H
#define QUICKXORHASH_H

#include <stdint.h>

typedef struct qxhash qxhash;

qxhash *qxh_new(void);
void    qxh_update(qxhash *, uint8_t *, size_t);
char *  qxh_finalize(qxhash *);
void    qxh_free(qxhash *);

#endif /* QUICKXORHASH_H */
