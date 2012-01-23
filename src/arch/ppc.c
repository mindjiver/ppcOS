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

#include "arch/ppc.h"
#include "arch/ppc440.h"

#include "log/log.h"

#define CPU_NAME_LEN 30
#define CPU_INFO_LEN 255

void cpu_version(void)
{
        U32 pvr; 
        U32 pir;
        char cpu_name[CPU_NAME_LEN];
        char cpu_info[CPU_INFO_LEN];

        MFSPR(pvr, PVR);
        MFSPR(pir, PIR);

        switch(pvr) {
        case PVR_PPC440GP:
                strncpy(cpu_name, "PPC440GP", CPU_NAME_LEN);
                break;
        default:
                strncpy(cpu_name, "Unknown", CPU_NAME_LEN);
                break;
        }

        sprintf(cpu_info, "CPU%x: %s", pir, cpu_name);
        INFO(cpu_info);
}
