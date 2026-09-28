struct __attribute__((packed)) S {
  int a, b, c;
};

static int __attribute__((noinline, noclone)) extract(const char *p) {
  struct S s;
  __builtin_memcpy(&s, p, sizeof(struct S));
  return s.a;
}

volatile int i;

int main(void) {
  char p[sizeof(struct S) + 1];

  __builtin_memset(p, 0, sizeof(struct S) + 1);
  i = extract(p + 1);

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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:     } [size=12, align=1, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     global %4 i: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %10 @__builtin_memcpy(%7 <unnamed>: ptr<void>, %8 <unnamed>: ptr<const void>, %9 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %1 @extract(%2 p: ptr<const i8>) -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 s: @type0 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%10, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type0>>(%3)), pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%2)), const<u64>(12));
// DEFAULT-NEXT:         return read<i32>(field0(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @__builtin_memset(%11 <unnamed>: ptr<void>, %12 <unnamed>: i32, %13 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 p: array<i8, 13> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%14, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%6)), const<i32>(0), add<u64, overflow=wrap>(const<u64>(12), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         write<i32, volatile>(%4, call<i32, signature=fn(ptr<const i8>) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(13)>(%6), const<i32>(1)))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(13)>(%6), const<i32>(1))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
