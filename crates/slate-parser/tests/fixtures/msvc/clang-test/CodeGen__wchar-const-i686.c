// This should pass for any endianness combination of host and target.

// This bit is taken from Sema/wchar.c so we can avoid the wchar.h include.
typedef __WCHAR_TYPE__ wchar_t;
#if defined(_WIN32) || defined(_M_IX86) || defined(__CYGWIN__) \
  || defined(_M_X64) || defined(SHORT_WCHAR)
  #define WCHAR_T_TYPE unsigned short
#elif defined(__sun)
  #define WCHAR_T_TYPE long
#else /* Solaris. */
  #define WCHAR_T_TYPE int
#endif


extern void foo(const wchar_t* p);
int main (int argc, const char * argv[])
{
 foo(L"This is some text");
 return 0;
}

// SLATE-FILECHECK-FLAVOR clang
// SLATE-FILECHECK-ARGS -target=i686-pc-windows-msvc
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 4;
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
// DEFAULT-NEXT:     type @type0 wchar_t = u16;
// DEFAULT-NEXT:     global %6 .str6: array<u16, 18> [storage=static] = code_units<array<u16, 18>>([84, 104, 105, 115, 32, 105, 115, 32, 115, 111, 109, 101, 32, 116, 101, 120, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @foo(%5 p: ptr<const u16>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @main(%3 argc: i32, %4 argv: ptr<ptr<const i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const u16>) -> void>(%1, pointer_cast<ptr<const u16>, reason=arg>(array_decay<ptr<u16>, length=Some(18)>(%6)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
