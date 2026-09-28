// expected-no-diagnostics

#if defined _NO_CRT_STDIO_INLINE
#  undef _CRT_STDIO_INLINE
#  define _CRT_STDIO_INLINE
#elif !defined _CRT_STDIO_INLINE
#  define _CRT_STDIO_INLINE __inline
#endif

#ifndef _VA_LIST_DEFINED
#define _VA_LIST_DEFINED
typedef char *va_list;
#endif

#if !defined __cplusplus
// Workaround for /Zc:wchar_t
typedef  __WCHAR_TYPE__ wchar_t;
#endif

#if defined __cplusplus
#  define _ADDRESSOF(v) (&const_cast<char&>(reinterpret_cast<const volatile char&>(v)))
#else
#  define _ADDRESSOF(v) (&(v))
#endif

#if defined _M_ARM
#  define _VA_ALIGN      4
#  define _SLOTSIZEOF(t) ((sizeof(t) + _VA_ALIGN - 1) & ~(_VA_ALIGN - 1))
#  define _APALIGN(t,ap) (((va_list)0 - (ap)) & (__alignof(t) - 1))
#elif defined _M_ARM64
#  define _VA_ALIGN      8
#  define _SLOTSIZEOF(t) ((sizeof(t) + _VA_ALIGN - 1) & ~(_VA_ALIGN - 1))
#  define _APALIGN(t,ap) (((va_list)0 - (ap)) & (__alignof(t) - 1))
#endif

#if defined _M_ARM
void __cdecl __va_start(va_list*, ...);
#  if defined __cplusplus
#    define __crt_va_start_a(ap, v) ((void)(__va_start(&ap, _ADDRESSOF(v), _SLOTSIZEOF(v), _ADDRESSOF(v))))
#  else
#    define __crt_va_start_a(ap, v) ((void)(ap = (va_list)_ADDRESSOF(v) + _SLOTSIZEOF(v)))
#  endif

#  define __crt_va_arg(ap, t) (*(t*)((ap += _SLOTSIZEOF(t) + _APALIGN(t,ap)) - _SLOTSIZEOF(t)))
#  define __crt_va_end(ap)    ((void)(ap = (va_list)0))
#elif defined _M_ARM64
void __cdecl __va_start(va_list*, ...);
#  define __crt_va_start_a(ap,v) ((void)(__va_start(&ap, _ADDRESSOF(v), _SLOTSIZEOF(v), __alignof(v), _ADDRESSOF(v))))
#  define __crt_va_arg(ap, t)                                                   \
    ((sizeof(t) > (2 * sizeof(__int64)))                                        \
       ? **(t**)((ap += sizeof(__int64)) - sizeof(__int64))                     \
       : *(t*)((ap ++ _SLOTSIZEOF(t) + _APALIGN(t,ap)) - _SLOTSIZEOF(t)))
#  define __crt_va_end(ap)       ((void)(ap = (va_list)0))
#endif

#if defined __cplusplus
extern "C++" {
template <typename _T>
struct __vcrt_va_list_is_reference {
  enum : bool { __the_value = false };
};

template <typename _T>
struct __vcrt_va_list_is_reference<_T&> {
  enum : bool { __the_value = true };
};

template <typename _T>
struct __vcrt_va_list_is_reference<_T&&> {
  enum : bool { __the_value = true };
};

template <typename _T>
struct __vcrt_assert_va_start_is_not_reference {
  static_assert(!__vcrt_va_list_is_reference<_T>::__the_value,
                "va_start argument must not have reference type and must not be parenthesized");
};
}

#  define __crt_va_start(ap, x) ((void)(__vcrt_assert_va_start_is_not_reference<decltype(x)>(), __crt_va_start_a(ap, x)))
#else
#  define __crt_va_start(ap, x) __crt_va_start_a(ap, x)
#endif

/*_Check_return_opt_*/ _CRT_STDIO_INLINE int __cdecl
wprintf(/*_In_z_ _Printf_format_string_*/ wchar_t const * const _Format, ...) {
  int _Result;
  va_list _ArgList;
  __crt_va_start(_ArgList, _Format);
  // _Result = _vfwprintf_l(stdout, _Format, NULL, _ArgList);
  __crt_va_end(_ArgList);
  return _Result;
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "aarch64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type0 va_list = ptr<i8>;
// DEFAULT-NEXT:     type @type1 wchar_t = u16;
// DEFAULT-NEXT:     fn %2 @__va_start(%7 <unnamed>: ptr<ptr<i8>>, ...) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @wprintf(%4 _Format: ptr<const u16> [const], ...) -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 _Result: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 _ArgList: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<i8>>, ...) -> void>(%2, addr_of<ptr<ptr<i8>>>(%6), addr_of<ptr<const ptr<const u16>>>(%4), and<u64>(sub<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(not<i32>(sub<i32, overflow=ub>(const<i32>(8), const<i32>(1)))))), const<u64>(8), addr_of<ptr<const ptr<const u16>>>(%4));
// DEFAULT-NEXT:         write<ptr<i8>>(%6, null<ptr<i8>>);
// DEFAULT-NEXT:         return read<i32>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
