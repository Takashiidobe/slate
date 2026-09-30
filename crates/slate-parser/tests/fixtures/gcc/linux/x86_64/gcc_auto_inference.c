// SLATE-FILECHECK-DEFINES VALID
// SLATE-FILECHECK-DEFINES POINTER POINTER
// SLATE-FILECHECK-DEFINES MULTIPLE MULTIPLE
// SLATE-FILECHECK-IR-ERROR POINTER
// SLATE-FILECHECK-IR-ERROR MULTIPLE
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu23

const int ci = 1;
int arr[3];
_Atomic int ai;

void deduce(int *ip) {
  __auto_type a1 = ci;
  _Static_assert(_Generic(&a1, int *: 1, default: 0));
  const __auto_type a2 = arr;
  _Static_assert(_Generic(&a2, int *const *: 1, default: 0));
  auto a3 = ip;
  _Static_assert(_Generic(&a3, int **: 1, default: 0));
  static auto a4 = 4;
  auto a5 = ai;
  _Static_assert(_Generic(&a5, int *: 1, default: 0));
  (void)a1, (void)a2, (void)a3, (void)a4, (void)a5;

#ifdef POINTER
  auto *p = ip;
#endif
#ifdef MULTIPLE
  auto x = 1, y = 2;
#endif
}

// SLATE-FILECHECK-BEGIN POINTER
// POINTER: Error:   × semantic analysis failed
// POINTER: Error:
// POINTER: × 'auto' requires a plain identifier as declarator
// POINTER: ╭─[tests/fixtures/gcc/linux/x86_64/gcc_auto_inference.c:19:8]
// POINTER: 18 │ #ifdef POINTER
// POINTER: 19 │   auto *p = ip;
// POINTER: ·        ───────
// POINTER: 20 │ #endif
// POINTER: ╰────
// SLATE-FILECHECK-END POINTER
// SLATE-FILECHECK-BEGIN MULTIPLE
// MULTIPLE: Error:   × semantic analysis failed
// MULTIPLE: Error:
// MULTIPLE: × 'auto' may only be used with a single declarator
// MULTIPLE: ╭─[tests/fixtures/gcc/linux/x86_64/gcc_auto_inference.c:22:15]
// MULTIPLE: 21 │ #ifdef MULTIPLE
// MULTIPLE: 22 │   auto x = 1, y = 2;
// MULTIPLE: ·               ─────
// MULTIPLE: 23 │ #endif
// MULTIPLE: ╰────
// SLATE-FILECHECK-END MULTIPLE
// SLATE-FILECHECK-BEGIN VALID
// VALID: module {
// VALID-NEXT:     target "x86_64-unknown-linux-gnu" {
// VALID-NEXT:         endian = little;
// VALID-NEXT:         pointer [size=8, align=8];
// VALID-NEXT:         stack_alignment = 16;
// VALID-NEXT:         long_double = f80;
// VALID-NEXT:         storage bool [size=1, align=1];
// VALID-NEXT:         storage i8, u8 [size=1, align=1];
// VALID-NEXT:         storage i16, u16 [size=2, align=2];
// VALID-NEXT:         storage i32, u32 [size=4, align=4];
// VALID-NEXT:         storage i64, u64 [size=8, align=8];
// VALID-NEXT:         storage i128, u128 [size=16, align=16];
// VALID-NEXT:         storage bf16 [size=2, align=2];
// VALID-NEXT:         storage f16 [size=2, align=2];
// VALID-NEXT:         storage f32 [size=4, align=4];
// VALID-NEXT:         storage f64 [size=8, align=8];
// VALID-NEXT:         storage f80 [size=16, align=16];
// VALID-NEXT:         storage f128 [size=16, align=16];
// VALID-NEXT:         storage d32 [size=4, align=4];
// VALID-NEXT:         storage d64 [size=8, align=8];
// VALID-NEXT:         storage d128 [size=16, align=16];
// VALID-NEXT:     }
// VALID-NEXT:     global %[[VALUE_ci:[0-9]+]] ci: i32 [storage=static] [const] = const<i32>(1) [linkage=external];
// VALID-NEXT:     global %[[VALUE_arr:[0-9]+]] arr: array<i32, 3> [storage=static] [linkage=external];
// VALID-NEXT:     global %[[VALUE_ai:[0-9]+]] ai: atomic i32 [storage=static] [linkage=external];
// VALID-NEXT:     global %[[VALUE_a4:[0-9]+]] a4: i32 [storage=static] = const<i32>(4) [linkage=internal];
// VALID-NEXT:     fn %[[VALUE_deduce:[0-9]+]] @deduce(%[[VALUE_ip:[0-9]+]] ip: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// VALID-NEXT:         let %[[VALUE_a1:[0-9]+]] a1: i32 [storage=automatic] = read<i32>(%[[VALUE_ci]]);
// VALID-NEXT:         let %[[VALUE_a2:[0-9]+]] a2: ptr<i32> [storage=automatic] [const] = array_decay<ptr<i32>, length=Some(3)>(%[[VALUE_arr]]);
// VALID-NEXT:         let %[[VALUE_a3:[0-9]+]] a3: ptr<i32> [storage=automatic] = read<ptr<i32>>(%[[VALUE_ip]]);
// VALID-NEXT:         let %[[VALUE_a5:[0-9]+]] a5: i32 [storage=automatic] = read<i32, atomic=seq_cst>(%[[VALUE_ai]]);
// VALID-NEXT:         read<i32>(%[[VALUE_a1]]);
// VALID-NEXT:         read<ptr<i32>>(%[[VALUE_a2]]);
// VALID-NEXT:         read<ptr<i32>>(%[[VALUE_a3]]);
// VALID-NEXT:         read<i32>(%[[VALUE_a4]]);
// VALID-NEXT:         read<i32>(%[[VALUE_a5]]);
// VALID-NEXT:     }
// VALID-NEXT: }
// SLATE-FILECHECK-END VALID
