// SLATE-FILECHECK-ARGS --trace
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PP DEFAULT
// SLATE-FILECHECK-DEFINES USE -DUSE_HEADER
// SLATE-FILECHECK-PP USE
// SLATE-FILECHECK-DEFINES BOTH -DUSE_HEADER -DHEADER_OPTION
// SLATE-FILECHECK-PP BOTH
#ifdef USE_HEADER
#include "preprocessor_trace_include.h"
#endif
int after;

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: if #ifdef <main>:1 not-taken names=[USE_HEADER] reads=[USE_HEADER:undefined]
// DEFAULT-NEXT: tokens <main>:4 `int after;` context=[] macros=[] roots=[]
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN USE
// USE: if #ifdef <main>:1 taken names=[USE_HEADER] reads=[USE_HEADER:defined@<command line>]
// USE-NEXT: if #ifndef preprocessor_trace_include.h:1 taken names=[PREPROCESSOR_TRACE_INCLUDE_H] reads=[PREPROCESSOR_TRACE_INCLUDE_H:undefined]
// USE-NEXT: if #ifdef preprocessor_trace_include.h:4 not-taken names=[HEADER_OPTION] reads=[HEADER_OPTION:undefined]
// USE-NEXT: tokens preprocessor_trace_include.h:3 `int from_header;` context=[#ifdef@<main>:1, #ifndef@preprocessor_trace_include.h:1] macros=[] roots=[PREPROCESSOR_TRACE_INCLUDE_H, USE_HEADER]
// USE-NEXT: tokens <main>:4 `int after;` context=[] macros=[] roots=[]
// SLATE-FILECHECK-END USE
// SLATE-FILECHECK-BEGIN BOTH
// BOTH: if #ifdef <main>:1 taken names=[USE_HEADER] reads=[USE_HEADER:defined@<command line>]
// BOTH-NEXT: if #ifndef preprocessor_trace_include.h:1 taken names=[PREPROCESSOR_TRACE_INCLUDE_H] reads=[PREPROCESSOR_TRACE_INCLUDE_H:undefined]
// BOTH-NEXT: if #ifdef preprocessor_trace_include.h:4 taken names=[HEADER_OPTION] reads=[HEADER_OPTION:defined@<command line>]
// BOTH-NEXT: tokens preprocessor_trace_include.h:3 `int from_header;` context=[#ifdef@<main>:1, #ifndef@preprocessor_trace_include.h:1] macros=[] roots=[PREPROCESSOR_TRACE_INCLUDE_H, USE_HEADER]
// BOTH-NEXT: tokens preprocessor_trace_include.h:5 `int header_option;` context=[#ifdef@<main>:1, #ifndef@preprocessor_trace_include.h:1, #ifdef@preprocessor_trace_include.h:4] macros=[] roots=[HEADER_OPTION, PREPROCESSOR_TRACE_INCLUDE_H, USE_HEADER]
// BOTH-NEXT: tokens <main>:4 `int after;` context=[] macros=[] roots=[]
// SLATE-FILECHECK-END BOTH
