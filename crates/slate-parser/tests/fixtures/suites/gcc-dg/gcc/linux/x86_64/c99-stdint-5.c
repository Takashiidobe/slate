/* Verify that GCC's internal notions of types in <stdint.h> agree
   with any system header (which GCC will use by default for hosted
   compilations).  */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1999 -pedantic-errors" } */
/* { dg-additional-options "-DSIGNAL_SUPPRESS" { target { ! signal } } } */

#include <stdint.h>
#ifndef SIGNAL_SUPPRESS
#include <signal.h>
#endif

#define CHECK_TYPES(TYPE1, TYPE2) \
  do { TYPE1 a; TYPE2 *b = &a; TYPE2 c; TYPE1 *d = &c; } while (0)

void
check_types (void)
{
#ifdef __INT8_TYPE__
  CHECK_TYPES(__INT8_TYPE__, int8_t);
#endif
#ifdef __INT16_TYPE__
  CHECK_TYPES(__INT16_TYPE__, int16_t);
#endif
#ifdef __INT32_TYPE__
  CHECK_TYPES(__INT32_TYPE__, int32_t);
#endif
#ifdef __INT64_TYPE__
  CHECK_TYPES(__INT64_TYPE__, int64_t);
#endif
#ifdef __UINT8_TYPE__
  CHECK_TYPES(__UINT8_TYPE__, uint8_t);
#endif
#ifdef __UINT16_TYPE__
  CHECK_TYPES(__UINT16_TYPE__, uint16_t);
#endif
#ifdef __UINT32_TYPE__
  CHECK_TYPES(__UINT32_TYPE__, uint32_t);
#endif
#ifdef __UINT64_TYPE__
  CHECK_TYPES(__UINT64_TYPE__, uint64_t);
#endif
  CHECK_TYPES(__INT_LEAST8_TYPE__, int_least8_t);
  CHECK_TYPES(__INT_LEAST16_TYPE__, int_least16_t);
  CHECK_TYPES(__INT_LEAST32_TYPE__, int_least32_t);
  CHECK_TYPES(__INT_LEAST64_TYPE__, int_least64_t);
  CHECK_TYPES(__UINT_LEAST8_TYPE__, uint_least8_t);
  CHECK_TYPES(__UINT_LEAST16_TYPE__, uint_least16_t);
  CHECK_TYPES(__UINT_LEAST32_TYPE__, uint_least32_t);
  CHECK_TYPES(__UINT_LEAST64_TYPE__, uint_least64_t);
  CHECK_TYPES(__INT_FAST8_TYPE__, int_fast8_t);
  CHECK_TYPES(__INT_FAST16_TYPE__, int_fast16_t);
  CHECK_TYPES(__INT_FAST32_TYPE__, int_fast32_t);
  CHECK_TYPES(__INT_FAST64_TYPE__, int_fast64_t);
  CHECK_TYPES(__UINT_FAST8_TYPE__, uint_fast8_t);
  CHECK_TYPES(__UINT_FAST16_TYPE__, uint_fast16_t);
  CHECK_TYPES(__UINT_FAST32_TYPE__, uint_fast32_t);
  CHECK_TYPES(__UINT_FAST64_TYPE__, uint_fast64_t);
#ifdef __INTPTR_TYPE__
  CHECK_TYPES(__INTPTR_TYPE__, intptr_t);
#endif
#ifdef __UINTPTR_TYPE__
  CHECK_TYPES(__UINTPTR_TYPE__, uintptr_t);
#endif
  CHECK_TYPES(__INTMAX_TYPE__, intmax_t);
  CHECK_TYPES(__UINTMAX_TYPE__, uintmax_t);
#ifndef SIGNAL_SUPPRESS
  CHECK_TYPES(__SIG_ATOMIC_TYPE__, sig_atomic_t);
#endif
}

