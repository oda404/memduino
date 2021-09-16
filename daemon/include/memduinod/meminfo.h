
#ifndef MEMDUINO_MEMINFO_H
#define MEMDUINO_MEMINFO_H

#include<stdint.h>

typedef enum
E_MemInfoUnits
{
    MEMINFO_B,
    MEMINFO_KB,
    MEMINFO_MB,
    MEMINFO_GB
} MemInfoUnits;

/** 
 * Returns how much memory is in use by the system. 
 * @param unit The memory unit.
 * @returns Memory used, or -1 if anything goes wrong.
*/
double meminfo_get_used_mem(uint8_t unit);

#endif //!MEMDUINO_MEMINFO_H
