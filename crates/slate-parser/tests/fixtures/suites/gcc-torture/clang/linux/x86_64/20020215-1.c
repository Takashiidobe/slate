/* Test failed on an architecture that:

   - had 16-bit registers,
   - passed 64-bit structures in registers,
   - only allowed SImode values in even numbered registers.

   Before reload, s.i2 in foo() was represented as:

        (subreg:SI (reg:DI 0) 2)

   find_dummy_reload would return (reg:SI 1) for the subreg reload,
   despite that not being a valid register.  */

void abort(void);
void exit(int);

struct s {
  short i1;
  long  i2;
  short i3;
};

struct s foo(struct s s) {
  s.i2++;
  return s;
}

int main() {
  struct s s = foo((struct s){1000, 2000L, 3000});
  if (s.i1 != 1000 || s.i2 != 2001L || s.i3 != 3000)
    abort();
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
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 i1: i16;
// DEFAULT-NEXT:         field1 i2: i64;
// DEFAULT-NEXT:         field2 i3: i16;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_s:[0-9]+]] s: @type[[TYPE_s]]) -> @type[[TYPE_s]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i64 [synthetic] = read<i64>(field1(%[[VALUE_s]]));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE1]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(field1(%[[VALUE_s]]), read<i64>(%[[VALUE2]]));
// DEFAULT-NEXT:         return copy<@type[[TYPE_s]], reason=return>(read<@type[[TYPE_s]]>(%[[VALUE_s]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_s_2:[0-9]+]] s: @type[[TYPE_s]] [storage=automatic] = copy<@type[[TYPE_s]], reason=assign>(call<@type[[TYPE_s]], signature=fn(@type[[TYPE_s]]) -> @type[[TYPE_s]], abi=sysv64(native_c) -> native_c>(%[[VALUE_foo]], copy<@type[[TYPE_s]], reason=arg>(read<@type[[TYPE_s]]>(compound_literal %[[VALUE3:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_s]], zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(1000)), field1 = const<i64>(2000), field2 = truncate<i16, reason=assign, fits=always>(const<i32>(3000)))))));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i16>(field0(%[[VALUE_s_2]]))), const<i32>(1000)), ne<i64>(read<i64>(field1(%[[VALUE_s_2]])), const<i64>(2001))), ne<i32>(widen<i32, reason=promotion>(read<i16>(field2(%[[VALUE_s_2]]))), const<i32>(3000)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
