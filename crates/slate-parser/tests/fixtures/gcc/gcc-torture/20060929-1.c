/* PR c/29154 */

extern void abort(void);

void foo(int **p, int *q) { *(*p++)++ = *q++; }

void bar(int **p, int *q) {
  **p = *q++;
  *(*p++)++;
}

void baz(int **p, int *q) {
  **p = *q++;
  (*p++)++;
}

int main(void) {
  int  i = 42, j = 0;
  int *p = &i;
  foo(&p, &j);
  if (p - 1 != &i || j != 0 || i != 0)
    abort();
  i = 43;
  p = &i;
  bar(&p, &j);
  if (p - 1 != &i || j != 0 || i != 0)
    abort();
  i = 44;
  p = &i;
  baz(&p, &j);
  if (p - 1 != &i || j != 0 || i != 0)
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
// DEFAULT-NEXT:     fn %1 @foo(%2 p: ptr<ptr<i32>>, %3 q: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %14: ptr<i32> [synthetic] = read<ptr<i32>>(%3);
// DEFAULT-NEXT:         let %15: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%14), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%3, read<ptr<i32>>(%15));
// DEFAULT-NEXT:         let %16: ptr<ptr<i32>> [synthetic] = read<ptr<ptr<i32>>>(%2);
// DEFAULT-NEXT:         let %17: ptr<ptr<i32>> [synthetic] = ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(read<ptr<ptr<i32>>>(%16), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<ptr<i32>>>(%2, read<ptr<ptr<i32>>>(%17));
// DEFAULT-NEXT:         let %18: ptr<ptr<i32>> [synthetic] = read<ptr<ptr<i32>>>(%16);
// DEFAULT-NEXT:         let %19: ptr<i32> [synthetic] = read<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%18)));
// DEFAULT-NEXT:         let %20: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%19), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%18)), read<ptr<i32>>(%20));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%19)), read<i32>(deref(read<ptr<i32>>(%14))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @bar(%5 p: ptr<ptr<i32>>, %6 q: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %21: ptr<i32> [synthetic] = read<ptr<i32>>(%6);
// DEFAULT-NEXT:         let %22: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%21), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%6, read<ptr<i32>>(%22));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%5)))), read<i32>(deref(read<ptr<i32>>(%21))));
// DEFAULT-NEXT:         let %23: ptr<ptr<i32>> [synthetic] = read<ptr<ptr<i32>>>(%5);
// DEFAULT-NEXT:         let %24: ptr<ptr<i32>> [synthetic] = ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(read<ptr<ptr<i32>>>(%23), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<ptr<i32>>>(%5, read<ptr<ptr<i32>>>(%24));
// DEFAULT-NEXT:         let %25: ptr<ptr<i32>> [synthetic] = read<ptr<ptr<i32>>>(%23);
// DEFAULT-NEXT:         let %26: ptr<i32> [synthetic] = read<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%25)));
// DEFAULT-NEXT:         let %27: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%26), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%25)), read<ptr<i32>>(%27));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @baz(%8 p: ptr<ptr<i32>>, %9 q: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %28: ptr<i32> [synthetic] = read<ptr<i32>>(%9);
// DEFAULT-NEXT:         let %29: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%28), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(%9, read<ptr<i32>>(%29));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%8)))), read<i32>(deref(read<ptr<i32>>(%28))));
// DEFAULT-NEXT:         let %30: ptr<ptr<i32>> [synthetic] = read<ptr<ptr<i32>>>(%8);
// DEFAULT-NEXT:         let %31: ptr<ptr<i32>> [synthetic] = ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(read<ptr<ptr<i32>>>(%30), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<ptr<i32>>>(%8, read<ptr<ptr<i32>>>(%31));
// DEFAULT-NEXT:         let %32: ptr<ptr<i32>> [synthetic] = read<ptr<ptr<i32>>>(%30);
// DEFAULT-NEXT:         let %33: ptr<i32> [synthetic] = read<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%32)));
// DEFAULT-NEXT:         let %34: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%33), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%32)), read<ptr<i32>>(%34));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 i: i32 [storage=automatic] = const<i32>(42);
// DEFAULT-NEXT:         let %12 j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %13 p: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%11);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<i32>>, ptr<i32>) -> void>(%1, addr_of<ptr<ptr<i32>>>(%13), addr_of<ptr<i32>>(%12));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<ptr<i32>>(ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%13), const<i32>(1)), addr_of<ptr<i32>>(%11)), ne<i32>(read<i32>(%12), const<i32>(0))), ne<i32>(read<i32>(%11), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%11, const<i32>(43));
// DEFAULT-NEXT:         write<ptr<i32>>(%13, addr_of<ptr<i32>>(%11));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<i32>>, ptr<i32>) -> void>(%4, addr_of<ptr<ptr<i32>>>(%13), addr_of<ptr<i32>>(%12));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<ptr<i32>>(ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%13), const<i32>(1)), addr_of<ptr<i32>>(%11)), ne<i32>(read<i32>(%12), const<i32>(0))), ne<i32>(read<i32>(%11), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%11, const<i32>(44));
// DEFAULT-NEXT:         write<ptr<i32>>(%13, addr_of<ptr<i32>>(%11));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<i32>>, ptr<i32>) -> void>(%7, addr_of<ptr<ptr<i32>>>(%13), addr_of<ptr<i32>>(%12));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<ptr<i32>>(ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%13), const<i32>(1)), addr_of<ptr<i32>>(%11)), ne<i32>(read<i32>(%12), const<i32>(0))), ne<i32>(read<i32>(%11), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
