// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS -fblocks --dump-ir --compact-ir
#ifndef __BLOCKS__
#error -fblocks defines __BLOCKS__
#endif

typedef int (^binop)(int, int);

extern int (^global_block)(int);
int run(int (^callback)(int), binop op, void (^handlers[2])(void));
int (^make_adder(int))(int);
void (^const frozen)(void);
int (^*block_pointer_pointer)(int);
unsigned long block_size = sizeof(global_block);
unsigned long block_array_size = sizeof(void (^[3])(void));

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
// DEFAULT-NEXT:     type @type[[TYPE_binop:[0-9]+]] binop = block<fn(i32, i32) -> i32>;
// DEFAULT-NEXT:     extern %[[VALUE_global_block:[0-9]+]] global_block: block<fn(i32) -> i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_frozen:[0-9]+]] frozen: block<fn() -> void> [storage=static] [const] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_block_pointer_pointer:[0-9]+]] block_pointer_pointer: ptr<block<fn(i32) -> i32>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_block_size:[0-9]+]] block_size: u64 [storage=static] = size_of<u64, block<fn(i32) -> i32>>(8) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_block_array_size:[0-9]+]] block_array_size: u64 [storage=static] = size_of<u64, array<block<fn() -> void>, 3>>(24) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_run:[0-9]+]] @run(%[[VALUE_callback:[0-9]+]] callback: block<fn(i32) -> i32>, %[[VALUE_op:[0-9]+]] op: block<fn(i32, i32) -> i32> [c="binop"], %[[VALUE_handlers:[0-9]+]] handlers: ptr<block<fn() -> void>> [array=2]) -> i32 [linkage=external] [c="int(int (^)(int), binop, void (^[2])(void))"];
// DEFAULT-NEXT:     fn %[[VALUE_make_adder:[0-9]+]] @make_adder(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> block<fn(i32) -> i32> [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