// SLATE-FILECHECK-STD DEFAULT iso9899:1999
// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
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
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type0 __int8_t = i8;
// DEFAULT-NEXT:     type @type1 __uint8_t = u8;
// DEFAULT-NEXT:     type @type2 __int16_t = i16;
// DEFAULT-NEXT:     type @type3 __uint16_t = u16;
// DEFAULT-NEXT:     type @type4 __int32_t = i32;
// DEFAULT-NEXT:     type @type5 __uint32_t = u32;
// DEFAULT-NEXT:     type @type6 __int64_t = i64;
// DEFAULT-NEXT:     type @type7 __uint64_t = u64;
// DEFAULT-NEXT:     type @type8 __int_least8_t = i8;
// DEFAULT-NEXT:     type @type9 __uint_least8_t = u8;
// DEFAULT-NEXT:     type @type10 __int_least16_t = i16;
// DEFAULT-NEXT:     type @type11 __uint_least16_t = u16;
// DEFAULT-NEXT:     type @type12 __int_least32_t = i32;
// DEFAULT-NEXT:     type @type13 __uint_least32_t = u32;
// DEFAULT-NEXT:     type @type14 __int_least64_t = i64;
// DEFAULT-NEXT:     type @type15 __uint_least64_t = u64;
// DEFAULT-NEXT:     type @type16 __intmax_t = i64;
// DEFAULT-NEXT:     type @type17 __uintmax_t = u64;
// DEFAULT-NEXT:     type @type18 __sig_atomic_t = i32;
// DEFAULT-NEXT:     type @type19 int8_t = i8;
// DEFAULT-NEXT:     type @type20 int16_t = i16;
// DEFAULT-NEXT:     type @type21 int32_t = i32;
// DEFAULT-NEXT:     type @type22 int64_t = i64;
// DEFAULT-NEXT:     type @type23 uint8_t = u8;
// DEFAULT-NEXT:     type @type24 uint16_t = u16;
// DEFAULT-NEXT:     type @type25 uint32_t = u32;
// DEFAULT-NEXT:     type @type26 uint64_t = u64;
// DEFAULT-NEXT:     type @type27 int_least8_t = i8;
// DEFAULT-NEXT:     type @type28 int_least16_t = i16;
// DEFAULT-NEXT:     type @type29 int_least32_t = i32;
// DEFAULT-NEXT:     type @type30 int_least64_t = i64;
// DEFAULT-NEXT:     type @type31 uint_least8_t = u8;
// DEFAULT-NEXT:     type @type32 uint_least16_t = u16;
// DEFAULT-NEXT:     type @type33 uint_least32_t = u32;
// DEFAULT-NEXT:     type @type34 uint_least64_t = u64;
// DEFAULT-NEXT:     type @type35 int_fast8_t = i8;
// DEFAULT-NEXT:     type @type36 int_fast16_t = i64;
// DEFAULT-NEXT:     type @type37 int_fast32_t = i64;
// DEFAULT-NEXT:     type @type38 int_fast64_t = i64;
// DEFAULT-NEXT:     type @type39 uint_fast8_t = u8;
// DEFAULT-NEXT:     type @type40 uint_fast16_t = u64;
// DEFAULT-NEXT:     type @type41 uint_fast32_t = u64;
// DEFAULT-NEXT:     type @type42 uint_fast64_t = u64;
// DEFAULT-NEXT:     type @type43 intptr_t = i64;
// DEFAULT-NEXT:     type @type44 uintptr_t = u64;
// DEFAULT-NEXT:     type @type45 intmax_t = i64;
// DEFAULT-NEXT:     type @type46 uintmax_t = u64;
// DEFAULT-NEXT:     type @type47 sig_atomic_t = i32;
// DEFAULT-NEXT:     fn %48 @check_types() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %165
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %49 a: i8 [storage=automatic];
// DEFAULT-NEXT:                 let %50 b: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(%49);
// DEFAULT-NEXT:                 let %51 c: i8 [storage=automatic];
// DEFAULT-NEXT:                 let %52 d: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(%51);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %166
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %53 a: i16 [storage=automatic];
// DEFAULT-NEXT:                 let %54 b: ptr<i16> [storage=automatic] = addr_of<ptr<i16>>(%53);
// DEFAULT-NEXT:                 let %55 c: i16 [storage=automatic];
// DEFAULT-NEXT:                 let %56 d: ptr<i16> [storage=automatic] = addr_of<ptr<i16>>(%55);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %167
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %57 a: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %58 b: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%57);
// DEFAULT-NEXT:                 let %59 c: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %60 d: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%59);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %168
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %61 a: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %62 b: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%61);
// DEFAULT-NEXT:                 let %63 c: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %64 d: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%63);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %169
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %65 a: u8 [storage=automatic];
// DEFAULT-NEXT:                 let %66 b: ptr<u8> [storage=automatic] = addr_of<ptr<u8>>(%65);
// DEFAULT-NEXT:                 let %67 c: u8 [storage=automatic];
// DEFAULT-NEXT:                 let %68 d: ptr<u8> [storage=automatic] = addr_of<ptr<u8>>(%67);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %170
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %69 a: u16 [storage=automatic];
// DEFAULT-NEXT:                 let %70 b: ptr<u16> [storage=automatic] = addr_of<ptr<u16>>(%69);
// DEFAULT-NEXT:                 let %71 c: u16 [storage=automatic];
// DEFAULT-NEXT:                 let %72 d: ptr<u16> [storage=automatic] = addr_of<ptr<u16>>(%71);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %171
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %73 a: u32 [storage=automatic];
// DEFAULT-NEXT:                 let %74 b: ptr<u32> [storage=automatic] = addr_of<ptr<u32>>(%73);
// DEFAULT-NEXT:                 let %75 c: u32 [storage=automatic];
// DEFAULT-NEXT:                 let %76 d: ptr<u32> [storage=automatic] = addr_of<ptr<u32>>(%75);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %172
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %77 a: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %78 b: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%77);
// DEFAULT-NEXT:                 let %79 c: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %80 d: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%79);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %173
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %81 a: i8 [storage=automatic];
// DEFAULT-NEXT:                 let %82 b: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(%81);
// DEFAULT-NEXT:                 let %83 c: i8 [storage=automatic];
// DEFAULT-NEXT:                 let %84 d: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(%83);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %174
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %85 a: i16 [storage=automatic];
// DEFAULT-NEXT:                 let %86 b: ptr<i16> [storage=automatic] = addr_of<ptr<i16>>(%85);
// DEFAULT-NEXT:                 let %87 c: i16 [storage=automatic];
// DEFAULT-NEXT:                 let %88 d: ptr<i16> [storage=automatic] = addr_of<ptr<i16>>(%87);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %175
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %89 a: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %90 b: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%89);
// DEFAULT-NEXT:                 let %91 c: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %92 d: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%91);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %176
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %93 a: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %94 b: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%93);
// DEFAULT-NEXT:                 let %95 c: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %96 d: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%95);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %177
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %97 a: u8 [storage=automatic];
// DEFAULT-NEXT:                 let %98 b: ptr<u8> [storage=automatic] = addr_of<ptr<u8>>(%97);
// DEFAULT-NEXT:                 let %99 c: u8 [storage=automatic];
// DEFAULT-NEXT:                 let %100 d: ptr<u8> [storage=automatic] = addr_of<ptr<u8>>(%99);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %178
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %101 a: u16 [storage=automatic];
// DEFAULT-NEXT:                 let %102 b: ptr<u16> [storage=automatic] = addr_of<ptr<u16>>(%101);
// DEFAULT-NEXT:                 let %103 c: u16 [storage=automatic];
// DEFAULT-NEXT:                 let %104 d: ptr<u16> [storage=automatic] = addr_of<ptr<u16>>(%103);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %179
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %105 a: u32 [storage=automatic];
// DEFAULT-NEXT:                 let %106 b: ptr<u32> [storage=automatic] = addr_of<ptr<u32>>(%105);
// DEFAULT-NEXT:                 let %107 c: u32 [storage=automatic];
// DEFAULT-NEXT:                 let %108 d: ptr<u32> [storage=automatic] = addr_of<ptr<u32>>(%107);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %180
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %109 a: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %110 b: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%109);
// DEFAULT-NEXT:                 let %111 c: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %112 d: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%111);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %181
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %113 a: i8 [storage=automatic];
// DEFAULT-NEXT:                 let %114 b: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(%113);
// DEFAULT-NEXT:                 let %115 c: i8 [storage=automatic];
// DEFAULT-NEXT:                 let %116 d: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(%115);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %182
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %117 a: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %118 b: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%117);
// DEFAULT-NEXT:                 let %119 c: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %120 d: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%119);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %183
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %121 a: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %122 b: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%121);
// DEFAULT-NEXT:                 let %123 c: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %124 d: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%123);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %184
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %125 a: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %126 b: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%125);
// DEFAULT-NEXT:                 let %127 c: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %128 d: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%127);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %185
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %129 a: u8 [storage=automatic];
// DEFAULT-NEXT:                 let %130 b: ptr<u8> [storage=automatic] = addr_of<ptr<u8>>(%129);
// DEFAULT-NEXT:                 let %131 c: u8 [storage=automatic];
// DEFAULT-NEXT:                 let %132 d: ptr<u8> [storage=automatic] = addr_of<ptr<u8>>(%131);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %186
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %133 a: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %134 b: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%133);
// DEFAULT-NEXT:                 let %135 c: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %136 d: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%135);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %187
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %137 a: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %138 b: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%137);
// DEFAULT-NEXT:                 let %139 c: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %140 d: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%139);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %188
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %141 a: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %142 b: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%141);
// DEFAULT-NEXT:                 let %143 c: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %144 d: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%143);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %189
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %145 a: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %146 b: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%145);
// DEFAULT-NEXT:                 let %147 c: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %148 d: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%147);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %190
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %149 a: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %150 b: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%149);
// DEFAULT-NEXT:                 let %151 c: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %152 d: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%151);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %191
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %153 a: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %154 b: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%153);
// DEFAULT-NEXT:                 let %155 c: i64 [storage=automatic];
// DEFAULT-NEXT:                 let %156 d: ptr<i64> [storage=automatic] = addr_of<ptr<i64>>(%155);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %192
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %157 a: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %158 b: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%157);
// DEFAULT-NEXT:                 let %159 c: u64 [storage=automatic];
// DEFAULT-NEXT:                 let %160 d: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%159);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %193
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %161 a: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %162 b: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%161);
// DEFAULT-NEXT:                 let %163 c: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %164 d: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%163);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
