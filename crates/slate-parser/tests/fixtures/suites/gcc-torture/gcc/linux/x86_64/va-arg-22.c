#include <stdarg.h>

extern void abort(void);
extern void exit(int);

void bar(int n, int c) {
  static int lastn = -1, lastc = -1;

  if (lastn != n) {
    if (lastc != lastn)
      abort();
    lastc = 0;
    lastn = n;
  }

  if (c != (char)(lastc ^ (n << 3)))
    abort();
  lastc++;
}

#define D(N)                                                                   \
  typedef struct {                                                             \
    char x[N];                                                                 \
  } A##N;
D(0)
D(1)
D(2)
D(3)
D(4)
D(5) D(6) D(7) D(8) D(9) D(10) D(11) D(12) D(13) D(14) D(15) D(16) D(31) D(32)
    D(35) D(72)
#undef D

        void foo(int size, ...) {
#define D(N) A##N a##N;
  D(0)
  D(1)
  D(2)
  D(3)
  D(4)
  D(5) D(6) D(7) D(8) D(9) D(10) D(11) D(12) D(13) D(14) D(15) D(16) D(31) D(32)
      D(35) D(72)
#undef D
          va_list ap;
  int             i;

  if (size != 21)
    abort();
  va_start(ap, size);
#define D(N)                                                                   \
  a##N = va_arg(ap, typeof(a##N));                                             \
  for (i = 0; i < N; i++)                                                      \
    bar(N, a##N.x[i]);
  D(0)
  D(1)
  D(2)
  D(3)
  D(4)
  D(5) D(6) D(7) D(8) D(9) D(10) D(11) D(12) D(13) D(14) D(15) D(16) D(31) D(32)
      D(35) D(72)
#undef D
          va_end(ap);
}

int main(void) {
#define D(N) A##N a##N;
  D(0)
  D(1)
  D(2)
  D(3)
  D(4)
  D(5) D(6) D(7) D(8) D(9) D(10) D(11) D(12) D(13) D(14) D(15) D(16) D(31) D(32)
      D(35) D(72)
#undef D
          int i;

#define D(N)                                                                   \
  for (i = 0; i < N; i++)                                                      \
    a##N.x[i] = i ^ (N << 3);
  D(0)
  D(1)
  D(2)
  D(3)
  D(4)
  D(5) D(6) D(7) D(8) D(9) D(10) D(11) D(12) D(13) D(14) D(15) D(16) D(31) D(32)
      D(35) D(72)
#undef D

          foo(21
#define D(N) , a##N
              D(0) D(1) D(2) D(3) D(4) D(5) D(6) D(7) D(8) D(9) D(10) D(11)
                  D(12) D(13) D(14) D(15) D(16) D(31) D(32) D(35) D(72)
#undef D
          );
  exit(0);
}


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
// DEFAULT-NEXT:     type @type0 __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type1 va_list = va_list;
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 0>;
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type3 A0 = @type2;
// DEFAULT-NEXT:     type @type4 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 1>;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type5 A1 = @type4;
// DEFAULT-NEXT:     type @type6 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 2>;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type7 A2 = @type6;
// DEFAULT-NEXT:     type @type8 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 3>;
// DEFAULT-NEXT:     } [size=3, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type9 A3 = @type8;
// DEFAULT-NEXT:     type @type10 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 4>;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type11 A4 = @type10;
// DEFAULT-NEXT:     type @type12 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 5>;
// DEFAULT-NEXT:     } [size=5, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type13 A5 = @type12;
// DEFAULT-NEXT:     type @type14 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 6>;
// DEFAULT-NEXT:     } [size=6, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type15 A6 = @type14;
// DEFAULT-NEXT:     type @type16 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 7>;
// DEFAULT-NEXT:     } [size=7, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type17 A7 = @type16;
// DEFAULT-NEXT:     type @type18 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 8>;
// DEFAULT-NEXT:     } [size=8, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type19 A8 = @type18;
// DEFAULT-NEXT:     type @type20 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 9>;
// DEFAULT-NEXT:     } [size=9, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type21 A9 = @type20;
// DEFAULT-NEXT:     type @type22 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 10>;
// DEFAULT-NEXT:     } [size=10, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type23 A10 = @type22;
// DEFAULT-NEXT:     type @type24 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 11>;
// DEFAULT-NEXT:     } [size=11, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type25 A11 = @type24;
// DEFAULT-NEXT:     type @type26 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 12>;
// DEFAULT-NEXT:     } [size=12, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type27 A12 = @type26;
// DEFAULT-NEXT:     type @type28 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 13>;
// DEFAULT-NEXT:     } [size=13, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type29 A13 = @type28;
// DEFAULT-NEXT:     type @type30 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 14>;
// DEFAULT-NEXT:     } [size=14, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type31 A14 = @type30;
// DEFAULT-NEXT:     type @type32 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 15>;
// DEFAULT-NEXT:     } [size=15, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type33 A15 = @type32;
// DEFAULT-NEXT:     type @type34 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 16>;
// DEFAULT-NEXT:     } [size=16, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type35 A16 = @type34;
// DEFAULT-NEXT:     type @type36 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 31>;
// DEFAULT-NEXT:     } [size=31, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type37 A31 = @type36;
// DEFAULT-NEXT:     type @type38 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 32>;
// DEFAULT-NEXT:     } [size=32, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type39 A32 = @type38;
// DEFAULT-NEXT:     type @type40 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 35>;
// DEFAULT-NEXT:     } [size=35, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type41 A35 = @type40;
// DEFAULT-NEXT:     type @type42 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 72>;
// DEFAULT-NEXT:     } [size=72, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type43 A72 = @type42;
// DEFAULT-NEXT:     global %7 lastn: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %8 lastc: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @exit(%99 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @bar(%5 n: i32, %6 c: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%7), read<i32>(%5))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%8), read<i32>(%7))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                 write<i32>(%8, const<i32>(0));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%5));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%6), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(xor<i32>(read<i32>(%8), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%5), const<i32>(3))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         let %142: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:         let %143: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%142), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%8, read<i32>(%143));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %51 @foo(%52 size: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %53 a0: @type2 [storage=automatic];
// DEFAULT-NEXT:         let %54 a1: @type4 [storage=automatic];
// DEFAULT-NEXT:         let %55 a2: @type6 [storage=automatic];
// DEFAULT-NEXT:         let %56 a3: @type8 [storage=automatic];
// DEFAULT-NEXT:         let %57 a4: @type10 [storage=automatic];
// DEFAULT-NEXT:         let %58 a5: @type12 [storage=automatic];
// DEFAULT-NEXT:         let %59 a6: @type14 [storage=automatic];
// DEFAULT-NEXT:         let %60 a7: @type16 [storage=automatic];
// DEFAULT-NEXT:         let %61 a8: @type18 [storage=automatic];
// DEFAULT-NEXT:         let %62 a9: @type20 [storage=automatic];
// DEFAULT-NEXT:         let %63 a10: @type22 [storage=automatic];
// DEFAULT-NEXT:         let %64 a11: @type24 [storage=automatic];
// DEFAULT-NEXT:         let %65 a12: @type26 [storage=automatic];
// DEFAULT-NEXT:         let %66 a13: @type28 [storage=automatic];
// DEFAULT-NEXT:         let %67 a14: @type30 [storage=automatic];
// DEFAULT-NEXT:         let %68 a15: @type32 [storage=automatic];
// DEFAULT-NEXT:         let %69 a16: @type34 [storage=automatic];
// DEFAULT-NEXT:         let %70 a31: @type36 [storage=automatic];
// DEFAULT-NEXT:         let %71 a32: @type38 [storage=automatic];
// DEFAULT-NEXT:         let %72 a35: @type40 [storage=automatic];
// DEFAULT-NEXT:         let %73 a72: @type42 [storage=automatic];
// DEFAULT-NEXT:         let %74 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %75 i: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%52), const<i32>(21))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         va_start(%74);
// DEFAULT-NEXT:         write<@type2>(%53, copy<@type2, reason=assign>(va_arg<@type2>(%74)));
// DEFAULT-NEXT:         copy<@type2, reason=assign>(va_arg<@type2>(%74));
// DEFAULT-NEXT:         for %100
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%75, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%75), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %144: i32 [synthetic] = read<i32>(%75);
// DEFAULT-NEXT:                 let %145: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%144), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%75, read<i32>(%145));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%4, const<i32>(0), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(0)>(field0(%53)), read<i32>(%75))))));
// DEFAULT-NEXT:         write<@type4>(%54, copy<@type4, reason=assign>(va_arg<@type4>(%74)));
// DEFAULT-NEXT:         copy<@type4, reason=assign>(va_arg<@type4>(%74));
// DEFAULT-NEXT:         for %101
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%75, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%75), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %146: i32 [synthetic] = read<i32>(%75);
// DEFAULT-NEXT:                 let %147: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%146), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%75, read<i32>(%147));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%4, const<i32>(1), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field0(%54)), read<i32>(%75))))));
// DEFAULT-NEXT:         write<@type6>(%55, copy<@type6, reason=assign>(va_arg<@type6>(%74)));
// DEFAULT-NEXT:         copy<@type6, reason=assign>(va_arg<@type6>(%74));
// DEFAULT-NEXT:         for %102
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%75, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%75), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %148: i32 [synthetic] = read<i32>(%75);
// DEFAULT-NEXT:                 let %149: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%148), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%75, read<i32>(%149));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%4, const<i32>(2), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(field0(%55)), read<i32>(%75))))));
// DEFAULT-NEXT:         write<@type8>(%56, copy<@type8, reason=assign>(va_arg<@type8>(%74)));
// DEFAULT-NEXT:         copy<@type8, reason=assign>(va_arg<@type8>(%74));
// DEFAULT-NEXT:         for %103
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%75, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%75), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %150: i32 [synthetic] = read<i32>(%75);
// DEFAULT-NEXT:                 let %151: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%150), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%75, read<i32>(%151));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%4, const<i32>(3), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(field0(%56)), read<i32>(%75))))));
// DEFAULT-NEXT:         write<@type10>(%57, copy<@type10, reason=assign>(va_arg<@type10>(%74)));
// DEFAULT-NEXT:         copy<@type10, reason=assign>(va_arg<@type10>(%74));
// DEFAULT-NEXT:         for %104
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%75, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%75), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %152: i32 [synthetic] = read<i32>(%75);
// DEFAULT-NEXT:                 let %153: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%152), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%75, read<i32>(%153));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%4, const<i32>(4), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(field0(%57)), read<i32>(%75))))));
// DEFAULT-NEXT:         write<@type12>(%58, copy<@type12, reason=assign>(va_arg<@type12>(%74)));
// DEFAULT-NEXT:         copy<@type12, reason=assign>(va_arg<@type12>(%74));
// DEFAULT-NEXT:         for %105
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%75, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%75), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %154: i32 [synthetic] = read<i32>(%75);
// DEFAULT-NEXT:                 let %155: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%154), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%75, read<i32>(%155));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%4, const<i32>(5), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(field0(%58)), read<i32>(%75))))));
// DEFAULT-NEXT:         write<@type14>(%59, copy<@type14, reason=assign>(va_arg<@type14>(%74)));
// DEFAULT-NEXT:         copy<@type14, reason=assign>(va_arg<@type14>(%74));
// DEFAULT-NEXT:         for %106
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%75, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%75), const<i32>(6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %156: i32 [synthetic] = read<i32>(%75);
// DEFAULT-NEXT:                 let %157: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%156), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%75, read<i32>(%157));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%4, const<i32>(6), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(field0(%59)), read<i32>(%75))))));
// DEFAULT-NEXT:         write<@type16>(%60, copy<@type16, reason=assign>(va_arg<@type16>(%74)));
// DEFAULT-NEXT:         copy<@type16, reason=assign>(va_arg<@type16>(%74));
// DEFAULT-NEXT:         for %107
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%75, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%75), const<i32>(7))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %158: i32 [synthetic] = read<i32>(%75);
// DEFAULT-NEXT:                 let %159: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%158), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%75, read<i32>(%159));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%4, const<i32>(7), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(7)>(field0(%60)), read<i32>(%75))))));
// DEFAULT-NEXT:         write<@type18>(%61, copy<@type18, reason=assign>(va_arg<@type18>(%74)));
// DEFAULT-NEXT:         copy<@type18, reason=assign>(va_arg<@type18>(%74));
// DEFAULT-NEXT:         for %108
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%75, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%75), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %160: i32 [synthetic] = read<i32>(%75);
// DEFAULT-NEXT:                 let %161: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%160), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%75, read<i32>(%161));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%4, const<i32>(8), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field0(%61)), read<i32>(%75))))));
// DEFAULT-NEXT:         write<@type20>(%62, copy<@type20, reason=assign>(va_arg<@type20>(%74)));
// DEFAULT-NEXT:         copy<@type20, reason=assign>(va_arg<@type20>(%74));
// DEFAULT-NEXT:         for %109
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%75, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%75), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %162: i32 [synthetic] = read<i32>(%75);
// DEFAULT-NEXT:                 let %163: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%162), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%75, read<i32>(%163));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%4, const<i32>(9), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field0(%62)), read<i32>(%75))))));
// DEFAULT-NEXT:         write<@type22>(%63, copy<@type22, reason=assign>(va_arg<@type22>(%74)));
// DEFAULT-NEXT:         copy<@type22, reason=assign>(va_arg<@type22>(%74));
// DEFAULT-NEXT:         for %110
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%75, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%75), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %164: i32 [synthetic] = read<i32>(%75);
// DEFAULT-NEXT:                 let %165: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%164), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%75, read<i32>(%165));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%4, const<i32>(10), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(field0(%63)), read<i32>(%75))))));
// DEFAULT-NEXT:         write<@type24>(%64, copy<@type24, reason=assign>(va_arg<@type24>(%74)));
// DEFAULT-NEXT:         copy<@type24, reason=assign>(va_arg<@type24>(%74));
// DEFAULT-NEXT:         for %111
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%75, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%75), const<i32>(11))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %166: i32 [synthetic] = read<i32>(%75);
// DEFAULT-NEXT:                 let %167: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%166), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%75, read<i32>(%167));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%4, const<i32>(11), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(field0(%64)), read<i32>(%75))))));
// DEFAULT-NEXT:         write<@type26>(%65, copy<@type26, reason=assign>(va_arg<@type26>(%74)));
// DEFAULT-NEXT:         copy<@type26, reason=assign>(va_arg<@type26>(%74));
// DEFAULT-NEXT:         for %112
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%75, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%75), const<i32>(12))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %168: i32 [synthetic] = read<i32>(%75);
// DEFAULT-NEXT:                 let %169: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%168), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%75, read<i32>(%169));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%4, const<i32>(12), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(12)>(field0(%65)), read<i32>(%75))))));
// DEFAULT-NEXT:         write<@type28>(%66, copy<@type28, reason=assign>(va_arg<@type28>(%74)));
// DEFAULT-NEXT:         copy<@type28, reason=assign>(va_arg<@type28>(%74));
// DEFAULT-NEXT:         for %113
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%75, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%75), const<i32>(13))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %170: i32 [synthetic] = read<i32>(%75);
// DEFAULT-NEXT:                 let %171: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%170), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%75, read<i32>(%171));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%4, const<i32>(13), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(13)>(field0(%66)), read<i32>(%75))))));
// DEFAULT-NEXT:         write<@type30>(%67, copy<@type30, reason=assign>(va_arg<@type30>(%74)));
// DEFAULT-NEXT:         copy<@type30, reason=assign>(va_arg<@type30>(%74));
// DEFAULT-NEXT:         for %114
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%75, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%75), const<i32>(14))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %172: i32 [synthetic] = read<i32>(%75);
// DEFAULT-NEXT:                 let %173: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%172), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%75, read<i32>(%173));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%4, const<i32>(14), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(14)>(field0(%67)), read<i32>(%75))))));
// DEFAULT-NEXT:         write<@type32>(%68, copy<@type32, reason=assign>(va_arg<@type32>(%74)));
// DEFAULT-NEXT:         copy<@type32, reason=assign>(va_arg<@type32>(%74));
// DEFAULT-NEXT:         for %115
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%75, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%75), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %174: i32 [synthetic] = read<i32>(%75);
// DEFAULT-NEXT:                 let %175: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%174), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%75, read<i32>(%175));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%4, const<i32>(15), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(15)>(field0(%68)), read<i32>(%75))))));
// DEFAULT-NEXT:         write<@type34>(%69, copy<@type34, reason=assign>(va_arg<@type34>(%74)));
// DEFAULT-NEXT:         copy<@type34, reason=assign>(va_arg<@type34>(%74));
// DEFAULT-NEXT:         for %116
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%75, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%75), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %176: i32 [synthetic] = read<i32>(%75);
// DEFAULT-NEXT:                 let %177: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%176), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%75, read<i32>(%177));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%4, const<i32>(16), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(field0(%69)), read<i32>(%75))))));
// DEFAULT-NEXT:         write<@type36>(%70, copy<@type36, reason=assign>(va_arg<@type36>(%74)));
// DEFAULT-NEXT:         copy<@type36, reason=assign>(va_arg<@type36>(%74));
// DEFAULT-NEXT:         for %117
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%75, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%75), const<i32>(31))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %178: i32 [synthetic] = read<i32>(%75);
// DEFAULT-NEXT:                 let %179: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%178), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%75, read<i32>(%179));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%4, const<i32>(31), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%70)), read<i32>(%75))))));
// DEFAULT-NEXT:         write<@type38>(%71, copy<@type38, reason=assign>(va_arg<@type38>(%74)));
// DEFAULT-NEXT:         copy<@type38, reason=assign>(va_arg<@type38>(%74));
// DEFAULT-NEXT:         for %118
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%75, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%75), const<i32>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %180: i32 [synthetic] = read<i32>(%75);
// DEFAULT-NEXT:                 let %181: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%180), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%75, read<i32>(%181));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%4, const<i32>(32), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(32)>(field0(%71)), read<i32>(%75))))));
// DEFAULT-NEXT:         write<@type40>(%72, copy<@type40, reason=assign>(va_arg<@type40>(%74)));
// DEFAULT-NEXT:         copy<@type40, reason=assign>(va_arg<@type40>(%74));
// DEFAULT-NEXT:         for %119
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%75, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%75), const<i32>(35))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %182: i32 [synthetic] = read<i32>(%75);
// DEFAULT-NEXT:                 let %183: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%182), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%75, read<i32>(%183));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%4, const<i32>(35), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(35)>(field0(%72)), read<i32>(%75))))));
// DEFAULT-NEXT:         write<@type42>(%73, copy<@type42, reason=assign>(va_arg<@type42>(%74)));
// DEFAULT-NEXT:         copy<@type42, reason=assign>(va_arg<@type42>(%74));
// DEFAULT-NEXT:         for %120
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%75, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%75), const<i32>(72))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %184: i32 [synthetic] = read<i32>(%75);
// DEFAULT-NEXT:                 let %185: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%184), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%75, read<i32>(%185));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%4, const<i32>(72), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(72)>(field0(%73)), read<i32>(%75))))));
// DEFAULT-NEXT:         va_end(%74);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %76 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %77 a0: @type2 [storage=automatic];
// DEFAULT-NEXT:         let %78 a1: @type4 [storage=automatic];
// DEFAULT-NEXT:         let %79 a2: @type6 [storage=automatic];
// DEFAULT-NEXT:         let %80 a3: @type8 [storage=automatic];
// DEFAULT-NEXT:         let %81 a4: @type10 [storage=automatic];
// DEFAULT-NEXT:         let %82 a5: @type12 [storage=automatic];
// DEFAULT-NEXT:         let %83 a6: @type14 [storage=automatic];
// DEFAULT-NEXT:         let %84 a7: @type16 [storage=automatic];
// DEFAULT-NEXT:         let %85 a8: @type18 [storage=automatic];
// DEFAULT-NEXT:         let %86 a9: @type20 [storage=automatic];
// DEFAULT-NEXT:         let %87 a10: @type22 [storage=automatic];
// DEFAULT-NEXT:         let %88 a11: @type24 [storage=automatic];
// DEFAULT-NEXT:         let %89 a12: @type26 [storage=automatic];
// DEFAULT-NEXT:         let %90 a13: @type28 [storage=automatic];
// DEFAULT-NEXT:         let %91 a14: @type30 [storage=automatic];
// DEFAULT-NEXT:         let %92 a15: @type32 [storage=automatic];
// DEFAULT-NEXT:         let %93 a16: @type34 [storage=automatic];
// DEFAULT-NEXT:         let %94 a31: @type36 [storage=automatic];
// DEFAULT-NEXT:         let %95 a32: @type38 [storage=automatic];
// DEFAULT-NEXT:         let %96 a35: @type40 [storage=automatic];
// DEFAULT-NEXT:         let %97 a72: @type42 [storage=automatic];
// DEFAULT-NEXT:         let %98 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %121
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%98, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%98), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %186: i32 [synthetic] = read<i32>(%98);
// DEFAULT-NEXT:                 let %187: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%186), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%98, read<i32>(%187));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(0)>(field0(%77)), read<i32>(%98))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%98), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(0), const<i32>(3)))));
// DEFAULT-NEXT:         for %122
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%98, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%98), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %188: i32 [synthetic] = read<i32>(%98);
// DEFAULT-NEXT:                 let %189: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%188), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%98, read<i32>(%189));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field0(%78)), read<i32>(%98))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%98), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(3)))));
// DEFAULT-NEXT:         for %123
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%98, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%98), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %190: i32 [synthetic] = read<i32>(%98);
// DEFAULT-NEXT:                 let %191: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%190), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%98, read<i32>(%191));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(field0(%79)), read<i32>(%98))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%98), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(2), const<i32>(3)))));
// DEFAULT-NEXT:         for %124
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%98, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%98), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %192: i32 [synthetic] = read<i32>(%98);
// DEFAULT-NEXT:                 let %193: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%192), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%98, read<i32>(%193));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(field0(%80)), read<i32>(%98))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%98), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(3), const<i32>(3)))));
// DEFAULT-NEXT:         for %125
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%98, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%98), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %194: i32 [synthetic] = read<i32>(%98);
// DEFAULT-NEXT:                 let %195: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%194), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%98, read<i32>(%195));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(field0(%81)), read<i32>(%98))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%98), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(4), const<i32>(3)))));
// DEFAULT-NEXT:         for %126
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%98, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%98), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %196: i32 [synthetic] = read<i32>(%98);
// DEFAULT-NEXT:                 let %197: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%196), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%98, read<i32>(%197));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(field0(%82)), read<i32>(%98))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%98), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(5), const<i32>(3)))));
// DEFAULT-NEXT:         for %127
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%98, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%98), const<i32>(6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %198: i32 [synthetic] = read<i32>(%98);
// DEFAULT-NEXT:                 let %199: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%198), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%98, read<i32>(%199));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(field0(%83)), read<i32>(%98))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%98), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(6), const<i32>(3)))));
// DEFAULT-NEXT:         for %128
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%98, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%98), const<i32>(7))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %200: i32 [synthetic] = read<i32>(%98);
// DEFAULT-NEXT:                 let %201: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%200), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%98, read<i32>(%201));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(7)>(field0(%84)), read<i32>(%98))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%98), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(7), const<i32>(3)))));
// DEFAULT-NEXT:         for %129
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%98, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%98), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %202: i32 [synthetic] = read<i32>(%98);
// DEFAULT-NEXT:                 let %203: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%202), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%98, read<i32>(%203));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field0(%85)), read<i32>(%98))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%98), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(8), const<i32>(3)))));
// DEFAULT-NEXT:         for %130
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%98, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%98), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %204: i32 [synthetic] = read<i32>(%98);
// DEFAULT-NEXT:                 let %205: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%204), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%98, read<i32>(%205));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field0(%86)), read<i32>(%98))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%98), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(9), const<i32>(3)))));
// DEFAULT-NEXT:         for %131
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%98, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%98), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %206: i32 [synthetic] = read<i32>(%98);
// DEFAULT-NEXT:                 let %207: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%206), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%98, read<i32>(%207));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(field0(%87)), read<i32>(%98))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%98), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(10), const<i32>(3)))));
// DEFAULT-NEXT:         for %132
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%98, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%98), const<i32>(11))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %208: i32 [synthetic] = read<i32>(%98);
// DEFAULT-NEXT:                 let %209: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%208), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%98, read<i32>(%209));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(field0(%88)), read<i32>(%98))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%98), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(11), const<i32>(3)))));
// DEFAULT-NEXT:         for %133
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%98, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%98), const<i32>(12))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %210: i32 [synthetic] = read<i32>(%98);
// DEFAULT-NEXT:                 let %211: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%210), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%98, read<i32>(%211));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(12)>(field0(%89)), read<i32>(%98))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%98), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(12), const<i32>(3)))));
// DEFAULT-NEXT:         for %134
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%98, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%98), const<i32>(13))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %212: i32 [synthetic] = read<i32>(%98);
// DEFAULT-NEXT:                 let %213: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%212), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%98, read<i32>(%213));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(13)>(field0(%90)), read<i32>(%98))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%98), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(13), const<i32>(3)))));
// DEFAULT-NEXT:         for %135
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%98, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%98), const<i32>(14))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %214: i32 [synthetic] = read<i32>(%98);
// DEFAULT-NEXT:                 let %215: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%214), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%98, read<i32>(%215));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(14)>(field0(%91)), read<i32>(%98))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%98), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(14), const<i32>(3)))));
// DEFAULT-NEXT:         for %136
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%98, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%98), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %216: i32 [synthetic] = read<i32>(%98);
// DEFAULT-NEXT:                 let %217: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%216), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%98, read<i32>(%217));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(15)>(field0(%92)), read<i32>(%98))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%98), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(15), const<i32>(3)))));
// DEFAULT-NEXT:         for %137
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%98, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%98), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %218: i32 [synthetic] = read<i32>(%98);
// DEFAULT-NEXT:                 let %219: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%218), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%98, read<i32>(%219));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(field0(%93)), read<i32>(%98))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%98), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(16), const<i32>(3)))));
// DEFAULT-NEXT:         for %138
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%98, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%98), const<i32>(31))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %220: i32 [synthetic] = read<i32>(%98);
// DEFAULT-NEXT:                 let %221: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%220), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%98, read<i32>(%221));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%94)), read<i32>(%98))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%98), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(31), const<i32>(3)))));
// DEFAULT-NEXT:         for %139
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%98, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%98), const<i32>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %222: i32 [synthetic] = read<i32>(%98);
// DEFAULT-NEXT:                 let %223: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%222), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%98, read<i32>(%223));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(32)>(field0(%95)), read<i32>(%98))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%98), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(32), const<i32>(3)))));
// DEFAULT-NEXT:         for %140
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%98, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%98), const<i32>(35))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %224: i32 [synthetic] = read<i32>(%98);
// DEFAULT-NEXT:                 let %225: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%224), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%98, read<i32>(%225));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(35)>(field0(%96)), read<i32>(%98))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%98), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(35), const<i32>(3)))));
// DEFAULT-NEXT:         for %141
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%98, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%98), const<i32>(72))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %226: i32 [synthetic] = read<i32>(%98);
// DEFAULT-NEXT:                 let %227: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%226), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%98, read<i32>(%227));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(72)>(field0(%97)), read<i32>(%98))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%98), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(72), const<i32>(3)))));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c) -> void>(%51, const<i32>(21), copy<@type2, reason=vararg>(read<@type2>(%77)), copy<@type4, reason=vararg>(read<@type4>(%78)), copy<@type6, reason=vararg>(read<@type6>(%79)), copy<@type8, reason=vararg>(read<@type8>(%80)), copy<@type10, reason=vararg>(read<@type10>(%81)), copy<@type12, reason=vararg>(read<@type12>(%82)), copy<@type14, reason=vararg>(read<@type14>(%83)), copy<@type16, reason=vararg>(read<@type16>(%84)), copy<@type18, reason=vararg>(read<@type18>(%85)), copy<@type20, reason=vararg>(read<@type20>(%86)), copy<@type22, reason=vararg>(read<@type22>(%87)), copy<@type24, reason=vararg>(read<@type24>(%88)), copy<@type26, reason=vararg>(read<@type26>(%89)), copy<@type28, reason=vararg>(read<@type28>(%90)), copy<@type30, reason=vararg>(read<@type30>(%91)), copy<@type32, reason=vararg>(read<@type32>(%92)), copy<@type34, reason=vararg>(read<@type34>(%93)), copy<@type36, reason=vararg>(read<@type36>(%94)), copy<@type38, reason=vararg>(read<@type38>(%95)), copy<@type40, reason=vararg>(read<@type40>(%96)), copy<@type42, reason=vararg>(read<@type42>(%97)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
