/* PR middle-end/36013 */

extern void abort(void);

void __attribute__((noinline)) foo(int **__restrict p, int **__restrict q) {
  *p[0] = 1;
  *q[0] = 2;
  if (*p[0] != 2)
    abort();
}

int main(void) {
  int  a;
  int *p1 = &a, *p2 = &a;
  foo(&p1, &p2);
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
// DEFAULT-NEXT:     fn %1 @foo(%2 p: ptr<ptr<i32>> [restrict], %3 q: ptr<ptr<i32>> [restrict]) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(deref(ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(read<ptr<ptr<i32>>>(%2), const<i32>(0))))), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(deref(ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(read<ptr<ptr<i32>>>(%3), const<i32>(0))))), const<i32>(2));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(read<ptr<i32>>(deref(ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(read<ptr<ptr<i32>>>(%2), const<i32>(0)))))), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 a: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 p1: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%5);
// DEFAULT-NEXT:         let %7 p2: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%5);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<i32>>, ptr<ptr<i32>>) -> void>(%1, addr_of<ptr<ptr<i32>>>(%6), addr_of<ptr<ptr<i32>>>(%7));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
