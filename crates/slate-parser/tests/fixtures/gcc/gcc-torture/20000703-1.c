void abort(void);
void exit(int);
struct baz {
  char         a[17];
  char         b[3];
  unsigned int c;
  unsigned int d;
};

void foo(struct baz *p, unsigned int c, unsigned int d) {
  __builtin_memcpy(p->b, "abc", 3);
  p->c = c;
  p->d = d;
}

void bar(struct baz *p, unsigned int c, unsigned int d) {
  ({
    void *s = (p);
    __builtin_memset(s, '\0', sizeof(struct baz));
    s;
  });
  __builtin_memcpy(p->a, "01234567890123456", 17);
  __builtin_memcpy(p->b, "abc", 3);
  p->c = c;
  p->d = d;
}

int main() {
  struct baz p;
  foo(&p, 71, 18);
  if (p.c != 71 || p.d != 18)
    abort();
  bar(&p, 59, 26);
  if (p.c != 59 || p.d != 26)
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
// DEFAULT-NEXT:     type @type0 baz = struct {
// DEFAULT-NEXT:         field0 a: array<i8, 17>;
// DEFAULT-NEXT:         field1 b: array<i8, 3>;
// DEFAULT-NEXT:         field2 c: u32;
// DEFAULT-NEXT:         field3 d: u32;
// DEFAULT-NEXT:     } [size=28, align=4, offsets=[0, 17, 20, 24]];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %24 .str24: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 49, 50, 51, 52, 53, 54, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %25 .str25: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%14 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %18 @__builtin_memcpy(%15 <unnamed>: ptr<void>, %16 <unnamed>: ptr<const void>, %17 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo(%4 p: ptr<@type0>, %5 c: u32, %6 d: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%18, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(field1(deref(read<ptr<@type0>>(%4))))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%19)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         write<u32>(field2(deref(read<ptr<@type0>>(%4))), read<u32>(%5));
// DEFAULT-NEXT:         write<u32>(field3(deref(read<ptr<@type0>>(%4))), read<u32>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @__builtin_memset(%20 <unnamed>: ptr<void>, %21 <unnamed>: i32, %22 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %7 @bar(%8 p: ptr<@type0>, %9 c: u32, %10 d: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %26: ptr<void> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %11 s: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(read<ptr<@type0>>(%8));
// DEFAULT-NEXT:             call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%23, read<ptr<void>>(%11), const<i32>(0), const<u64>(28));
// DEFAULT-NEXT:             write<ptr<void>>(%26, read<ptr<void>>(%11));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%18, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(17)>(field0(deref(read<ptr<@type0>>(%8))))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%24)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(17))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%18, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(field1(deref(read<ptr<@type0>>(%8))))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%25)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         write<u32>(field2(deref(read<ptr<@type0>>(%8))), read<u32>(%9));
// DEFAULT-NEXT:         write<u32>(field3(deref(read<ptr<@type0>>(%8))), read<u32>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 p: @type0 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, u32, u32) -> void>(%3, addr_of<ptr<@type0>>(%13), reinterpret<u32, reason=arg, fits=always>(const<i32>(71)), reinterpret<u32, reason=arg, fits=always>(const<i32>(18)));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(read<u32>(field2(%13)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(71))), ne<u32>(read<u32>(field3(%13)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(18))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, u32, u32) -> void>(%7, addr_of<ptr<@type0>>(%13), reinterpret<u32, reason=arg, fits=always>(const<i32>(59)), reinterpret<u32, reason=arg, fits=always>(const<i32>(26)));
// DEFAULT-NEXT:         if logical_or<bool>(ne<u32>(read<u32>(field2(%13)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(59))), ne<u32>(read<u32>(field3(%13)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(26))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
