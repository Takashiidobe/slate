void abort(void);
void exit(int);

struct tiny {
  short c;
  short d;
};

void f(int n, struct tiny x, struct tiny y, struct tiny z, long l) {
  if (x.c != 10)
    abort();
  if (x.d != 20)
    abort();

  if (y.c != 11)
    abort();
  if (y.d != 21)
    abort();

  if (z.c != 12)
    abort();
  if (z.d != 22)
    abort();

  if (l != 123)
    abort();
}

int main(void) {
  struct tiny x[3];
  x[0].c = 10;
  x[1].c = 11;
  x[2].c = 12;
  x[0].d = 20;
  x[1].d = 21;
  x[2].d = 22;
  f(3, x[0], x[1], x[2], (long)123);
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
// DEFAULT-NEXT:     type @type0 tiny = struct {
// DEFAULT-NEXT:         field0 c: i16;
// DEFAULT-NEXT:         field1 d: i16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%11 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @f(%4 n: i32, %5 x: @type0, %6 y: @type0, %7 z: @type0, %8 l: i64) -> void [linkage=external] [abi=sysv64(scalar, coerce<i32>, coerce<i32>, coerce<i32>, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(field0(%5))), const<i32>(10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(field1(%5))), const<i32>(20))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(field0(%6))), const<i32>(11))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(field1(%6))), const<i32>(21))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(field0(%7))), const<i32>(12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(field1(%7))), const<i32>(22))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%8), widen<i64, reason=usual_arith>(const<i32>(123)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %10 x: array<@type0, 3> [storage=automatic];
// DEFAULT-NEXT:         write<i16>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(%10), const<i32>(0)))), truncate<i16, reason=assign, fits=always>(const<i32>(10)));
// DEFAULT-NEXT:         write<i16>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(%10), const<i32>(1)))), truncate<i16, reason=assign, fits=always>(const<i32>(11)));
// DEFAULT-NEXT:         write<i16>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(%10), const<i32>(2)))), truncate<i16, reason=assign, fits=always>(const<i32>(12)));
// DEFAULT-NEXT:         write<i16>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(%10), const<i32>(0)))), truncate<i16, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i16>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(%10), const<i32>(1)))), truncate<i16, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<i16>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(%10), const<i32>(2)))), truncate<i16, reason=assign, fits=always>(const<i32>(22)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, @type0, @type0, @type0, i64) -> void, abi=sysv64(scalar, coerce<i32>, coerce<i32>, coerce<i32>, scalar) -> void>(%3, const<i32>(3), copy<@type0, reason=arg>(read<@type0>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(%10), const<i32>(0))))), copy<@type0, reason=arg>(read<@type0>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(%10), const<i32>(1))))), copy<@type0, reason=arg>(read<@type0>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(%10), const<i32>(2))))), widen<i64, reason=explicit>(const<i32>(123)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
