/* RUN: %clang_cc1 -std=c99 -ffreestanding -triple x86_64-unknown-linux -fsyntax-only -verify -pedantic -Wno-c11-extensions %s
   RUN: %clang_cc1 -std=c99 -ffreestanding -triple x86_64-unknown-win32 -fms-compatibility -fsyntax-only -verify -pedantic -Wno-c11-extensions %s
   RUN: %clang_cc1 -std=c11 -ffreestanding -fsyntax-only -verify -pedantic %s
   RUN: %clang_cc1 -std=c17 -ffreestanding -fsyntax-only -verify -pedantic %s
   RUN: %clang_cc1 -std=c2x -ffreestanding -fsyntax-only -verify -pedantic %s
 */
// expected-no-diagnostics

/* WG14 DR209: partial
 * Problem implementing INTN_C macros
 */
#include <stdint.h>

#if INT8_C(0) != 0
#error "uh oh"
#elif INT16_C(0) != 0
#error "uh oh"
#elif INT32_C(0) != 0
#error "uh oh"
#elif INT64_C(0) != 0LL
#error "uh oh"
#elif UINT8_C(0) != 0U
#error "uh oh"
#elif UINT16_C(0) != 0U
#error "uh oh"
#elif UINT32_C(0) != 0U
#error "uh oh"
#elif UINT64_C(0) != 0ULL
#error "uh oh"
#endif

void dr209(void) {
  (void)_Generic(INT8_C(0), __typeof__(+(int_least8_t){0}) : 1);
  (void)_Generic(INT16_C(0), __typeof__(+(int_least16_t){0}) : 1);
  (void)_Generic(INT32_C(0), __typeof__(+(int_least32_t){0}) : 1);
  (void)_Generic(INT64_C(0), __typeof__(+(int_least64_t){0}) : 1);
  // The type of the expanded value in both of these cases should be 'int',
  //
  // C99 7.18.4p3: The type of the expression shall have the same type as would
  // an expression of the corresponding type converted according to the integer
  // promotions.
  //
  // C99 7.18.4.1p1: The macro UINTN_C(value) shall expand to an integer
  // constant expression corresponding to the type uint_leastN_t.
  //
  // C99 7.18.1.2p2: The typedef name uint_leastN_t designates an unsigned
  // integer type with a width of at least N, ...
  //
  // So the value's type is the same underlying type as uint_leastN_t, which is
  // unsigned char for uint_least8_t, and unsigned short for uint_least16_t,
  // but then the value undergoes integer promotions which would convert both
  // of those types to int.
  //
  (void)_Generic(UINT8_C(0), __typeof__(+(uint_least8_t){0}) : 1);
  (void)_Generic(UINT16_C(0), __typeof__(+(uint_least16_t){0}) : 1);
  (void)_Generic(UINT32_C(0), __typeof__(+(uint_least32_t){0}) : 1);
  (void)_Generic(UINT64_C(0), __typeof__(+(uint_least64_t){0}) : 1);
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c99
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT -Wno-c11-extensions

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
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
// DEFAULT-NEXT:     type @type[[TYPE_int_least8_t:[0-9]+]] int_least8_t = i8;
// DEFAULT-NEXT:     type @type[[TYPE_int_least16_t:[0-9]+]] int_least16_t = i16;
// DEFAULT-NEXT:     type @type[[TYPE_int_least32_t:[0-9]+]] int_least32_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE_int_least64_t:[0-9]+]] int_least64_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_uint_least8_t:[0-9]+]] uint_least8_t = u8;
// DEFAULT-NEXT:     type @type[[TYPE_uint_least16_t:[0-9]+]] uint_least16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE_uint_least32_t:[0-9]+]] uint_least32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE_uint_least64_t:[0-9]+]] uint_least64_t = u64;
// DEFAULT-NEXT:     fn %[[VALUE_dr209:[0-9]+]] @dr209() -> void [linkage=external] [fallthrough=ret_void] {
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
