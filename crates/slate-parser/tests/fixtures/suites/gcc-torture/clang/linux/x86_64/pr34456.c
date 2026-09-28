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
// DEFAULT-NEXT:     global %8 errors: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %19 array: array<@type2, 2> [storage=static] [align=16] = aggregate<array<@type2, 2>, zero_fill=false>(index0 = aggregate<@type2, zero_fill=false>(field0 = const<i32>(1), field1 = function_decay<ptr<fn(i32) -> i32>>(%17)), index1 = aggregate<@type2, zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(1)), field1 = function_decay<ptr<fn(i32) -> i32>>(%17))) [linkage=external];
// DEFAULT-NEXT:     fn %6 @qsort(%21 __base: ptr<void>, %22 __nmemb: u64, %23 __size: u64, %24 __compar: ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @debug() -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @compare(%11 x: ptr<const void>, %12 y: ptr<const void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %13 s1: ptr<const @type2> [storage=automatic] = pointer_cast<ptr<const @type2>, reason=assign>(read<ptr<const void>>(%11));
// DEFAULT-NEXT:         let %14 s2: ptr<const @type2> [storage=automatic] = pointer_cast<ptr<const @type2>, reason=assign>(read<ptr<const void>>(%12));
// DEFAULT-NEXT:         let %15 compare1: ptr<fn(i32) -> i32> [storage=automatic];
// DEFAULT-NEXT:         let %16 elt2: i32 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<fn(i32) -> i32>>(%15, read<ptr<fn(i32) -> i32>>(field1(deref(read<ptr<const @type2>>(%13)))));
// DEFAULT-NEXT:         write<i32>(%16, read<i32>(field0(deref(read<ptr<const @type2>>(%14)))));
// DEFAULT-NEXT:         let %25: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%16), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%25, ne<i32>(call<i32, signature=fn() -> i32>(%7), const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%25, const<bool>(false));
// DEFAULT-NEXT:         let %26: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%25)
// DEFAULT-NEXT:             write<bool>(%26, ne<i32>(call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(%15), read<i32>(field0(deref(read<ptr<const @type2>>(%13))))), const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%26, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%26)
// DEFAULT-NEXT:             let %27: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:             let %28: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%27), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%8, read<i32>(%28));
// DEFAULT-NEXT:         return call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(%15), read<i32>(%16));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @bad_compare(%18 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(read<i32>(%18));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64, u64, ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> void>(%6, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type2>, length=Some(2)>(%19)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), const<u64>(16), function_decay<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>(%10));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(%8), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
