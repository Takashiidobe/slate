// SLATE-FILECHECK-DEFINES DEFAULT

/* { dg-do compile { target { lp64 || llp64 } } } */
struct A { int b[1]; };

void
foo (struct A *d)
{
  d->b[0] = d->b[-144115188075855873LL] + d->b[11] * d->b[2]
          + d->b[0] % d->b[1025] + d->b[5];
  d->b[0] = d->b[144678138029277184LL] + d->b[0] & d->b[-3] * d->b[053]
          + d->b[7] ^ d->b[-9] + d->b[14] + d->b[9] % d->b[49]
          + d->b[024] + d->b[82] & d->b[4096];
}

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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 b: array<i32, 1>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %1 @foo(%2 d: ptr<@type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(field0(deref(read<ptr<@type0>>(%2)))), const<i32>(0))), add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(field0(deref(read<ptr<@type0>>(%2)))), neg<i64, overflow=ub>(const<i64>(144115188075855873))))), mul<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(field0(deref(read<ptr<@type0>>(%2)))), const<i32>(11)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(field0(deref(read<ptr<@type0>>(%2)))), const<i32>(2)))))), rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(field0(deref(read<ptr<@type0>>(%2)))), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(field0(deref(read<ptr<@type0>>(%2)))), const<i32>(1025)))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(field0(deref(read<ptr<@type0>>(%2)))), const<i32>(5))))));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(field0(deref(read<ptr<@type0>>(%2)))), const<i32>(0))), xor<i32>(and<i32>(add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(field0(deref(read<ptr<@type0>>(%2)))), const<i64>(144678138029277184)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(field0(deref(read<ptr<@type0>>(%2)))), const<i32>(0))))), add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(field0(deref(read<ptr<@type0>>(%2)))), neg<i32, overflow=ub>(const<i32>(3))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(field0(deref(read<ptr<@type0>>(%2)))), const<i32>(43))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(field0(deref(read<ptr<@type0>>(%2)))), const<i32>(7)))))), and<i32>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(field0(deref(read<ptr<@type0>>(%2)))), neg<i32, overflow=ub>(const<i32>(9))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(field0(deref(read<ptr<@type0>>(%2)))), const<i32>(14))))), rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(field0(deref(read<ptr<@type0>>(%2)))), const<i32>(9)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(field0(deref(read<ptr<@type0>>(%2)))), const<i32>(49)))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(field0(deref(read<ptr<@type0>>(%2)))), const<i32>(20))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(field0(deref(read<ptr<@type0>>(%2)))), const<i32>(82))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(field0(deref(read<ptr<@type0>>(%2)))), const<i32>(4096)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
