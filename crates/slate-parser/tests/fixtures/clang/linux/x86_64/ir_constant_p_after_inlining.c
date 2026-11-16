// SLATE-FILECHECK-DEFINES O0
// SLATE-FILECHECK-DEFINES O2
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-PREFIX-ARGS O0 -O0
// SLATE-FILECHECK-PREFIX-ARGS O2 -O2

int g;

int scalar(int x, double d, _Bool b) {
  return __builtin_constant_p(x) + __builtin_constant_p(d) + __builtin_constant_p(b);
}

int folded_or_rejected(int *p, int x) {
  return __builtin_constant_p(3) + __builtin_constant_p(p) + __builtin_constant_p(g++) +
         __builtin_constant_p(x = 1);
}

enum freq { NORMAL, LIKELY };

int enum_operand(const enum freq fr) {
  return __builtin_constant_p(fr) + __builtin_constant_p(LIKELY);
}

int local_min(void) {
  int y = 2;
  return __builtin_constant_p(y) && y >= 0;
}

void __compiletime_assert_1(void);

int query_branch_not_folded(const enum freq fr, int x) {
  if (!__builtin_constant_p(fr))
    __compiletime_assert_1();
  return x;
}

static inline __attribute__((always_inline)) void mark(int n) {
  __asm__ volatile("# mark %0" ::"i"(n));
}

int assert_kept_beside_asm(int len) {
  while (len > 0) {
    if (!(__builtin_constant_p(len >= 0) && len >= 0))
      __compiletime_assert_1();
    len -= 3;
    mark(1);
  }
  return len;
}

void asm_arm_pruned(int n) {
  if (__builtin_constant_p(n))
    mark(n);
  else
    g = n;
}

