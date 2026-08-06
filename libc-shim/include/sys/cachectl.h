#ifndef _SYS_CACHECTL_H
#define _SYS_CACHECTL_H

#define ICACHE      (1 << 0)
#define DCACHE      (1 << 1)
#define BCACHE      (ICACHE | DCACHE)
#define CACHEABLE   0
#define UNCACHEABLE 1

int cachectl(void *, int, int);
int cacheflush(void *, int, int);
int _flush_cache(void *, int, int);

#endif
