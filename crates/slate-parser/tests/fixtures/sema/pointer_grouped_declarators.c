// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

int *(*array_pointer)[4];
int **(*nested_pointer)[4];
int *(*function_pointer)(void);
char *(*function_pointers[2])(int);
int *plain_function(void);

static_assert(sizeof(function_pointers) == 16);
static_assert(sizeof(array_pointer) == 8);

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
// IR-NEXT:     global %0 array_pointer: ptr<array<ptr<i32>, 4>> [storage=static] [linkage=external];
// IR-NEXT:     global %1 nested_pointer: ptr<array<ptr<ptr<i32>>, 4>> [storage=static] [linkage=external];
// IR-NEXT:     global %2 function_pointer: ptr<fn() -> ptr<i32>> [storage=static] [linkage=external];
// IR-NEXT:     global %3 function_pointers: array<ptr<fn(i32) -> ptr<i8>>, 2> [storage=static] [linkage=external];
// IR-NEXT:     fn %4 @plain_function() -> ptr<i32> [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
