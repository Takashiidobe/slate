// SLATE-FILECHECK-DEFINES DEFAULT

static int      gl_cnt = 0;
static char     gl_buf[1024];

void
gl_yank()
{
  int  i;

  for (i=gl_cnt; i >= 0; i--)
    gl_buf[i+10] = gl_buf[i];
}

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
// DEFAULT-NEXT:     global %0 gl_cnt: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %1 gl_buf: array<i8, 1024> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     fn %2 @gl_yank() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %4
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%0));
// DEFAULT-NEXT:             condition: ge<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %5: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %6: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%5), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%6));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1024)>(%1), add<i32, overflow=ub>(read<i32>(%3), const<i32>(10)))), read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1024)>(%1), read<i32>(%3)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
