// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

void fill(char *cursor, char *end, char value) {
    while (cursor < end) *cursor++ = value;
}
unsigned long walk(const char *s) {
    unsigned long n = 0;
    while (*s) { n++; s++; }
    return n;
}
void acquire(int **out, int *stored) {
    *out = stored;
}
int roundtrip(int *p) {
    unsigned long address = (unsigned long)p;
    int *again = (int *)address;
    return *again + (p == 0);
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
// IR-NEXT:     fn %0 @fill(%1 cursor: ptr<i8>, %2 end: ptr<i8>, %3 value: i8) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         while %14 lt<ptr<i8>>(read<ptr<i8>>(%1), read<ptr<i8>>(%2))
// IR-NEXT:             let %16: ptr<i8> [synthetic] = read<ptr<i8>>(%1);
// IR-NEXT:             let %17: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%16), const<i32>(1));
// IR-NEXT:             write<ptr<i8>>(%1, read<ptr<i8>>(%17));
// IR-NEXT:             write<i8>(deref(read<ptr<i8>>(%16)), read<i8>(%3));
// IR-NEXT:     }
// IR-NEXT:     fn %4 @walk(%5 s: ptr<const i8>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %6 n: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// IR-NEXT:         while %15 ne<i8>(read<i8>(deref(read<ptr<const i8>>(%5))), const<i8>(0))
// IR-NEXT:             {
// IR-NEXT:                 let %18: u64 [synthetic] = read<u64>(%6);
// IR-NEXT:                 let %19: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%18), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// IR-NEXT:                 write<u64>(%6, read<u64>(%19));
// IR-NEXT:                 let %20: ptr<const i8> [synthetic] = read<ptr<const i8>>(%5);
// IR-NEXT:                 let %21: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%20), const<i32>(1));
// IR-NEXT:                 write<ptr<const i8>>(%5, read<ptr<const i8>>(%21));
// IR-NEXT:             }
// IR-NEXT:         return read<u64>(%6);
// IR-NEXT:     }
// IR-NEXT:     fn %7 @acquire(%8 out: ptr<ptr<i32>>, %9 stored: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%8)), read<ptr<i32>>(%9));
// IR-NEXT:     }
// IR-NEXT:     fn %10 @roundtrip(%11 p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %12 address: u64 [storage=automatic] = ptr_to_int<u64, reason=explicit>(read<ptr<i32>>(%11));
// IR-NEXT:         let %13 again: ptr<i32> [storage=automatic] = int_to_ptr<ptr<i32>, reason=explicit>(read<u64>(%12));
// IR-NEXT:         return add<i32, overflow=ub>(read<i32>(deref(read<ptr<i32>>(%13))), from_bool<i32, reason=promotion>(eq<ptr<i32>>(read<ptr<i32>>(%11), null<ptr<i32>>)));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
