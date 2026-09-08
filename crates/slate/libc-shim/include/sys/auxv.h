#ifndef _SYS_AUXV_H
#define _SYS_AUXV_H

#include <elf.h>
#include <bits/hwcap.h>

unsigned long getauxval(unsigned long);

#endif
