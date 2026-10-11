
volatile unsigned char sink = 0;
void test32(long *base, long idx) {
  sink = _bittest(base, idx);
  sink = _bittestandcomplement(base, idx);
  sink = _bittestandreset(base, idx);
  sink = _bittestandset(base, idx);
  sink = _interlockedbittestandreset(base, idx);
  sink = _interlockedbittestandset(base, idx);
}

void test64(__int64 *base, __int64 idx) {
  sink = _bittest64(base, idx);
  sink = _bittestandcomplement64(base, idx);
  sink = _bittestandreset64(base, idx);
  sink = _bittestandset64(base, idx);
  sink = _interlockedbittestandreset64(base, idx);
  sink = _interlockedbittestandset64(base, idx);
}

#if defined(_M_ARM) || defined(_M_ARM64)
void test_arm(long *base, long idx) {
  sink = _interlockedbittestandreset_acq(base, idx);
  sink = _interlockedbittestandreset_rel(base, idx);
  sink = _interlockedbittestandreset_nf(base, idx);
  sink = _interlockedbittestandset_acq(base, idx);
  sink = _interlockedbittestandset_rel(base, idx);
  sink = _interlockedbittestandset_nf(base, idx);
}
#endif

#if defined(_M_ARM64)
void test_arm64(__int64 *base, __int64 idx) {
  sink = _interlockedbittestandreset64_acq(base, idx);
  sink = _interlockedbittestandreset64_rel(base, idx);
  sink = _interlockedbittestandreset64_nf(base, idx);
  sink = _interlockedbittestandset64_acq(base, idx);
  sink = _interlockedbittestandset64_rel(base, idx);
  sink = _interlockedbittestandset64_nf(base, idx);
}
#endif















