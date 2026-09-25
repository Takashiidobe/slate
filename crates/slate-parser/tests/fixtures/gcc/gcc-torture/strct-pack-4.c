void abort(void);
void exit(int);

typedef struct {
  unsigned char  a __attribute__((packed));
  unsigned short b __attribute__((packed));
} three_char_t;

unsigned char my_set_a(void) { return 0xab; }

unsigned short my_set_b(void) { return 0x1234; }

int main(void) {
  three_char_t three_char;

  three_char.a = my_set_a();
  three_char.b = my_set_b();
  if (three_char.a != 0xab || three_char.b != 0x1234)
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 a: u8;
// DEFAULT-NEXT:         field1 b: u16;
// DEFAULT-NEXT:     } [size=3, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type1 three_char_t = @type0;
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%8 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @my_set_a() -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u8, reason=return, fits=unknown>(truncate<i8, reason=return, fits=unknown>(const<i32>(171)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @my_set_b() -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=always>(const<i32>(4660)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 three_char: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<u8>(field0(%7), call<u8, signature=fn() -> u8>(%4));
// DEFAULT-NEXT:         call<u8, signature=fn() -> u8>(%4);
// DEFAULT-NEXT:         write<u16>(field1(%7), call<u16, signature=fn() -> u16>(%5));
// DEFAULT-NEXT:         call<u16, signature=fn() -> u16>(%5);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field0(%7)))), const<i32>(171)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field1(%7)))), const<i32>(4660)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
