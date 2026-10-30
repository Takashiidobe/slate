// SLATE-FILECHECK-ARGS --trace
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PP DEFAULT
// SLATE-FILECHECK-DEFINES OFF -DNO_FORMAT
// SLATE-FILECHECK-PP OFF
// SLATE-FILECHECK-DEFINES RENAME -DWITH_PREFIX
// SLATE-FILECHECK-PP RENAME
#ifndef NO_FORMAT
#define FORMAT
#endif
#ifdef WITH_PREFIX
#define entry prefixed_entry
#endif
#ifdef FORMAT
int with_format;
#else
int without_format;
#endif
int entry;

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: if #ifndef <main>:1 taken names=[NO_FORMAT] reads=[NO_FORMAT:undefined]
// DEFAULT-NEXT: if #ifdef <main>:4 not-taken names=[WITH_PREFIX] reads=[WITH_PREFIX:undefined]
// DEFAULT-NEXT: if #ifdef <main>:7 taken names=[FORMAT] reads=[FORMAT:defined@<main>:2]
// DEFAULT-NEXT: arm #else <main>:9 not-evaluated names=[] reads=[]
// DEFAULT-NEXT: tokens <main>:8 `int with_format;` context=[#ifdef@<main>:7] macros=[] roots=[NO_FORMAT]
// DEFAULT-NEXT: tokens <main>:12 `int` context=[] macros=[] roots=[]
// DEFAULT-NEXT: tokens <main>:12 `entry` context=[] macros=[skipped entry@<main>:5] roots=[WITH_PREFIX]
// DEFAULT-NEXT: tokens <main>:12 `;` context=[] macros=[] roots=[]
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN OFF
// OFF: if #ifndef <main>:1 not-taken names=[NO_FORMAT] reads=[NO_FORMAT:defined@<command line>]
// OFF-NEXT: if #ifdef <main>:4 not-taken names=[WITH_PREFIX] reads=[WITH_PREFIX:undefined]
// OFF-NEXT: if #ifdef <main>:7 not-taken names=[FORMAT] reads=[FORMAT:undefined]
// OFF-NEXT: arm #else <main>:9 taken names=[] reads=[]
// OFF-NEXT: tokens <main>:10 `int without_format;` context=[#else@<main>:9] macros=[] roots=[FORMAT, NO_FORMAT]
// OFF-NEXT: tokens <main>:12 `int` context=[] macros=[] roots=[]
// OFF-NEXT: tokens <main>:12 `entry` context=[] macros=[skipped entry@<main>:5] roots=[WITH_PREFIX]
// OFF-NEXT: tokens <main>:12 `;` context=[] macros=[] roots=[]
// SLATE-FILECHECK-END OFF
// SLATE-FILECHECK-BEGIN RENAME
// RENAME: if #ifndef <main>:1 taken names=[NO_FORMAT] reads=[NO_FORMAT:undefined]
// RENAME-NEXT: if #ifdef <main>:4 taken names=[WITH_PREFIX] reads=[WITH_PREFIX:defined@<command line>]
// RENAME-NEXT: if #ifdef <main>:7 taken names=[FORMAT] reads=[FORMAT:defined@<main>:2]
// RENAME-NEXT: arm #else <main>:9 not-evaluated names=[] reads=[]
// RENAME-NEXT: expansion entry <main>:12 context=[] macros=[entry@<main>:5] roots=[WITH_PREFIX]
// RENAME-NEXT: tokens <main>:8 `int with_format;` context=[#ifdef@<main>:7] macros=[] roots=[NO_FORMAT]
// RENAME-NEXT: tokens <main>:12 `int` context=[] macros=[] roots=[]
// RENAME-NEXT: tokens <main>:12 `prefixed_entry` context=[] macros=[entry@<main>:5] roots=[WITH_PREFIX]
// RENAME-NEXT: tokens <main>:12 `;` context=[] macros=[] roots=[]
// SLATE-FILECHECK-END RENAME
