
#include<memduinod/meminfo.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

typedef struct
S_MemInfo
{
    uint32_t total;
    uint32_t buffers;
    uint32_t cached;
    uint32_t free;
    uint32_t shared;
    uint32_t s_reclaimable;
} MemInfo;

#define PRINTFF(fmt, ...) printf("[meminfo] " fmt, ##__VA_ARGS__);

static MemInfo g_meminfo;

double meminfo_get_used_mem(uint8_t unit)
{
    FILE *file = fopen("/proc/meminfo", "r");
    if(!file)
    {
        PRINTFF("Error: %s\n", strerror(errno))
        return -1;
    }

#define BUF_LEN 64

    char buf[BUF_LEN + 1];
    while(fgets(buf, BUF_LEN, file))
    {
        if(strstr(buf, "MemTotal:") == buf)
            g_meminfo.total = strtoul(strchr(buf, ':') + 1, NULL, 10);
        else if(strstr(buf, "Buffers:") == buf)
            g_meminfo.buffers = strtoul(strchr(buf, ':') + 1, NULL, 10);
        else if(strstr(buf, "Cached:") == buf)
            g_meminfo.cached = strtoul(strchr(buf, ':') + 1, NULL, 10);
        else if(strstr(buf, "MemFree:") == buf)
            g_meminfo.free = strtoul(strchr(buf, ':') + 1, NULL, 10);
        else if(strstr(buf, "Shmem:") == buf)
            g_meminfo.shared = strtoul(strchr(buf, ':') + 1, NULL, 10);
        else if(strstr(buf, "SReclaimable:") == buf)
            g_meminfo.s_reclaimable = strtoul(strchr(buf, ':') + 1, NULL, 10);
    }

    fclose(file);

    /** Respectfully stolen from: 
     * https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290
    */
    uint32_t usedmem_kb = g_meminfo.total - g_meminfo.free - (
		g_meminfo.buffers + (
			g_meminfo.cached + g_meminfo.s_reclaimable - g_meminfo.shared
		)
	);

    /* From what I know /proc/meminfo alawys use kB so we start from there. */
    switch(unit)
    {
    case MEMINFO_B:
        return usedmem_kb * 1000.0;
    case MEMINFO_KB:
        return usedmem_kb;

    case MEMINFO_MB:
        return usedmem_kb / 1000.0;

    case MEMINFO_GB:
        return usedmem_kb / 1000000.0;

    default:
        PRINTFF("Unknown unit with enum value: %d\n", unit);
        return -1;
    }
}
