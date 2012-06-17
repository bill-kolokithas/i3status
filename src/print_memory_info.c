/*
 * Copyright (c) 2012 Mateusz Loskot <mateusz at loskot dot net>
 * 
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of i3status nor the names of contributors
 *   may be used to endorse or promote products derived from this software
 *   without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdio.h>
#include <sys/sysinfo.h>

#include "i3status.h"

/* TODO: Fix for Linux pre-2.3.23 */

/*
 * Reads the memory utilization using Linux sysinfo() call.
 *
 */
void print_memory_info(yajl_gen json_gen, char *buffer, const char *title, const char *format) {
        char *outwalk = buffer;
        uint64_t mem_unit = 0;
        
        INSTANCE(title);

#if defined(LINUX)
        const char *walk;

        struct sysinfo info;
        if (sysinfo(&info) == -1)
                goto error;
        mem_unit = (uint64_t)info.mem_unit;
        
        for (walk = format; *walk != '\0'; walk++) {
                if (*walk != '%') {
                        *(outwalk++) = *walk;
                        continue;
                }

                /* Total usable main memory size */
                if (BEGINS_WITH(walk+1, "total")) {
                        outwalk += print_bytes_human(outwalk, (uint64_t)info.totalram * mem_unit);
                        walk += strlen("total");
                }
                
                /* Available memory size */
                if (BEGINS_WITH(walk+1, "free")) {
                        outwalk += print_bytes_human(outwalk, (uint64_t)info.freeram * mem_unit);
                        walk += strlen("free");
                }

                /* Total swap space size */
                if (BEGINS_WITH(walk+1, "swaptotal")) {
                        outwalk += print_bytes_human(outwalk, (uint64_t)info.totalswap * mem_unit);
                        walk += strlen("swaptotal");
                }
                
                /* swap space still available */
                if (BEGINS_WITH(walk+1, "swapfree")) {
                        outwalk += print_bytes_human(outwalk, (uint64_t)info.freeswap * mem_unit);
                        walk += strlen("swapfree");
                }
        }

        *outwalk = '\0';
        OUTPUT_FULL_TEXT(buffer);
        return;
#endif

error:
        OUTPUT_FULL_TEXT("cant read memory usage");
        (void)fputs("i3status: Cannot read memory usage\n", stderr);
}