/* { dg-options "-Wno-psabi" } */
/* { dg-require-effective-target int32plus } */
void abort(void);

struct S2848 {
  unsigned int a;
  _Complex int b;
  struct {
  } __attribute__((aligned)) c;
};

struct S2848 s2848;

int fails;

void __attribute__((noinline)) check2848va(int z, ...) {
  struct S2848      arg;
  __builtin_va_list ap;

  __builtin_va_start(ap, z);

  arg = __builtin_va_arg(ap, struct S2848);

  if (s2848.a != arg.a)
    ++fails;
  if (s2848.b != arg.b)
    ++fails;

  __builtin_va_end(ap);
}

int main(void) {
  s2848.a = 4027477739U;
  s2848.b = (723419448 + -218144346 * __extension__ 1i);

  check2848va(1, s2848);

  if (fails)
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
// DEFAULT-NEXT:     type @type[[TYPE_S2848:[0-9]+]] S2848 = struct {
// DEFAULT-NEXT:         field0 a: u32;
// DEFAULT-NEXT:         field1 b: complex<i32>;
// DEFAULT-NEXT:         field2 c: @type[[TYPE0:[0-9]+]];
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 4, 16]];
// DEFAULT-NEXT:     type @type[[TYPE0]] = struct {
// DEFAULT-NEXT:     } [size=0, align=16, offsets=[]];
// DEFAULT-NEXT:     global %[[VALUE_s2848:[0-9]+]] s2848: @type[[TYPE_S2848]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_fails:[0-9]+]] fails: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_check2848va:[0-9]+]] @check2848va(%[[VALUE_z:[0-9]+]] z: i32, ...) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_arg:[0-9]+]] arg: @type[[TYPE_S2848]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap]]);
// DEFAULT-NEXT:         write<@type[[TYPE_S2848]]>(%[[VALUE_arg]], copy<@type[[TYPE_S2848]], reason=assign>(va_arg<@type[[TYPE_S2848]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         copy<@type[[TYPE_S2848]], reason=assign>(va_arg<@type[[TYPE_S2848]]>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(field0(%[[VALUE_s2848]])), read<u32>(field0(%[[VALUE_arg]])))
// DEFAULT-NEXT:             let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_fails]]);
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_fails]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         if ne<complex<i32>>(read<complex<i32>>(field1(%[[VALUE_s2848]])), read<complex<i32>>(field1(%[[VALUE_arg]])))
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_fails]]);
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_fails]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<u32>(field0(%[[VALUE_s2848]]), const<u32>(4027477739));
// DEFAULT-NEXT:         write<complex<i32>>(field1(%[[VALUE_s2848]]), add<complex<i32>, complex=true, overflow=ub>(const<i32>(723419448), mul<complex<i32>, complex=true, overflow=ub>(neg<i32, overflow=ub>(const<i32>(218144346)), aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c) -> void>(%[[VALUE_check2848va]], const<i32>(1), copy<@type[[TYPE_S2848]], reason=vararg>(read<@type[[TYPE_S2848]]>(%[[VALUE_s2848]])));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_fails]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
