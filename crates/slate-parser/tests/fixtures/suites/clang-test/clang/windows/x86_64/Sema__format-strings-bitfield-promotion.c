
int printf(const char *restrict, ...);

struct bitfields {
  long a : 2;
  unsigned long b : 2;
  long c : 32;          // assumes that int is 32 bits
  unsigned long d : 32; // assumes that int is 32 bits
} bf;

void bitfield_promotion(void) {
  printf("%ld", bf.a); // expected-warning {{format specifies type 'long' but the argument has type 'int'}}
  printf("%lu", bf.b); // expected-warning {{format specifies type 'unsigned long' but the argument has type 'int'}}
  printf("%ld", bf.c); // expected-warning {{format specifies type 'long' but the argument has type 'int'}}
  printf("%lu", bf.d); // expected-warning {{format specifies type 'unsigned long' but the argument has type 'unsigned int'}}
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type0 bitfields = struct {
// DEFAULT-NEXT:         field0 a: i32 : 2;
// DEFAULT-NEXT:         field1 b: u32 : 2;
// DEFAULT-NEXT:         field2 c: i32 : 32;
// DEFAULT-NEXT:         field3 d: u32 : 32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 0, 4, 8], bit_offsets=[Some(0), Some(2), Some(32), Some(64)], bit_units=[(0, 1), (4, 8)], field_units=[Some(0), Some(0), Some(1), Some(1)]];
// DEFAULT-NEXT:     global %2 bf: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 .str5: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 108, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %6 .str6: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 108, 117, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %7 .str7: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 108, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 108, 117, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%4 <unnamed>: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @bitfield_promotion() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%5)), read<i32>(bitfield0<unit=0, bytes=0..1, bits=0..2>(%2)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%6)), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..1, bits=2..4>(%2))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%7)), read<i32>(bitfield2<unit=1, bytes=4..12, bits=0..32>(%2)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%8)), read<u32>(bitfield3<unit=1, bytes=4..12, bits=32..64>(%2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
