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

#include <shmem.h>
#include <stdlib.h>
int n1=101;
float *x, *y;

void variable_allocation(int n) {
    int nn = (n-1) / _num_pes();
    int n_local0 = 1 + _my_pe() * nn;
    int n_local1 = 1 + (_my_pe()+1) * nn;
  
    //  y = (float*) shmalloc((n_local1 - n_local0 + 2)*sizeof(float));  // forgot to initialize pointer

}
int main(int argc, char *argv[]) {
    int n = 100;
    // int n1=101;
    start_pes(0);
    int nn = (n-1) / _num_pes();
    int n_local0 = 1 + _my_pe() * nn;
    int n_local1 = 1 + (_my_pe()+1) * nn;
    // allocate only local part + ghost zone of the arrays x,y
 
    variable_allocation(n);
  
    x = (float*) malloc((n_local1 - n_local0 + 2)*sizeof(float));

    shmem_barrier_all();

    //... // fill x, y

    // fill ghost zone
    if (_my_pe() > 0)
	shmem_float_get(x,y,n1,1); // extra code
    shmem_float_put(y,x, 1, _my_pe()-1);

    shmem_barrier_all();

    // do computation
    float e = 0;
    int i;
    for (i=n_local0; i<n_local1; ++i) {
	x[i] += ( y[i+1] + y[i-1] )*.5;
	e += y[i] * y[i];
    }

    static float work[_SHMEM_REDUCE_SYNC_SIZE];
    static long sync[_SHMEM_REDUCE_SYNC_SIZE];
    static float el, es;
    el = e;
    shmem_float_sum_to_all(&es, &el, 1,
			   0, 0, _num_pes(), work, sync);
    e = es;

    // ... // output x, e

    shfree(x);
    shfree(y);
    return 0;
}
