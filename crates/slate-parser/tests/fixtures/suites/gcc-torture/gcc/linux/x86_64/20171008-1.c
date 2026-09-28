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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 c1: i8;
// DEFAULT-NEXT:         field1 c2: i8;
// DEFAULT-NEXT:         field2 c3: i8;
// DEFAULT-NEXT:         field3 c4: i8;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 1, 2, 3]];
// DEFAULT-NEXT:     global %3 i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @bar(%4 p: ptr<ptr<i8>>) -> i8 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(%3, const<i32>(1));
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=always>(const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @foo() -> @type0 [linkage=internal] [inline=never] [definition=emitted] [abi=sysv64() -> coerce<i32>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 ret: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %6 r: i8 [storage=automatic];
// DEFAULT-NEXT:         let %7 s: i8 [storage=automatic];
// DEFAULT-NEXT:         let %8 c1: i8 [storage=automatic];
// DEFAULT-NEXT:         let %9 c2: i8 [storage=automatic];
// DEFAULT-NEXT:         let %10 p: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(%6);
// DEFAULT-NEXT:         write<i8>(%7, call<i8, signature=fn(ptr<ptr<i8>>) -> i8>(%1, addr_of<ptr<ptr<i8>>>(%10)));
// DEFAULT-NEXT:         call<i8, signature=fn(ptr<ptr<i8>>) -> i8>(%1, addr_of<ptr<ptr<i8>>>(%10));
// DEFAULT-NEXT:         if ne<i8>(read<i8>(%7), const<i8>(0))
// DEFAULT-NEXT:             write<i8>(%9, read<i8>(deref(read<ptr<i8>>(%10))));
// DEFAULT-NEXT:         write<i8>(%8, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(field0(%5), read<i8>(%8));
// DEFAULT-NEXT:         write<i8>(field1(%5), read<i8>(%9));
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 s: @type0 [storage=automatic] = copy<@type0, reason=assign>(call<@type0, signature=fn() -> @type0, abi=sysv64() -> coerce<i32>>(%2));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%12))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%14);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
