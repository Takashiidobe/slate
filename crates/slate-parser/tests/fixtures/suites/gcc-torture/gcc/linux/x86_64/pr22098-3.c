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
// DEFAULT-NEXT:     type @type[[TYPE_uintptr_t:[0-9]+]] uintptr_t = u64;
// DEFAULT-NEXT:     global %[[VALUE_n:[0-9]+]] n: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: u64 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_p]], addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(compound_literal %[[VALUE3:[0-9]+]] [storage=automatic] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(0), index1 = call<i32, signature=fn() -> i32>(%[[VALUE_f]]), index2 = const<i32>(2))), const<i32>(1)))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_b]], ptr_to_int<u64, reason=explicit>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(compound_literal %[[VALUE3]] [storage=automatic] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(0), index1 = call<i32, signature=fn() -> i32>(%[[VALUE_f]]), index2 = const<i32>(2))), const<i32>(1))))));
// DEFAULT-NEXT:         ptr_to_int<u64, reason=explicit>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(compound_literal %[[VALUE3]] [storage=automatic] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(0), index1 = call<i32, signature=fn() -> i32>(%[[VALUE_f]]), index2 = const<i32>(2))), const<i32>(1)))));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_p]]))), const<i32>(1)), ne<i32>(read<i32>(deref(int_to_ptr<ptr<i32>, reason=explicit>(read<u64>(%[[VALUE_b]])))), const<i32>(1))), ne<i32>(read<i32>(%[[VALUE_n]]), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
