/* { dg-do compile { target fpic } } */
/* { dg-options "-fPIC -g -O2 -fpermissive -w" } */
/* var-tracking failed to clobber the reg holding v at the asm insn,
   so v ended up bound to an intermediate PIC expression.  */

struct A { unsigned a1; char a2[15]; };
struct B { long b1; unsigned char b2; long b3; };
struct C { void *c1; unsigned c2; unsigned c3; };

static struct A v1;
struct A *const v2 = &v1;

static inline
int foo (void)
{
  int *v;
  __asm__ __volatile__ ("" : "=r" (v));
  return v[1];
}

static void
bar (struct C *x)
{
  if (x->c2 == x->c3 && x->c1)
    f1 (foo (), x->c1, x->c3 * sizeof (x->c1[0]));
}

void
baz (struct B *y)
{
  int i;
  const char *j;
  char *k;
  char x[64];
  for (i = 0; i < sizeof (struct B); i++, y)
    {
      switch (y->b2)
        {
        case 0x20:
          if (__builtin_strchr (j, '='))
            continue;
        }
      switch (y->b2)
        {
        case 0x80:
          bar (&x);
          f2 (y->b3);
        case 0x2e:
        case 0x4e:
          break;
        default:
          if (v2->a1)
            f2 (y->b2);
        }
      k[0] = '\0';
      if (v2->a1)
        f2 (y->b1);
    }
}

// SLATE-FILECHECK-ARGS -fpermissive
// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 a1: u32;
// DEFAULT-NEXT:         field1 a2: array<i8, 15>;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_B:[0-9]+]] B = struct {
// DEFAULT-NEXT:         field0 b1: i64;
// DEFAULT-NEXT:         field1 b2: u8;
// DEFAULT-NEXT:         field2 b3: i64;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_C:[0-9]+]] C = struct {
// DEFAULT-NEXT:         field0 c1: ptr<void>;
// DEFAULT-NEXT:         field1 c2: u32;
// DEFAULT-NEXT:         field2 c3: u32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 12]];
// DEFAULT-NEXT:     global %[[VALUE_v1:[0-9]+]] v1: @type[[TYPE_A]] [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v2:[0-9]+]] v2: ptr<@type[[TYPE_A]]> [storage=static] [const] = addr_of<ptr<@type[[TYPE_A]]>>(%[[VALUE_v1]]) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nomem,nostack] {
// DEFAULT-NEXT:             lateout 0 "r" [reg] width 64 place<ptr<i32>>(%[[VALUE_v]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_v]]), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x:[0-9]+]] x: ptr<@type[[TYPE_C]]>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_and<bool>(eq<u32>(read<u32>(field1(deref(read<ptr<@type[[TYPE_C]]>>(%[[VALUE_x]])))), read<u32>(field2(deref(read<ptr<@type[[TYPE_C]]>>(%[[VALUE_x]]))))), ne<ptr<void>>(read<ptr<void>>(field0(deref(read<ptr<@type[[TYPE_C]]>>(%[[VALUE_x]])))), null<ptr<void>>))
// DEFAULT-NEXT:             call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_f1]], call<i32, signature=fn() -> i32>(%[[VALUE_foo]]), read<ptr<void>>(field0(deref(read<ptr<@type[[TYPE_C]]>>(%[[VALUE_x]])))), mul<u64, overflow=wrap>(widen<u64, reason=usual_arith>(read<u32>(field2(deref(read<ptr<@type[[TYPE_C]]>>(%[[VALUE_x]]))))), const<u64>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strchr:[0-9]+]] @__builtin_strchr(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE1:[0-9]+]] <unnamed>: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_y:[0-9]+]] y: ptr<@type[[TYPE_B]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: ptr<const i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: array<i8, 64> [storage=automatic] [align=16];
// DEFAULT-NEXT:         for %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), const<u64>(24))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     switch %[[VALUE5:[0-9]+]] reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field1(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_y]]))))))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             case %[[VALUE5]] const<i32>(32):
// DEFAULT-NEXT:                                 if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE___builtin_strchr]], read<ptr<const i8>>(%[[VALUE_j]]), const<i32>(61)), null<ptr<i8>>)
// DEFAULT-NEXT:                                     continue %[[VALUE2]];
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     switch %[[VALUE6:[0-9]+]] reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field1(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_y]]))))))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             case %[[VALUE6]] const<i32>(128):
// DEFAULT-NEXT:                                 call<void, signature=fn(ptr<@type[[TYPE_C]]>) -> void>(%[[VALUE_bar]], pointer_cast<ptr<@type[[TYPE_C]]>, reason=arg>(addr_of<ptr<array<i8, 64>>>(%[[VALUE_x_2]])));
// DEFAULT-NEXT:                             call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_f2]], read<i64>(field2(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_y]])))));
// DEFAULT-NEXT:                             case %[[VALUE6]] const<i32>(46):
// DEFAULT-NEXT:                                 case %[[VALUE6]] const<i32>(78):
// DEFAULT-NEXT:                                     break %[[VALUE6]];
// DEFAULT-NEXT:                             default %[[VALUE6]]:
// DEFAULT-NEXT:                                 if ne<u32>(read<u32>(field0(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_v2]])))), const<u32>(0))
// DEFAULT-NEXT:                                     call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_f2]], reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(field1(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_y]])))))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_k]]), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     if ne<u32>(read<u32>(field0(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_v2]])))), const<u32>(0))
// DEFAULT-NEXT:                         call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_f2]], read<i64>(field0(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_y]])))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
