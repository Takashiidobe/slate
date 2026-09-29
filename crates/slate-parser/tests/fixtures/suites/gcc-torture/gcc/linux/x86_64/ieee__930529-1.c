/* { dg-do run } */
void abort(void);
void exit(int);

int main(void) {
  union {
    double        d;
    unsigned char c[8];
  } d;

  d.d = 1.0 / 7.0;

  if (sizeof(char) * 8 == sizeof(double)) {
    if (d.c[0] == 0x92 && d.c[1] == 0x24 && d.c[2] == 0x49 && d.c[3] == 0x92 &&
        d.c[4] == 0x24 && d.c[5] == 0x49 && d.c[6] == 0xc2 && d.c[7] == 0x3f)
      exit(0);
    if (d.c[7] == 0x92 && d.c[6] == 0x24 && d.c[5] == 0x49 && d.c[4] == 0x92 &&
        d.c[3] == 0x24 && d.c[2] == 0x49 && d.c[1] == 0xc2 && d.c[0] == 0x3f)
      exit(0);
#if defined __arm__ || defined __thumb__
    if (d.c[4] == 0x92 && d.c[5] == 0x24 && d.c[6] == 0x49 && d.c[7] == 0x92 &&
        d.c[0] == 0x24 && d.c[1] == 0x49 && d.c[2] == 0xc2 && d.c[3] == 0x3f)
      exit(0);
#endif
    abort();
  }

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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 d: f64;
// DEFAULT-NEXT:         field1 c: array<u8, 8>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         write<f64>(field0(%[[VALUE_d]]), div<f64, rounding=nearest_even, exceptions=observable, contract=fast>(const<f64>(1.0), const<f64>(7.0)));
// DEFAULT-NEXT:         if eq<u64>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), const<u64>(8))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>,
// DEFAULT-SAME: length=Some(8)>(field1(%[[VALUE_d]])), const<i32>(0)))))), const<i32>(146)), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%[[VALUE_d]])),
// DEFAULT-SAME: const<i32>(1)))))), const<i32>(36))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>,
// DEFAULT-SAME: length=Some(8)>(field1(%[[VALUE_d]])), const<i32>(2)))))), const<i32>(73))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%[[VALUE_d]])),
// DEFAULT-SAME: const<i32>(3)))))), const<i32>(146))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>,
// DEFAULT-SAME: length=Some(8)>(field1(%[[VALUE_d]])), const<i32>(4)))))), const<i32>(36))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%[[VALUE_d]])),
// DEFAULT-SAME: const<i32>(5)))))), const<i32>(73))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>,
// DEFAULT-SAME: length=Some(8)>(field1(%[[VALUE_d]])), const<i32>(6)))))), const<i32>(194))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%[[VALUE_d]])),
// DEFAULT-SAME: const<i32>(7)))))), const<i32>(63)))
// DEFAULT-NEXT:                     call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:                 if logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>,
// DEFAULT-SAME: length=Some(8)>(field1(%[[VALUE_d]])), const<i32>(7)))))), const<i32>(146)), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%[[VALUE_d]])),
// DEFAULT-SAME: const<i32>(6)))))), const<i32>(36))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>,
// DEFAULT-SAME: length=Some(8)>(field1(%[[VALUE_d]])), const<i32>(5)))))), const<i32>(73))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%[[VALUE_d]])),
// DEFAULT-SAME: const<i32>(4)))))), const<i32>(146))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>,
// DEFAULT-SAME: length=Some(8)>(field1(%[[VALUE_d]])), const<i32>(3)))))), const<i32>(36))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%[[VALUE_d]])),
// DEFAULT-SAME: const<i32>(2)))))), const<i32>(73))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>,
// DEFAULT-SAME: length=Some(8)>(field1(%[[VALUE_d]])), const<i32>(1)))))), const<i32>(194))), eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%[[VALUE_d]])),
// DEFAULT-SAME: const<i32>(0)))))), const<i32>(63)))
// DEFAULT-NEXT:                     call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
