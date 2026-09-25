[[msvc::noinline]] void noinline_fn(void);
[[msvc::forceinline]] inline void inline_fn(void);
__declspec(thread) int thread_local_value;
__declspec(selectany) int selected = 1;
__declspec(noalias) int no_alias(int *);
__declspec(restrict) void *restricted(unsigned long);
__declspec(nothrow) void no_throw(void);
__declspec(allocate(".data.custom")) int placed;
__declspec(code_seg(".text.custom")) void code(void);
__declspec(dllimport) int imported;
__declspec(dllexport) void exported(void);
__declspec(align(32)) int aligned;
void local(void) { [[msvc::noinline]] void nested(void); }
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23
// SLATE-FILECHECK-FLAVOR msvc

// SLATE-FILECHECK-IR-ERROR C23

// SLATE-FILECHECK-BEGIN C23
// C23: Error:   × unsupported in numeric IR lowering: code segment attribute
// SLATE-FILECHECK-END C23
