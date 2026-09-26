struct S {
  char  c;
  char  arr[4];
  float f;
};

char A[4] = {'1', '2', '3', '4'};

void foo(struct S s) {
  if (__builtin_memcmp(s.arr, A, 4))
    __builtin_abort();
}

int main(void) {
  struct S s;
  __builtin_memcpy(s.arr, A, 4);
  foo(s);
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
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 arr: array<i8, 4>;
// DEFAULT-NEXT:         field2 f: f32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 1, 8]];
// DEFAULT-NEXT:     global %1 A: array<i8, 4> [storage=static] = aggregate<array<i8, 4>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(49)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(50)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(51)), index3 = truncate<i8, reason=assign, fits=always>(const<i32>(52))) [linkage=external];
// DEFAULT-NEXT:     fn %9 @__builtin_memcmp(%6 <unnamed>: ptr<const void>, %7 <unnamed>: ptr<const void>, %8 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %10 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @foo(%3 s: @type0) -> void [linkage=external] [abi=sysv64(native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%9, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(field1(%3))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%1)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @__builtin_memcpy(%11 <unnamed>: ptr<void>, %12 <unnamed>: ptr<const void>, %13 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 s: @type0 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%14, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(field1(%5))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%1)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:         call<void, signature=fn(@type0) -> void, abi=sysv64(native_c) -> void>(%2, copy<@type0, reason=arg>(read<@type0>(%5)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
