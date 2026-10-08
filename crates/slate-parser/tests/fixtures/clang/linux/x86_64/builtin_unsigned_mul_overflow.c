// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata
_Bool mul(unsigned a, unsigned b, unsigned *p) {
  return __builtin_umul_overflow(a, b, p);
}
_Bool mull(unsigned long a, unsigned long b, unsigned long *p) {
  return __builtin_umull_overflow(a, b, p);
}
_Bool mulll(unsigned long long a, unsigned long long b, unsigned long long *p) {
  return __builtin_umulll_overflow(a, b, p);
}
_Bool converted(unsigned long long a, int b, unsigned *p) {
  return __builtin_umul_overflow(a, b, p);
}
_Bool side_effects(unsigned *a, unsigned *b, unsigned **p) {
  return __builtin_umul_overflow((*a)++, (*b)++, (*p)++);
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_mul:[0-9]+]] @mul(%[[VALUE_a:[0-9]+]] a: u32 [c="unsigned int"], %[[VALUE_b:[0-9]+]] b: u32 [c="unsigned int"], %[[VALUE_p:[0-9]+]] p: ptr<u32> [c="unsigned int *"]) -> bool [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="_Bool"] [c="_Bool(unsigned int, unsigned int, unsigned int *)"] {
// IR-NEXT:         return overflow_mul<bool>(read<u32>(%[[VALUE_a]]), read<u32>(%[[VALUE_b]]), deref(read<ptr<u32>>(%[[VALUE_p]]))) [c_builtin="__builtin_umul_overflow"];
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_mull:[0-9]+]] @mull(%[[VALUE_a_2:[0-9]+]] a: u64 [c="unsigned long"], %[[VALUE_b_2:[0-9]+]] b: u64 [c="unsigned long"], %[[VALUE_p_2:[0-9]+]] p: ptr<u64> [c="unsigned long *"]) -> bool [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="_Bool"] [c="_Bool(unsigned long, unsigned long, unsigned long *)"] {
// IR-NEXT:         return overflow_mul<bool>(read<u64>(%[[VALUE_a_2]]), read<u64>(%[[VALUE_b_2]]), deref(read<ptr<u64>>(%[[VALUE_p_2]]))) [c_builtin="__builtin_umull_overflow"];
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_mulll:[0-9]+]] @mulll(%[[VALUE_a_3:[0-9]+]] a: u64 [c="unsigned long long"], %[[VALUE_b_3:[0-9]+]] b: u64 [c="unsigned long long"], %[[VALUE_p_3:[0-9]+]] p: ptr<u64> [c="unsigned long long *"]) -> bool [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="_Bool"] [c="_Bool(unsigned long long, unsigned long long, unsigned long long *)"] {
// IR-NEXT:         return overflow_mul<bool>(read<u64>(%[[VALUE_a_3]]), read<u64>(%[[VALUE_b_3]]), deref(read<ptr<u64>>(%[[VALUE_p_3]]))) [c_builtin="__builtin_umulll_overflow"];
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_converted:[0-9]+]] @converted(%[[VALUE_a_4:[0-9]+]] a: u64 [c="unsigned long long"], %[[VALUE_b_4:[0-9]+]] b: i32 [c="int"], %[[VALUE_p_4:[0-9]+]] p: ptr<u32> [c="unsigned int *"]) -> bool [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="_Bool"] [c="_Bool(unsigned long long, int, unsigned int *)"] {
// IR-NEXT:         return overflow_mul<bool>(truncate<u32, reason=arg, fits=unknown>(read<u64>(%[[VALUE_a_4]])), reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_b_4]])), deref(read<ptr<u32>>(%[[VALUE_p_4]]))) [c_builtin="__builtin_umul_overflow"];
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_side_effects:[0-9]+]] @side_effects(%[[VALUE_a_5:[0-9]+]] a: ptr<u32> [c="unsigned int *"], %[[VALUE_b_5:[0-9]+]] b: ptr<u32> [c="unsigned int *"], %[[VALUE_p_5:[0-9]+]] p: ptr<ptr<u32>> [c="unsigned int **"]) -> bool [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="_Bool"] [c="_Bool(unsigned int *, unsigned int *, unsigned int **)"] {
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<u32> [synthetic] = read<ptr<u32>>(%[[VALUE_a_5]]);
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: u32 [synthetic] = read<u32>(deref(read<ptr<u32>>(%[[VALUE0]])));
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE1]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// IR-NEXT:         write<u32>(deref(read<ptr<u32>>(%[[VALUE0]])), read<u32>(%[[VALUE2]]));
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<u32> [synthetic] = read<ptr<u32>>(%[[VALUE_b_5]]);
// IR-NEXT:         let %[[VALUE4:[0-9]+]]: u32 [synthetic] = read<u32>(deref(read<ptr<u32>>(%[[VALUE3]])));
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE4]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// IR-NEXT:         write<u32>(deref(read<ptr<u32>>(%[[VALUE3]])), read<u32>(%[[VALUE5]]));
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: ptr<ptr<u32>> [synthetic] = read<ptr<ptr<u32>>>(%[[VALUE_p_5]]);
// IR-NEXT:         let %[[VALUE7:[0-9]+]]: ptr<u32> [synthetic] = read<ptr<u32>>(deref(read<ptr<ptr<u32>>>(%[[VALUE6]])));
// IR-NEXT:         let %[[VALUE8:[0-9]+]]: ptr<u32> [synthetic] = ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE7]]), const<i32>(1));
// IR-NEXT:         write<ptr<u32>>(deref(read<ptr<ptr<u32>>>(%[[VALUE6]])), read<ptr<u32>>(%[[VALUE8]]));
// IR-NEXT:         return overflow_mul<bool>(read<u32>(%[[VALUE1]]), read<u32>(%[[VALUE4]]), deref(read<ptr<u32>>(%[[VALUE7]]))) [c_builtin="__builtin_umul_overflow"];
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
