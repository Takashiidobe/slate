void abort(void);

void f(int *p, int **q) {
  int i;
  for (i = 0; i < 40; i++) {
    *q++ = &p[i];
  }
}

int main() {
  void         *p;
  int          *q[40];
  __SIZE_TYPE__ start;

  /* Find the signed middle of the address space.  */
  if (sizeof(start) == sizeof(int))
    start = (__SIZE_TYPE__)__INT_MAX__;
  else if (sizeof(start) == sizeof(long))
    start = (__SIZE_TYPE__)__LONG_MAX__;
  else if (sizeof(start) == sizeof(long long))
    start = (__SIZE_TYPE__)__LONG_LONG_MAX__;
  else
    return 0;

  /* Arbitrarily align the pointer.  */
  start &= -32;

  /* Pretend that's good enough to start address arithmetic.  */
  p = (void *)start;

  /* Verify that GIV replacement computes the correct results.  */
  q[39] = 0;
  f(p, q);
  if (q[39] != (int *)p + 39)
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
// DEFAULT-NEXT:     fn %1 @f(%2 p: ptr<i32>, %3 q: ptr<ptr<i32>>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %9
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%4, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%4), const<i32>(40))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%11));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %12: ptr<ptr<i32>> [synthetic] = read<ptr<ptr<i32>>>(%3);
// DEFAULT-NEXT:                     let %13: ptr<ptr<i32>> [synthetic] = ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(read<ptr<ptr<i32>>>(%12), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<ptr<i32>>>(%3, read<ptr<ptr<i32>>>(%13));
// DEFAULT-NEXT:                     write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%12)), addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%2), read<i32>(%4)))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 p: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         let %7 q: array<ptr<i32>, 40> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %8 start: u64 [storage=automatic];
// DEFAULT-NEXT:         if eq<u64>(const<u64>(8), const<u64>(4))
// DEFAULT-NEXT:             write<u64>(%8, reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2147483647))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if eq<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                 write<u64>(%8, reinterpret<u64, reason=explicit, fits=always>(const<i64>(9223372036854775807)));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 if eq<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:                     write<u64>(%8, reinterpret<u64, reason=explicit, fits=always>(const<i64>(9223372036854775807)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     return const<i32>(0);
// DEFAULT-NEXT:         let %14: u64 [synthetic] = read<u64>(%8);
// DEFAULT-NEXT:         let %15: u64 [synthetic] = and<u64>(read<u64>(%14), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(32)))));
// DEFAULT-NEXT:         write<u64>(%8, read<u64>(%15));
// DEFAULT-NEXT:         write<ptr<void>>(%6, int_to_ptr<ptr<void>, reason=explicit>(read<u64>(%8)));
// DEFAULT-NEXT:         write<ptr<i32>>(deref(ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(array_decay<ptr<ptr<i32>>, length=Some(40)>(%7), const<i32>(39))), null<ptr<i32>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, ptr<ptr<i32>>) -> void>(%1, pointer_cast<ptr<i32>, reason=arg>(read<ptr<void>>(%6)), array_decay<ptr<ptr<i32>>, length=Some(40)>(%7));
// DEFAULT-NEXT:         if ne<ptr<i32>>(read<ptr<i32>>(deref(ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(array_decay<ptr<ptr<i32>>, length=Some(40)>(%7), const<i32>(39)))), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(pointer_cast<ptr<i32>, reason=explicit>(read<ptr<void>>(%6)), const<i32>(39)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
