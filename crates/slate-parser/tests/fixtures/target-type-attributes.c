__attribute__((cpu_dispatch(generic, haswell))) int dispatched(void);
__attribute__((cpu_specific(haswell))) int specific(void);
__attribute__((target_clones("default", "arch=x86-64-v2"))) int cloned(void);
__attribute__((ifunc("resolver"))) int indirect(void);
__attribute__((dllimport)) int imported;
__attribute__((weak_import)) extern int weak_platform;
__attribute__((stdcall, nomips16)) int calling_convention(void);
__attribute__((availability(macos, introduced=12.0))) int platform_api;
typedef int vector_type __attribute__((ext_vector_type(2)));
__attribute__((scalar_storage_order("big-endian"))) int ordered;
union union_value {
  int value;
} __attribute__((transparent_union));
struct __attribute__((ms_struct)) ms_platform_struct {
  int value;
};
struct __attribute__((gcc_struct)) gcc_platform_struct {
  int value;
};
__attribute__((format(printf, 1, 2))) int formatted(char *format, ...);
__attribute__((format_arg(1))) char *format_argument(char *value);
__attribute__((common, nocommon)) int common_value;

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: polyvariant:
// DEFAULT-NEXT: decl[0]: declaration type=int declarator=function(name=dispatched,params=,variadic=false) [attributes=cpu_dispatch(generic,haswell)]
// DEFAULT-NEXT: decl[1]: declaration type=int declarator=function(name=specific,params=,variadic=false) [attributes=cpu_specific(haswell)]
// DEFAULT-NEXT: decl[2]: declaration type=int declarator=function(name=cloned,params=,variadic=false) [attributes=target_clones("default","arch=x86-64-v2")]
// DEFAULT-NEXT: decl[3]: declaration type=int declarator=function(name=indirect,params=,variadic=false) [attributes=ifunc("resolver")]
// DEFAULT-NEXT: decl[4]: declaration type=int declarator=name=imported [attributes=dllimport]
// DEFAULT-NEXT: decl[5]: declaration type=int [storage=extern] declarator=name=weak_platform [attributes=weak_import]
// DEFAULT-NEXT: decl[6]: declaration type=int declarator=function(name=calling_convention,params=,variadic=false) [attributes=stdcall,nomips16]
// DEFAULT-NEXT: decl[7]: declaration type=int declarator=name=platform_api [attributes=availability(macos,introduced = 12.0)]
// DEFAULT-NEXT: decl[8]: typedef name=vector_type type=int [attributes=ext_vector_type(2)]
// DEFAULT-NEXT: decl[9]: declaration type=int declarator=name=ordered [attributes=scalar_storage_order("big-endian")]
// DEFAULT-NEXT: decl[10]: union name=union_value [attributes=transparent_union]
// DEFAULT-NEXT:   field: type=int declarator=name=value
// DEFAULT-NEXT: decl[11]: struct name=ms_platform_struct [attributes=ms_struct]
// DEFAULT-NEXT:   field: type=int declarator=name=value
// DEFAULT-NEXT: decl[12]: struct name=gcc_platform_struct [attributes=gcc_struct]
// DEFAULT-NEXT:   field: type=int declarator=name=value
// DEFAULT-NEXT: decl[13]: declaration type=int declarator=function(name=formatted,params=ptr(char format),variadic=true) [attributes=format(printf,1,2)]
// DEFAULT-NEXT: decl[14]: declaration type=char declarator=function(pointer(name=format_argument),params=ptr(char value),variadic=false) [attributes=format_arg(1)]
// DEFAULT-NEXT: decl[15]: declaration type=int declarator=name=common_value [attributes=common,nocommon]
// DEFAULT-NEXT: concrete:
// DEFAULT-NEXT: decl[0]: declaration type=int declarator=function(name=dispatched,params=,variadic=false) [attributes=cpu_dispatch(generic,haswell)]
// DEFAULT-NEXT: decl[1]: declaration type=int declarator=function(name=specific,params=,variadic=false) [attributes=cpu_specific(haswell)]
// DEFAULT-NEXT: decl[2]: declaration type=int declarator=function(name=cloned,params=,variadic=false) [attributes=target_clones("default","arch=x86-64-v2")]
// DEFAULT-NEXT: decl[3]: declaration type=int declarator=function(name=indirect,params=,variadic=false) [attributes=ifunc("resolver")]
// DEFAULT-NEXT: decl[4]: declaration type=int declarator=name=imported [attributes=dllimport]
// DEFAULT-NEXT: decl[5]: declaration type=int [storage=extern] declarator=name=weak_platform [attributes=weak_import]
// DEFAULT-NEXT: decl[6]: declaration type=int declarator=function(name=calling_convention,params=,variadic=false) [attributes=stdcall,nomips16]
// DEFAULT-NEXT: decl[7]: declaration type=int declarator=name=platform_api [attributes=availability(macos,introduced = 12.0)]
// DEFAULT-NEXT: decl[8]: typedef name=vector_type type=int [attributes=ext_vector_type(2)]
// DEFAULT-NEXT: decl[9]: declaration type=int declarator=name=ordered [attributes=scalar_storage_order("big-endian")]
// DEFAULT-NEXT: decl[10]: union name=union_value
// DEFAULT-NEXT: decl[11]: struct name=ms_platform_struct
// DEFAULT-NEXT: decl[12]: struct name=gcc_platform_struct
// DEFAULT-NEXT: decl[13]: declaration type=int declarator=function(name=formatted,params=ptr(char format),variadic=true) [attributes=format(printf,1,2)]
// DEFAULT-NEXT: decl[14]: declaration type=char declarator=function(pointer(name=format_argument),params=ptr(char value),variadic=false) [attributes=format_arg(1)]
// DEFAULT-NEXT: decl[15]: declaration type=int declarator=name=common_value [attributes=common,nocommon]
// SLATE-FILECHECK-END DEFAULT
