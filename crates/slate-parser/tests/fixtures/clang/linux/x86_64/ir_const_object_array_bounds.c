// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu11

const int file_count = 3;
const unsigned char wrapped = 300;
const long wide = 2;
int file_array[file_count];
extern int declared[wide];
int wrapped_array[wrapped];

int plain = 4;
int plain_array[4];

int block(void) {
  const int local = 5;
  static int local_static[local];
  static int file_static[file_count];
  int automatic[local];
  return sizeof local_static + sizeof file_static + sizeof automatic + sizeof file_array;
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
// IR-NEXT:     global %[[VALUE_file_count:[0-9]+]] file_count: i32 [storage=static] [const] = const<i32>(3) [linkage=external];
// IR-NEXT:     global %[[VALUE_wrapped:[0-9]+]] wrapped: u8 [storage=static] [const] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(300))) [linkage=external];
// IR-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: i64 [storage=static] [const] = widen<i64, reason=assign>(const<i32>(2)) [linkage=external];
// IR-NEXT:     global %[[VALUE_file_array:[0-9]+]] file_array: array<i32, 3> [storage=static] [linkage=external];
// IR-NEXT:     extern %[[VALUE_declared:[0-9]+]] declared: array<i32, 2> [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_wrapped_array:[0-9]+]] wrapped_array: array<i32, 44> [storage=static] [align=16] [linkage=external];
// IR-NEXT:     global %[[VALUE_plain:[0-9]+]] plain: i32 [storage=static] = const<i32>(4) [linkage=external];
// IR-NEXT:     global %[[VALUE_plain_array:[0-9]+]] plain_array: array<i32, 4> [storage=static] [align=16] [linkage=external];
// IR-NEXT:     global %[[VALUE_local_static:[0-9]+]] local_static: array<i32, 5> [storage=static] [align=16] [linkage=internal];
// IR-NEXT:     global %[[VALUE_file_static:[0-9]+]] file_static: array<i32, 3> [storage=static] [linkage=internal];
// IR-NEXT:     fn %[[VALUE_block:[0-9]+]] @block() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_local:[0-9]+]] local: i32 [storage=automatic] [const] = const<i32>(5);
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_local]])));
// IR-NEXT:         let %[[VALUE_automatic:[0-9]+]] automatic: vla<i32, %[[VALUE0]]> [storage=automatic];
// IR-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(20), const<u64>(12)), mul<u64, overflow=wrap>(read<u64>(%[[VALUE0]]), const<u64>(4))), const<u64>(12))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
