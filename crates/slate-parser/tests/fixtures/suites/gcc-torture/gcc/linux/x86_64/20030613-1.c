/* PR optimization/10955 */
/* Originator: <heinrich.brand@fujitsu-siemens.com> */

/* This used to fail on SPARC32 at -O3 because the loop unroller
   wrongly thought it could eliminate a pseudo in a loop, while
   the pseudo was used outside the loop.  */

extern void abort(void);

#define COMPLEX struct CS

COMPLEX {
  long x;
  long y;
};

static COMPLEX CCID(COMPLEX x) {
  COMPLEX a;

  a.x = x.x;
  a.y = x.y;

  return a;
}

static COMPLEX CPOW(COMPLEX x, int y) {
  COMPLEX a;
  a = x;

  while (--y > 0)
    a = CCID(a);

  return a;
}

static int c5p(COMPLEX x) {
  COMPLEX a, b;
  a = CPOW(x, 2);
  b = CCID(CPOW(a, 2));

  return (b.x == b.y);
}

int main(void) {
  COMPLEX x;

  x.x = -7;
  x.y = -7;

  if (!c5p(x))
    abort();

  return 0;
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
// DEFAULT-NEXT:     type @type[[TYPE_CS:[0-9]+]] CS = struct {
// DEFAULT-NEXT:         field0 x: i64;
// DEFAULT-NEXT:         field1 y: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_CCID:[0-9]+]] @CCID(%[[VALUE_x:[0-9]+]] x: @type[[TYPE_CS]]) -> @type[[TYPE_CS]] [linkage=internal] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: @type[[TYPE_CS]] [storage=automatic];
// DEFAULT-NEXT:         write<i64>(field0(%[[VALUE_a]]), read<i64>(field0(%[[VALUE_x]])));
// DEFAULT-NEXT:         write<i64>(field1(%[[VALUE_a]]), read<i64>(field1(%[[VALUE_x]])));
// DEFAULT-NEXT:         return copy<@type[[TYPE_CS]], reason=return>(read<@type[[TYPE_CS]]>(%[[VALUE_a]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_CPOW:[0-9]+]] @CPOW(%[[VALUE_x_2:[0-9]+]] x: @type[[TYPE_CS]], %[[VALUE_y:[0-9]+]] y: i32) -> @type[[TYPE_CS]] [linkage=internal] [abi=sysv64(native_c, scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: @type[[TYPE_CS]] [storage=automatic];
// DEFAULT-NEXT:         write<@type[[TYPE_CS]]>(%[[VALUE_a_2]], copy<@type[[TYPE_CS]], reason=assign>(read<@type[[TYPE_CS]]>(%[[VALUE_x_2]])));
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_y]]);
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_y]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%[[VALUE2]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             write<@type[[TYPE_CS]]>(%[[VALUE_a_2]], copy<@type[[TYPE_CS]], reason=assign>(call<@type[[TYPE_CS]], signature=fn(@type[[TYPE_CS]]) -> @type[[TYPE_CS]], abi=sysv64(native_c) -> native_c>(%[[VALUE_CCID]], copy<@type[[TYPE_CS]], reason=arg>(read<@type[[TYPE_CS]]>(%[[VALUE_a_2]])))));
// DEFAULT-NEXT:         return copy<@type[[TYPE_CS]], reason=return>(read<@type[[TYPE_CS]]>(%[[VALUE_a_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c5p:[0-9]+]] @c5p(%[[VALUE_x_3:[0-9]+]] x: @type[[TYPE_CS]]) -> i32 [linkage=internal] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a_3:[0-9]+]] a: @type[[TYPE_CS]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: @type[[TYPE_CS]] [storage=automatic];
// DEFAULT-NEXT:         write<@type[[TYPE_CS]]>(%[[VALUE_a_3]], copy<@type[[TYPE_CS]], reason=assign>(call<@type[[TYPE_CS]], signature=fn(@type[[TYPE_CS]], i32) -> @type[[TYPE_CS]], abi=sysv64(native_c, scalar) -> native_c>(%[[VALUE_CPOW]], copy<@type[[TYPE_CS]], reason=arg>(read<@type[[TYPE_CS]]>(%[[VALUE_x_3]])), const<i32>(2))));
// DEFAULT-NEXT:         write<@type[[TYPE_CS]]>(%[[VALUE_b]], copy<@type[[TYPE_CS]], reason=assign>(call<@type[[TYPE_CS]], signature=fn(@type[[TYPE_CS]]) -> @type[[TYPE_CS]], abi=sysv64(native_c) -> native_c>(%[[VALUE_CCID]], copy<@type[[TYPE_CS]], reason=arg>(call<@type[[TYPE_CS]], signature=fn(@type[[TYPE_CS]], i32) -> @type[[TYPE_CS]], abi=sysv64(native_c, scalar) -> native_c>(%[[VALUE_CPOW]], copy<@type[[TYPE_CS]], reason=arg>(read<@type[[TYPE_CS]]>(%[[VALUE_a_3]])), const<i32>(2))))));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i64>(read<i64>(field0(%[[VALUE_b]])), read<i64>(field1(%[[VALUE_b]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x_4:[0-9]+]] x: @type[[TYPE_CS]] [storage=automatic];
// DEFAULT-NEXT:         write<i64>(field0(%[[VALUE_x_4]]), widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(7))));
// DEFAULT-NEXT:         write<i64>(field1(%[[VALUE_x_4]]), widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(7))));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(@type[[TYPE_CS]]) -> i32, abi=sysv64(native_c) -> scalar>(%[[VALUE_c5p]], copy<@type[[TYPE_CS]], reason=arg>(read<@type[[TYPE_CS]]>(%[[VALUE_x_4]]))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
