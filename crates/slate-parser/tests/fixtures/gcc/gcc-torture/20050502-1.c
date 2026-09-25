/* PR rtl-optimization/21330 */

extern void abort(void);
extern int  strcmp(const char *, const char *);

int __attribute__((noinline)) bar(const char **x) { return *(*x)++; }

int __attribute__((noinline)) baz(int c) { return c != '@'; }

void __attribute__((noinline)) foo(const char **w, char *x, _Bool y, _Bool z) {
  char c = bar(w);
  int  i = 0;

  while (1) {
    x[i++] = c;
    c      = bar(w);
    if (y && c == '\'')
      break;
    if (z && c == '\"')
      break;
    if (!y && !z && !baz(c))
      break;
  }
  x[i] = 0;
}

int main(void) {
  char        buf[64];
  const char *p;
  p = "abcde'fgh";
  foo(&p, buf, 1, 0);
  if (strcmp(p, "fgh") != 0 || strcmp(buf, "abcde") != 0)
    abort();
  p = "ABCDEFG\"HI";
  foo(&p, buf, 0, 1);
  if (strcmp(p, "HI") != 0 || strcmp(buf, "ABCDEFG") != 0)
    abort();
  p = "abcd\"e'fgh";
  foo(&p, buf, 1, 1);
  if (strcmp(p, "e'fgh") != 0 || strcmp(buf, "abcd") != 0)
    abort();
  p = "ABCDEF'G\"HI";
  foo(&p, buf, 1, 1);
  if (strcmp(p, "G\"HI") != 0 || strcmp(buf, "ABCDEF") != 0)
    abort();
  p = "abcdef@gh";
  foo(&p, buf, 0, 0);
  if (strcmp(p, "gh") != 0 || strcmp(buf, "abcdef") != 0)
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
// DEFAULT-NEXT:     global %19 .str19: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([97, 98, 99, 100, 101, 39, 102, 103, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([102, 103, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([97, 98, 99, 100, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([65, 66, 67, 68, 69, 70, 71, 34, 72, 73, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([72, 73, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %24 .str24: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([65, 66, 67, 68, 69, 70, 71, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %25 .str25: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([97, 98, 99, 100, 34, 101, 39, 102, 103, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %26 .str26: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([101, 39, 102, 103, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 98, 99, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([65, 66, 67, 68, 69, 70, 39, 71, 34, 72, 73, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %29 .str29: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([71, 34, 72, 73, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %30 .str30: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([65, 66, 67, 68, 69, 70, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([97, 98, 99, 100, 101, 102, 64, 103, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([103, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %33 .str33: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([97, 98, 99, 100, 101, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @strcmp(%16 <unnamed>: ptr<const i8>, %17 <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @bar(%3 x: ptr<ptr<const i8>>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %34: ptr<ptr<const i8>> [synthetic] = read<ptr<ptr<const i8>>>(%3);
// DEFAULT-NEXT:         let %35: ptr<const i8> [synthetic] = read<ptr<const i8>>(deref(read<ptr<ptr<const i8>>>(%34)));
// DEFAULT-NEXT:         let %36: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%35), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<const i8>>(deref(read<ptr<ptr<const i8>>>(%34)), read<ptr<const i8>>(%36));
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(read<ptr<const i8>>(%35))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @baz(%5 c: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(read<i32>(%5), const<i32>(64)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @foo(%7 w: ptr<ptr<const i8>>, %8 x: ptr<i8>, %9 y: bool, %10 z: bool) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %11 c: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(call<i32, signature=fn(ptr<ptr<const i8>>) -> i32>(%2, read<ptr<ptr<const i8>>>(%7)));
// DEFAULT-NEXT:         let %12 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         while %18 ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %37: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                 let %38: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%37), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%12, read<i32>(%38));
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%8), read<i32>(%37))), read<i8>(%11));
// DEFAULT-NEXT:                 write<i8>(%11, truncate<i8, reason=assign, fits=unknown>(call<i32, signature=fn(ptr<ptr<const i8>>) -> i32>(%2, read<ptr<ptr<const i8>>>(%7))));
// DEFAULT-NEXT:                 truncate<i8, reason=assign, fits=unknown>(call<i32, signature=fn(ptr<ptr<const i8>>) -> i32>(%2, read<ptr<ptr<const i8>>>(%7)));
// DEFAULT-NEXT:                 if logical_and<bool>(read<bool>(%9), eq<i32>(widen<i32, reason=promotion>(read<i8>(%11)), const<i32>(39)))
// DEFAULT-NEXT:                     break %18;
// DEFAULT-NEXT:                 if logical_and<bool>(read<bool>(%10), eq<i32>(widen<i32, reason=promotion>(read<i8>(%11)), const<i32>(34)))
// DEFAULT-NEXT:                     break %18;
// DEFAULT-NEXT:                 let %39: bool [synthetic];
// DEFAULT-NEXT:                 if logical_and<bool>(not<bool>(read<bool>(%9)), not<bool>(read<bool>(%10)))
// DEFAULT-NEXT:                     write<bool>(%39, not<bool>(ne<i32>(call<i32, signature=fn(i32) -> i32>(%4, widen<i32, reason=arg>(read<i8>(%11))), const<i32>(0))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%39, const<bool>(false));
// DEFAULT-NEXT:                 if read<bool>(%39)
// DEFAULT-NEXT:                     break %18;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%8), read<i32>(%12))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14 buf: array<i8, 64> [storage=automatic];
// DEFAULT-NEXT:         let %15 p: ptr<const i8> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<const i8>>(%15, pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%19)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<const i8>>, ptr<i8>, bool, bool) -> void>(%6, addr_of<ptr<ptr<const i8>>>(%15), array_decay<ptr<i8>, length=Some(64)>(%14), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         let %40: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%1, read<ptr<const i8>>(%15), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%20))), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%40, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%40, ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%14)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%21))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%40)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<ptr<const i8>>(%15, pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(11)>(%22)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<const i8>>, ptr<i8>, bool, bool) -> void>(%6, addr_of<ptr<ptr<const i8>>>(%15), array_decay<ptr<i8>, length=Some(64)>(%14), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         let %41: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%1, read<ptr<const i8>>(%15), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%23))), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%41, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%41, ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%14)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%24))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%41)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<ptr<const i8>>(%15, pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(11)>(%25)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<const i8>>, ptr<i8>, bool, bool) -> void>(%6, addr_of<ptr<ptr<const i8>>>(%15), array_decay<ptr<i8>, length=Some(64)>(%14), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         let %42: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%1, read<ptr<const i8>>(%15), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%26))), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%42, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%42, ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%14)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%27))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%42)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<ptr<const i8>>(%15, pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(12)>(%28)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<const i8>>, ptr<i8>, bool, bool) -> void>(%6, addr_of<ptr<ptr<const i8>>>(%15), array_decay<ptr<i8>, length=Some(64)>(%14), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         let %43: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%1, read<ptr<const i8>>(%15), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%29))), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%43, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%43, ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%14)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%30))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%43)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<ptr<const i8>>(%15, pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%31)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<const i8>>, ptr<i8>, bool, bool) -> void>(%6, addr_of<ptr<ptr<const i8>>>(%15), array_decay<ptr<i8>, length=Some(64)>(%14), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         let %44: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%1, read<ptr<const i8>>(%15), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%32))), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%44, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%44, ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%14)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%33))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%44)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
