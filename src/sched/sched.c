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

#include <stdlib.h>
#include <string.h>

#include "sched/sched.h"
#include "arch/ppc440.h"
#include "log/log.h"
#include "timer/timer.h"

void _idle(void);
void procA(void);
void procB(void);

#define PROC_NAME_LEN 255
#define GPR_NUM 32
#define SPR_NUM 4

typedef struct pcb
{
        U32 gpr[32];
        U32 xer;
        U32 tbl;
        U32 tbu;
        U32 cr;
        U32 ctr;
        U32 lr;
        U32 spr[4];
        U32 usprg0;
} pcb;

typedef struct process {
        char name[PROC_NAME_LEN];
        U32 pid;
        U32 ppid;
        void (*entry)(void);
        struct pcb pcb;
        struct process *next;
} process;

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

process *create_proc(char *name, U32 ppid, void (*func)(void))
{
        process *proc = malloc(sizeof(process));

        strncpy(proc->name, name, PROC_NAME_LEN);
        proc->pid = pid++;
        proc->ppid = ppid;
        proc->entry = func;

        // clear the cpu registers.
        memset(&(proc->pcb), 0, sizeof(pcb));

        return proc;
}

void schedule(void)
{
        MTTCR((FIT_TIME_PERIOD_4 | FIT_INT_ENABLE));

        INFO("Starting round robin scheduling");

        process *idle   = create_proc("Idle", 0, _idle);
        process *proc_a = create_proc("Process A", 0, procA);
        process *proc_b = create_proc("Process B", 0, procB);

        process *current = idle;

        idle->next   = proc_a;
        proc_a->next = proc_b;
        proc_b->next = proc_a;

        //MTMSR(MSR_PROBLEM_STATE);

        while(current->next != NULL) {
                /* Since we are co-operative here we switch off
                 * external interrupts. The running function needs to
                 * yield when they are done.
                 */
                WRTEEI(0);
                current->entry();
                current = current->next;
        }
}

void _idle(void)
{
        INFO("OS idle");
        yield();
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

