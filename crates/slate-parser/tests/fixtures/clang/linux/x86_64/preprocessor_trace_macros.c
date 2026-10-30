// SLATE-FILECHECK-ARGS --trace
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PP DEFAULT
// SLATE-FILECHECK-DEFINES DEBUG -DDEBUG_MODE
// SLATE-FILECHECK-PP DEBUG
#ifdef DEBUG_MODE
#define Assert(cond, msg) check(cond, #cond, msg)
#define LIMIT 10
#else
#define Assert(cond, msg)
#define LIMIT 20
#endif
#define INDIRECT LIMIT
#define NAME(suffix) prefix_##suffix
#define STR(x) #x
#if LIMIT > 15
#define BIG 1
#endif

void check(int, const char *, const char *);

int f(int x) {
    Assert(x > 0, "positive");
    return x + INDIRECT;
}
int NAME(joined) = 1;
const char *text = STR(LIMIT);
#ifdef BIG
int big;
#endif

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: if #ifdef <main>:1 not-taken names=[DEBUG_MODE] reads=[DEBUG_MODE:undefined]
// DEFAULT-NEXT: arm #else <main>:4 taken names=[] reads=[]
// DEFAULT-NEXT: if #if <main>:11 taken names=[LIMIT] reads=[LIMIT:defined@<main>:6]
// DEFAULT-NEXT: if #ifdef <main>:23 taken names=[BIG] reads=[BIG:defined@<main>:12]
// DEFAULT-NEXT: expansion Assert <main>:18 empty context=[] macros=[skipped Assert@<main>:2, Assert@<main>:5] roots=[DEBUG_MODE]
// DEFAULT-NEXT: expansion INDIRECT <main>:19 context=[] macros=[INDIRECT@<main>:8] roots=[]
// DEFAULT-NEXT: expansion LIMIT <main>:19 context=[] macros=[skipped LIMIT@<main>:3, LIMIT@<main>:6, INDIRECT@<main>:8] roots=[DEBUG_MODE]
// DEFAULT-NEXT: expansion NAME <main>:21 context=[] macros=[NAME@<main>:9] roots=[]
// DEFAULT-NEXT: expansion STR <main>:22 context=[] macros=[STR@<main>:10] roots=[]
// DEFAULT-NEXT: tokens <main>:15 `void check(int, const char *, const char *);` context=[] macros=[] roots=[]
// DEFAULT-NEXT: tokens <main>:17 `int f(int x) {` context=[] macros=[] roots=[]
// DEFAULT-NEXT: tokens <main>:18 `;` context=[] macros=[] roots=[]
// DEFAULT-NEXT: tokens <main>:19 `return x +` context=[] macros=[] roots=[]
// DEFAULT-NEXT: tokens <main>:19 `20` context=[] macros=[skipped LIMIT@<main>:3, LIMIT@<main>:6, INDIRECT@<main>:8] roots=[DEBUG_MODE]
// DEFAULT-NEXT: tokens <main>:19 `;` context=[] macros=[] roots=[]
// DEFAULT-NEXT: tokens <main>:20 `}` context=[] macros=[] roots=[]
// DEFAULT-NEXT: tokens <main>:21 `int` context=[] macros=[] roots=[]
// DEFAULT-NEXT: tokens <main>:21 `prefix_joined` context=[] macros=[NAME@<main>:9] roots=[]
// DEFAULT-NEXT: tokens <main>:21 `= 1;` context=[] macros=[] roots=[]
// DEFAULT-NEXT: tokens <main>:22 `const char *text =` context=[] macros=[] roots=[]
// DEFAULT-NEXT: tokens <main>:22 `"LIMIT"` context=[] macros=[STR@<main>:10] roots=[]
// DEFAULT-NEXT: tokens <main>:22 `;` context=[] macros=[] roots=[]
// DEFAULT-NEXT: tokens <main>:24 `int big;` context=[#ifdef@<main>:23] macros=[] roots=[DEBUG_MODE]
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN DEBUG
// DEBUG: if #ifdef <main>:1 taken names=[DEBUG_MODE] reads=[DEBUG_MODE:defined@<command line>]
// DEBUG-NEXT: arm #else <main>:4 not-evaluated names=[] reads=[]
// DEBUG-NEXT: if #if <main>:11 not-taken names=[LIMIT] reads=[LIMIT:defined@<main>:3]
// DEBUG-NEXT: if #ifdef <main>:23 not-taken names=[BIG] reads=[BIG:undefined]
// DEBUG-NEXT: expansion Assert <main>:18 context=[] macros=[Assert@<main>:2, skipped Assert@<main>:5] roots=[DEBUG_MODE]
// DEBUG-NEXT: expansion INDIRECT <main>:19 context=[] macros=[INDIRECT@<main>:8] roots=[]
// DEBUG-NEXT: expansion LIMIT <main>:19 context=[] macros=[LIMIT@<main>:3, skipped LIMIT@<main>:6, INDIRECT@<main>:8] roots=[DEBUG_MODE]
// DEBUG-NEXT: expansion NAME <main>:21 context=[] macros=[NAME@<main>:9] roots=[]
// DEBUG-NEXT: expansion STR <main>:22 context=[] macros=[STR@<main>:10] roots=[]
// DEBUG-NEXT: tokens <main>:15 `void check(int, const char *, const char *);` context=[] macros=[] roots=[]
// DEBUG-NEXT: tokens <main>:17 `int f(int x) {` context=[] macros=[] roots=[]
// DEBUG-NEXT: tokens <main>:18 `check(x > 0, "x > 0", "positive")` context=[] macros=[Assert@<main>:2, skipped Assert@<main>:5] roots=[DEBUG_MODE]
// DEBUG-NEXT: tokens <main>:18 `;` context=[] macros=[] roots=[]
// DEBUG-NEXT: tokens <main>:19 `return x +` context=[] macros=[] roots=[]
// DEBUG-NEXT: tokens <main>:19 `10` context=[] macros=[LIMIT@<main>:3, skipped LIMIT@<main>:6, INDIRECT@<main>:8] roots=[DEBUG_MODE]
// DEBUG-NEXT: tokens <main>:19 `;` context=[] macros=[] roots=[]
// DEBUG-NEXT: tokens <main>:20 `}` context=[] macros=[] roots=[]
// DEBUG-NEXT: tokens <main>:21 `int` context=[] macros=[] roots=[]
// DEBUG-NEXT: tokens <main>:21 `prefix_joined` context=[] macros=[NAME@<main>:9] roots=[]
// DEBUG-NEXT: tokens <main>:21 `= 1;` context=[] macros=[] roots=[]
// DEBUG-NEXT: tokens <main>:22 `const char *text =` context=[] macros=[] roots=[]
// DEBUG-NEXT: tokens <main>:22 `"LIMIT"` context=[] macros=[STR@<main>:10] roots=[]
// DEBUG-NEXT: tokens <main>:22 `;` context=[] macros=[] roots=[]
// SLATE-FILECHECK-END DEBUG
