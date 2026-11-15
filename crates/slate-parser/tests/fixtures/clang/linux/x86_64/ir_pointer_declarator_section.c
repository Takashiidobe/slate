// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir
char *unpack(char *buf);
char * __attribute__((__section__(".init.text"))) __attribute__((__cold__)) unpack(char *buf) { return buf; }
void * __attribute__((section(".init.text"), used)) table(void) { return 0; }
int ** __attribute__((visibility("hidden"))) nested(void) { return 0; }
char * __attribute__((section(".init.data"))) message = "boot";

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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([98, 111, 111, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_message:[0-9]+]] message: ptr<i8> [storage=static] = array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str]]) [linkage=external] [section=".init.data"];
// DEFAULT-NEXT:     fn %[[VALUE_unpack:[0-9]+]] @unpack(%[[VALUE_buf:[0-9]+]] buf: ptr<i8>) -> ptr<i8> [linkage=external] [section=".init.text"] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<ptr<i8>>(%[[VALUE_buf]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_table:[0-9]+]] @table() -> ptr<void> [linkage=external] [section=".init.text"] [used] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nested:[0-9]+]] @nested() -> ptr<ptr<i32>> [linkage=external] [visibility=hidden] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
