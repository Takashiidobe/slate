// SLATE-FILECHECK-ARGS --trace
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PP DEFAULT
// SLATE-FILECHECK-DEFINES OUTER -DOUTER
// SLATE-FILECHECK-PP OUTER
int always;
#ifdef OUTER
int outer;
#if INNER > 1
int inner;
#elif defined(OTHER)
int other;
#else
int neither;
#endif
#else
#ifdef UNSEEN
int unseen;
#endif
int not_outer;
#endif

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: if #ifdef <main>:2 not-taken names=[OUTER] reads=[OUTER:undefined]
// DEFAULT-NEXT: arm #else <main>:11 taken names=[] reads=[]
// DEFAULT-NEXT: if #if <main>:4 not-evaluated names=[INNER] reads=[INNER:undefined]
// DEFAULT-NEXT: arm #elif <main>:6 not-evaluated names=[OTHER] reads=[OTHER:undefined]
// DEFAULT-NEXT: arm #else <main>:8 not-evaluated names=[] reads=[]
// DEFAULT-NEXT: if #ifdef <main>:12 not-taken names=[UNSEEN] reads=[UNSEEN:undefined]
// DEFAULT-NEXT: tokens <main>:1 `int always;` context=[] macros=[] roots=[]
// DEFAULT-NEXT: tokens <main>:15 `int not_outer;` context=[#else@<main>:11] macros=[] roots=[OUTER]
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN OUTER
// OUTER: if #ifdef <main>:2 taken names=[OUTER] reads=[OUTER:defined@<command line>]
// OUTER-NEXT: arm #else <main>:11 not-evaluated names=[] reads=[]
// OUTER-NEXT: if #if <main>:4 not-taken names=[INNER] reads=[INNER:undefined]
// OUTER-NEXT: arm #elif <main>:6 not-taken names=[OTHER] reads=[OTHER:undefined]
// OUTER-NEXT: arm #else <main>:8 taken names=[] reads=[]
// OUTER-NEXT: if #ifdef <main>:12 not-evaluated names=[UNSEEN] reads=[UNSEEN:undefined]
// OUTER-NEXT: tokens <main>:1 `int always;` context=[] macros=[] roots=[]
// OUTER-NEXT: tokens <main>:3 `int outer;` context=[#ifdef@<main>:2] macros=[] roots=[OUTER]
// OUTER-NEXT: tokens <main>:9 `int neither;` context=[#ifdef@<main>:2, #else@<main>:8] macros=[] roots=[INNER, OTHER, OUTER]
// SLATE-FILECHECK-END OUTER
