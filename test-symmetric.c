/*
 *
 * Copyright (c) 2011 - 2015
 *   University of Houston System and UT-Battelle, LLC.
 * Copyright (c) 2009 - 2015
 *   Silicon Graphics International Corp.  SHMEM is copyrighted
 *   by Silicon Graphics International Corp. (SGI) The OpenSHMEM API
 *   (shmem) is released by Open Source Software Solutions, Inc., under an
 *   agreement with Silicon Graphics International Corp. (SGI).
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * o Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * o Neither the name of the University of Houston System, 
 *   UT-Battelle, LLC. nor the names of its contributors may be used to
 *   endorse or promote products derived from this software without specific
 *   prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#define N 10

#include <stdio.h>
#include <shmem.h>

int targ_g[N];
long lsrc_g[N];
int src_g[N];

int main(void)
{
    int i, src[N], targ[N];
    long ltarg[N];
    static int starg[N];
    int nbr, lnbr;
    int my_pe, npes;

    start_pes(0);

    my_pe = shmem_my_pe();
    npes = shmem_n_pes();

    nbr = (my_pe + 1) % npes;
    lnbr = (my_pe - 1) % npes;

    for(i=0; i< N; i++) {
        src[i] = my_pe + i;
        lsrc_g[i] = my_pe + i*i;
    }

    /* non-symmetric variable */
    shmem_int_put(targ, src, N, nbr);

    shmem_int_put(targ_g, src_g, N, nbr);

    shmem_int_put(starg, src, N, nbr);

    shmem_long_get(ltarg, lsrc_g, N, nbr);

    shmem_barrier_all();  /* sync sender and receiver */

    if (my_pe == 0) {
        printf("targ[0] = %d (should be %d)\n", targ[0], lnbr);
        printf("targ_g[0] = %d (should be %d)\n", targ_g[0], lnbr);
        printf("starg[0] = %d (should be %d)\n", starg[0], lnbr);
        printf("ltarg[0] = %d (should be %d)\n", ltarg[0], lnbr);
    }

    return 0;
}

