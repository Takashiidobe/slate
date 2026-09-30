/* This code shows up in worse_state in ipa-pure-const.cc:
   *looping = MAX (*looping, looping2);
   was miscompiling it as just `return 1` though instead of
   `MAX_EXPR<*a, b>` (which should be transformed into `*a | b`
   note MAX_EXPR<bool, bool> is really `bool | bool` so we
   use that to compare against here.
 */
#define bool _Bool
bool __attribute__((noipa)) f(bool *a, bool b) {
  bool t = *a;
  if (t <= b)
    return b;
  return t;
}
bool __attribute__((noipa)) f1(bool *a, bool b) { return *a | b; }

int main() {
  int i = 0;
  int j = 0;

  for (i = 0; i <= 1; i++)
    for (j = 0; j <= 1; j++) {
      bool a = i;
      if (f(&a, j) != f1(&a, j))
        __builtin_abort();
    }
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
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_a:[0-9]+]] a: ptr<bool>, %[[VALUE_b:[0-9]+]] b: bool) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: bool [storage=automatic] = read<bool>(deref(read<ptr<bool>>(%[[VALUE_a]])));
// DEFAULT-NEXT:         if le<i32>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_t]])), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_b]])))
// DEFAULT-NEXT:             return read<bool>(%[[VALUE_b]]);
// DEFAULT-NEXT:         return read<bool>(%[[VALUE_t]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_a_2:[0-9]+]] a: ptr<bool>, %[[VALUE_b_2:[0-9]+]] b: bool) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return ne<i32, reason=return>(or<i32>(from_bool<i32, reason=promotion>(read<bool>(deref(read<ptr<bool>>(%[[VALUE_a_2]])))), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_b_2]]))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_i]]), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_j]], const<i32>(0));
// DEFAULT-NEXT:                     condition: le<i32>(read<i32>(%[[VALUE_j]]), const<i32>(1))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %[[VALUE_a_3:[0-9]+]] a: bool [storage=automatic] = ne<i32, reason=assign>(read<i32>(%[[VALUE_i]]), const<i32>(0));
// DEFAULT-NEXT:                             if ne<i32>(from_bool<i32, reason=promotion>(call<bool, signature=fn(ptr<bool>, bool) -> bool>(%[[VALUE_f]], addr_of<ptr<bool>>(%[[VALUE_a_3]]), ne<i32, reason=arg>(read<i32>(%[[VALUE_j]]), const<i32>(0)))), from_bool<i32, reason=promotion>(call<bool, signature=fn(ptr<bool>, bool) -> bool>(%[[VALUE_f1]], addr_of<ptr<bool>>(%[[VALUE_a_3]]), ne<i32, reason=arg>(read<i32>(%[[VALUE_j]]), const<i32>(0)))))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
