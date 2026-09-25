void abort(void);
void exit(int);

static unsigned int expr_hash_table_size = 1;

int main() {
  int          del = 1;
  unsigned int i   = 0;

  if (i < expr_hash_table_size && del)
    exit(0);
  abort();
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
// DEFAULT-NEXT:     global %2 expr_hash_table_size: u32 [storage=static] = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%6 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 del: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %5 i: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         if logical_and<bool>(lt<u32>(read<u32>(%5), read<u32>(%2)), ne<i32>(read<i32>(%4), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
