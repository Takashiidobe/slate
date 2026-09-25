void f(void) {
#if __STDC_VERSION__ >= 202311L
  auto z = 1.0;
#else
  auto int z = 1;
#endif
  int *p = nullptr;
  _Bool t = true;
  _Bool u = false;
}

// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23

// SLATE-FILECHECK-IR-ERROR C17
// SLATE-FILECHECK-IR-ERROR C23

// SLATE-FILECHECK-BEGIN C17
// C17: Error:   × unresolved ordinary name `nullptr`
// SLATE-FILECHECK-END C17
// SLATE-FILECHECK-BEGIN C23
// C23: Error:   × unsupported in numeric IR lowering: target builtin type
// SLATE-FILECHECK-END C23
