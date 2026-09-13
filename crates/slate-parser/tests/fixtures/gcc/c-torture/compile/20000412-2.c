// SLATE-FILECHECK-DEFINES DEFAULT

char list[250][64];

int f(int idx) { return (__builtin_strlen(list[idx])); }

