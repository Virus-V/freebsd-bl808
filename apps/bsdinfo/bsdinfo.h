/*-
 * Copyright (c) 2012, Jakub Szafrański <samu@pirc.pl>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */


#define O_RDONLY 0x0000
#define _POSIX2_LINE_MAX 2048
#define _PATH_DEVNULL "/dev/null"


int psysctl(char *name)
{
    char buf[256];
    size_t size = sizeof(buf);

    if (sysctlbyname(name, &buf, &size, NULL, 0) != 0) {
        return (1);
    }
    printf("%s", buf);
    return (0);
}

int printval(char *val, char *name)
{
    printf("\033[1;31m%s:\033[0;0m ", val);
    psysctl(name);
}

int printuptime()
{
    struct timespec tp;
    time_t uptime;
    int days, hrs, i, mins, secs;
    char buf[256];

    if (clock_gettime(CLOCK_UPTIME, &tp) != -1) {
        uptime = tp.tv_sec;
        if (uptime > 60)
            uptime += 30;
        days = uptime / 86400;
        uptime %= 86400;
        hrs = uptime / 3600;
        uptime %= 3600;
        mins = uptime / 60;
        secs = uptime % 60;
        (void)printf("\033[1;31mUptime: \033[0;0m");
        if (days > 0)
            (void)printf("%d day%s ", days, days > 1 ? "s" : "");
        if (hrs > 0 && mins > 0)
            (void)printf("%d:%02d", hrs, mins);
        else if (hrs == 0 && mins > 0)
            (void)printf("0:%02d", mins);
        else
            (void)printf("0:00");
    }
}

int printshell()
{
    char* shell = getenv("SHELL");
    if (shell != NULL) {
        int slen = strlen(shell);
        for (slen; slen > 0; --slen) if (shell[slen] == '/') break;
        slen = slen+1;
        while(shell[slen] != '\0') {
            printf("%c", shell[slen]);
            ++slen;
        }
        if (slen == 1) printf("%s", shell);
    }
    return (0);
}

int printmem()
{
    printf("\033[1;31mRAM:\033[0;0m ");

    long int ram_free = 0;
    size_t size_ram_free;
    size_ram_free = sizeof(ram_free);
    sysctlbyname("vm.stats.vm.v_free_count", &ram_free, &size_ram_free, NULL, 0);

    long int ram_pagesize = 0;
    size_t size_ram_pagesize;
    size_ram_pagesize = sizeof(ram_pagesize);
    sysctlbyname("hw.pagesize", &ram_pagesize, &size_ram_pagesize, NULL, 0);
    ram_free = ram_free*ram_pagesize;
    ram_free = ram_free/1024/1024;


    long int ram_size = 0;
    size_t size_ram_size;
    size_ram_size = sizeof(ram_size);
    sysctlbyname("hw.physmem", &ram_size, &size_ram_size, NULL, 0);
    ram_size = ram_size/1024/1024;

    ram_free = ram_size - ram_free;

    printf("%dM / ", ram_free);
    printf("%dM", ram_size);

    return (0);

}

int printrmws(char *str) {
    short int space = 0;
    int i = 0;
    int s = strlen(str);
    for (i; i < s; ++i) {
        if (str[i] != ' ') space = 0;
        if (space == 0) printf("%c", str[i]);
        if (str[i] == ' ') space = 1;
        else space = 0;
    }
    return (0);
}

int printcpu()
{
    printf("\033[1;31mCPU:\033[0;0m ");
    char buf[256];
    size_t size = sizeof(buf);

    if (sysctlbyname("hw.model", &buf, &size, NULL, 0) != 0) {
        return (1);
    }
    printrmws(buf);
    return (0);
}

int printbootmethod()
{
    printf("\033[1;31mBootmethod:\033[0;0m ");
    char buf[256];
    size_t size = sizeof(buf);

    if (sysctlbyname("machdep.bootmethod", &buf, &size, NULL, 0) != 0) {
        return (1);
    }
    printrmws(buf);
    return (0);
}
