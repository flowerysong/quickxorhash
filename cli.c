/* This file is part of quickxorhash and distributed under the terms of the
 * MIT license. See COPYING.
 */

#include <config.h>

#include <stdio.h>

#include "quickxorhash.h"

int
main(int argc, char **argv) {
    FILE *         fp;
    struct qxhash *q;
    uint8_t        buf[ 4096 ];
    size_t         len;

    if (argc != 2) {
        fprintf(stderr, "Usage: quickxorhash <filename>\n");
        return 1;
    }

    fp = fopen(argv[ 1 ], "rb");

    if (!fp) {
        fprintf(stderr, "Failed to open %s\n", argv[ 1 ]);
        return 1;
    }

    q = qxh_new();

    while ((len = fread(buf, 1, 4096, fp)) > 0) {
        qxh_update(q, buf, len);
    }

    printf("%s\n", qxh_finalize(q));

    qxh_free(q);
    return 0;
}
