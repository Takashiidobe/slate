extern void abort();
extern int  abs(int __x) __attribute__((__nothrow__, __leaf__))
__attribute__((__const__));

static int foo(signed char *w, int i, signed char *x, int j) {
  int tot = 0;
  for (int a = 0; a < 16; a++) {
    for (int b = 0; b < 16; b++)
      tot += abs(w[b] - x[b]);
    w += i;
    x += j;
  }
  return tot;
}

void bar(signed char *w, signed char *x, int i, int *result) {
  *result = foo(w, 16, x, i);
}

int main(void) {
  signed char m[256];
  signed char n[256];
  int         sum, i;

  for (i = 0; i < 256; ++i)
    if (i % 2 == 0) {
      m[i] = (i % 8) * 2 + 1;
      n[i] = -(i % 8);
    } else {
      m[i] = -((i % 8) * 2 + 2);
      n[i] = -((i % 8) >> 1);
    }

  bar(m, n, 16, &sum);

  if (sum != 2368)
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @abs(%20 __x: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 w: ptr<i8>, %4 i: i32, %5 x: ptr<i8>, %6 j: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 tot: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %21
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %8 a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%8), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %24: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%25));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %22
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %9 b: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%9), const<i32>(16))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %26: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                             let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%9, read<i32>(%27));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             let %28: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                             let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), call<i32, signature=fn(i32) -> i32>(abs, sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%3), read<i32>(%9))))), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%5), read<i32>(%9))))))));
// DEFAULT-NEXT:                             write<i32>(%7, read<i32>(%29));
// DEFAULT-NEXT:                     let %30: ptr<i8> [synthetic] = read<ptr<i8>>(%3);
// DEFAULT-NEXT:                     let %31: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%30), read<i32>(%4));
// DEFAULT-NEXT:                     write<ptr<i8>>(%3, read<ptr<i8>>(%31));
// DEFAULT-NEXT:                     let %32: ptr<i8> [synthetic] = read<ptr<i8>>(%5);
// DEFAULT-NEXT:                     let %33: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%32), read<i32>(%6));
// DEFAULT-NEXT:                     write<ptr<i8>>(%5, read<ptr<i8>>(%33));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @bar(%11 w: ptr<i8>, %12 x: ptr<i8>, %13 i: i32, %14 result: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%14)), call<i32, signature=fn(ptr<i8>, i32, ptr<i8>, i32) -> i32>(%2, read<ptr<i8>>(%11), const<i32>(16), read<ptr<i8>>(%12), read<i32>(%13)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i8>, i32, ptr<i8>, i32) -> i32>(%2, read<ptr<i8>>(%11), const<i32>(16), read<ptr<i8>>(%12), read<i32>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %16 m: array<i8, 256> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %17 n: array<i8, 256> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %18 sum: i32 [storage=automatic];
// DEFAULT-NEXT:         let %19 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %23
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%19, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%19), const<i32>(256))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %34: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                 let %35: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%34), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%19, read<i32>(%35));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if eq<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%19), const<i32>(2)), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%16), read<i32>(%19))), truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(mul<i32, overflow=ub>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%19), const<i32>(8)), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:                         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%17), read<i32>(%19))), truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%19), const<i32>(8)))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%16), read<i32>(%19))), truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%19), const<i32>(8)), const<i32>(2)), const<i32>(2)))));
// DEFAULT-NEXT:                         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(256)>(%17), read<i32>(%19))), truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%19), const<i32>(8)), const<i32>(1)))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, ptr<i8>, i32, ptr<i32>) -> void>(%10, array_decay<ptr<i8>, length=Some(256)>(%16), array_decay<ptr<i8>, length=Some(256)>(%17), const<i32>(16), addr_of<ptr<i32>>(%18));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%18), const<i32>(2368))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
