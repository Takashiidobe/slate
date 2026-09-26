/* Bombed with a segfault on powerpc-linux.  doloop.c generated wrong
   loop count.  */
void abort(void);

void foo(unsigned long *start, unsigned long *end) {
  unsigned long *temp = end - 1;

  while (end > start)
    *end-- = *temp--;
}

int main(void) {
  unsigned long a[5];
  int           start, end, k;

  for (start = 0; start < 5; start++)
    for (end = 0; end < 5; end++) {
      for (k = 0; k < 5; k++)
        a[k] = k;

      foo(a + start, a + end);

      for (k = 0; k <= start; k++)
        if (a[k] != k)
          abort();

      for (k = start + 1; k <= end; k++)
        if (a[k] != k - 1)
          abort();

      for (k = end + 1; k < 5; k++)
        if (a[k] != k)
          abort();
    }

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
// DEFAULT-NEXT:     fn %1 @foo(%2 start: ptr<u64>, %3 end: ptr<u64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 temp: ptr<u64> [storage=automatic] = ptr_offset<ptr<u64>, subtract=true, element=u64, overflow=ub>(read<ptr<u64>>(%3), const<i32>(1));
// DEFAULT-NEXT:         while %10 gt<ptr<u64>>(read<ptr<u64>>(%3), read<ptr<u64>>(%2))
// DEFAULT-NEXT:             let %17: ptr<u64> [synthetic] = read<ptr<u64>>(%4);
// DEFAULT-NEXT:             let %18: ptr<u64> [synthetic] = ptr_offset<ptr<u64>, subtract=true, element=u64, overflow=ub>(read<ptr<u64>>(%17), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<u64>>(%4, read<ptr<u64>>(%18));
// DEFAULT-NEXT:             let %19: ptr<u64> [synthetic] = read<ptr<u64>>(%3);
// DEFAULT-NEXT:             let %20: ptr<u64> [synthetic] = ptr_offset<ptr<u64>, subtract=true, element=u64, overflow=ub>(read<ptr<u64>>(%19), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<u64>>(%3, read<ptr<u64>>(%20));
// DEFAULT-NEXT:             write<u64>(deref(read<ptr<u64>>(%19)), read<u64>(deref(read<ptr<u64>>(%17))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 a: array<u64, 5> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %7 start: i32 [storage=automatic];
// DEFAULT-NEXT:         let %8 end: i32 [storage=automatic];
// DEFAULT-NEXT:         let %9 k: i32 [storage=automatic];
// DEFAULT-NEXT:         for %11
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%7), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %21: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%22));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %12
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%8, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%8), const<i32>(5))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %23: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                         let %24: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%23), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%8, read<i32>(%24));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             for %13
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%9, const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<i32>(read<i32>(%9), const<i32>(5))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %25: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                                     let %26: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%25), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%9, read<i32>(%26));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(5)>(%6), read<i32>(%9))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%9))));
// DEFAULT-NEXT:                             call<void, signature=fn(ptr<u64>, ptr<u64>) -> void>(%1, ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(5)>(%6), read<i32>(%7)), ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(5)>(%6), read<i32>(%8)));
// DEFAULT-NEXT:                             for %14
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%9, const<i32>(0));
// DEFAULT-NEXT:                                 condition: le<i32>(read<i32>(%9), read<i32>(%7))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %27: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                                     let %28: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%27), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%9, read<i32>(%28));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     if ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(5)>(%6), read<i32>(%9)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%9))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             for %15
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%9, add<i32, overflow=ub>(read<i32>(%7), const<i32>(1)));
// DEFAULT-NEXT:                                 condition: le<i32>(read<i32>(%9), read<i32>(%8))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %29: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                                     let %30: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%29), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%9, read<i32>(%30));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     if ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(5)>(%6), read<i32>(%9)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(read<i32>(%9), const<i32>(1)))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                             for %16
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%9, add<i32, overflow=ub>(read<i32>(%8), const<i32>(1)));
// DEFAULT-NEXT:                                 condition: lt<i32>(read<i32>(%9), const<i32>(5))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %31: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                                     let %32: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%31), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%9, read<i32>(%32));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     if ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(5)>(%6), read<i32>(%9)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%9))))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
