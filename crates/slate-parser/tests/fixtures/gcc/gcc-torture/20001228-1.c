void abort(void);
void exit(int);

int foo1(void) {
  union {
    char     a[sizeof(unsigned)];
    unsigned b;
  } u;

  u.b = 0x01;
  return u.a[0];
}

int foo2(void) {
  volatile union {
    char     a[sizeof(unsigned)];
    unsigned b;
  } u;

  u.b = 0x01;
  return u.a[0];
}

int main(void) {
  if (foo1() != foo2())
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
// DEFAULT-NEXT:     type @type0 = union {
// DEFAULT-NEXT:         field0 a: array<i8, 4>;
// DEFAULT-NEXT:         field1 b: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type1 = union {
// DEFAULT-NEXT:         field0 a: array<i8, 4>;
// DEFAULT-NEXT:         field1 b: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%9 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 u: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(field1(%4), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(field0(%4)), const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @foo2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 u: volatile @type1 [storage=automatic];
// DEFAULT-NEXT:         write<u32, volatile>(field1(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8, volatile>(deref(ptr_offset<ptr<volatile i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<volatile i8>, length=Some(4)>(field0(%7)), const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%2), call<i32, signature=fn() -> i32>(%5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