// SLATE-FILECHECK-BEGIN O0
// O0: module {
// O0-NEXT:     target "x86_64-unknown-linux-gnu" {
// O0-NEXT:         endian = little;
// O0-NEXT:         pointer [size=8, align=8];
// O0-NEXT:         stack_alignment = 16;
// O0-NEXT:         long_double = f80;
// O0-NEXT:         storage bool [size=1, align=1];
// O0-NEXT:         storage i8, u8 [size=1, align=1];
// O0-NEXT:         storage i16, u16 [size=2, align=2];
// O0-NEXT:         storage i32, u32 [size=4, align=4];
// O0-NEXT:         storage i64, u64 [size=8, align=8];
// O0-NEXT:         storage i128, u128 [size=16, align=16];
// O0-NEXT:         storage bf16 [size=2, align=2];
// O0-NEXT:         storage f16 [size=2, align=2];
// O0-NEXT:         storage f32 [size=4, align=4];
// O0-NEXT:         storage f64 [size=8, align=8];
// O0-NEXT:         storage f80 [size=16, align=16];
// O0-NEXT:         storage f128 [size=16, align=16];
// O0-NEXT:         storage d32 [size=4, align=4];
// O0-NEXT:         storage d64 [size=8, align=8];
// O0-NEXT:         storage d128 [size=16, align=16];
// O0-NEXT:     }
// O0-NEXT:     type @type[[TYPE_freq:[0-9]+]] freq = enum : u32 {
// O0-NEXT:         %[[VALUE_NORMAL:[0-9]+]] NORMAL = const<i32>(0);
// O0-NEXT:         %[[VALUE_LIKELY:[0-9]+]] LIKELY = const<i32>(1);
// O0-NEXT:     } [size=4, align=4];
// O0-NEXT:     global %[[VALUE_NORMAL]] g: i32 [storage=static] [linkage=external];
// O0-NEXT:     fn %[[VALUE_LIKELY]] @scalar(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_d:[0-9]+]] d: f64, %[[VALUE_b:[0-9]+]] b: bool) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// O0-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(0), const<i32>(0)), const<i32>(0));
// O0-NEXT:     }
// O0-NEXT:     fn %[[VALUE_folded_or_rejected:[0-9]+]] @folded_or_rejected(%[[VALUE_p:[0-9]+]] p: ptr<i32>, %[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// O0-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(1), const<i32>(0)), const<i32>(0)), const<i32>(0));
// O0-NEXT:     }
// O0-NEXT:     fn %[[VALUE_enum_operand:[0-9]+]] @enum_operand(%[[VALUE_fr:[0-9]+]] fr: @type[[TYPE_freq]] [const]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="int(const enum freq)"] {
// O0-NEXT:         return add<i32, overflow=ub>(const<i32>(0), const<i32>(1));
// O0-NEXT:     }
// O0-NEXT:     fn %[[VALUE_local_min:[0-9]+]] @local_min() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// O0-NEXT:         let %[[VALUE_y:[0-9]+]] y: i32 [storage=automatic] = const<i32>(2);
// O0-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(ne<i32>(const<i32>(0), const<i32>(0)), ge<i32>(read<i32>(%[[VALUE_y]]), const<i32>(0))));
// O0-NEXT:     }
// O0-NEXT:     fn %[[VALUE___compiletime_assert_1:[0-9]+]] @__compiletime_assert_1() -> void [linkage=external];
// O0-NEXT:     fn %[[VALUE_query_branch_not_folded:[0-9]+]] @query_branch_not_folded(%[[VALUE_fr_2:[0-9]+]] fr: @type[[TYPE_freq]] [const], %[[VALUE_x_3:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="int(const enum freq, int)"] {
// O0-NEXT:         if not<bool>(ne<i32>(const<i32>(0), const<i32>(0))) [folded=true]
// O0-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___compiletime_assert_1]]);
// O0-NEXT:         return read<i32>(%[[VALUE_x_3]]);
// O0-NEXT:     }
// O0-NEXT:     fn %[[VALUE_mark:[0-9]+]] @mark(%[[VALUE_n:[0-9]+]] n: i32) -> void [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ret_void] {
// O0-NEXT:         asm volatile "# mark %0" [dialect=att] [options=nostack] [alternative=none] {
// O0-NEXT:             template: "# mark " %0;
// O0-NEXT:             in 0 "i" [imm | sym] width 32 read<i32>(%[[VALUE_n]]);
// O0-NEXT:             rejected: 0 (operand 0: not-constant);
// O0-NEXT:         }
// O0-NEXT:     }
// O0-NEXT:     fn %[[VALUE_assert_kept_beside_asm:[0-9]+]] @assert_kept_beside_asm(%[[VALUE_len:[0-9]+]] len: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// O0-NEXT:         while %[[VALUE0:[0-9]+]] gt<i32>(read<i32>(%[[VALUE_len]]), const<i32>(0))
// O0-NEXT:             {
// O0-NEXT:                 if not<bool>(logical_and<bool>(ne<i32>(const<i32>(0), const<i32>(0)), ge<i32>(read<i32>(%[[VALUE_len]]), const<i32>(0)))) [folded=true]
// O0-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___compiletime_assert_1]]);
// O0-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_len]]);
// O0-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(3));
// O0-NEXT:                 write<i32>(%[[VALUE_len]], read<i32>(%[[VALUE2]]));
// O0-NEXT:                 call<void, signature=fn(i32) -> void>(%[[VALUE_mark]], const<i32>(1));
// O0-NEXT:             }
// O0-NEXT:         return read<i32>(%[[VALUE_len]]);
// O0-NEXT:     }
// O0-NEXT:     fn %[[VALUE_asm_arm_pruned:[0-9]+]] @asm_arm_pruned(%[[VALUE_n_2:[0-9]+]] n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// O0-NEXT:         if ne<i32>(const<i32>(0), const<i32>(0)) [folded=false]
// O0-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_mark]], read<i32>(%[[VALUE_n_2]]));
// O0-NEXT:         else
// O0-NEXT:             write<i32>(%[[VALUE_NORMAL]], read<i32>(%[[VALUE_n_2]]));
// O0-NEXT:     }
// O0-NEXT: }
// SLATE-FILECHECK-END O0
// SLATE-FILECHECK-BEGIN O2
// O2: module {
// O2-NEXT:     target "x86_64-unknown-linux-gnu" {
// O2-NEXT:         endian = little;
// O2-NEXT:         pointer [size=8, align=8];
// O2-NEXT:         stack_alignment = 16;
// O2-NEXT:         long_double = f80;
// O2-NEXT:         storage bool [size=1, align=1];
// O2-NEXT:         storage i8, u8 [size=1, align=1];
// O2-NEXT:         storage i16, u16 [size=2, align=2];
// O2-NEXT:         storage i32, u32 [size=4, align=4];
// O2-NEXT:         storage i64, u64 [size=8, align=8];
// O2-NEXT:         storage i128, u128 [size=16, align=16];
// O2-NEXT:         storage bf16 [size=2, align=2];
// O2-NEXT:         storage f16 [size=2, align=2];
// O2-NEXT:         storage f32 [size=4, align=4];
// O2-NEXT:         storage f64 [size=8, align=8];
// O2-NEXT:         storage f80 [size=16, align=16];
// O2-NEXT:         storage f128 [size=16, align=16];
// O2-NEXT:         storage d32 [size=4, align=4];
// O2-NEXT:         storage d64 [size=8, align=8];
// O2-NEXT:         storage d128 [size=16, align=16];
// O2-NEXT:     }
// O2-NEXT:     type @type[[TYPE_freq:[0-9]+]] freq = enum : u32 {
// O2-NEXT:         %[[VALUE_NORMAL:[0-9]+]] NORMAL = const<i32>(0);
// O2-NEXT:         %[[VALUE_LIKELY:[0-9]+]] LIKELY = const<i32>(1);
// O2-NEXT:     } [size=4, align=4];
// O2-NEXT:     global %[[VALUE_NORMAL]] g: i32 [storage=static] [linkage=external];
// O2-NEXT:     fn %[[VALUE_LIKELY]] @scalar(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_d:[0-9]+]] d: f64, %[[VALUE_b:[0-9]+]] b: bool) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// O2-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(intrinsic<bool, llvm.is.constant>(read<i32>(%[[VALUE_x]]))), from_bool<i32, reason=promotion>(intrinsic<bool, llvm.is.constant>(read<f64>(%[[VALUE_d]])))), from_bool<i32, reason=promotion>(intrinsic<bool, llvm.is.constant>(read<bool>(%[[VALUE_b]]))));
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_folded_or_rejected:[0-9]+]] @folded_or_rejected(%[[VALUE_p:[0-9]+]] p: ptr<i32>, %[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// O2-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(1), const<i32>(0)), const<i32>(0)), const<i32>(0));
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_enum_operand:[0-9]+]] @enum_operand(%[[VALUE_fr:[0-9]+]] fr: @type[[TYPE_freq]] [const]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="int(const enum freq)"] {
// O2-NEXT:         return add<i32, overflow=ub>(from_bool<i32, reason=promotion>(intrinsic<bool, llvm.is.constant>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_freq]]>(%[[VALUE_fr]])))), const<i32>(1));
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_local_min:[0-9]+]] @local_min() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// O2-NEXT:         let %[[VALUE_y:[0-9]+]] y: i32 [storage=automatic] = const<i32>(2);
// O2-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(intrinsic<bool, llvm.is.constant>(read<i32>(%[[VALUE_y]])), ge<i32>(read<i32>(%[[VALUE_y]]), const<i32>(0))));
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE___compiletime_assert_1:[0-9]+]] @__compiletime_assert_1() -> void [linkage=external];
// O2-NEXT:     fn %[[VALUE_query_branch_not_folded:[0-9]+]] @query_branch_not_folded(%[[VALUE_fr_2:[0-9]+]] fr: @type[[TYPE_freq]] [const], %[[VALUE_x_3:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="int(const enum freq, int)"] {
// O2-NEXT:         if not<bool>(intrinsic<bool, llvm.is.constant>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_freq]]>(%[[VALUE_fr_2]]))))
// O2-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___compiletime_assert_1]]);
// O2-NEXT:         return read<i32>(%[[VALUE_x_3]]);
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_assert_kept_beside_asm:[0-9]+]] @assert_kept_beside_asm(%[[VALUE_len:[0-9]+]] len: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// O2-NEXT:         while %[[VALUE0:[0-9]+]] gt<i32>(read<i32>(%[[VALUE_len]]), const<i32>(0))
// O2-NEXT:             {
// O2-NEXT:                 if not<bool>(logical_and<bool>(intrinsic<bool, llvm.is.constant>(ge<i32>(read<i32>(%[[VALUE_len]]), const<i32>(0))), ge<i32>(read<i32>(%[[VALUE_len]]), const<i32>(0))))
// O2-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___compiletime_assert_1]]);
// O2-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_len]]);
// O2-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(3));
// O2-NEXT:                 write<i32>(%[[VALUE_len]], read<i32>(%[[VALUE2]]));
// O2-NEXT:                 call<void, signature=fn(i32) -> void>(%[[VALUE_mark__const1:[0-9]+]], const<i32>(1));
// O2-NEXT:             }
// O2-NEXT:         return read<i32>(%[[VALUE_len]]);
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_asm_arm_pruned:[0-9]+]] @asm_arm_pruned(%[[VALUE_n:[0-9]+]] n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// O2-NEXT:         {
// O2-NEXT:             write<i32>(%[[VALUE_NORMAL]], read<i32>(%[[VALUE_n]]));
// O2-NEXT:         }
// O2-NEXT:     }
// O2-NEXT:     fn %[[VALUE_mark__const1]] @mark__const1(%[[VALUE_n_2:[0-9]+]] n: i32) -> void [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ret_void] {
// O2-NEXT:         asm volatile "# mark %0" [dialect=att] [options=nostack] {
// O2-NEXT:             template: "# mark " %0;
// O2-NEXT:             in 0 "i" [imm | sym] -> imm width 32 const<i32>(1);
// O2-NEXT:         }
// O2-NEXT:     }
// O2-NEXT: }
// SLATE-FILECHECK-END O2
