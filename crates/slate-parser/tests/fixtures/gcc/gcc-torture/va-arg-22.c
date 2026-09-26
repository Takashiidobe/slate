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
// DEFAULT-NEXT:     type @type0 va_list = va_list;
// DEFAULT-NEXT:     type @type1 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 0>;
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type2 A0 = @type1;
// DEFAULT-NEXT:     type @type3 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 1>;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type4 A1 = @type3;
// DEFAULT-NEXT:     type @type5 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 2>;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type6 A2 = @type5;
// DEFAULT-NEXT:     type @type7 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 3>;
// DEFAULT-NEXT:     } [size=3, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type8 A3 = @type7;
// DEFAULT-NEXT:     type @type9 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 4>;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type10 A4 = @type9;
// DEFAULT-NEXT:     type @type11 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 5>;
// DEFAULT-NEXT:     } [size=5, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type12 A5 = @type11;
// DEFAULT-NEXT:     type @type13 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 6>;
// DEFAULT-NEXT:     } [size=6, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type14 A6 = @type13;
// DEFAULT-NEXT:     type @type15 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 7>;
// DEFAULT-NEXT:     } [size=7, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type16 A7 = @type15;
// DEFAULT-NEXT:     type @type17 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 8>;
// DEFAULT-NEXT:     } [size=8, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type18 A8 = @type17;
// DEFAULT-NEXT:     type @type19 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 9>;
// DEFAULT-NEXT:     } [size=9, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type20 A9 = @type19;
// DEFAULT-NEXT:     type @type21 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 10>;
// DEFAULT-NEXT:     } [size=10, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type22 A10 = @type21;
// DEFAULT-NEXT:     type @type23 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 11>;
// DEFAULT-NEXT:     } [size=11, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type24 A11 = @type23;
// DEFAULT-NEXT:     type @type25 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 12>;
// DEFAULT-NEXT:     } [size=12, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type26 A12 = @type25;
// DEFAULT-NEXT:     type @type27 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 13>;
// DEFAULT-NEXT:     } [size=13, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type28 A13 = @type27;
// DEFAULT-NEXT:     type @type29 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 14>;
// DEFAULT-NEXT:     } [size=14, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type30 A14 = @type29;
// DEFAULT-NEXT:     type @type31 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 15>;
// DEFAULT-NEXT:     } [size=15, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type32 A15 = @type31;
// DEFAULT-NEXT:     type @type33 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 16>;
// DEFAULT-NEXT:     } [size=16, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type34 A16 = @type33;
// DEFAULT-NEXT:     type @type35 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 31>;
// DEFAULT-NEXT:     } [size=31, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type36 A31 = @type35;
// DEFAULT-NEXT:     type @type37 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 32>;
// DEFAULT-NEXT:     } [size=32, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type38 A32 = @type37;
// DEFAULT-NEXT:     type @type39 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 35>;
// DEFAULT-NEXT:     } [size=35, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type40 A35 = @type39;
// DEFAULT-NEXT:     type @type41 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 72>;
// DEFAULT-NEXT:     } [size=72, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type42 A72 = @type41;
// DEFAULT-NEXT:     global %6 lastn: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %7 lastc: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @exit(%98 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @bar(%4 n: i32, %5 c: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%6), read<i32>(%4))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%7), read<i32>(%6))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%4));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%5), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(xor<i32>(read<i32>(%7), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%4), const<i32>(3))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %141: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:         let %142: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%141), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%7, read<i32>(%142));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %50 @foo(%51 size: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %52 a0: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %53 a1: @type3 [storage=automatic];
// DEFAULT-NEXT:         let %54 a2: @type5 [storage=automatic];
// DEFAULT-NEXT:         let %55 a3: @type7 [storage=automatic];
// DEFAULT-NEXT:         let %56 a4: @type9 [storage=automatic];
// DEFAULT-NEXT:         let %57 a5: @type11 [storage=automatic];
// DEFAULT-NEXT:         let %58 a6: @type13 [storage=automatic];
// DEFAULT-NEXT:         let %59 a7: @type15 [storage=automatic];
// DEFAULT-NEXT:         let %60 a8: @type17 [storage=automatic];
// DEFAULT-NEXT:         let %61 a9: @type19 [storage=automatic];
// DEFAULT-NEXT:         let %62 a10: @type21 [storage=automatic];
// DEFAULT-NEXT:         let %63 a11: @type23 [storage=automatic];
// DEFAULT-NEXT:         let %64 a12: @type25 [storage=automatic];
// DEFAULT-NEXT:         let %65 a13: @type27 [storage=automatic];
// DEFAULT-NEXT:         let %66 a14: @type29 [storage=automatic];
// DEFAULT-NEXT:         let %67 a15: @type31 [storage=automatic];
// DEFAULT-NEXT:         let %68 a16: @type33 [storage=automatic];
// DEFAULT-NEXT:         let %69 a31: @type35 [storage=automatic];
// DEFAULT-NEXT:         let %70 a32: @type37 [storage=automatic];
// DEFAULT-NEXT:         let %71 a35: @type39 [storage=automatic];
// DEFAULT-NEXT:         let %72 a72: @type41 [storage=automatic];
// DEFAULT-NEXT:         let %73 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %74 i: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%51), const<i32>(21))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         va_start(%73);
// DEFAULT-NEXT:         write<@type1>(%52, copy<@type1, reason=assign>(va_arg<@type1>(%73)));
// DEFAULT-NEXT:         copy<@type1, reason=assign>(va_arg<@type1>(%73));
// DEFAULT-NEXT:         for %99
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%74, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%74), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %143: i32 [synthetic] = read<i32>(%74);
// DEFAULT-NEXT:                 let %144: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%143), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%74, read<i32>(%144));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%3, const<i32>(0), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(0)>(field0(%52)), read<i32>(%74))))));
// DEFAULT-NEXT:         write<@type3>(%53, copy<@type3, reason=assign>(va_arg<@type3>(%73)));
// DEFAULT-NEXT:         copy<@type3, reason=assign>(va_arg<@type3>(%73));
// DEFAULT-NEXT:         for %100
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%74, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%74), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %145: i32 [synthetic] = read<i32>(%74);
// DEFAULT-NEXT:                 let %146: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%145), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%74, read<i32>(%146));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%3, const<i32>(1), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field0(%53)), read<i32>(%74))))));
// DEFAULT-NEXT:         write<@type5>(%54, copy<@type5, reason=assign>(va_arg<@type5>(%73)));
// DEFAULT-NEXT:         copy<@type5, reason=assign>(va_arg<@type5>(%73));
// DEFAULT-NEXT:         for %101
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%74, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%74), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %147: i32 [synthetic] = read<i32>(%74);
// DEFAULT-NEXT:                 let %148: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%147), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%74, read<i32>(%148));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%3, const<i32>(2), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(field0(%54)), read<i32>(%74))))));
// DEFAULT-NEXT:         write<@type7>(%55, copy<@type7, reason=assign>(va_arg<@type7>(%73)));
// DEFAULT-NEXT:         copy<@type7, reason=assign>(va_arg<@type7>(%73));
// DEFAULT-NEXT:         for %102
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%74, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%74), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %149: i32 [synthetic] = read<i32>(%74);
// DEFAULT-NEXT:                 let %150: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%149), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%74, read<i32>(%150));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%3, const<i32>(3), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(field0(%55)), read<i32>(%74))))));
// DEFAULT-NEXT:         write<@type9>(%56, copy<@type9, reason=assign>(va_arg<@type9>(%73)));
// DEFAULT-NEXT:         copy<@type9, reason=assign>(va_arg<@type9>(%73));
// DEFAULT-NEXT:         for %103
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%74, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%74), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %151: i32 [synthetic] = read<i32>(%74);
// DEFAULT-NEXT:                 let %152: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%151), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%74, read<i32>(%152));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%3, const<i32>(4), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(field0(%56)), read<i32>(%74))))));
// DEFAULT-NEXT:         write<@type11>(%57, copy<@type11, reason=assign>(va_arg<@type11>(%73)));
// DEFAULT-NEXT:         copy<@type11, reason=assign>(va_arg<@type11>(%73));
// DEFAULT-NEXT:         for %104
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%74, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%74), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %153: i32 [synthetic] = read<i32>(%74);
// DEFAULT-NEXT:                 let %154: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%153), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%74, read<i32>(%154));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%3, const<i32>(5), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(field0(%57)), read<i32>(%74))))));
// DEFAULT-NEXT:         write<@type13>(%58, copy<@type13, reason=assign>(va_arg<@type13>(%73)));
// DEFAULT-NEXT:         copy<@type13, reason=assign>(va_arg<@type13>(%73));
// DEFAULT-NEXT:         for %105
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%74, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%74), const<i32>(6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %155: i32 [synthetic] = read<i32>(%74);
// DEFAULT-NEXT:                 let %156: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%155), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%74, read<i32>(%156));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%3, const<i32>(6), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(field0(%58)), read<i32>(%74))))));
// DEFAULT-NEXT:         write<@type15>(%59, copy<@type15, reason=assign>(va_arg<@type15>(%73)));
// DEFAULT-NEXT:         copy<@type15, reason=assign>(va_arg<@type15>(%73));
// DEFAULT-NEXT:         for %106
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%74, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%74), const<i32>(7))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %157: i32 [synthetic] = read<i32>(%74);
// DEFAULT-NEXT:                 let %158: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%157), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%74, read<i32>(%158));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%3, const<i32>(7), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(7)>(field0(%59)), read<i32>(%74))))));
// DEFAULT-NEXT:         write<@type17>(%60, copy<@type17, reason=assign>(va_arg<@type17>(%73)));
// DEFAULT-NEXT:         copy<@type17, reason=assign>(va_arg<@type17>(%73));
// DEFAULT-NEXT:         for %107
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%74, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%74), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %159: i32 [synthetic] = read<i32>(%74);
// DEFAULT-NEXT:                 let %160: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%159), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%74, read<i32>(%160));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%3, const<i32>(8), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field0(%60)), read<i32>(%74))))));
// DEFAULT-NEXT:         write<@type19>(%61, copy<@type19, reason=assign>(va_arg<@type19>(%73)));
// DEFAULT-NEXT:         copy<@type19, reason=assign>(va_arg<@type19>(%73));
// DEFAULT-NEXT:         for %108
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%74, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%74), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %161: i32 [synthetic] = read<i32>(%74);
// DEFAULT-NEXT:                 let %162: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%161), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%74, read<i32>(%162));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%3, const<i32>(9), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field0(%61)), read<i32>(%74))))));
// DEFAULT-NEXT:         write<@type21>(%62, copy<@type21, reason=assign>(va_arg<@type21>(%73)));
// DEFAULT-NEXT:         copy<@type21, reason=assign>(va_arg<@type21>(%73));
// DEFAULT-NEXT:         for %109
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%74, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%74), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %163: i32 [synthetic] = read<i32>(%74);
// DEFAULT-NEXT:                 let %164: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%163), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%74, read<i32>(%164));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%3, const<i32>(10), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(field0(%62)), read<i32>(%74))))));
// DEFAULT-NEXT:         write<@type23>(%63, copy<@type23, reason=assign>(va_arg<@type23>(%73)));
// DEFAULT-NEXT:         copy<@type23, reason=assign>(va_arg<@type23>(%73));
// DEFAULT-NEXT:         for %110
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%74, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%74), const<i32>(11))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %165: i32 [synthetic] = read<i32>(%74);
// DEFAULT-NEXT:                 let %166: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%165), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%74, read<i32>(%166));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%3, const<i32>(11), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(field0(%63)), read<i32>(%74))))));
// DEFAULT-NEXT:         write<@type25>(%64, copy<@type25, reason=assign>(va_arg<@type25>(%73)));
// DEFAULT-NEXT:         copy<@type25, reason=assign>(va_arg<@type25>(%73));
// DEFAULT-NEXT:         for %111
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%74, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%74), const<i32>(12))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %167: i32 [synthetic] = read<i32>(%74);
// DEFAULT-NEXT:                 let %168: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%167), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%74, read<i32>(%168));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%3, const<i32>(12), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(12)>(field0(%64)), read<i32>(%74))))));
// DEFAULT-NEXT:         write<@type27>(%65, copy<@type27, reason=assign>(va_arg<@type27>(%73)));
// DEFAULT-NEXT:         copy<@type27, reason=assign>(va_arg<@type27>(%73));
// DEFAULT-NEXT:         for %112
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%74, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%74), const<i32>(13))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %169: i32 [synthetic] = read<i32>(%74);
// DEFAULT-NEXT:                 let %170: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%169), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%74, read<i32>(%170));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%3, const<i32>(13), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(13)>(field0(%65)), read<i32>(%74))))));
// DEFAULT-NEXT:         write<@type29>(%66, copy<@type29, reason=assign>(va_arg<@type29>(%73)));
// DEFAULT-NEXT:         copy<@type29, reason=assign>(va_arg<@type29>(%73));
// DEFAULT-NEXT:         for %113
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%74, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%74), const<i32>(14))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %171: i32 [synthetic] = read<i32>(%74);
// DEFAULT-NEXT:                 let %172: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%171), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%74, read<i32>(%172));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%3, const<i32>(14), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(14)>(field0(%66)), read<i32>(%74))))));
// DEFAULT-NEXT:         write<@type31>(%67, copy<@type31, reason=assign>(va_arg<@type31>(%73)));
// DEFAULT-NEXT:         copy<@type31, reason=assign>(va_arg<@type31>(%73));
// DEFAULT-NEXT:         for %114
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%74, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%74), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %173: i32 [synthetic] = read<i32>(%74);
// DEFAULT-NEXT:                 let %174: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%173), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%74, read<i32>(%174));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%3, const<i32>(15), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(15)>(field0(%67)), read<i32>(%74))))));
// DEFAULT-NEXT:         write<@type33>(%68, copy<@type33, reason=assign>(va_arg<@type33>(%73)));
// DEFAULT-NEXT:         copy<@type33, reason=assign>(va_arg<@type33>(%73));
// DEFAULT-NEXT:         for %115
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%74, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%74), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %175: i32 [synthetic] = read<i32>(%74);
// DEFAULT-NEXT:                 let %176: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%175), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%74, read<i32>(%176));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%3, const<i32>(16), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(field0(%68)), read<i32>(%74))))));
// DEFAULT-NEXT:         write<@type35>(%69, copy<@type35, reason=assign>(va_arg<@type35>(%73)));
// DEFAULT-NEXT:         copy<@type35, reason=assign>(va_arg<@type35>(%73));
// DEFAULT-NEXT:         for %116
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%74, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%74), const<i32>(31))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %177: i32 [synthetic] = read<i32>(%74);
// DEFAULT-NEXT:                 let %178: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%177), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%74, read<i32>(%178));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%3, const<i32>(31), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%69)), read<i32>(%74))))));
// DEFAULT-NEXT:         write<@type37>(%70, copy<@type37, reason=assign>(va_arg<@type37>(%73)));
// DEFAULT-NEXT:         copy<@type37, reason=assign>(va_arg<@type37>(%73));
// DEFAULT-NEXT:         for %117
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%74, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%74), const<i32>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %179: i32 [synthetic] = read<i32>(%74);
// DEFAULT-NEXT:                 let %180: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%179), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%74, read<i32>(%180));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%3, const<i32>(32), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(32)>(field0(%70)), read<i32>(%74))))));
// DEFAULT-NEXT:         write<@type39>(%71, copy<@type39, reason=assign>(va_arg<@type39>(%73)));
// DEFAULT-NEXT:         copy<@type39, reason=assign>(va_arg<@type39>(%73));
// DEFAULT-NEXT:         for %118
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%74, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%74), const<i32>(35))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %181: i32 [synthetic] = read<i32>(%74);
// DEFAULT-NEXT:                 let %182: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%181), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%74, read<i32>(%182));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%3, const<i32>(35), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(35)>(field0(%71)), read<i32>(%74))))));
// DEFAULT-NEXT:         write<@type41>(%72, copy<@type41, reason=assign>(va_arg<@type41>(%73)));
// DEFAULT-NEXT:         copy<@type41, reason=assign>(va_arg<@type41>(%73));
// DEFAULT-NEXT:         for %119
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%74, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%74), const<i32>(72))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %183: i32 [synthetic] = read<i32>(%74);
// DEFAULT-NEXT:                 let %184: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%183), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%74, read<i32>(%184));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%3, const<i32>(72), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(72)>(field0(%72)), read<i32>(%74))))));
// DEFAULT-NEXT:         va_end(%73);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %75 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %76 a0: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %77 a1: @type3 [storage=automatic];
// DEFAULT-NEXT:         let %78 a2: @type5 [storage=automatic];
// DEFAULT-NEXT:         let %79 a3: @type7 [storage=automatic];
// DEFAULT-NEXT:         let %80 a4: @type9 [storage=automatic];
// DEFAULT-NEXT:         let %81 a5: @type11 [storage=automatic];
// DEFAULT-NEXT:         let %82 a6: @type13 [storage=automatic];
// DEFAULT-NEXT:         let %83 a7: @type15 [storage=automatic];
// DEFAULT-NEXT:         let %84 a8: @type17 [storage=automatic];
// DEFAULT-NEXT:         let %85 a9: @type19 [storage=automatic];
// DEFAULT-NEXT:         let %86 a10: @type21 [storage=automatic];
// DEFAULT-NEXT:         let %87 a11: @type23 [storage=automatic];
// DEFAULT-NEXT:         let %88 a12: @type25 [storage=automatic];
// DEFAULT-NEXT:         let %89 a13: @type27 [storage=automatic];
// DEFAULT-NEXT:         let %90 a14: @type29 [storage=automatic];
// DEFAULT-NEXT:         let %91 a15: @type31 [storage=automatic];
// DEFAULT-NEXT:         let %92 a16: @type33 [storage=automatic];
// DEFAULT-NEXT:         let %93 a31: @type35 [storage=automatic];
// DEFAULT-NEXT:         let %94 a32: @type37 [storage=automatic];
// DEFAULT-NEXT:         let %95 a35: @type39 [storage=automatic];
// DEFAULT-NEXT:         let %96 a72: @type41 [storage=automatic];
// DEFAULT-NEXT:         let %97 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %120
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%97, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%97), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %185: i32 [synthetic] = read<i32>(%97);
// DEFAULT-NEXT:                 let %186: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%185), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%97, read<i32>(%186));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(0)>(field0(%76)), read<i32>(%97))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%97), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(0), const<i32>(3)))));
// DEFAULT-NEXT:         for %121
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%97, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%97), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %187: i32 [synthetic] = read<i32>(%97);
// DEFAULT-NEXT:                 let %188: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%187), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%97, read<i32>(%188));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field0(%77)), read<i32>(%97))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%97), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(3)))));
// DEFAULT-NEXT:         for %122
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%97, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%97), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %189: i32 [synthetic] = read<i32>(%97);
// DEFAULT-NEXT:                 let %190: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%189), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%97, read<i32>(%190));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(field0(%78)), read<i32>(%97))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%97), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(2), const<i32>(3)))));
// DEFAULT-NEXT:         for %123
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%97, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%97), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %191: i32 [synthetic] = read<i32>(%97);
// DEFAULT-NEXT:                 let %192: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%191), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%97, read<i32>(%192));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(field0(%79)), read<i32>(%97))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%97), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(3), const<i32>(3)))));
// DEFAULT-NEXT:         for %124
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%97, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%97), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %193: i32 [synthetic] = read<i32>(%97);
// DEFAULT-NEXT:                 let %194: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%193), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%97, read<i32>(%194));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(field0(%80)), read<i32>(%97))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%97), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(4), const<i32>(3)))));
// DEFAULT-NEXT:         for %125
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%97, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%97), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %195: i32 [synthetic] = read<i32>(%97);
// DEFAULT-NEXT:                 let %196: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%195), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%97, read<i32>(%196));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(field0(%81)), read<i32>(%97))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%97), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(5), const<i32>(3)))));
// DEFAULT-NEXT:         for %126
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%97, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%97), const<i32>(6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %197: i32 [synthetic] = read<i32>(%97);
// DEFAULT-NEXT:                 let %198: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%197), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%97, read<i32>(%198));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(field0(%82)), read<i32>(%97))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%97), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(6), const<i32>(3)))));
// DEFAULT-NEXT:         for %127
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%97, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%97), const<i32>(7))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %199: i32 [synthetic] = read<i32>(%97);
// DEFAULT-NEXT:                 let %200: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%199), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%97, read<i32>(%200));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(7)>(field0(%83)), read<i32>(%97))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%97), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(7), const<i32>(3)))));
// DEFAULT-NEXT:         for %128
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%97, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%97), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %201: i32 [synthetic] = read<i32>(%97);
// DEFAULT-NEXT:                 let %202: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%201), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%97, read<i32>(%202));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field0(%84)), read<i32>(%97))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%97), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(8), const<i32>(3)))));
// DEFAULT-NEXT:         for %129
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%97, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%97), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %203: i32 [synthetic] = read<i32>(%97);
// DEFAULT-NEXT:                 let %204: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%203), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%97, read<i32>(%204));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field0(%85)), read<i32>(%97))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%97), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(9), const<i32>(3)))));
// DEFAULT-NEXT:         for %130
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%97, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%97), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %205: i32 [synthetic] = read<i32>(%97);
// DEFAULT-NEXT:                 let %206: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%205), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%97, read<i32>(%206));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(field0(%86)), read<i32>(%97))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%97), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(10), const<i32>(3)))));
// DEFAULT-NEXT:         for %131
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%97, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%97), const<i32>(11))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %207: i32 [synthetic] = read<i32>(%97);
// DEFAULT-NEXT:                 let %208: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%207), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%97, read<i32>(%208));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(field0(%87)), read<i32>(%97))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%97), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(11), const<i32>(3)))));
// DEFAULT-NEXT:         for %132
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%97, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%97), const<i32>(12))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %209: i32 [synthetic] = read<i32>(%97);
// DEFAULT-NEXT:                 let %210: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%209), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%97, read<i32>(%210));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(12)>(field0(%88)), read<i32>(%97))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%97), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(12), const<i32>(3)))));
// DEFAULT-NEXT:         for %133
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%97, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%97), const<i32>(13))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %211: i32 [synthetic] = read<i32>(%97);
// DEFAULT-NEXT:                 let %212: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%211), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%97, read<i32>(%212));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(13)>(field0(%89)), read<i32>(%97))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%97), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(13), const<i32>(3)))));
// DEFAULT-NEXT:         for %134
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%97, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%97), const<i32>(14))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %213: i32 [synthetic] = read<i32>(%97);
// DEFAULT-NEXT:                 let %214: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%213), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%97, read<i32>(%214));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(14)>(field0(%90)), read<i32>(%97))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%97), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(14), const<i32>(3)))));
// DEFAULT-NEXT:         for %135
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%97, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%97), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %215: i32 [synthetic] = read<i32>(%97);
// DEFAULT-NEXT:                 let %216: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%215), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%97, read<i32>(%216));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(15)>(field0(%91)), read<i32>(%97))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%97), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(15), const<i32>(3)))));
// DEFAULT-NEXT:         for %136
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%97, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%97), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %217: i32 [synthetic] = read<i32>(%97);
// DEFAULT-NEXT:                 let %218: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%217), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%97, read<i32>(%218));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(field0(%92)), read<i32>(%97))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%97), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(16), const<i32>(3)))));
// DEFAULT-NEXT:         for %137
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%97, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%97), const<i32>(31))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %219: i32 [synthetic] = read<i32>(%97);
// DEFAULT-NEXT:                 let %220: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%219), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%97, read<i32>(%220));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%93)), read<i32>(%97))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%97), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(31), const<i32>(3)))));
// DEFAULT-NEXT:         for %138
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%97, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%97), const<i32>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %221: i32 [synthetic] = read<i32>(%97);
// DEFAULT-NEXT:                 let %222: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%221), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%97, read<i32>(%222));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(32)>(field0(%94)), read<i32>(%97))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%97), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(32), const<i32>(3)))));
// DEFAULT-NEXT:         for %139
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%97, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%97), const<i32>(35))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %223: i32 [synthetic] = read<i32>(%97);
// DEFAULT-NEXT:                 let %224: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%223), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%97, read<i32>(%224));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(35)>(field0(%95)), read<i32>(%97))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%97), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(35), const<i32>(3)))));
// DEFAULT-NEXT:         for %140
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%97, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%97), const<i32>(72))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %225: i32 [synthetic] = read<i32>(%97);
// DEFAULT-NEXT:                 let %226: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%225), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%97, read<i32>(%226));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(72)>(field0(%96)), read<i32>(%97))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%97), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(72), const<i32>(3)))));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c) -> void>(%50, const<i32>(21), copy<@type1, reason=vararg>(read<@type1>(%76)), copy<@type3, reason=vararg>(read<@type3>(%77)), copy<@type5, reason=vararg>(read<@type5>(%78)), copy<@type7, reason=vararg>(read<@type7>(%79)), copy<@type9, reason=vararg>(read<@type9>(%80)), copy<@type11, reason=vararg>(read<@type11>(%81)), copy<@type13, reason=vararg>(read<@type13>(%82)), copy<@type15, reason=vararg>(read<@type15>(%83)), copy<@type17, reason=vararg>(read<@type17>(%84)), copy<@type19, reason=vararg>(read<@type19>(%85)), copy<@type21, reason=vararg>(read<@type21>(%86)), copy<@type23, reason=vararg>(read<@type23>(%87)), copy<@type25, reason=vararg>(read<@type25>(%88)), copy<@type27, reason=vararg>(read<@type27>(%89)), copy<@type29, reason=vararg>(read<@type29>(%90)), copy<@type31, reason=vararg>(read<@type31>(%91)), copy<@type33, reason=vararg>(read<@type33>(%92)), copy<@type35, reason=vararg>(read<@type35>(%93)), copy<@type37, reason=vararg>(read<@type37>(%94)), copy<@type39, reason=vararg>(read<@type39>(%95)), copy<@type41, reason=vararg>(read<@type41>(%96)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
