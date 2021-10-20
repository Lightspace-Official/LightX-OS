/*
 * (c) Copyright 2012-2016 by Einar Saukas. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * The name of its author may not be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "zx7.h"

int main(int argc, char *argv[]) {
    unsigned char *input_data;
    unsigned char *output_data;
    FILE *ifp;
    FILE *ofp;
    size_t size;
    size_t input_size;
    size_t output_size;
    size_t partial_counter;
    size_t total_counter;
    long delta;
    int i, segments, ptr;

    if(argc != 3) {
        fprintf(stderr, "Usage: %s input output\n", argv[0]);

        exit(1);
    }

    /* open input file */
    ifp = fopen(argv[1], "rb");
    if (!ifp) {
        fprintf(stderr, "Error: Cannot access input file %s\n", argv[1]);
        exit(1);
    }

    /* determine input size */
    fseek(ifp, 0L, SEEK_END);
    input_size = ftell(ifp);
    fseek(ifp, 0L, SEEK_SET);
    if (!input_size) {
        fprintf(stderr, "Error: Empty input file %s\n", argv[1]);
        exit(1);
    }

    /* allocate input buffer */
    input_data = (unsigned char *)malloc(input_size);
    if (!input_data) {
        fprintf(stderr, "Error: Insufficient memory\n");
        exit(1);
    }

    /* read input file */
    total_counter = 0;
    do {
        partial_counter = fread(input_data+total_counter, sizeof(char), input_size-total_counter, ifp);
        total_counter += partial_counter;
    } while (partial_counter > 0);

    if (total_counter != input_size) {
        fprintf(stderr, "Error: Cannot read input file %s\n", argv[1]);
        exit(1);
    }

    /* close input file */
    fclose(ifp);

    /* create output file */
    ofp = fopen(argv[2], "wb");
    if (!ofp) {
        fprintf(stderr, "Error: Cannot create output file %s\n", argv[2]);
        exit(1);
    }

    /* generate output file */
    segments = ((input_size - 1) >> 16) + 1;
    fputc(segments, ofp);

    ptr = segments * 2 + 1;

    for(i = 0; i < segments; i++) {
        size = (input_size > 0x10000) ? 0x10000 : input_size;

        output_data = compress(optimize(input_data + (i << 16), size), input_data + (i << 16), size, &output_size, &delta);
        input_size -= 0x10000;

        fseek(ofp, ptr, SEEK_SET);

        /* write output file */
        if (fwrite(output_data, sizeof(char), output_size, ofp) != output_size) {
            fprintf(stderr, "Error: Cannot write output file %s\n", argv[2]);
            exit(1);
        }

        fseek(ofp, i * 2 + 1, SEEK_SET);
        fputc(ptr & 0xFF, ofp);
        fputc(ptr >> 8, ofp);

        ptr += output_size;
    }

    /* close output file */
    fclose(ofp);
}
