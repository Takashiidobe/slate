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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 __compar_fn_t = ptr<fn(ptr<const void>, ptr<const void>) -> i32>;
// DEFAULT-NEXT:     type @type2 s = struct {
// DEFAULT-NEXT:         field0 elt: i32;
// DEFAULT-NEXT:         field1 compare: ptr<fn(i32) -> i32>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %4 errors: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %15 array: array<@type2, 2> [storage=static] = aggregate<array<@type2, 2>, zero_fill=false>(index0 = aggregate<@type2, zero_fill=false>(field0 = const<i32>(1), field1 = function_decay<ptr<fn(i32) -> i32>>(%13)), index1 = aggregate<@type2, zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(1)), field1 = function_decay<ptr<fn(i32) -> i32>>(%13))) [linkage=external];
// DEFAULT-NEXT:     fn %2 @qsort(%17 __base: ptr<void>, %18 __nmemb: u64, %19 __size: u64, %20 __compar: ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @debug() -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @compare(%7 x: ptr<const void>, %8 y: ptr<const void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 s1: ptr<const @type2> [storage=automatic] = pointer_cast<ptr<const @type2>, reason=assign>(read<ptr<const void>>(%7));
// DEFAULT-NEXT:         let %10 s2: ptr<const @type2> [storage=automatic] = pointer_cast<ptr<const @type2>, reason=assign>(read<ptr<const void>>(%8));
// DEFAULT-NEXT:         let %11 compare1: ptr<fn(i32) -> i32> [storage=automatic];
// DEFAULT-NEXT:         let %12 elt2: i32 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<fn(i32) -> i32>>(%11, read<ptr<fn(i32) -> i32>>(field1(deref(read<ptr<const @type2>>(%9)))));
// DEFAULT-NEXT:         write<i32>(%12, read<i32>(field0(deref(read<ptr<const @type2>>(%10)))));
// DEFAULT-NEXT:         let %21: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%12), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%21, ne<i32>(call<i32, signature=fn() -> i32>(%3), const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%21, const<bool>(false));
// DEFAULT-NEXT:         let %22: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%21)
// DEFAULT-NEXT:             write<bool>(%22, ne<i32>(call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(%11), read<i32>(field0(deref(read<ptr<const @type2>>(%9))))), const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%22, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%22)
// DEFAULT-NEXT:             let %23: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:             let %24: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%23), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%4, read<i32>(%24));
// DEFAULT-NEXT:         return call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(%11), read<i32>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @bad_compare(%14 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(read<i32>(%14));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64, u64, ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> void>(%2, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%15)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), const<u64>(16), function_decay<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>(%6));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%4), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
