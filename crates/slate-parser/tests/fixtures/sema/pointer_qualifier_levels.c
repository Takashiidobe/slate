// SLATE-FILECHECK-DEFINES VALID
// SLATE-FILECHECK-DEFINES INNER_CONST INNER_CONST
// SLATE-FILECHECK-DEFINES OUTER_CONST OUTER_CONST
// SLATE-FILECHECK-IR-ERROR INNER_CONST
// SLATE-FILECHECK-IR-ERROR OUTER_CONST
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

int *const *inner;
int **const outer = 0;
int *volatile *restrict mixed;

_Static_assert(_Generic(inner, int *const *: 1, default: 0));
_Static_assert(_Generic(outer, int **: 1, default: 0));
_Static_assert(_Generic(mixed, int *volatile *: 1, default: 0));
_Static_assert(_Generic((int *const *)0, int *const *: 1, default: 0));

void assign(int **source) {
  inner = source;
  **outer = 1;
  *mixed = 0;
#ifdef INNER_CONST
  *inner = 0;
#endif
#ifdef OUTER_CONST
  outer = source;
#endif
}

// SLATE-FILECHECK-BEGIN INNER_CONST
// INNER_CONST: Error:   × invalid in this context: cannot assign to a const-qualified lvalue
// SLATE-FILECHECK-END INNER_CONST
// SLATE-FILECHECK-BEGIN OUTER_CONST
// OUTER_CONST: Error:   × invalid in this context: cannot assign to a const-qualified lvalue
// SLATE-FILECHECK-END OUTER_CONST
// SLATE-FILECHECK-BEGIN VALID
// VALID: module {
// VALID-NEXT:     target "x86_64-unknown-linux-gnu" {
// VALID-NEXT:         endian = little;
// VALID-NEXT:         pointer [size=8, align=8];
// VALID-NEXT:         stack_alignment = 16;
// VALID-NEXT:         long_double = f80;
// VALID-NEXT:         storage bool [size=1, align=1];
// VALID-NEXT:         storage i8, u8 [size=1, align=1];
// VALID-NEXT:         storage i16, u16 [size=2, align=2];
// VALID-NEXT:         storage i32, u32 [size=4, align=4];
// VALID-NEXT:         storage i64, u64 [size=8, align=8];
// VALID-NEXT:         storage i128, u128 [size=16, align=16];
// VALID-NEXT:         storage bf16 [size=2, align=2];
// VALID-NEXT:         storage f16 [size=2, align=2];
// VALID-NEXT:         storage f32 [size=4, align=4];
// VALID-NEXT:         storage f64 [size=8, align=8];
// VALID-NEXT:         storage f80 [size=16, align=16];
// VALID-NEXT:         storage f128 [size=16, align=16];
// VALID-NEXT:         storage d32 [size=4, align=4];
// VALID-NEXT:         storage d64 [size=8, align=8];
// VALID-NEXT:         storage d128 [size=16, align=16];
// VALID-NEXT:     }
// VALID-NEXT:     global %0 inner: ptr<const ptr<i32>> [storage=static] [linkage=external];
// VALID-NEXT:     global %1 outer: ptr<ptr<i32>> [storage=static] [const] = null<ptr<ptr<i32>>> [linkage=external];
// VALID-NEXT:     global %2 mixed: ptr<volatile ptr<i32>> [storage=static] [restrict] [linkage=external];
// VALID-NEXT:     fn %3 @assign(%4 source: ptr<ptr<i32>>) -> void [linkage=external] [fallthrough=ret_void] {
// VALID-NEXT:         write<ptr<const ptr<i32>>>(%0, pointer_cast<ptr<const ptr<i32>>, reason=assign>(read<ptr<ptr<i32>>>(%4)));
// VALID-NEXT:         write<i32>(deref(read<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%1)))), const<i32>(1));
// VALID-NEXT:         write<ptr<i32>, volatile>(deref(read<ptr<volatile ptr<i32>>>(%2)), null<ptr<i32>>);
// VALID-NEXT:     }
// VALID-NEXT: }
// SLATE-FILECHECK-END VALID
