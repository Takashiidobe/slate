// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu11

extern int raw_open(const char *path, int flags, ...);

extern __inline __attribute__((gnu_inline, always_inline, overloadable)) int
fancy_open(const char *path, int flags) {
  return raw_open(path, flags);
}

extern __inline __attribute__((gnu_inline, always_inline, overloadable)) int
fancy_open(const char *path, int flags, unsigned mode) {
  return raw_open(path, flags, mode);
}

int plain(const char *path) { return fancy_open(path, 0); }
int with_mode(const char *path) { return fancy_open(path, 64, 0600); }

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
// IR-NEXT:     fn %[[VALUE_raw_open:[0-9]+]] @raw_open(%[[VALUE_path:[0-9]+]] path: ptr<const i8>, %[[VALUE_flags:[0-9]+]] flags: i32, ...) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_fancy_open:[0-9]+]] @fancy_open(%[[VALUE_path_2:[0-9]+]] path: ptr<const i8>, %[[VALUE_flags_2:[0-9]+]] flags: i32) -> i32 [linkage=external] [inline=always] [definition=inline_only] [overloadable] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(ptr<const i8>, i32, ...) -> i32>(%[[VALUE_raw_open]], read<ptr<const i8>>(%[[VALUE_path_2]]), read<i32>(%[[VALUE_flags_2]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_fancy_open_2:[0-9]+]] @fancy_open(%[[VALUE_path_3:[0-9]+]] path: ptr<const i8>, %[[VALUE_flags_3:[0-9]+]] flags: i32, %[[VALUE_mode:[0-9]+]] mode: u32) -> i32 [linkage=external] [inline=always] [definition=inline_only] [overloadable] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(ptr<const i8>, i32, ...) -> i32>(%[[VALUE_raw_open]], read<ptr<const i8>>(%[[VALUE_path_3]]), read<i32>(%[[VALUE_flags_3]]), read<u32>(%[[VALUE_mode]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_plain:[0-9]+]] @plain(%[[VALUE_path_4:[0-9]+]] path: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(ptr<const i8>, i32) -> i32>(%[[VALUE_fancy_open]], read<ptr<const i8>>(%[[VALUE_path_4]]), const<i32>(0));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_with_mode:[0-9]+]] @with_mode(%[[VALUE_path_5:[0-9]+]] path: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(ptr<const i8>, i32, u32) -> i32>(%[[VALUE_fancy_open_2]], read<ptr<const i8>>(%[[VALUE_path_5]]), const<i32>(64), reinterpret<u32, reason=arg, fits=always>(const<i32>(384)));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
