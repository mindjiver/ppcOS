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

#include "sched/sched.h"
#include "arch/ppc440.h"
#include "log/log.h"
#include "timer/timer.h"

#include <stdio.h>

void procA(void);
void procB(void);
void procC(void);
void procD(void);
void procE(void);
void procF(void);
void procG(void);

#define NUM_PROC 7;
void (*proc_table[])(void) = {procA, procB, procC, procD, procE, procF, procG};

/**
 *
 * Co-operative multi tasking here. Each function needs to call this
 * function when they are "done". 
 * 
 * Increment the Process ID counter
 * Re-enable External Interrupts.
 * Switch Machine State Register to wait for the next interrupt.
 *
 */
void yield()
{
        U32 msr = 0;
        
        pid++;

        WRTEEI(1);
        MFMSR(msr);
        msr = msr | MSR_WAIT_STATE_ENABLE;
        MTMSR(msr);
}

extern U32 pid;

void schedule(void)
{
        U32 proc = 0;

        pid = 0;

        MTTCR((FIT_TIME_PERIOD_4 | FIT_INT_ENABLE));

        while(1) {
                WRTEEI(0);
                proc = pid % NUM_PROC;
                proc_table[proc]();
        }
}

void procA(void)
{
        INFO("Process A");
        yield();
}

void procB(void)
{
        INFO("Process B");
        yield();
}

void procC(void)
{
        INFO("Process C");
        yield();
}

void procD(void)
{
        INFO("Process D");
        yield();
}

void procE(void)
{
        INFO("Process E");
        yield();
}

void procF(void)
{
        INFO("Process F");
        yield();
}

void procG(void)
{
        INFO("Process G");
        yield();
}
