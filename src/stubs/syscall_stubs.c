/* Copyright (c) 2011, Joakim Östlund
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * The name of the author may not be used to endorse or promote products
 *      derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* This file contains temporary stubs for newlib */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>

#undef errno
extern int errno;

extern char stack_ptr;

/* Used by abort() */
void _exit()
{
}

/* Used by malloc() */
char *heap_end = 0;

caddr_t sbrk(int incr)
{
     extern char heap_low; /* Defined by the linker */
     extern char heap_top; /* Defined by the linker */
     char *prev_heap_end;
	 
     if(heap_end == 0)
     {
	  heap_end = &heap_low;
     }

     prev_heap_end = heap_end;
	 
     if (heap_end + incr > &heap_top)
     {
	  /* Heap and stack collision */
	  return (caddr_t)0;
     }
     
     heap_end += incr;
     return (caddr_t) prev_heap_end;
}

int kill(int pid, int sig)
{
     errno = EINVAL;
     return -1;
}

int getpid(void)
{
     return 1;
}

int close(int file)
{
     return -1;
}

int fstat(int file, struct stat *st)
{
     st->st_mode = S_IFCHR;
     return 0;
}

int isatty(int file)
{
     return 1;
}

int lseek(int file, int ptr, int dir)
{
     return 0;
}

int read(int file, char *ptr, int len)
{
     return 0;
}
