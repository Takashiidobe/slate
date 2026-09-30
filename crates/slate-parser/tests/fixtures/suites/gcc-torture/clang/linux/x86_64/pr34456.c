/* { dg-skip-if "requires qsort" { freestanding } }  */

#include <stdlib.h>

int __attribute__((noinline)) debug(void) { return 1; }
int                           errors;

struct s {
  int elt;
  int (*compare)(int);
};

static int compare(const void *x, const void *y) {
  const struct s *s1 = x, *s2 = y;
  int             (*compare1)(int);
  int             elt2;

  compare1 = s1->compare;
  elt2     = s2->elt;
  if (elt2 != 0 && debug() && compare1(s1->elt) != 0)
    errors++;
  return compare1(elt2);
}

int      bad_compare(int x) { return -x; }
struct s array[2] = {{1, bad_compare}, {-1, bad_compare}};

int main(void) {
  qsort(array, 2, sizeof(struct s), compare);
  return errors == 0;
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___compar_fn_t:[0-9]+]] __compar_fn_t = ptr<fn(ptr<const void>, ptr<const void>) -> i32>;
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 elt: i32;
// DEFAULT-NEXT:         field1 compare: ptr<fn(i32) -> i32>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_errors:[0-9]+]] errors: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_array:[0-9]+]] array: array<@type[[TYPE_s]], 2> [storage=static] [align=16] = aggregate<array<@type[[TYPE_s]], 2>, zero_fill=false>(index0 = aggregate<@type[[TYPE_s]], zero_fill=false>(field0 = const<i32>(1), field1 = function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_bad_compare:[0-9]+]])), index1 = aggregate<@type[[TYPE_s]], zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(1)), field1 = function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_bad_compare]]))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_qsort:[0-9]+]] @qsort(%[[VALUE___base:[0-9]+]] __base: ptr<void>, %[[VALUE___nmemb:[0-9]+]] __nmemb: u64, %[[VALUE___size:[0-9]+]] __size: u64, %[[VALUE___compar:[0-9]+]] __compar: ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_debug:[0-9]+]] @debug() -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_compare:[0-9]+]] @compare(%[[VALUE_x:[0-9]+]] x: ptr<const void>, %[[VALUE_y:[0-9]+]] y: ptr<const void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_s1:[0-9]+]] s1: ptr<const @type[[TYPE_s]]> [storage=automatic] = pointer_cast<ptr<const @type[[TYPE_s]]>, reason=assign>(read<ptr<const void>>(%[[VALUE_x]]));
// DEFAULT-NEXT:         let %[[VALUE_s2:[0-9]+]] s2: ptr<const @type[[TYPE_s]]> [storage=automatic] = pointer_cast<ptr<const @type[[TYPE_s]]>, reason=assign>(read<ptr<const void>>(%[[VALUE_y]]));
// DEFAULT-NEXT:         let %[[VALUE_compare1:[0-9]+]] compare1: ptr<fn(i32) -> i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_elt2:[0-9]+]] elt2: i32 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<fn(i32) -> i32>>(%[[VALUE_compare1]], read<ptr<fn(i32) -> i32>>(field1(deref(read<ptr<const @type[[TYPE_s]]>>(%[[VALUE_s1]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_elt2]], read<i32>(field0(deref(read<ptr<const @type[[TYPE_s]]>>(%[[VALUE_s2]])))));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_elt2]]), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_debug]]), const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(false));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE0]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], ne<i32>(call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(%[[VALUE_compare1]]), read<i32>(field0(deref(read<ptr<const @type[[TYPE_s]]>>(%[[VALUE_s1]]))))), const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE1]])
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_errors]]);
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_errors]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:         return call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(%[[VALUE_compare1]]), read<i32>(%[[VALUE_elt2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bad_compare]] @bad_compare(%[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(read<i32>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64, u64, ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> void>(%[[VALUE_qsort]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type[[TYPE_s]]>, length=Some(2)>(%[[VALUE_array]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), const<u64>(16), function_decay<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>(%[[VALUE_compare]]));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%[[VALUE_errors]]), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
