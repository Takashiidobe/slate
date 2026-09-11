__attribute__((visibility("hidden"))) int hidden_data;
extern int weak_data __attribute__((weak, used, section(".data")));

int declared(int *p) __attribute__((nonnull(1), noinline));

__attribute__((noinline)) int definition() __attribute__((pure)) {
  return 0;
}

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: declaration type=int declarator=name=hidden_data [attributes=visibility("hidden")]
// DEFAULT-NEXT: decl[1]: declaration type=int [storage=extern] declarator=name=weak_data [attributes=weak,used,section(".data")]
// DEFAULT-NEXT: decl[2]: declaration type=int declarator=function(name=declared,params=ptr(int p),variadic=false) [attributes=nonnull(1),noinline]
// DEFAULT-NEXT: decl[3]: function name=definition return=int [attributes=noinline,pure]
// DEFAULT-NEXT:   stmt[0]: return 0
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: declaration type=int declarator=name=hidden_data [attributes=visibility("hidden")]
// DEFAULT-NEXT: decl[1]: declaration type=int [storage=extern] declarator=name=weak_data [attributes=weak,used,section(".data")]
// DEFAULT-NEXT: decl[2]: declaration type=int declarator=function(name=declared,params=ptr(int p),variadic=false) [attributes=nonnull(1),noinline]
// DEFAULT-NEXT: decl[3]: function name=definition return=int [attributes=noinline,pure]
// DEFAULT-NEXT:   stmt[0]: return 0
// SLATE-FILECHECK-END DEFAULT
