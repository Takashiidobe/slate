/* This tests an insn length of sign extension on h8300 port.  */

extern void exit(int);

volatile signed char *q;
volatile signed int   n;

void foo(void) {
  signed char *p;

  for (;;) {
    p = (signed char *)q;
    n = p[2];
    p = (signed char *)q;
    n = p[2];
    p = (signed char *)q;
    n = p[2];
    p = (signed char *)q;
    n = p[2];
    p = (signed char *)q;
    n = p[2];
    p = (signed char *)q;
    n = p[2];
    p = (signed char *)q;
    n = p[2];
    p = (signed char *)q;
    n = p[2];
    p = (signed char *)q;
    n = p[2];
    p = (signed char *)q;
    n = p[2];
    p = (signed char *)q;
    n = p[2];
  }
}

int main() { exit(0); }


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
// DEFAULT-NEXT:     global %[[VALUE_q:[0-9]+]] q: ptr<volatile i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_n:[0-9]+]] n: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=explicit>(read<ptr<volatile i8>>(%[[VALUE_q]])));
// DEFAULT-NEXT:                     write<i32, volatile>(%[[VALUE_n]], widen<i32, reason=assign>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), const<i32>(2))))));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=explicit>(read<ptr<volatile i8>>(%[[VALUE_q]])));
// DEFAULT-NEXT:                     write<i32, volatile>(%[[VALUE_n]], widen<i32, reason=assign>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), const<i32>(2))))));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=explicit>(read<ptr<volatile i8>>(%[[VALUE_q]])));
// DEFAULT-NEXT:                     write<i32, volatile>(%[[VALUE_n]], widen<i32, reason=assign>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), const<i32>(2))))));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=explicit>(read<ptr<volatile i8>>(%[[VALUE_q]])));
// DEFAULT-NEXT:                     write<i32, volatile>(%[[VALUE_n]], widen<i32, reason=assign>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), const<i32>(2))))));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=explicit>(read<ptr<volatile i8>>(%[[VALUE_q]])));
// DEFAULT-NEXT:                     write<i32, volatile>(%[[VALUE_n]], widen<i32, reason=assign>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), const<i32>(2))))));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=explicit>(read<ptr<volatile i8>>(%[[VALUE_q]])));
// DEFAULT-NEXT:                     write<i32, volatile>(%[[VALUE_n]], widen<i32, reason=assign>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), const<i32>(2))))));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=explicit>(read<ptr<volatile i8>>(%[[VALUE_q]])));
// DEFAULT-NEXT:                     write<i32, volatile>(%[[VALUE_n]], widen<i32, reason=assign>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), const<i32>(2))))));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=explicit>(read<ptr<volatile i8>>(%[[VALUE_q]])));
// DEFAULT-NEXT:                     write<i32, volatile>(%[[VALUE_n]], widen<i32, reason=assign>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), const<i32>(2))))));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=explicit>(read<ptr<volatile i8>>(%[[VALUE_q]])));
// DEFAULT-NEXT:                     write<i32, volatile>(%[[VALUE_n]], widen<i32, reason=assign>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), const<i32>(2))))));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=explicit>(read<ptr<volatile i8>>(%[[VALUE_q]])));
// DEFAULT-NEXT:                     write<i32, volatile>(%[[VALUE_n]], widen<i32, reason=assign>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), const<i32>(2))))));
// DEFAULT-NEXT:                     write<ptr<i8>>(%[[VALUE_p]], pointer_cast<ptr<i8>, reason=explicit>(read<ptr<volatile i8>>(%[[VALUE_q]])));
// DEFAULT-NEXT:                     write<i32, volatile>(%[[VALUE_n]], widen<i32, reason=assign>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_p]]), const<i32>(2))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
