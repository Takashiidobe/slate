
/* WG14 N629: yes
 * integer constant type rules
 */

// expected-no-diagnostics

void test_decimal_constants(void) {
  // Easy cases where the value fits into the type you'd expect.
  (void)_Generic(2,    int : 1);
  (void)_Generic(2u,   unsigned int : 1);
  (void)_Generic(2l,   long : 1);
  (void)_Generic(2ul,  unsigned long : 1);
  (void)_Generic(2ll,  long long : 1);
  (void)_Generic(2ull, unsigned long long : 1);

#if __INT_WIDTH__ == 16
  #if __LONG_WIDTH__ > 16
    (void)_Generic(65536, long : 1);
    (void)_Generic(65536U, unsigned long : 1);
  #else
    (void)_Generic(65536, long long : 1);
    (void)_Generic(65536U, unsigned long : 1);
  #endif // __LONG_WIDTH__ > 16
#elif __INT_WIDTH__ == 32
  #if __LONG_WIDTH__ > 32
    (void)_Generic(4294967296, long : 1);
    (void)_Generic(4294967296U, unsigned long : 1);
  #else
    (void)_Generic(4294967296, long long : 1);
    (void)_Generic(4294967296U, unsigned long long : 1);
  #endif // __LONG_WIDTH__ > 32
#endif

#if __LONG_WIDTH__ > 32
  (void)_Generic(4294967296L, long : 1);
  (void)_Generic(4294967296U, unsigned long : 1);
#else
  (void)_Generic(4294967296L, long long : 1);
  (void)_Generic(4294967296U, unsigned long long : 1);
#endif
}

void test_octal_constants(void) {
  (void)_Generic(02,    int : 1);
  (void)_Generic(02u,   unsigned int : 1);
  (void)_Generic(02l,   long : 1);
  (void)_Generic(02ul,  unsigned long : 1);
  (void)_Generic(02ll,  long long : 1);
  (void)_Generic(02ull, unsigned long long : 1);

#if __INT_WIDTH__ == 16
  #if __LONG_WIDTH__ > 16
    (void)_Generic(0200000, long : 1);
    (void)_Generic(0200000U, unsigned long : 1);
  #else
    (void)_Generic(0200000, long long : 1);
    (void)_Generic(0200000U, unsigned long : 1);
  #endif // __LONG_WIDTH__ > 16
#elif __INT_WIDTH__ == 32
  #if __LONG_WIDTH__ > 32
    (void)_Generic(040000000000, long : 1);
    (void)_Generic(040000000000U, unsigned long : 1);
  #else
    (void)_Generic(040000000000, long long : 1);
    (void)_Generic(040000000000U, unsigned long long : 1);
  #endif // __LONG_WIDTH__ > 32
#endif

#if __LONG_WIDTH__ > 32
  (void)_Generic(040000000000L, long : 1);
  (void)_Generic(040000000000U, unsigned long : 1);
#else
  (void)_Generic(040000000000L, long long : 1);
  (void)_Generic(040000000000U, unsigned long long : 1);
#endif
}

void test_hexadecimal_constants(void) {
  (void)_Generic(0x2,    int : 1);
  (void)_Generic(0x2u,   unsigned int : 1);
  (void)_Generic(0x2l,   long : 1);
  (void)_Generic(0x2ul,  unsigned long : 1);
  (void)_Generic(0x2ll,  long long : 1);
  (void)_Generic(0x2ull, unsigned long long : 1);

#if __INT_WIDTH__ == 16
  #if __LONG_WIDTH__ > 16
    (void)_Generic(0x10000, long : 1);
    (void)_Generic(0x10000U, unsigned long : 1);
  #else
    (void)_Generic(0x10000, long long : 1);
    (void)_Generic(0x10000U, unsigned long : 1);
  #endif // __LONG_WIDTH__ > 16
#elif __INT_WIDTH__ == 32
  #if __LONG_WIDTH__ > 32
    (void)_Generic(0x100000000, long : 1);
    (void)_Generic(0x100000000U, unsigned long : 1);
  #else
    (void)_Generic(0x100000000, long long : 1);
    (void)_Generic(0x100000000U, unsigned long long : 1);
  #endif // __LONG_WIDTH__ > 32
#endif

#if __LONG_WIDTH__ > 32
  (void)_Generic(0x100000000L, long : 1);
  (void)_Generic(0x100000000U, unsigned long : 1);
#else
  (void)_Generic(0x100000000L, long long : 1);
  (void)_Generic(0x100000000U, unsigned long long : 1);
#endif
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c17

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
// DEFAULT-NEXT:     fn %[[VALUE_test_decimal_constants:[0-9]+]] @test_decimal_constants() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         const<i32>(1);
// DEFAULT-NEXT:         const<i32>(1);
// DEFAULT-NEXT:         const<i32>(1);
// DEFAULT-NEXT:         const<i32>(1);
// DEFAULT-NEXT:         const<i32>(1);
// DEFAULT-NEXT:         const<i32>(1);
// DEFAULT-NEXT:         const<i32>(1);
// DEFAULT-NEXT:         const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_octal_constants:[0-9]+]] @test_octal_constants() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         const<i32>(1);
// DEFAULT-NEXT:         const<i32>(1);
// DEFAULT-NEXT:         const<i32>(1);
// DEFAULT-NEXT:         const<i32>(1);
// DEFAULT-NEXT:         const<i32>(1);
// DEFAULT-NEXT:         const<i32>(1);
// DEFAULT-NEXT:         const<i32>(1);
// DEFAULT-NEXT:         const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_hexadecimal_constants:[0-9]+]] @test_hexadecimal_constants() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         const<i32>(1);
// DEFAULT-NEXT:         const<i32>(1);
// DEFAULT-NEXT:         const<i32>(1);
// DEFAULT-NEXT:         const<i32>(1);
// DEFAULT-NEXT:         const<i32>(1);
// DEFAULT-NEXT:         const<i32>(1);
// DEFAULT-NEXT:         const<i32>(1);
// DEFAULT-NEXT:         const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
