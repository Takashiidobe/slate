void abort(void);
void exit(int);

long long acc;

void addhi(short a) { acc += (long long)a << 32; }

void subhi(short a) { acc -= (long long)a << 32; }

int main(void) {
  acc = 0xffff00000000ll;
  addhi(1);
  if (acc != 0x1000000000000ll)
    abort();
  subhi(1);
  if (acc != 0xffff00000000ll)
    abort();
  exit(0);
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
// DEFAULT-NEXT:     global %2 acc: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%8 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @addhi(%4 a: i16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %10: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%9), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(read<i16>(%4)), const<i32>(32)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @subhi(%6 a: i16) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %11: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %12: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%11), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(read<i16>(%6)), const<i32>(32)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i64>(%2, const<i64>(281470681743360));
// DEFAULT-NEXT:         call<void, signature=fn(i16) -> void>(%3, truncate<i16, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%2), const<i64>(281474976710656))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i16) -> void>(%5, truncate<i16, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%2), const<i64>(281470681743360))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
