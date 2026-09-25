void abort(void);
void exit(int);

struct s {
  long a;
  int  b;
};

int foo(int x, void *y) {
  switch (x) {
  case 0:
    return ((struct s *)y)->a;
  case 1:
    return *(signed char *)y;
  case 2:
    return *(short *)y;
  }
  abort();
}

int main() {
  struct s    s;
  short       sh[10];
  signed char c[10];
  int         i;

  s.a = 1;
  s.b = 2;
  for (i = 0; i < 10; i++) {
    sh[i] = i;
    c[i]  = i;
  }

  if (foo(0, &s) != 1)
    abort();
  if (foo(1, c + 3) != 3)
    abort();
  if (foo(2, sh + 3) != 3)
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
// DEFAULT-NEXT:     type @type0 s = struct {
// DEFAULT-NEXT:         field0 a: i64;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%11 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo(%4 x: i32, %5 y: ptr<void>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %12 read<i32>(%4)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %12 const<i32>(0):
// DEFAULT-NEXT:                     return truncate<i32, reason=return, fits=unknown>(read<i64>(field0(deref(pointer_cast<ptr<@type0>, reason=explicit>(read<ptr<void>>(%5))))));
// DEFAULT-NEXT:                 case %12 const<i32>(1):
// DEFAULT-NEXT:                     return widen<i32, reason=return>(read<i8>(deref(pointer_cast<ptr<i8>, reason=explicit>(read<ptr<void>>(%5)))));
// DEFAULT-NEXT:                 case %12 const<i32>(2):
// DEFAULT-NEXT:                     return widen<i32, reason=return>(read<i16>(deref(pointer_cast<ptr<i16>, reason=explicit>(read<ptr<void>>(%5)))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 s: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %8 sh: array<i16, 10> [storage=automatic];
// DEFAULT-NEXT:         let %9 c: array<i8, 10> [storage=automatic];
// DEFAULT-NEXT:         let %10 i: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(field0(%7), widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(field1(%7), const<i32>(2));
// DEFAULT-NEXT:         for %13
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%10), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%15));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(10)>(%8), read<i32>(%10))), truncate<i16, reason=assign, fits=unknown>(read<i32>(%10)));
// DEFAULT-NEXT:                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%9), read<i32>(%10))), truncate<i8, reason=assign, fits=unknown>(read<i32>(%10)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, ptr<void>) -> i32>(%3, const<i32>(0), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type0>>(%7))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, ptr<void>) -> i32>(%3, const<i32>(1), pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(10)>(%9), const<i32>(3)))), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, ptr<void>) -> i32>(%3, const<i32>(2), pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(10)>(%8), const<i32>(3)))), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
