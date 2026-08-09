#include "shared.h"

long long read_mtime(struct stat *info) { return (long long)info->st_mtime; }
