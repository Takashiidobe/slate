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
// DEFAULT-NEXT:     type @type[[TYPE___gnuc_va_list:[0-9]+]] __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 0>;
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_A0:[0-9]+]] A0 = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 1>;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_A1:[0-9]+]] A1 = @type[[TYPE1]];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 2>;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_A2:[0-9]+]] A2 = @type[[TYPE2]];
// DEFAULT-NEXT:     type @type[[TYPE3:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 3>;
// DEFAULT-NEXT:     } [size=3, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_A3:[0-9]+]] A3 = @type[[TYPE3]];
// DEFAULT-NEXT:     type @type[[TYPE4:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 4>;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_A4:[0-9]+]] A4 = @type[[TYPE4]];
// DEFAULT-NEXT:     type @type[[TYPE5:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 5>;
// DEFAULT-NEXT:     } [size=5, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_A5:[0-9]+]] A5 = @type[[TYPE5]];
// DEFAULT-NEXT:     type @type[[TYPE6:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 6>;
// DEFAULT-NEXT:     } [size=6, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_A6:[0-9]+]] A6 = @type[[TYPE6]];
// DEFAULT-NEXT:     type @type[[TYPE7:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 7>;
// DEFAULT-NEXT:     } [size=7, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_A7:[0-9]+]] A7 = @type[[TYPE7]];
// DEFAULT-NEXT:     type @type[[TYPE8:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 8>;
// DEFAULT-NEXT:     } [size=8, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_A8:[0-9]+]] A8 = @type[[TYPE8]];
// DEFAULT-NEXT:     type @type[[TYPE9:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 9>;
// DEFAULT-NEXT:     } [size=9, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_A9:[0-9]+]] A9 = @type[[TYPE9]];
// DEFAULT-NEXT:     type @type[[TYPE10:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 10>;
// DEFAULT-NEXT:     } [size=10, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_A10:[0-9]+]] A10 = @type[[TYPE10]];
// DEFAULT-NEXT:     type @type[[TYPE11:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 11>;
// DEFAULT-NEXT:     } [size=11, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_A11:[0-9]+]] A11 = @type[[TYPE11]];
// DEFAULT-NEXT:     type @type[[TYPE12:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 12>;
// DEFAULT-NEXT:     } [size=12, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_A12:[0-9]+]] A12 = @type[[TYPE12]];
// DEFAULT-NEXT:     type @type[[TYPE13:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 13>;
// DEFAULT-NEXT:     } [size=13, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_A13:[0-9]+]] A13 = @type[[TYPE13]];
// DEFAULT-NEXT:     type @type[[TYPE14:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 14>;
// DEFAULT-NEXT:     } [size=14, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_A14:[0-9]+]] A14 = @type[[TYPE14]];
// DEFAULT-NEXT:     type @type[[TYPE15:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 15>;
// DEFAULT-NEXT:     } [size=15, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_A15:[0-9]+]] A15 = @type[[TYPE15]];
// DEFAULT-NEXT:     type @type[[TYPE16:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 16>;
// DEFAULT-NEXT:     } [size=16, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_A16:[0-9]+]] A16 = @type[[TYPE16]];
// DEFAULT-NEXT:     type @type[[TYPE17:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 31>;
// DEFAULT-NEXT:     } [size=31, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_A31:[0-9]+]] A31 = @type[[TYPE17]];
// DEFAULT-NEXT:     type @type[[TYPE18:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 32>;
// DEFAULT-NEXT:     } [size=32, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_A32:[0-9]+]] A32 = @type[[TYPE18]];
// DEFAULT-NEXT:     type @type[[TYPE19:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 35>;
// DEFAULT-NEXT:     } [size=35, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_A35:[0-9]+]] A35 = @type[[TYPE19]];
// DEFAULT-NEXT:     type @type[[TYPE20:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 72>;
// DEFAULT-NEXT:     } [size=72, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_A72:[0-9]+]] A72 = @type[[TYPE20]];
// DEFAULT-NEXT:     global %[[VALUE_lastn:[0-9]+]] lastn: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_lastc:[0-9]+]] lastc: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_n:[0-9]+]] n: i32, %[[VALUE_c:[0-9]+]] c: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_lastn]]), read<i32>(%[[VALUE_n]]))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_lastc]]), read<i32>(%[[VALUE_lastn]]))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_lastc]], const<i32>(0));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_lastn]], read<i32>(%[[VALUE_n]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_c]]), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(xor<i32>(read<i32>(%[[VALUE_lastc]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%[[VALUE_n]]), const<i32>(3))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_lastc]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_lastc]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_size:[0-9]+]] size: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a0:[0-9]+]] a0: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a1:[0-9]+]] a1: @type[[TYPE1]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a2:[0-9]+]] a2: @type[[TYPE2]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a3:[0-9]+]] a3: @type[[TYPE3]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a4:[0-9]+]] a4: @type[[TYPE4]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a5:[0-9]+]] a5: @type[[TYPE5]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a6:[0-9]+]] a6: @type[[TYPE6]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a7:[0-9]+]] a7: @type[[TYPE7]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a8:[0-9]+]] a8: @type[[TYPE8]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a9:[0-9]+]] a9: @type[[TYPE9]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a10:[0-9]+]] a10: @type[[TYPE10]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a11:[0-9]+]] a11: @type[[TYPE11]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a12:[0-9]+]] a12: @type[[TYPE12]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a13:[0-9]+]] a13: @type[[TYPE13]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a14:[0-9]+]] a14: @type[[TYPE14]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a15:[0-9]+]] a15: @type[[TYPE15]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a16:[0-9]+]] a16: @type[[TYPE16]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a31:[0-9]+]] a31: @type[[TYPE17]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a32:[0-9]+]] a32: @type[[TYPE18]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a35:[0-9]+]] a35: @type[[TYPE19]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a72:[0-9]+]] a72: @type[[TYPE20]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_size]]), const<i32>(21))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         va_start(%[[VALUE_ap]]);
// DEFAULT-NEXT:         write<@type[[TYPE0]]>(%[[VALUE_a0]], copy<@type[[TYPE0]], reason=assign>(va_arg<@type[[TYPE0]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%[[VALUE_bar]], const<i32>(0), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(0)>(field0(%[[VALUE_a0]])), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:         write<@type[[TYPE1]]>(%[[VALUE_a1]], copy<@type[[TYPE1]], reason=assign>(va_arg<@type[[TYPE1]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%[[VALUE_bar]], const<i32>(1), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field0(%[[VALUE_a1]])), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:         write<@type[[TYPE2]]>(%[[VALUE_a2]], copy<@type[[TYPE2]], reason=assign>(va_arg<@type[[TYPE2]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         for %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%[[VALUE_bar]], const<i32>(2), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(field0(%[[VALUE_a2]])), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:         write<@type[[TYPE3]]>(%[[VALUE_a3]], copy<@type[[TYPE3]], reason=assign>(va_arg<@type[[TYPE3]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         for %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE13]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE14]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%[[VALUE_bar]], const<i32>(3), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(field0(%[[VALUE_a3]])), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:         write<@type[[TYPE4]]>(%[[VALUE_a4]], copy<@type[[TYPE4]], reason=assign>(va_arg<@type[[TYPE4]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         for %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%[[VALUE_bar]], const<i32>(4), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(field0(%[[VALUE_a4]])), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:         write<@type[[TYPE5]]>(%[[VALUE_a5]], copy<@type[[TYPE5]], reason=assign>(va_arg<@type[[TYPE5]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         for %[[VALUE18:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE19:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE20:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE19]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE20]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%[[VALUE_bar]], const<i32>(5), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(field0(%[[VALUE_a5]])), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:         write<@type[[TYPE6]]>(%[[VALUE_a6]], copy<@type[[TYPE6]], reason=assign>(va_arg<@type[[TYPE6]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         for %[[VALUE21:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE23:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE22]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%[[VALUE_bar]], const<i32>(6), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(field0(%[[VALUE_a6]])), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:         write<@type[[TYPE7]]>(%[[VALUE_a7]], copy<@type[[TYPE7]], reason=assign>(va_arg<@type[[TYPE7]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         for %[[VALUE24:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(7))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE25:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE26:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE25]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE26]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%[[VALUE_bar]], const<i32>(7), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(7)>(field0(%[[VALUE_a7]])), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:         write<@type[[TYPE8]]>(%[[VALUE_a8]], copy<@type[[TYPE8]], reason=assign>(va_arg<@type[[TYPE8]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         for %[[VALUE27:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE28:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE29:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE28]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE29]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%[[VALUE_bar]], const<i32>(8), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field0(%[[VALUE_a8]])), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:         write<@type[[TYPE9]]>(%[[VALUE_a9]], copy<@type[[TYPE9]], reason=assign>(va_arg<@type[[TYPE9]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         for %[[VALUE30:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE31:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE32:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE31]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE32]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%[[VALUE_bar]], const<i32>(9), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field0(%[[VALUE_a9]])), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:         write<@type[[TYPE10]]>(%[[VALUE_a10]], copy<@type[[TYPE10]], reason=assign>(va_arg<@type[[TYPE10]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         for %[[VALUE33:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE34:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE35:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE34]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE35]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%[[VALUE_bar]], const<i32>(10), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(field0(%[[VALUE_a10]])), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:         write<@type[[TYPE11]]>(%[[VALUE_a11]], copy<@type[[TYPE11]], reason=assign>(va_arg<@type[[TYPE11]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         for %[[VALUE36:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(11))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE37:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE38:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE37]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE38]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%[[VALUE_bar]], const<i32>(11), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(field0(%[[VALUE_a11]])), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:         write<@type[[TYPE12]]>(%[[VALUE_a12]], copy<@type[[TYPE12]], reason=assign>(va_arg<@type[[TYPE12]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         for %[[VALUE39:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(12))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE40:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE41:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE40]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE41]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%[[VALUE_bar]], const<i32>(12), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(12)>(field0(%[[VALUE_a12]])), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:         write<@type[[TYPE13]]>(%[[VALUE_a13]], copy<@type[[TYPE13]], reason=assign>(va_arg<@type[[TYPE13]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         for %[[VALUE42:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(13))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE43:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE44:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE43]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE44]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%[[VALUE_bar]], const<i32>(13), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(13)>(field0(%[[VALUE_a13]])), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:         write<@type[[TYPE14]]>(%[[VALUE_a14]], copy<@type[[TYPE14]], reason=assign>(va_arg<@type[[TYPE14]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         for %[[VALUE45:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(14))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE46:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE47:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE46]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE47]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%[[VALUE_bar]], const<i32>(14), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(14)>(field0(%[[VALUE_a14]])), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:         write<@type[[TYPE15]]>(%[[VALUE_a15]], copy<@type[[TYPE15]], reason=assign>(va_arg<@type[[TYPE15]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         for %[[VALUE48:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE49:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE50:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE49]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE50]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%[[VALUE_bar]], const<i32>(15), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(15)>(field0(%[[VALUE_a15]])), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:         write<@type[[TYPE16]]>(%[[VALUE_a16]], copy<@type[[TYPE16]], reason=assign>(va_arg<@type[[TYPE16]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         for %[[VALUE51:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE52:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE53:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE52]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE53]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%[[VALUE_bar]], const<i32>(16), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(field0(%[[VALUE_a16]])), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:         write<@type[[TYPE17]]>(%[[VALUE_a31]], copy<@type[[TYPE17]], reason=assign>(va_arg<@type[[TYPE17]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         for %[[VALUE54:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(31))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE55:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE56:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE55]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE56]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%[[VALUE_bar]], const<i32>(31), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_a31]])), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:         write<@type[[TYPE18]]>(%[[VALUE_a32]], copy<@type[[TYPE18]], reason=assign>(va_arg<@type[[TYPE18]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         for %[[VALUE57:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE58:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE59:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE58]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE59]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%[[VALUE_bar]], const<i32>(32), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(32)>(field0(%[[VALUE_a32]])), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:         write<@type[[TYPE19]]>(%[[VALUE_a35]], copy<@type[[TYPE19]], reason=assign>(va_arg<@type[[TYPE19]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         for %[[VALUE60:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(35))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE61:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE62:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE61]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE62]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%[[VALUE_bar]], const<i32>(35), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(35)>(field0(%[[VALUE_a35]])), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:         write<@type[[TYPE20]]>(%[[VALUE_a72]], copy<@type[[TYPE20]], reason=assign>(va_arg<@type[[TYPE20]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         for %[[VALUE63:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(72))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE64:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE65:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE64]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE65]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%[[VALUE_bar]], const<i32>(72), widen<i32, reason=arg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(72)>(field0(%[[VALUE_a72]])), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a0_2:[0-9]+]] a0: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a1_2:[0-9]+]] a1: @type[[TYPE1]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a2_2:[0-9]+]] a2: @type[[TYPE2]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a3_2:[0-9]+]] a3: @type[[TYPE3]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a4_2:[0-9]+]] a4: @type[[TYPE4]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a5_2:[0-9]+]] a5: @type[[TYPE5]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a6_2:[0-9]+]] a6: @type[[TYPE6]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a7_2:[0-9]+]] a7: @type[[TYPE7]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a8_2:[0-9]+]] a8: @type[[TYPE8]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a9_2:[0-9]+]] a9: @type[[TYPE9]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a10_2:[0-9]+]] a10: @type[[TYPE10]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a11_2:[0-9]+]] a11: @type[[TYPE11]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a12_2:[0-9]+]] a12: @type[[TYPE12]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a13_2:[0-9]+]] a13: @type[[TYPE13]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a14_2:[0-9]+]] a14: @type[[TYPE14]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a15_2:[0-9]+]] a15: @type[[TYPE15]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a16_2:[0-9]+]] a16: @type[[TYPE16]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a31_2:[0-9]+]] a31: @type[[TYPE17]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a32_2:[0-9]+]] a32: @type[[TYPE18]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a35_2:[0-9]+]] a35: @type[[TYPE19]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a72_2:[0-9]+]] a72: @type[[TYPE20]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE66:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE67:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE68:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE67]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE68]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(0)>(field0(%[[VALUE_a0_2]])), read<i32>(%[[VALUE_i_2]]))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%[[VALUE_i_2]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(0), const<i32>(3)))));
// DEFAULT-NEXT:         for %[[VALUE69:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE70:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE71:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE70]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE71]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field0(%[[VALUE_a1_2]])), read<i32>(%[[VALUE_i_2]]))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%[[VALUE_i_2]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(3)))));
// DEFAULT-NEXT:         for %[[VALUE72:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE73:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE74:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE73]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE74]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(field0(%[[VALUE_a2_2]])), read<i32>(%[[VALUE_i_2]]))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%[[VALUE_i_2]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(2), const<i32>(3)))));
// DEFAULT-NEXT:         for %[[VALUE75:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE76:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE77:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE76]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE77]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(field0(%[[VALUE_a3_2]])), read<i32>(%[[VALUE_i_2]]))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%[[VALUE_i_2]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(3), const<i32>(3)))));
// DEFAULT-NEXT:         for %[[VALUE78:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE79:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE80:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE79]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE80]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(field0(%[[VALUE_a4_2]])), read<i32>(%[[VALUE_i_2]]))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%[[VALUE_i_2]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(4), const<i32>(3)))));
// DEFAULT-NEXT:         for %[[VALUE81:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE82:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE83:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE82]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE83]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(field0(%[[VALUE_a5_2]])), read<i32>(%[[VALUE_i_2]]))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%[[VALUE_i_2]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(5), const<i32>(3)))));
// DEFAULT-NEXT:         for %[[VALUE84:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE85:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE86:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE85]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE86]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(6)>(field0(%[[VALUE_a6_2]])), read<i32>(%[[VALUE_i_2]]))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%[[VALUE_i_2]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(6), const<i32>(3)))));
// DEFAULT-NEXT:         for %[[VALUE87:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(7))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE88:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE89:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE88]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE89]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(7)>(field0(%[[VALUE_a7_2]])), read<i32>(%[[VALUE_i_2]]))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%[[VALUE_i_2]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(7), const<i32>(3)))));
// DEFAULT-NEXT:         for %[[VALUE90:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE91:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE92:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE91]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE92]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(field0(%[[VALUE_a8_2]])), read<i32>(%[[VALUE_i_2]]))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%[[VALUE_i_2]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(8), const<i32>(3)))));
// DEFAULT-NEXT:         for %[[VALUE93:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE94:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE95:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE94]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE95]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(9)>(field0(%[[VALUE_a9_2]])), read<i32>(%[[VALUE_i_2]]))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%[[VALUE_i_2]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(9), const<i32>(3)))));
// DEFAULT-NEXT:         for %[[VALUE96:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE97:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE98:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE97]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE98]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(field0(%[[VALUE_a10_2]])), read<i32>(%[[VALUE_i_2]]))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%[[VALUE_i_2]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(10), const<i32>(3)))));
// DEFAULT-NEXT:         for %[[VALUE99:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(11))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE100:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE101:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE100]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE101]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(field0(%[[VALUE_a11_2]])), read<i32>(%[[VALUE_i_2]]))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%[[VALUE_i_2]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(11), const<i32>(3)))));
// DEFAULT-NEXT:         for %[[VALUE102:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(12))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE103:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE104:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE103]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE104]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(12)>(field0(%[[VALUE_a12_2]])), read<i32>(%[[VALUE_i_2]]))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%[[VALUE_i_2]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(12), const<i32>(3)))));
// DEFAULT-NEXT:         for %[[VALUE105:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(13))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE106:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE107:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE106]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE107]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(13)>(field0(%[[VALUE_a13_2]])), read<i32>(%[[VALUE_i_2]]))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%[[VALUE_i_2]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(13), const<i32>(3)))));
// DEFAULT-NEXT:         for %[[VALUE108:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(14))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE109:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE110:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE109]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE110]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(14)>(field0(%[[VALUE_a14_2]])), read<i32>(%[[VALUE_i_2]]))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%[[VALUE_i_2]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(14), const<i32>(3)))));
// DEFAULT-NEXT:         for %[[VALUE111:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE112:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE113:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE112]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE113]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(15)>(field0(%[[VALUE_a15_2]])), read<i32>(%[[VALUE_i_2]]))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%[[VALUE_i_2]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(15), const<i32>(3)))));
// DEFAULT-NEXT:         for %[[VALUE114:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE115:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE116:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE115]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE116]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(field0(%[[VALUE_a16_2]])), read<i32>(%[[VALUE_i_2]]))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%[[VALUE_i_2]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(16), const<i32>(3)))));
// DEFAULT-NEXT:         for %[[VALUE117:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(31))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE118:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE119:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE118]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE119]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(31)>(field0(%[[VALUE_a31_2]])), read<i32>(%[[VALUE_i_2]]))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%[[VALUE_i_2]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(31), const<i32>(3)))));
// DEFAULT-NEXT:         for %[[VALUE120:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE121:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE122:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE121]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE122]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(32)>(field0(%[[VALUE_a32_2]])), read<i32>(%[[VALUE_i_2]]))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%[[VALUE_i_2]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(32), const<i32>(3)))));
// DEFAULT-NEXT:         for %[[VALUE123:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(35))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE124:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE125:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE124]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE125]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(35)>(field0(%[[VALUE_a35_2]])), read<i32>(%[[VALUE_i_2]]))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%[[VALUE_i_2]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(35), const<i32>(3)))));
// DEFAULT-NEXT:         for %[[VALUE126:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(72))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE127:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE128:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE127]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE128]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(72)>(field0(%[[VALUE_a72_2]])), read<i32>(%[[VALUE_i_2]]))), truncate<i8, reason=assign, fits=unknown>(xor<i32>(read<i32>(%[[VALUE_i_2]]), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(72), const<i32>(3)))));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c, native_c) -> void>(%[[VALUE_foo]], const<i32>(21), copy<@type[[TYPE0]], reason=vararg>(read<@type[[TYPE0]]>(%[[VALUE_a0_2]])), copy<@type[[TYPE1]], reason=vararg>(read<@type[[TYPE1]]>(%[[VALUE_a1_2]])), copy<@type[[TYPE2]], reason=vararg>(read<@type[[TYPE2]]>(%[[VALUE_a2_2]])), copy<@type[[TYPE3]], reason=vararg>(read<@type[[TYPE3]]>(%[[VALUE_a3_2]])), copy<@type[[TYPE4]], reason=vararg>(read<@type[[TYPE4]]>(%[[VALUE_a4_2]])), copy<@type[[TYPE5]], reason=vararg>(read<@type[[TYPE5]]>(%[[VALUE_a5_2]])), copy<@type[[TYPE6]], reason=vararg>(read<@type[[TYPE6]]>(%[[VALUE_a6_2]])), copy<@type[[TYPE7]], reason=vararg>(read<@type[[TYPE7]]>(%[[VALUE_a7_2]])), copy<@type[[TYPE8]], reason=vararg>(read<@type[[TYPE8]]>(%[[VALUE_a8_2]])), copy<@type[[TYPE9]], reason=vararg>(read<@type[[TYPE9]]>(%[[VALUE_a9_2]])), copy<@type[[TYPE10]], reason=vararg>(read<@type[[TYPE10]]>(%[[VALUE_a10_2]])), copy<@type[[TYPE11]], reason=vararg>(read<@type[[TYPE11]]>(%[[VALUE_a11_2]])), copy<@type[[TYPE12]], reason=vararg>(read<@type[[TYPE12]]>(%[[VALUE_a12_2]])), copy<@type[[TYPE13]], reason=vararg>(read<@type[[TYPE13]]>(%[[VALUE_a13_2]])), copy<@type[[TYPE14]], reason=vararg>(read<@type[[TYPE14]]>(%[[VALUE_a14_2]])), copy<@type[[TYPE15]], reason=vararg>(read<@type[[TYPE15]]>(%[[VALUE_a15_2]])), copy<@type[[TYPE16]], reason=vararg>(read<@type[[TYPE16]]>(%[[VALUE_a16_2]])), copy<@type[[TYPE17]], reason=vararg>(read<@type[[TYPE17]]>(%[[VALUE_a31_2]])), copy<@type[[TYPE18]], reason=vararg>(read<@type[[TYPE18]]>(%[[VALUE_a32_2]])), copy<@type[[TYPE19]], reason=vararg>(read<@type[[TYPE19]]>(%[[VALUE_a35_2]])), copy<@type[[TYPE20]], reason=vararg>(read<@type[[TYPE20]]>(%[[VALUE_a72_2]])));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
