/* PR c/117641 */
/* { dg-do compile { target bitint575 } } */
/* { dg-options "-std=c23" } */

void
foo (_BitInt(128) *b)
{
  __sync_add_and_fetch (b, 1);			/* { dg-error "incompatible" "" { target { ! int128 } } } */
  __sync_val_compare_and_swap (b, 0, 1);	/* { dg-error "incompatible" "" { target { ! int128 } } } */
  __sync_bool_compare_and_swap (b, 0, 1);	/* { dg-error "incompatible" "" { target { ! int128 } } } */
  __sync_lock_test_and_set (b, 1);		/* { dg-error "incompatible" "" { target { ! int128 } } } */
  __sync_lock_release (b);			/* { dg-error "incompatible" "" { target { ! int128 } } } */
}

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     fn %0 @foo(%1 b: ptr<i128b>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %2: i128b [synthetic] = update<i128b, result=new, atomic=seq_cst>(deref(read<ptr<i128b>>(%1)), add<i128b, overflow=wrap>(old<i128b>, widen<i128b, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %3: i128b [synthetic] = compare_exchange<i128b, form=old, weak=false, success=seq_cst, failure=seq_cst>(deref(read<ptr<i128b>>(%1)), widen<i128b, reason=arg>(const<i32>(0)), widen<i128b, reason=arg>(const<i32>(1)));
// DEFAULT-NEXT:         let %4: bool [synthetic] = compare_exchange<i128b, form=success, weak=false, success=seq_cst, failure=seq_cst>(deref(read<ptr<i128b>>(%1)), widen<i128b, reason=arg>(const<i32>(0)), widen<i128b, reason=arg>(const<i32>(1)));
// DEFAULT-NEXT:         let %5: i128b [synthetic] = update<i128b, result=old, atomic=acquire>(deref(read<ptr<i128b>>(%1)), widen<i128b, reason=arg>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128b, atomic=release>(deref(read<ptr<i128b>>(%1)), const<i128b>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
