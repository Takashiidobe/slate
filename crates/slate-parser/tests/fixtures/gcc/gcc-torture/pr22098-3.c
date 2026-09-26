extern void              abort(void);
extern void              exit(int);
typedef __UINTPTR_TYPE__ uintptr_t;
int                      n = 0;
int                      f(void) { return ++n; }
int                      main(void) {
  int       a = 0;
  int      *p;
  uintptr_t b;
  b = (uintptr_t)(p = &(int[]){0, f(), 2}[1]);
  if (*p != 1 || *(int *)b != 1 || n != 1)
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
// DEFAULT-NEXT:     type @type0 uintptr_t = u64;
// DEFAULT-NEXT:     global %3 n: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%9 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @f() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %12: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%11), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%12));
// DEFAULT-NEXT:         return read<i32>(%12);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %7 p: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %8 b: u64 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%7, addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(compound_literal %10 [storage=automatic] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(0), index1 = call<i32, signature=fn() -> i32>(%4), index2 = const<i32>(2))), const<i32>(1)))));
// DEFAULT-NEXT:         write<u64>(%8, ptr_to_int<u64, reason=explicit>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(compound_literal %10 [storage=automatic] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(0), index1 = call<i32, signature=fn() -> i32>(%4), index2 = const<i32>(2))), const<i32>(1))))));
// DEFAULT-NEXT:         ptr_to_int<u64, reason=explicit>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(compound_literal %10 [storage=automatic] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(0), index1 = call<i32, signature=fn() -> i32>(%4), index2 = const<i32>(2))), const<i32>(1)))));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(read<ptr<i32>>(%7))), const<i32>(1)), ne<i32>(read<i32>(deref(int_to_ptr<ptr<i32>, reason=explicit>(read<u64>(%8)))), const<i32>(1))), ne<i32>(read<i32>(%3), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
