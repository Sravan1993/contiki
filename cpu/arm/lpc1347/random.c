/**
 * \addtogroup mbxxx-platform
 *
 * @{
 */

/*
 * Copyright (c) 2013, Christian Taedcke <hacking@taedcke.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
* \file
*			Random number functions for LPC1347.
* \author
*			Christian Taedcke <hacking@taedcke.com>
*/

#include "lib/random.h"

#if (RANDOM_RAND_MAX != 0xffff)
#warning "RANDOM_RAND_MAX is not defined as 65535."
#endif

static unsigned short prev = 0;

/*--------------------------------------------------------------------------*/
unsigned short
random_rand(void)
{
    //see  Knuth, The Art of Computer Programming Vol. 2, Semi-numerical
    //Algorithms, Chapter 3 – Random Numbers
    prev = (16644525 * prev) + 32767;
    return prev;
}
/*--------------------------------------------------------------------------*/
void
random_init(unsigned short seed)
{
    prev = seed;
}
/** @} */
