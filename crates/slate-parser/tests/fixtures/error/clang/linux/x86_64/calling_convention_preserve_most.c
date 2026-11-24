// SLATE-FILECHECK-DEFINES DEFINITION DEFINITION
// SLATE-FILECHECK-DEFINES TYPEDEF TYPEDEF
// SLATE-FILECHECK-DEFINES POINTER POINTER
// SLATE-FILECHECK-DEFINES SCOPED SCOPED
// SLATE-FILECHECK-ERROR DEFINITION
// SLATE-FILECHECK-ERROR TYPEDEF
// SLATE-FILECHECK-ERROR POINTER
// SLATE-FILECHECK-ERROR SCOPED
// SLATE-FILECHECK-ARGS -std=c23

#if defined(DEFINITION)
__attribute__((preserve_most)) int callback(int x) { return x; }
#elif defined(TYPEDEF)
typedef __attribute__((__preserve_most__)) int callback(int x);
#elif defined(POINTER)
typedef int (__attribute__((preserve_most)) *callback)(int);
#elif defined(SCOPED)
[[clang::preserve_most]] int callback(int x);
#endif

// SLATE-FILECHECK-BEGIN DEFINITION
// DEFINITION: Error:   × unsupported calling convention `preserve_most`
// DEFINITION:   ╰─▶ unsupported calling convention `preserve_most`
// DEFINITION:    ╭─[tests/fixtures/error/clang/linux/x86_64/calling_convention_preserve_most.c:3:1]
// DEFINITION:  2 │ #if defined(DEFINITION)
// DEFINITION:  3 │ __attribute__((preserve_most)) int callback(int x) { return x; }
// DEFINITION:    · ─────────────
// DEFINITION:  4 │ #elif defined(TYPEDEF)
// DEFINITION:    ╰────
// SLATE-FILECHECK-END DEFINITION
// SLATE-FILECHECK-BEGIN TYPEDEF
// TYPEDEF: Error:   × unsupported calling convention `preserve_most`
// TYPEDEF:   ╰─▶ unsupported calling convention `preserve_most`
// TYPEDEF:    ╭─[tests/fixtures/error/clang/linux/x86_64/calling_convention_preserve_most.c:5:9]
// TYPEDEF:  4 │ #elif defined(TYPEDEF)
// TYPEDEF:  5 │ typedef __attribute__((__preserve_most__)) int callback(int x);
// TYPEDEF:    ·         ─────────────
// TYPEDEF:  6 │ #elif defined(POINTER)
// TYPEDEF:    ╰────
// SLATE-FILECHECK-END TYPEDEF
// SLATE-FILECHECK-BEGIN POINTER
// POINTER: Error:   × unsupported calling convention `preserve_most`
// POINTER:   ╰─▶ unsupported calling convention `preserve_most`
// POINTER:    ╭─[tests/fixtures/error/clang/linux/x86_64/calling_convention_preserve_most.c:7:14]
// POINTER:  6 │ #elif defined(POINTER)
// POINTER:  7 │ typedef int (__attribute__((preserve_most)) *callback)(int);
// POINTER:    ·              ─────────────
// POINTER:  8 │ #elif defined(SCOPED)
// POINTER:    ╰────
// SLATE-FILECHECK-END POINTER
// SLATE-FILECHECK-BEGIN SCOPED
// SCOPED: Error:   × unsupported calling convention `preserve_most`
// SCOPED:   ╰─▶ unsupported calling convention `preserve_most`
// SCOPED:     ╭─[tests/fixtures/error/clang/linux/x86_64/calling_convention_preserve_most.c:9:1]
// SCOPED:   8 │ #elif defined(SCOPED)
// SCOPED:   9 │ {{\[\[}}clang::preserve_most]] int callback(int x);
// SCOPED:     · ─
// SCOPED:  10 │ #endif
// SCOPED:     ╰────
// SLATE-FILECHECK-END SCOPED
