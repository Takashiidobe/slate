extern void abort(void);

struct container {
  unsigned char data[1];
};

unsigned char space[6] = {1, 2, 3, 4, 5, 6};

int foo(struct container *p) { return p->data[4]; }

int main() {
  if (foo((struct container *)space) != 5)
    abort();
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
// DEFAULT-NEXT:     type @type0 container = struct {
// DEFAULT-NEXT:         field0 data: array<u8, 1>;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %2 space: array<u8, 6> [storage=static] = aggregate<array<u8, 6>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(4))), index4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), index5 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(6)))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo(%4 p: ptr<@type0>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(widen<u32, reason=return>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1)>(field0(deref(read<ptr<@type0>>(%4)))), const<i32>(4))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type0>) -> i32>(%3, pointer_cast<ptr<@type0>, reason=explicit>(array_decay<ptr<u8>, length=Some(6)>(%2))), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
