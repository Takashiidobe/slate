extern void abort();
extern int  abs(int __x) __attribute__((__nothrow__, __leaf__))
__attribute__((__const__));

static int foo(unsigned char *w, int i, unsigned char *x, int j) {
  int tot = 0;
  for (int a = 0; a < 16; a++) {
    for (int b = 0; b < 16; b++)
      tot += abs(w[b] - x[b]);
    w += i;
    x += j;
  }
  return tot;
}

void bar(unsigned char *w, unsigned char *x, int i, int *result) {
  *result = foo(w, 16, x, i);
}

int main(void) {
  unsigned char m[256];
  unsigned char n[256];
  int           sum, i;

  for (i = 0; i < 256; ++i)
    if (i % 2 == 0) {
      m[i] = (i % 8) * 2 + 1;
      n[i] = -(i % 8);
    } else {
      m[i] = -((i % 8) * 2 + 2);
      n[i] = -((i % 8) >> 1);
    }

  bar(m, n, 16, &sum);

  if (sum != 32384)
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @abs(%21 __x: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %3 @foo(%4 w: ptr<u8>, %5 i: i32, %6 x: ptr<u8>, %7 j: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 tot: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %22
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %9 a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%9), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %25: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                 let %26: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%25), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%9, read<i32>(%26));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %23
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %10 b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%10), const<i32>(16))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %27: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                             let %28: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%27), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%10, read<i32>(%28));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             let %29: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                             let %30: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%29), call<i32, signature=fn(i32) -> i32>(%2, sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%4), read<i32>(%10)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%6), read<i32>(%10)))))))));
// DEFAULT-NEXT:                             write<i32>(%8, read<i32>(%30));
// DEFAULT-NEXT:                     let %31: ptr<u8> [synthetic] = read<ptr<u8>>(%4);
// DEFAULT-NEXT:                     let %32: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%31), read<i32>(%5));
// DEFAULT-NEXT:                     write<ptr<u8>>(%4, read<ptr<u8>>(%32));
// DEFAULT-NEXT:                     let %33: ptr<u8> [synthetic] = read<ptr<u8>>(%6);
// DEFAULT-NEXT:                     let %34: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%33), read<i32>(%7));
// DEFAULT-NEXT:                     write<ptr<u8>>(%6, read<ptr<u8>>(%34));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @bar(%12 w: ptr<u8>, %13 x: ptr<u8>, %14 i: i32, %15 result: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%15)), call<i32, signature=fn(ptr<u8>, i32, ptr<u8>, i32) -> i32>(%3, read<ptr<u8>>(%12), const<i32>(16), read<ptr<u8>>(%13), read<i32>(%14)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<u8>, i32, ptr<u8>, i32) -> i32>(%3, read<ptr<u8>>(%12), const<i32>(16), read<ptr<u8>>(%13), read<i32>(%14));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %17 m: array<u8, 256> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %18 n: array<u8, 256> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %19 sum: i32 [storage=automatic];
// DEFAULT-NEXT:         let %20 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %24
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%20, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%20), const<i32>(256))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %35: i32 [synthetic] = read<i32>(%20);
// DEFAULT-NEXT:                 let %36: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%35), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%20, read<i32>(%36));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if eq<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%20), const<i32>(2)), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(256)>(%17), read<i32>(%20))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(mul<i32, overflow=ub>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%20), const<i32>(8)), const<i32>(2)), const<i32>(1)))));
// DEFAULT-NEXT:                         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(256)>(%18), read<i32>(%20))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%20), const<i32>(8))))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(256)>(%17), read<i32>(%20))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%20), const<i32>(8)), const<i32>(2)), const<i32>(2))))));
// DEFAULT-NEXT:                         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(256)>(%18), read<i32>(%20))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%20), const<i32>(8)), const<i32>(1))))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u8>, ptr<u8>, i32, ptr<i32>) -> void>(%11, array_decay<ptr<u8>, length=Some(256)>(%17), array_decay<ptr<u8>, length=Some(256)>(%18), const<i32>(16), addr_of<ptr<i32>>(%19));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%19), const<i32>(32384))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
