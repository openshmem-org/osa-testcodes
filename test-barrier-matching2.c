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

#include <stdio.h>
#include <shmem.h>

long pSync[_SHMEM_REDUCE_SYNC_SIZE];
int pWrk[_SHMEM_REDUCE_SYNC_SIZE];
int x, y, old, i, temp;
 
int main(){
    int me, npes;
    start_pes(0);
    me = shmem_my_pe();
    npes = shmem_n_pes();
    for (i = 0; i < _SHMEM_REDUCE_SYNC_SIZE; i += 1)
	{
	    pSync[i] = _SHMEM_SYNC_VALUE;
	}
    x = 42; 
    y = 0;
    if(me==0){
	shmem_barrier_all();
	temp = x+y;
    }
    else { 
	shmem_barrier_all();
	if(me==1){
	    old = shmem_int_finc (&y, 0);
	    shmem_int_sum_to_all(&y,&x,1,1,0,npes-1,pWrk,pSync);
	    x= x+10;
	    shmem_int_get(&y,&y,1,0);
	}
	else{
	    shmem_int_sum_to_all(&y,&x, 1,1,0,npes-1,pWrk,pSync);
	    x=y*0.23;
	}
    }
    shmem_barrier_all();
    if (me == 0) {
	printf("value in temp is %d (should be 42)\n", temp);
    }
  
    return 0;
}