// Just look for the atomicrmw instructions.
// Just look for the atomicrmw instructions.

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_sink:[0-9]+]] sink: volatile u8 [storage=static] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test32:[0-9]+]] @test32(%[[VALUE_base:[0-9]+]] base: ptr<i32>, %[[VALUE_idx:[0-9]+]] idx: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_idx]]);
// DEFAULT-NEXT:         write<u8, volatile>(%[[VALUE_sink]], and<u8>(shr<u8, amount_out_of_range=ub, fill=zero_extend>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=wrap>(pointer_cast<ptr<const u8>, reason=explicit>(pointer_cast<ptr<const i32>, reason=arg>(read<ptr<i32>>(%[[VALUE_base]]))), shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE0]]), const<i32>(3))))), and<u8>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(read<i32>(%[[VALUE0]]))), const<u8>(7))), const<u8>(1)));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_idx]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=wrap>(pointer_cast<ptr<u8>, reason=explicit>(read<ptr<i32>>(%[[VALUE_base]])), shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE1]]), const<i32>(3)));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u8 [synthetic] = read<u8>(deref(read<ptr<u8>>(%[[VALUE2]])));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: u8 [synthetic] = xor<u8>(read<u8>(%[[VALUE3]]), shl<u8, overflow=wrap, amount_out_of_range=ub>(const<u8>(1), and<u8>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(read<i32>(%[[VALUE1]]))), const<u8>(7))));
// DEFAULT-NEXT:         write<u8>(deref(read<ptr<u8>>(%[[VALUE2]])), read<u8>(%[[VALUE4]]));
// DEFAULT-NEXT:         write<u8, volatile>(%[[VALUE_sink]], and<u8>(shr<u8, amount_out_of_range=ub, fill=zero_extend>(read<u8>(%[[VALUE3]]), and<u8>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(read<i32>(%[[VALUE1]]))), const<u8>(7))), const<u8>(1)));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_idx]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=wrap>(pointer_cast<ptr<u8>, reason=explicit>(read<ptr<i32>>(%[[VALUE_base]])), shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE5]]), const<i32>(3)));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: u8 [synthetic] = read<u8>(deref(read<ptr<u8>>(%[[VALUE6]])));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: u8 [synthetic] = and<u8>(read<u8>(%[[VALUE7]]), not<u8>(shl<u8, overflow=wrap, amount_out_of_range=ub>(const<u8>(1), and<u8>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(read<i32>(%[[VALUE5]]))), const<u8>(7)))));
// DEFAULT-NEXT:         write<u8>(deref(read<ptr<u8>>(%[[VALUE6]])), read<u8>(%[[VALUE8]]));
// DEFAULT-NEXT:         write<u8, volatile>(%[[VALUE_sink]], and<u8>(shr<u8, amount_out_of_range=ub, fill=zero_extend>(read<u8>(%[[VALUE7]]), and<u8>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(read<i32>(%[[VALUE5]]))), const<u8>(7))), const<u8>(1)));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_idx]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=wrap>(pointer_cast<ptr<u8>, reason=explicit>(read<ptr<i32>>(%[[VALUE_base]])), shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE9]]), const<i32>(3)));
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: u8 [synthetic] = read<u8>(deref(read<ptr<u8>>(%[[VALUE10]])));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: u8 [synthetic] = or<u8>(read<u8>(%[[VALUE11]]), shl<u8, overflow=wrap, amount_out_of_range=ub>(const<u8>(1), and<u8>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(read<i32>(%[[VALUE9]]))), const<u8>(7))));
// DEFAULT-NEXT:         write<u8>(deref(read<ptr<u8>>(%[[VALUE10]])), read<u8>(%[[VALUE12]]));
// DEFAULT-NEXT:         write<u8, volatile>(%[[VALUE_sink]], and<u8>(shr<u8, amount_out_of_range=ub, fill=zero_extend>(read<u8>(%[[VALUE11]]), and<u8>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(read<i32>(%[[VALUE9]]))), const<u8>(7))), const<u8>(1)));
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_idx]]);
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(deref(ptr_offset<ptr<volatile u8>, subtract=false, element=u8, overflow=wrap>(pointer_cast<ptr<volatile u8>, reason=explicit>(pointer_cast<ptr<volatile i32>, reason=arg>(read<ptr<i32>>(%[[VALUE_base]]))), shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE13]]), const<i32>(3)))), and<u8>(old<u8>, not<u8>(shl<u8, overflow=wrap, amount_out_of_range=ub>(const<u8>(1), and<u8>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(read<i32>(%[[VALUE13]]))), const<u8>(7))))));
// DEFAULT-NEXT:         write<u8, volatile>(%[[VALUE_sink]], and<u8>(shr<u8, amount_out_of_range=ub, fill=zero_extend>(read<u8>(%[[VALUE14]]), and<u8>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(read<i32>(%[[VALUE13]]))), const<u8>(7))), const<u8>(1)));
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_idx]]);
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(deref(ptr_offset<ptr<volatile u8>, subtract=false, element=u8, overflow=wrap>(pointer_cast<ptr<volatile u8>, reason=explicit>(pointer_cast<ptr<volatile i32>, reason=arg>(read<ptr<i32>>(%[[VALUE_base]]))), shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE15]]), const<i32>(3)))), or<u8>(old<u8>, shl<u8, overflow=wrap, amount_out_of_range=ub>(const<u8>(1), and<u8>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(read<i32>(%[[VALUE15]]))), const<u8>(7)))));
// DEFAULT-NEXT:         write<u8, volatile>(%[[VALUE_sink]], and<u8>(shr<u8, amount_out_of_range=ub, fill=zero_extend>(read<u8>(%[[VALUE16]]), and<u8>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(read<i32>(%[[VALUE15]]))), const<u8>(7))), const<u8>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test64:[0-9]+]] @test64(%[[VALUE_base_2:[0-9]+]] base: ptr<i64>, %[[VALUE_idx_2:[0-9]+]] idx: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_idx_2]]);
// DEFAULT-NEXT:         write<u8, volatile>(%[[VALUE_sink]], and<u8>(shr<u8, amount_out_of_range=ub, fill=zero_extend>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=wrap>(pointer_cast<ptr<const u8>, reason=explicit>(pointer_cast<ptr<const i64>, reason=arg>(read<ptr<i64>>(%[[VALUE_base_2]]))), shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE17]]), const<i64>(3))))), and<u8>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(read<i64>(%[[VALUE17]]))), const<u8>(7))), const<u8>(1)));
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_idx_2]]);
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=wrap>(pointer_cast<ptr<u8>, reason=explicit>(read<ptr<i64>>(%[[VALUE_base_2]])), shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE18]]), const<i64>(3)));
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: u8 [synthetic] = read<u8>(deref(read<ptr<u8>>(%[[VALUE19]])));
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: u8 [synthetic] = xor<u8>(read<u8>(%[[VALUE20]]), shl<u8, overflow=wrap, amount_out_of_range=ub>(const<u8>(1), and<u8>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(read<i64>(%[[VALUE18]]))), const<u8>(7))));
// DEFAULT-NEXT:         write<u8>(deref(read<ptr<u8>>(%[[VALUE19]])), read<u8>(%[[VALUE21]]));
// DEFAULT-NEXT:         write<u8, volatile>(%[[VALUE_sink]], and<u8>(shr<u8, amount_out_of_range=ub, fill=zero_extend>(read<u8>(%[[VALUE20]]), and<u8>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(read<i64>(%[[VALUE18]]))), const<u8>(7))), const<u8>(1)));
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_idx_2]]);
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=wrap>(pointer_cast<ptr<u8>, reason=explicit>(read<ptr<i64>>(%[[VALUE_base_2]])), shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE22]]), const<i64>(3)));
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: u8 [synthetic] = read<u8>(deref(read<ptr<u8>>(%[[VALUE23]])));
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: u8 [synthetic] = and<u8>(read<u8>(%[[VALUE24]]), not<u8>(shl<u8, overflow=wrap, amount_out_of_range=ub>(const<u8>(1), and<u8>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(read<i64>(%[[VALUE22]]))), const<u8>(7)))));
// DEFAULT-NEXT:         write<u8>(deref(read<ptr<u8>>(%[[VALUE23]])), read<u8>(%[[VALUE25]]));
// DEFAULT-NEXT:         write<u8, volatile>(%[[VALUE_sink]], and<u8>(shr<u8, amount_out_of_range=ub, fill=zero_extend>(read<u8>(%[[VALUE24]]), and<u8>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(read<i64>(%[[VALUE22]]))), const<u8>(7))), const<u8>(1)));
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_idx_2]]);
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=wrap>(pointer_cast<ptr<u8>, reason=explicit>(read<ptr<i64>>(%[[VALUE_base_2]])), shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE26]]), const<i64>(3)));
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: u8 [synthetic] = read<u8>(deref(read<ptr<u8>>(%[[VALUE27]])));
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: u8 [synthetic] = or<u8>(read<u8>(%[[VALUE28]]), shl<u8, overflow=wrap, amount_out_of_range=ub>(const<u8>(1), and<u8>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(read<i64>(%[[VALUE26]]))), const<u8>(7))));
// DEFAULT-NEXT:         write<u8>(deref(read<ptr<u8>>(%[[VALUE27]])), read<u8>(%[[VALUE29]]));
// DEFAULT-NEXT:         write<u8, volatile>(%[[VALUE_sink]], and<u8>(shr<u8, amount_out_of_range=ub, fill=zero_extend>(read<u8>(%[[VALUE28]]), and<u8>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(read<i64>(%[[VALUE26]]))), const<u8>(7))), const<u8>(1)));
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_idx_2]]);
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(deref(ptr_offset<ptr<volatile u8>, subtract=false, element=u8, overflow=wrap>(pointer_cast<ptr<volatile u8>, reason=explicit>(pointer_cast<ptr<volatile i64>, reason=arg>(read<ptr<i64>>(%[[VALUE_base_2]]))), shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE30]]), const<i64>(3)))), and<u8>(old<u8>, not<u8>(shl<u8, overflow=wrap, amount_out_of_range=ub>(const<u8>(1), and<u8>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(read<i64>(%[[VALUE30]]))), const<u8>(7))))));
// DEFAULT-NEXT:         write<u8, volatile>(%[[VALUE_sink]], and<u8>(shr<u8, amount_out_of_range=ub, fill=zero_extend>(read<u8>(%[[VALUE31]]), and<u8>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(read<i64>(%[[VALUE30]]))), const<u8>(7))), const<u8>(1)));
// DEFAULT-NEXT:         let %[[VALUE32:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_idx_2]]);
// DEFAULT-NEXT:         let %[[VALUE33:[0-9]+]]: u8 [synthetic] = update<u8, result=old, volatile, atomic=seq_cst>(deref(ptr_offset<ptr<volatile u8>, subtract=false, element=u8, overflow=wrap>(pointer_cast<ptr<volatile u8>, reason=explicit>(pointer_cast<ptr<volatile i64>, reason=arg>(read<ptr<i64>>(%[[VALUE_base_2]]))), shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE32]]), const<i64>(3)))), or<u8>(old<u8>, shl<u8, overflow=wrap, amount_out_of_range=ub>(const<u8>(1), and<u8>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(read<i64>(%[[VALUE32]]))), const<u8>(7)))));
// DEFAULT-NEXT:         write<u8, volatile>(%[[VALUE_sink]], and<u8>(shr<u8, amount_out_of_range=ub, fill=zero_extend>(read<u8>(%[[VALUE33]]), and<u8>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(read<i64>(%[[VALUE32]]))), const<u8>(7))), const<u8>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
