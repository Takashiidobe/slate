int alloc(int size, int align) __attribute__((malloc, alloc_size(1 + 0, 2), alloc_align(2), returns_nonnull));
int result(void) __attribute__((warn_unused_result));
int variadic(int value, ...) __attribute__((sentinel));
__attribute__((assume_aligned(16, 4))) int aligned_result(void);

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: declaration type=int declarator=function(name=alloc,params=int size,int align,variadic=false) [attributes=malloc,alloc_size((1 + 0),2),alloc_align(2),returns_nonnull]
// DEFAULT-NEXT: decl[1]: declaration type=int declarator=function(name=result,params=,variadic=false) [attributes=warn_unused_result]
// DEFAULT-NEXT: decl[2]: declaration type=int declarator=function(name=variadic,params=int value,variadic=true) [attributes=sentinel]
// DEFAULT-NEXT: decl[3]: declaration type=int declarator=function(name=aligned_result,params=,variadic=false) [attributes=assume_aligned(16,4)]
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: declaration type=int declarator=function(name=alloc,params=int size,int align,variadic=false) [attributes=malloc,alloc_size((1 + 0),2),alloc_align(2),returns_nonnull]
// DEFAULT-NEXT: decl[1]: declaration type=int declarator=function(name=result,params=,variadic=false) [attributes=warn_unused_result]
// DEFAULT-NEXT: decl[2]: declaration type=int declarator=function(name=variadic,params=int value,variadic=true) [attributes=sentinel]
// DEFAULT-NEXT: decl[3]: declaration type=int declarator=function(name=aligned_result,params=,variadic=false) [attributes=assume_aligned(16,4)]
// SLATE-FILECHECK-END DEFAULT
