struct {
  int input;
  int output;
} ffstesttab[] = {
#if __INT_MAX__ >= 2147483647
    /* at least 32-bit integers */
    {0x80000000, 32},
    {0xa5a5a5a5, 1},
    {0x5a5a5a5a, 2},
    {0xcafe0000, 18},
#endif
#if __INT_MAX__ >= 32767
    /* at least 16-bit integers */
    {0x8000, 16},
    {0xa5a5, 1},
    {0x5a5a, 2},
    {0x0ca0, 6},
#endif
#if __INT_MAX__ < 32767
#error integers are too small
#endif
};

#define NFFSTESTS (sizeof(ffstesttab) / sizeof(ffstesttab[0]))

extern void abort(void);
extern void exit(int);

int main(void) {
  int i;

  for (i = 0; i < NFFSTESTS; i++) {
    if (__builtin_ffs(ffstesttab[i].input) != ffstesttab[i].output)
      abort();
  }

  exit(0);
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 input: i32;
// DEFAULT-NEXT:         field1 output: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %[[VALUE_ffstesttab:[0-9]+]] ffstesttab:
// DEFAULT-SAME: array<@type[[TYPE0]], 8> [storage=static] [align=16] =
// DEFAULT-SAME: aggregate<array<@type[[TYPE0]], 8>, zero_fill=false>(index0 =
// DEFAULT-SAME: aggregate<@type[[TYPE0]], zero_fill=false>(field0 = reinterpret<i32, reason=assign, fits=unknown>(const<u32>(2147483648)), field1 = const<i32>(32)), index1 =
// DEFAULT-SAME: aggregate<@type[[TYPE0]], zero_fill=false>(field0 = reinterpret<i32, reason=assign, fits=unknown>(const<u32>(2779096485)), field1 = const<i32>(1)), index2 =
// DEFAULT-SAME: aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<i32>(1515870810), field1 = const<i32>(2)), index3 =
// DEFAULT-SAME: aggregate<@type[[TYPE0]], zero_fill=false>(field0 = reinterpret<i32, reason=assign, fits=unknown>(const<u32>(3405643776)), field1 = const<i32>(18)), index4 =
// DEFAULT-SAME: aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<i32>(32768), field1 = const<i32>(16)), index5 =
// DEFAULT-SAME: aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<i32>(42405), field1 = const<i32>(1)), index6 =
// DEFAULT-SAME: aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<i32>(23130), field1 = const<i32>(2)), index7 =
// DEFAULT-SAME: aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<i32>(3232), field1 = const<i32>(6))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ffs:[0-9]+]] @__builtin_ffs(%[[VALUE1:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), div<u64, by_zero=ub>(const<u64>(64), const<u64>(8)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_ffs]], read<i32>(field0(deref(ptr_offset<ptr<@type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(array_decay<ptr<@type[[TYPE0]]>, length=Some(8)>(%[[VALUE_ffstesttab]]), read<i32>(%[[VALUE_i]])))))), read<i32>(field1(deref(ptr_offset<ptr<@type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(array_decay<ptr<@type[[TYPE0]]>, length=Some(8)>(%[[VALUE_ffstesttab]]), read<i32>(%[[VALUE_i]]))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
