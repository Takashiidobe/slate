extern const int sysctl_vals[];
#define SYSCTL_ONE ((void *)&sysctl_vals[1])
#define SYSCTL_HUNDRED ((void *)&sysctl_vals[3])
struct ctl { const char *name; void *min; void *max; };
static struct ctl table[] = {{"one", SYSCTL_ONE, SYSCTL_HUNDRED}, {0, 0, 0}};
static const int *second = sysctl_vals + 2;
int table_sum(void) { return *(int *)table[0].min + *(int *)table[0].max + *second; }
