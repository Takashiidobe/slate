#ifndef ABI_REG_STRUCT_RETURN_H
#define ABI_REG_STRUCT_RETURN_H

struct one { char a; };
struct two { short a; };
struct four { short a; char b; };
struct eight { int a; int b; };
struct mixed { int a; float b; };
struct three { char a[3]; };
struct twelve { int a, b, c; };
struct single_float { float f; };
struct single_double { double d; };
union word { int i; char c; };

typedef struct one (*one_fn)(int);
typedef struct two (*two_fn)(int);
typedef struct four (*four_fn)(int);
typedef struct eight (*eight_fn)(int);
typedef struct mixed (*mixed_fn)(int);
typedef struct three (*three_fn)(int);
typedef struct twelve (*twelve_fn)(int);
typedef struct single_float (*single_float_fn)(int);
typedef struct single_double (*single_double_fn)(int);
typedef union word (*word_fn)(int);
typedef _Complex float (*complex_fn)(int);
typedef int (*sum_eight_fn)(eight_fn, int);
typedef double (*sum_double_fn)(single_double_fn, int);
typedef int (*sum_three_fn)(three_fn, int);

#endif
