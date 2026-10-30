// SLATE-FILECHECK-ARGS --trace
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PP DEFAULT
// SLATE-FILECHECK-DEFINES DROP -DDROP_IT
// SLATE-FILECHECK-PP DROP
#define VALUE 1
#ifdef DROP_IT
#undef VALUE
#endif
int value = VALUE;
#ifdef VALUE
int defined_value;
#endif

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: if #ifdef <main>:2 not-taken names=[DROP_IT] reads=[DROP_IT:undefined]
// DEFAULT-NEXT: if #ifdef <main>:6 taken names=[VALUE] reads=[VALUE:defined@<main>:1]
// DEFAULT-NEXT: expansion VALUE <main>:5 context=[] macros=[VALUE@<main>:1, skipped undef VALUE@<main>:3] roots=[DROP_IT]
// DEFAULT-NEXT: tokens <main>:5 `int value =` context=[] macros=[] roots=[]
// DEFAULT-NEXT: tokens <main>:5 `1` context=[] macros=[VALUE@<main>:1, skipped undef VALUE@<main>:3] roots=[DROP_IT]
// DEFAULT-NEXT: tokens <main>:5 `;` context=[] macros=[] roots=[]
// DEFAULT-NEXT: tokens <main>:7 `int defined_value;` context=[#ifdef@<main>:6] macros=[] roots=[DROP_IT]
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN DROP
// DROP: if #ifdef <main>:2 taken names=[DROP_IT] reads=[DROP_IT:defined@<command line>]
// DROP-NEXT: if #ifdef <main>:6 not-taken names=[VALUE] reads=[VALUE:undefined@<main>:3]
// DROP-NEXT: tokens <main>:5 `int value =` context=[] macros=[] roots=[]
// DROP-NEXT: tokens <main>:5 `VALUE` context=[] macros=[undef VALUE@<main>:3] roots=[DROP_IT]
// DROP-NEXT: tokens <main>:5 `;` context=[] macros=[] roots=[]
// SLATE-FILECHECK-END DROP
