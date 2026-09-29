int *ptr1 = 0, **ptr2 = &ptr1;

int *identity(int *p) { return p; }

void store_to_c(int *p) { *ptr2 = identity(p); }

int main() {
  int f;
  store_to_c(&f);
  if (ptr1 != &f)
    __builtin_abort();
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
// DEFAULT-NEXT:     global %[[VALUE_ptr1:[0-9]+]] ptr1: ptr<i32> [storage=static] = null<ptr<i32>> [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ptr2:[0-9]+]] ptr2: ptr<ptr<i32>> [storage=static] = addr_of<ptr<ptr<i32>>>(%[[VALUE_ptr1]]) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_identity:[0-9]+]] @identity(%[[VALUE_p:[0-9]+]] p: ptr<i32>) -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<ptr<i32>>(%[[VALUE_p]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_store_to_c:[0-9]+]] @store_to_c(%[[VALUE_p_2:[0-9]+]] p: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE_ptr2]])), call<ptr<i32>, signature=fn(ptr<i32>) -> ptr<i32>>(%[[VALUE_identity]], read<ptr<i32>>(%[[VALUE_p_2]])));
// DEFAULT-NEXT:         call<ptr<i32>, signature=fn(ptr<i32>) -> ptr<i32>>(%[[VALUE_identity]], read<ptr<i32>>(%[[VALUE_p_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: i32 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_store_to_c]], addr_of<ptr<i32>>(%[[VALUE_f]]));
// DEFAULT-NEXT:         if ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_ptr1]]), addr_of<ptr<i32>>(%[[VALUE_f]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
