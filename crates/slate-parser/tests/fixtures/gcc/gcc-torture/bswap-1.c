/* Test __builtin_bswap64 . */

unsigned long long g(unsigned long long a) __attribute__((noinline));
unsigned long long g(unsigned long long a) { return __builtin_bswap64(a); }

unsigned long long f(unsigned long long c) {
  union {
    unsigned long long a;
    unsigned char      b[8];
  } a, b;
  a.a    = c;
  b.b[0] = a.b[7];
  b.b[1] = a.b[6];
  b.b[2] = a.b[5];
  b.b[3] = a.b[4];
  b.b[4] = a.b[3];
  b.b[5] = a.b[2];
  b.b[6] = a.b[1];
  b.b[7] = a.b[0];
  return b.a;
}

int main(void) {
  unsigned long long i;
  /* The rest of the testcase assumes 8 byte long long. */
  if (sizeof(i) != sizeof(char) * 8)
    return 0;
  if (f(0x12) != g(0x12))
    __builtin_abort();
  if (f(0x1234) != g(0x1234))
    __builtin_abort();
  if (f(0x123456) != g(0x123456))
    __builtin_abort();
  if (f(0x12345678ull) != g(0x12345678ull))
    __builtin_abort();
  if (f(0x1234567890ull) != g(0x1234567890ull))
    __builtin_abort();
  if (f(0x123456789012ull) != g(0x123456789012ull))
    __builtin_abort();
  if (f(0x12345678901234ull) != g(0x12345678901234ull))
    __builtin_abort();
  if (f(0x1234567890123456ull) != g(0x1234567890123456ull))
    __builtin_abort();
  return 0;
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
// DEFAULT-NEXT:     type @type0 = union {
// DEFAULT-NEXT:         field0 a: u64;
// DEFAULT-NEXT:         field1 b: array<u8, 8>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %0 @g(%1 a: u64) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u64, signature=fn(u64) -> u64>(%11, read<u64>(%1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @__builtin_bswap64(%10 <unnamed>: u64) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %2 @f(%3 c: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 a: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %6 b: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(field0(%5), read<u64>(%3));
// DEFAULT-NEXT:         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%6)), const<i32>(0))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%5)), const<i32>(7)))));
// DEFAULT-NEXT:         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%6)), const<i32>(1))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%5)), const<i32>(6)))));
// DEFAULT-NEXT:         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%6)), const<i32>(2))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%5)), const<i32>(5)))));
// DEFAULT-NEXT:         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%6)), const<i32>(3))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%5)), const<i32>(4)))));
// DEFAULT-NEXT:         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%6)), const<i32>(4))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%5)), const<i32>(3)))));
// DEFAULT-NEXT:         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%6)), const<i32>(5))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%5)), const<i32>(2)))));
// DEFAULT-NEXT:         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%6)), const<i32>(6))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%5)), const<i32>(1)))));
// DEFAULT-NEXT:         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%6)), const<i32>(7))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%5)), const<i32>(0)))));
// DEFAULT-NEXT:         return read<u64>(field0(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 i: u64 [storage=automatic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(8), mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u64) -> u64>(%2, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(18)))), call<u64, signature=fn(u64) -> u64>(%0, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(18)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u64) -> u64>(%2, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4660)))), call<u64, signature=fn(u64) -> u64>(%0, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4660)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u64) -> u64>(%2, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1193046)))), call<u64, signature=fn(u64) -> u64>(%0, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1193046)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u64) -> u64>(%2, const<u64>(305419896)), call<u64, signature=fn(u64) -> u64>(%0, const<u64>(305419896)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u64) -> u64>(%2, const<u64>(78187493520)), call<u64, signature=fn(u64) -> u64>(%0, const<u64>(78187493520)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u64) -> u64>(%2, const<u64>(20015998341138)), call<u64, signature=fn(u64) -> u64>(%0, const<u64>(20015998341138)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u64) -> u64>(%2, const<u64>(5124095575331380)), call<u64, signature=fn(u64) -> u64>(%0, const<u64>(5124095575331380)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u64) -> u64>(%2, const<u64>(1311768467284833366)), call<u64, signature=fn(u64) -> u64>(%0, const<u64>(1311768467284833366)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
