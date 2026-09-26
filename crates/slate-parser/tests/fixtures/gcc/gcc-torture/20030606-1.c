void abort(void);
void exit(int);

int *foo(int *x, int b) {

  *(x++) = 55;
  if (b)
    *(x++) = b;

  return x;
}

int main(void) {
  int a[5];

  __builtin_memset(a, 1, sizeof(a));

  if (foo(a, 0) - a != 1 || a[0] != 55 || a[1] != a[4])
    abort();

  __builtin_memset(a, 1, sizeof(a));

  if (foo(a, 2) - a != 2 || a[0] != 55 || a[1] != 2)
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%7 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 x: ptr<i32>, %4 b: i32) -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// DEFAULT-NEXT:         let %9: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%8), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%3, read<ptr<i32>>(%9));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%8)), const<i32>(55));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:             let %10: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// DEFAULT-NEXT:             let %11: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%10), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i32>>(%3, read<ptr<i32>>(%11));
// DEFAULT-NEXT:             write<i32>(deref(read<ptr<i32>>(%10)), read<i32>(%4));
// DEFAULT-NEXT:         return read<ptr<i32>>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 a: array<i32, 5> [storage=automatic] [align=16];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i32>, length=Some(5)>(%6)), const<i32>(1), const<u64>(20));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i64>(ptr_diff<i64, element=i32, same_array=required, overflow=ub>(call<ptr<i32>, signature=fn(ptr<i32>, i32) -> ptr<i32>>(%2, array_decay<ptr<i32>, length=Some(5)>(%6), const<i32>(0)), array_decay<ptr<i32>, length=Some(5)>(%6)), widen<i64, reason=usual_arith>(const<i32>(1))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%6), const<i32>(0)))), const<i32>(55))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%6), const<i32>(1)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%6), const<i32>(4))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i32>, length=Some(5)>(%6)), const<i32>(1), const<u64>(20));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i64>(ptr_diff<i64, element=i32, same_array=required, overflow=ub>(call<ptr<i32>, signature=fn(ptr<i32>, i32) -> ptr<i32>>(%2, array_decay<ptr<i32>, length=Some(5)>(%6), const<i32>(2)), array_decay<ptr<i32>, length=Some(5)>(%6)), widen<i64, reason=usual_arith>(const<i32>(2))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%6), const<i32>(0)))), const<i32>(55))), ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%6), const<i32>(1)))), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
