void abort(void);
void exit(int);

void foo(unsigned int *p) {
  if ((signed char)(*p & 0xFF) == 17 || (signed char)(*p & 0xFF) == 18)
    return;
  else
    abort();
}

int main() {
  int i = 0x30011;
  foo(&i);
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%6 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 p: ptr<u32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(eq<i32>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(and<u32>(read<u32>(deref(read<ptr<u32>>(%3))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255)))))), const<i32>(17)), eq<i32>(widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(and<u32>(read<u32>(deref(read<ptr<u32>>(%3))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255)))))), const<i32>(18)))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 i: i32 [storage=automatic] = const<i32>(196625);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u32>) -> void>(%2, pointer_cast<ptr<u32>, reason=arg>(addr_of<ptr<i32>>(%5)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
