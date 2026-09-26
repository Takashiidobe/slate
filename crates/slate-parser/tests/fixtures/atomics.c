#include <stdatomic.h>
#include <stdio.h>

int main(void) {
  atomic_int a = 0;

  atomic_store(&a, 100);
  int loaded = atomic_load(&a);

  int fa   = atomic_fetch_add(&a, 5);    // returns 100, a = 105
  int fs   = atomic_fetch_sub(&a, 10);   // returns 105, a = 95
  int fand = atomic_fetch_and(&a, 0x3C); // 95 & 60 = 28
  int forr = atomic_fetch_or(&a, 0x01);  // 28 | 1 = 29
  int fxor = atomic_fetch_xor(&a, 0x0F); // 29 ^ 15 = 18

  int xchg_old = atomic_exchange(&a, 7); // returns 18, a = 7

  int expected = 7;
  int ok = atomic_compare_exchange_strong(&a, &expected, 42); // success, a = 42
  int expected2 = 999;
  int bad =
      atomic_compare_exchange_strong(&a, &expected2, 0); // fail, a stays 42

  atomic_thread_fence(memory_order_seq_cst);

  printf("%d %d %d %d %d %d %d %d %d %d %d %d\n", loaded, fa, fs, fand, forr,
         fxor, xchg_old, ok, expected, bad, expected2, (int)a);
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
// DEFAULT-NEXT:     type @type0 memory_order = enum : u32 {
// DEFAULT-NEXT:         %0 memory_order_relaxed = const<i32>(0);
// DEFAULT-NEXT:         %1 memory_order_consume = const<i32>(1);
// DEFAULT-NEXT:         %2 memory_order_acquire = const<i32>(2);
// DEFAULT-NEXT:         %3 memory_order_release = const<i32>(3);
// DEFAULT-NEXT:         %4 memory_order_acq_rel = const<i32>(4);
// DEFAULT-NEXT:         %5 memory_order_seq_cst = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 memory_order = @type0;
// DEFAULT-NEXT:     type @type2 atomic_int = i32;
// DEFAULT-NEXT:     global %24 .str24: array<i8, 37> [storage=static] = code_units<array<i8, 37>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %9 @printf(%23 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 a: atomic i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%11)), const<i32>(100));
// DEFAULT-NEXT:         let %12 loaded: i32 [storage=automatic] = read<i32, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%11)));
// DEFAULT-NEXT:         let %13 fa: i32 [storage=automatic];
// DEFAULT-NEXT:         let %25: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%11)), add<i32, overflow=wrap>(old<i32>, const<i32>(5)));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%25));
// DEFAULT-NEXT:         let %14 fs: i32 [storage=automatic];
// DEFAULT-NEXT:         let %26: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%11)), sub<i32, overflow=wrap>(old<i32>, const<i32>(10)));
// DEFAULT-NEXT:         write<i32>(%14, read<i32>(%26));
// DEFAULT-NEXT:         let %15 fand: i32 [storage=automatic];
// DEFAULT-NEXT:         let %27: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%11)), and<i32>(old<i32>, const<i32>(60)));
// DEFAULT-NEXT:         write<i32>(%15, read<i32>(%27));
// DEFAULT-NEXT:         let %16 forr: i32 [storage=automatic];
// DEFAULT-NEXT:         let %28: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%11)), or<i32>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(%16, read<i32>(%28));
// DEFAULT-NEXT:         let %17 fxor: i32 [storage=automatic];
// DEFAULT-NEXT:         let %29: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%11)), xor<i32>(old<i32>, const<i32>(15)));
// DEFAULT-NEXT:         write<i32>(%17, read<i32>(%29));
// DEFAULT-NEXT:         let %18 xchg_old: i32 [storage=automatic];
// DEFAULT-NEXT:         let %30: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%11)), const<i32>(7));
// DEFAULT-NEXT:         write<i32>(%18, read<i32>(%30));
// DEFAULT-NEXT:         let %19 expected: i32 [storage=automatic] = const<i32>(7);
// DEFAULT-NEXT:         let %20 ok: i32 [storage=automatic];
// DEFAULT-NEXT:         let %31: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<atomic i32>>(%11)), addr_of<ptr<i32>>(%19), const<i32>(42));
// DEFAULT-NEXT:         write<i32>(%20, from_bool<i32, reason=assign>(read<bool>(%31)));
// DEFAULT-NEXT:         let %21 expected2: i32 [storage=automatic] = const<i32>(999);
// DEFAULT-NEXT:         let %22 bad: i32 [storage=automatic];
// DEFAULT-NEXT:         let %32: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<atomic i32>>(%11)), addr_of<ptr<i32>>(%21), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%22, from_bool<i32, reason=assign>(read<bool>(%32)));
// DEFAULT-NEXT:         fence<scope=thread, order=seq_cst>;
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(37)>(%24)), read<i32>(%12), read<i32>(%13), read<i32>(%14), read<i32>(%15), read<i32>(%16), read<i32>(%17), read<i32>(%18), read<i32>(%20), read<i32>(%19), read<i32>(%22), read<i32>(%21), read<i32, atomic=seq_cst>(%11));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
