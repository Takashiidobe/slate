/* { dg-require-effective-target int32plus } */
struct big {
  int data[1000000];
};

struct small {
  int data[10];
};

union both {
  struct big   big;
  struct small small;
};

extern void *calloc(__SIZE_TYPE__, __SIZE_TYPE__);
extern void  free(void *);

static int __attribute__((noinline)) foo(int fail, union both *agg) {
  int r;
  if (fail)
    r = agg->big.data[999999];
  else
    r = agg->small.data[0];
  return r;
}

int main(int argc, char *argv[]) {
  union both *agg = calloc(1, sizeof(struct small));
  int         r;

  r = foo((argc > 2000), agg);

  free(agg);
  return r;
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
// DEFAULT-NEXT:     type @type[[TYPE_big:[0-9]+]] big = struct {
// DEFAULT-NEXT:         field0 data: array<i32, 1000000>;
// DEFAULT-NEXT:     } [size=4000000, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_small:[0-9]+]] small = struct {
// DEFAULT-NEXT:         field0 data: array<i32, 10>;
// DEFAULT-NEXT:     } [size=40, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_both:[0-9]+]] both = union {
// DEFAULT-NEXT:         field0 big: @type[[TYPE_big]];
// DEFAULT-NEXT:         field1 small: @type[[TYPE_small]];
// DEFAULT-NEXT:     } [size=4000000, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %[[VALUE_calloc:[0-9]+]] @calloc(%[[VALUE0:[0-9]+]] <unnamed>: u64, %[[VALUE1:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_free:[0-9]+]] @free(%[[VALUE2:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_fail:[0-9]+]] fail: i32, %[[VALUE_agg:[0-9]+]] agg: ptr<@type[[TYPE_both]]>) -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_fail]]), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_r]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1000000)>(field0(field0(deref(read<ptr<@type[[TYPE_both]]>>(%[[VALUE_agg]]))))), const<i32>(999999)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%[[VALUE_r]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(10)>(field0(field1(deref(read<ptr<@type[[TYPE_both]]>>(%[[VALUE_agg]]))))), const<i32>(0)))));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_r]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_agg_2:[0-9]+]] agg: ptr<@type[[TYPE_both]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_both]]>, reason=assign>(call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_calloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), const<u64>(40)));
// DEFAULT-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_2]], call<i32, signature=fn(i32, ptr<@type[[TYPE_both]]>) -> i32>(%[[VALUE_foo]], from_bool<i32, reason=arg>(gt<i32>(read<i32>(%[[VALUE_argc]]), const<i32>(2000))), read<ptr<@type[[TYPE_both]]>>(%[[VALUE_agg_2]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_both]]>>(%[[VALUE_agg_2]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_r_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
