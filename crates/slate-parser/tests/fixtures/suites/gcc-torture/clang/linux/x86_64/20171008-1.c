struct S {
  char c1, c2, c3, c4;
} __attribute__((aligned(4)));

static char     bar(char **p) __attribute__((noclone, noinline));
static struct S foo(void) __attribute__((noclone, noinline));

int i;

static char bar(char **p) {
  i = 1;
  return 0;
}

static struct S foo(void) {
  struct S ret;
  char     r, s, c1, c2;
  char    *p = &r;

  s = bar(&p);
  if (s)
    c2 = *p;
  c1 = 0;

  ret.c1 = c1;
  ret.c2 = c2;
  return ret;
}

int main(void) {
  struct S s = foo();
  if (s.c1 != 0)
    __builtin_abort();
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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 c1: i8;
// DEFAULT-NEXT:         field1 c2: i8;
// DEFAULT-NEXT:         field2 c3: i8;
// DEFAULT-NEXT:         field3 c4: i8;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 1, 2, 3]];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_p:[0-9]+]] p: ptr<ptr<i8>>) -> i8 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], const<i32>(1));
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=always>(const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> @type[[TYPE_S]] [linkage=internal] [inline=never] [definition=emitted] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ret:[0-9]+]] ret: @type[[TYPE_S]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: i8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: i8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c1:[0-9]+]] c1: i8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c2:[0-9]+]] c2: i8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(%[[VALUE_r]]);
// DEFAULT-NEXT:         write<i8>(%[[VALUE_s]], call<i8, signature=fn(ptr<ptr<i8>>) -> i8>(%[[VALUE_bar]], addr_of<ptr<ptr<i8>>>(%[[VALUE_p_2]])));
// DEFAULT-NEXT:         if ne<i8>(read<i8>(%[[VALUE_s]]), const<i8>(0))
// DEFAULT-NEXT:             write<i8>(%[[VALUE_c2]], read<i8>(deref(read<ptr<i8>>(%[[VALUE_p_2]]))));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_c1]], truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(field0(%[[VALUE_ret]]), read<i8>(%[[VALUE_c1]]));
// DEFAULT-NEXT:         write<i8>(field1(%[[VALUE_ret]]), read<i8>(%[[VALUE_c2]]));
// DEFAULT-NEXT:         return copy<@type[[TYPE_S]], reason=return>(read<@type[[TYPE_S]]>(%[[VALUE_ret]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_s_2:[0-9]+]] s: @type[[TYPE_S]] [storage=automatic] = copy<@type[[TYPE_S]], reason=assign>(call<@type[[TYPE_S]], signature=fn() -> @type[[TYPE_S]], abi=sysv64() -> native_c>(%[[VALUE_foo]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_s_2]]))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
