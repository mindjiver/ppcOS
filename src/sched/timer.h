/* Copyright (c) 2012, Peter Jönsson
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

#ifndef _ppcos_timer_h_
#define _ppcos_timer_h_

#include "krntypes.h"

#define TBU_READ 0x10C  /* Time Base Upper User for reading */ 
#define TBL_READ 0x10D  /* Time Base Lower User for reading */
#define TBU_WRITE 0x11C /* Time Base Upper Supervisor for writing */
#define TBL_WRITE 0x11D /* Time Base Lower Supervisor for writing */

#define DEC   0x016 /* Decrement */
#define DECAR 0x036 /* Decrement Auto-Reload */
#define TSR   0x150 /* Time Status Register */
#define TCR   0x154 /* Timer Control Register */

#define IVR   0x03F /* Interrupt Vector Prefix Register */

#define MFSPR(result, spr) asm("mfspr %0, %1" : "=r" (result) : "i" (spr));
#define MTSPR(value,  spr) asm("mtspr %0, %1" : : "r" (value), "i" (spr));

void timer(U32);
void print_tbr(void);

#endif
