/* { dg-options "-mtune=i686" { target { { i?86-*-* x86_64-*-* } && ia32 } } }
 */

extern void abort(void);
extern void exit(int);

long baz1(void *a) {
  static long l;
  return l++;
}

int baz2(const char *a) { return 0; }

int baz3(int i) {
  if (!i)
    abort();
  return 1;
}

void **bar;

int foo(void *a, long b, int c) {
  int    d = 0, e, f = 0, i;
  char   g[256];
  void **h;

  g[0] = '\n';
  g[1] = 0;

  while (baz1(a) < b) {
    if (g[0] != ' ' && g[0] != '\t') {
      f = 1;
      e = 0;
      if (!d && baz2(g) == 0) {
        if ((c & 0x10) == 0)
          continue;
        e = d = 1;
      }
      if (!((c & 0x10) && (c & 0x4000) && e) && (c & 2))
        continue;
      if ((c & 0x2000) && baz2(g) == 0)
        continue;
      if ((c & 0x1408) && baz2(g) == 0)
        continue;
      if ((c & 0x200) && baz2(g) == 0)
        continue;
      if (c & 0x80) {
        for (h = bar, i = 0; h; h = (void **)*h, i++)
          if (baz3(i))
            break;
      }
      f = 0;
    }
  }
  return 0;
}

int main() {
  void *n = 0;
  bar     = &n;
  foo(&n, 1, 0xc811);
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
// DEFAULT-NEXT:     global %4 l: i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %9 bar: ptr<ptr<void>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%22 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @baz1(%3 a: ptr<void>) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %25: i64 [synthetic] = read<i64>(%4);
// DEFAULT-NEXT:         let %26: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%25), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%4, read<i64>(%26));
// DEFAULT-NEXT:         return read<i64>(%25);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @baz2(%6 a: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @baz3(%8 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%8), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @foo(%11 a: ptr<void>, %12 b: i64, %13 c: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %14 d: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %15 e: i32 [storage=automatic];
// DEFAULT-NEXT:         let %16 f: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %17 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %18 g: array<i8, 256> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %19 h: ptr<ptr<void>> [storage=automatic];
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%18), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(10)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%18), const<i32>(1))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         while %23 lt<i64>(call<i64, signature=fn(ptr<void>) -> i64>(%2, read<ptr<void>>(%11)), read<i64>(%12))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_and<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%18), const<i32>(0))))), const<i32>(32)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%18), const<i32>(0))))), const<i32>(9)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i32>(%16, const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%15, const<i32>(0));
// DEFAULT-NEXT:                         let %27: bool [synthetic];
// DEFAULT-NEXT:                         if not<bool>(ne<i32>(read<i32>(%14), const<i32>(0)))
// DEFAULT-NEXT:                             write<bool>(%27, eq<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%5, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(256)>(%18))), const<i32>(0)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<bool>(%27, const<bool>(false));
// DEFAULT-NEXT:                         if read<bool>(%27)
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 if eq<i32>(and<i32>(read<i32>(%13), const<i32>(16)), const<i32>(0))
// DEFAULT-NEXT:                                     continue %23;
// DEFAULT-NEXT:                                 write<i32>(%14, const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%15, const<i32>(1));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         if logical_and<bool>(not<bool>(logical_and<bool>(logical_and<bool>(ne<i32>(and<i32>(read<i32>(%13), const<i32>(16)), const<i32>(0)), ne<i32>(and<i32>(read<i32>(%13), const<i32>(16384)), const<i32>(0))), ne<i32>(read<i32>(%15), const<i32>(0)))), ne<i32>(and<i32>(read<i32>(%13), const<i32>(2)), const<i32>(0)))
// DEFAULT-NEXT:                             continue %23;
// DEFAULT-NEXT:                         let %28: bool [synthetic];
// DEFAULT-NEXT:                         if ne<i32>(and<i32>(read<i32>(%13), const<i32>(8192)), const<i32>(0))
// DEFAULT-NEXT:                             write<bool>(%28, eq<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%5, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(256)>(%18))), const<i32>(0)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<bool>(%28, const<bool>(false));
// DEFAULT-NEXT:                         if read<bool>(%28)
// DEFAULT-NEXT:                             continue %23;
// DEFAULT-NEXT:                         let %29: bool [synthetic];
// DEFAULT-NEXT:                         if ne<i32>(and<i32>(read<i32>(%13), const<i32>(5128)), const<i32>(0))
// DEFAULT-NEXT:                             write<bool>(%29, eq<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%5, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(256)>(%18))), const<i32>(0)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<bool>(%29, const<bool>(false));
// DEFAULT-NEXT:                         if read<bool>(%29)
// DEFAULT-NEXT:                             continue %23;
// DEFAULT-NEXT:                         let %30: bool [synthetic];
// DEFAULT-NEXT:                         if ne<i32>(and<i32>(read<i32>(%13), const<i32>(512)), const<i32>(0))
// DEFAULT-NEXT:                             write<bool>(%30, eq<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%5, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(256)>(%18))), const<i32>(0)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<bool>(%30, const<bool>(false));
// DEFAULT-NEXT:                         if read<bool>(%30)
// DEFAULT-NEXT:                             continue %23;
// DEFAULT-NEXT:                         if ne<i32>(and<i32>(read<i32>(%13), const<i32>(128)), const<i32>(0))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 for %24
// DEFAULT-NEXT:                                     init:
// DEFAULT-NEXT:                                         write<ptr<ptr<void>>>(%19, read<ptr<ptr<void>>>(%9));
// DEFAULT-NEXT:                                         write<i32>(%17, const<i32>(0));
// DEFAULT-NEXT:                                     condition: ne<ptr<ptr<void>>>(read<ptr<ptr<void>>>(%19), null<ptr<ptr<void>>>)
// DEFAULT-NEXT:                                     increment: {
// DEFAULT-NEXT:                                         write<ptr<ptr<void>>>(%19, pointer_cast<ptr<ptr<void>>, reason=explicit>(read<ptr<void>>(deref(read<ptr<ptr<void>>>(%19)))));
// DEFAULT-NEXT:                                         let %31: i32 [synthetic] = read<i32>(%17);
// DEFAULT-NEXT:                                         let %32: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%31), const<i32>(1));
// DEFAULT-NEXT:                                         write<i32>(%17, read<i32>(%32));
// DEFAULT-NEXT:                                         yield void;
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     body:
// DEFAULT-NEXT:                                         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%7, read<i32>(%17)), const<i32>(0))
// DEFAULT-NEXT:                                             break %24;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         write<i32>(%16, const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %21 n: ptr<void> [storage=automatic] = null<ptr<void>>;
// DEFAULT-NEXT:         write<ptr<ptr<void>>>(%9, addr_of<ptr<ptr<void>>>(%21));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<void>, i64, i32) -> i32>(%10, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<ptr<void>>>(%21)), widen<i64, reason=arg>(const<i32>(1)), const<i32>(51217));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
