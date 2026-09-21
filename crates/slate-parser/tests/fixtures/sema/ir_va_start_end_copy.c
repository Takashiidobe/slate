// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

typedef __builtin_va_list va_list;

int sum(int count, ...) {
  va_list ap, copy;
  __builtin_va_start(ap, count);
  __builtin_va_copy(copy, ap);
  int total = 0;
  for (int i = 0; i < count; i++)
    total += __builtin_va_arg(ap, int);
  __builtin_va_end(ap);
  __builtin_va_end(copy);
  return total;
}

void forward(va_list source) {
  va_list dup;
  __builtin_va_copy(dup, source);
  __builtin_va_end(dup);
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
// IR-NEXT:     type @type0 va_list = va_list;
// IR-NEXT:     fn %1 @sum(%2 count: i32, ...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %3 ap: va_list [storage=automatic];
// IR-NEXT:         let %4 copy: va_list [storage=automatic];
// IR-NEXT:         va_start(%3);
// IR-NEXT:         va_copy(%4, %3);
// IR-NEXT:         let %5 total: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:         for %10
// IR-NEXT:             init:
// IR-NEXT:                 let %6 i: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:             condition: lt<i32>(read<i32>(%6), read<i32>(%2))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %11: i32 [synthetic] = read<i32>(%6);
// IR-NEXT:                 let %12: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%11), const<i32>(1));
// IR-NEXT:                 write<i32>(%6, read<i32>(%12));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 let %13: i32 [synthetic] = read<i32>(%5);
// IR-NEXT:                 let %14: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%13), va_arg<i32>(%3));
// IR-NEXT:                 write<i32>(%5, read<i32>(%14));
// IR-NEXT:         va_end(%3);
// IR-NEXT:         va_end(%4);
// IR-NEXT:         return read<i32>(%5);
// IR-NEXT:     }
// IR-NEXT:     fn %7 @forward(%8 source: va_list) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %9 dup: va_list [storage=automatic];
// IR-NEXT:         va_copy(%9, %8);
// IR-NEXT:         va_end(%9);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
