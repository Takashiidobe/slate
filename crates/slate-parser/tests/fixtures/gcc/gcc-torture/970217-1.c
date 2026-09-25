void exit(int);

int sub(int i, int array[i++]) { return i; }

int main(void) {
  int array[10];
  exit(sub(10, array) != 11);
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
// DEFAULT-NEXT:     fn %0 @exit(%6 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @sub(%2 i: i32, %3 array: ptr<i32> [array=%7]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %9: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%8), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%9));
// DEFAULT-NEXT:         let %7: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%8)));
// DEFAULT-NEXT:         return read<i32>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 array: array<i32, 10> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, from_bool<i32, reason=arg>(ne<i32>(call<i32, signature=fn(i32, ptr<i32>) -> i32>(%1, const<i32>(10), array_decay<ptr<i32>, length=Some(10)>(%5)), const<i32>(11))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
