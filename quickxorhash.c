/* This file is part of quickxorhash and distributed under the terms of the
 * MIT license. See COPYING.
 */

#include <config.h>

#include <stdlib.h>
#include <string.h>

#include <openssl/hmac.h>

#include "quickxorhash.h"

struct qxhash {
    size_t    width;
    size_t    shift;
    size_t    shifted;
    uint64_t  len;
    size_t    cell_len;
    uint64_t *cell;
};

qxhash *
qxh_new(void) {
    struct qxhash *ret;

    if ((ret = calloc(sizeof(struct qxhash), 1)) == NULL) {
        return NULL;
    }

    ret->width = 160;
    ret->shift = 11;
    ret->cell_len = ret->width / 64;
    if (ret->width % 64 > 0) {
        ret->cell_len++;
    }

    if ((ret->cell = calloc(sizeof(uint64_t), ret->cell_len)) == NULL) {
        free(ret);
        return NULL;
    }

    return ret;
}

void
qxh_update(qxhash *q, uint8_t *data, size_t len) {
    size_t  i, j, iterations;
    size_t  cell_index, next_cell;
    int     cell_bits, cell_bitpos;
    uint8_t new_byte;

    cell_index = q->shifted / 64;
    cell_bitpos = q->shifted % 64;

    iterations = (len > q->width) ? q->width : len;

    for (i = 0; i < iterations; i++) {
        next_cell = cell_index + 1;
        cell_bits = 64;
        if (next_cell == q->cell_len) {
            next_cell = 0;
            if (q->width % 64 > 0) {
                cell_bits = q->width % 64;
            }
        }

        new_byte = 0x0;
        for (j = i; j < len; j += q->width) {
            new_byte ^= data[ j ];
        }

        q->cell[ cell_index ] ^= (uint64_t)new_byte << cell_bitpos;

        if (cell_bitpos > cell_bits - 8) {
            q->cell[ next_cell ] ^=
                    (uint64_t)new_byte >> (cell_bits - cell_bitpos);
        }

        cell_bitpos += q->shift;
        if (cell_bitpos >= cell_bits) {
            cell_index = next_cell;
            cell_bitpos -= cell_bits;
        }
    }

    q->shifted += q->shift * (len % q->width);
    q->shifted %= q->width;
    q->len += len;
}

char *
qxh_finalize(qxhash *q) {
    size_t   b_data_len;
    uint8_t *b_data = NULL;
    size_t   b_len_len;
    uint8_t *b_len = NULL;
    size_t   i;
    BIO *    bmem = NULL;
    BIO *    b64 = NULL;
    char *   hash = NULL;

    b_data_len = q->width / 8;
    if ((b_data = calloc(1, b_data_len)) == NULL) {
        goto error;
    }
    memcpy(b_data, q->cell, b_data_len);

    b_len_len = sizeof(q->len);
    if ((b_len = calloc(1, b_len_len)) == NULL) {
        goto error;
    }
    memcpy(b_len, &q->len, b_len_len);

    for (i = 0; i < b_len_len; i++) {
        b_data[ b_data_len - b_len_len + i ] ^= b_len[ i ];
    }

    if ((bmem = BIO_new(BIO_s_mem())) == NULL) {
        goto error;
    }
    if ((b64 = BIO_new(BIO_f_base64())) == NULL) {
        goto error;
    }
    b64 = BIO_push(b64, bmem);
    BIO_write(b64, b_data, b_data_len);
    (void)BIO_flush(b64);

    if ((hash = calloc(b_data_len, 2)) != NULL) {
        BIO_read(bmem, hash, b_data_len * 2);
        /* Get rid of the trailing newline */
        hash[ strcspn(hash, "\r\n") ] = '\0';
    }

error:
    if (b64) {
        BIO_free_all(b64);
    } else if (bmem) {
        BIO_free_all(bmem);
    }
    free(b_data);
    free(b_len);

    return hash;
}

void
qxh_free(qxhash *q) {
    if (q) {
        free(q->cell);
        free(q);
    }
}
