__attribute__((cold, noinline, noclone, noipa)) int cold_function(void);
__attribute__((hot, flatten, leaf)) int hot_function(void);
__attribute__((optimize(0), naked)) int optimized_function(void);
__attribute__((no_split_stack)) int split_function(void);
__attribute__((returns_twice)) int returns_twice_function(void);

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: declaration type=int declarator=function(name=cold_function,params=,variadic=false) [attributes=cold,noinline,noclone,noipa]
// DEFAULT-NEXT: decl[1]: declaration type=int declarator=function(name=hot_function,params=,variadic=false) [attributes=hot,flatten,leaf]
// DEFAULT-NEXT: decl[2]: declaration type=int declarator=function(name=optimized_function,params=,variadic=false) [attributes=optimize(0),naked]
// DEFAULT-NEXT: decl[3]: declaration type=int declarator=function(name=split_function,params=,variadic=false) [attributes=no_split_stack]
// DEFAULT-NEXT: decl[4]: declaration type=int declarator=function(name=returns_twice_function,params=,variadic=false) [attributes=returns_twice]
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: declaration type=int declarator=function(name=cold_function,params=,variadic=false) [attributes=cold,noinline,noclone,noipa]
// DEFAULT-NEXT: decl[1]: declaration type=int declarator=function(name=hot_function,params=,variadic=false) [attributes=hot,flatten,leaf]
// DEFAULT-NEXT: decl[2]: declaration type=int declarator=function(name=optimized_function,params=,variadic=false) [attributes=optimize(0),naked]
// DEFAULT-NEXT: decl[3]: declaration type=int declarator=function(name=split_function,params=,variadic=false) [attributes=no_split_stack]
// DEFAULT-NEXT: decl[4]: declaration type=int declarator=function(name=returns_twice_function,params=,variadic=false) [attributes=returns_twice]
// SLATE-FILECHECK-END DEFAULT
