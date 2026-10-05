// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu11

typedef struct _IO_FILE FILE;
typedef __builtin_va_list va_list;

extern int fprintf(FILE *restrict stream, const char *restrict fmt, ...);

static __attribute__((unused)) __attribute__((overloadable)) int
fprintf(FILE *restrict const __attribute__((pass_object_size(1))) stream,
        const char *restrict fmt, ...) {
  va_list ap;
  __builtin_va_start(ap, fmt);
  int r = __builtin___vfprintf_chk(stream, 1, fmt, ap);
  __builtin_va_end(ap);
  return r;
}

int log_value(FILE *stream, int value) {
  return fprintf(stream, "%d\n", value);
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
// IR-NEXT:     type @type[[TYPE__IO_FILE:[0-9]+]] _IO_FILE = struct incomplete;
// IR-NEXT:     type @type[[TYPE_FILE:[0-9]+]] FILE = @type[[TYPE__IO_FILE]];
// IR-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// IR-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// IR-NEXT:     fn %[[VALUE_fprintf:[0-9]+]] @fprintf(%[[VALUE_stream:[0-9]+]] stream: ptr<@type[[TYPE__IO_FILE]]> [restrict], %[[VALUE_fmt:[0-9]+]] fmt: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE___builtin___vfprintf_chk:[0-9]+]] @__builtin___vfprintf_chk(%[[VALUE0:[0-9]+]] <unnamed>: ptr<@type[[TYPE__IO_FILE]]>, %[[VALUE1:[0-9]+]] <unnamed>: i32, %[[VALUE2:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE3:[0-9]+]] <unnamed>: va_list) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_fprintf_2:[0-9]+]] @fprintf(%[[VALUE_stream_2:[0-9]+]] stream: ptr<@type[[TYPE__IO_FILE]]> [restrict] [const], %[[VALUE_fmt_2:[0-9]+]] fmt: ptr<const i8> [restrict], ...) -> i32 [linkage=internal] [overloadable] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: va_list [storage=automatic];
// IR-NEXT:         va_start(%[[VALUE_ap]]);
// IR-NEXT:         let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic] = call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, i32, ptr<const i8>, va_list) -> i32>(%[[VALUE___builtin___vfprintf_chk]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stream_2]]), const<i32>(1), read<ptr<const i8>>(%[[VALUE_fmt_2]]), read<va_list>(%[[VALUE_ap]]));
// IR-NEXT:         va_end(%[[VALUE_ap]]);
// IR-NEXT:         return read<i32>(%[[VALUE_r]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_log_value:[0-9]+]] @log_value(%[[VALUE_stream_3:[0-9]+]] stream: ptr<@type[[TYPE__IO_FILE]]>, %[[VALUE_value:[0-9]+]] value: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, ptr<const i8>, ...) -> i32>(%[[VALUE_fprintf_2]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stream_3]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), read<i32>(%[[VALUE_value]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
