/* PR c/102989 */
/* { dg-do compile { target { bitint } } } */
/* { dg-options "-std=c23 -fpermissive -Wint-in-bool-context -Waddress -Wpointer-to-int-cast -Wint-to-pointer-cast -Wint-conversion -Wshift-negative-value -Wshift-count-overflow -Wdiv-by-zero" } */

extern char *ax[];

void bar (int);

void
foo (_BitInt(61) x, long long *y)
{
  if (x << 15)						/* { dg-warning "'<<' in boolean context, did you mean '<'" } */
    ;
  bar ((_BitInt(sizeof (void *) * __CHAR_BIT__)) (ax + 0) == 0);
  bar ((unsigned _BitInt(sizeof (void *) * __CHAR_BIT__)) (ax + 1) == 0);
  char *z = __builtin_assume_aligned (y, 32wb, 8wb);
  *z = 42;
  _BitInt(sizeof (void *) * __CHAR_BIT__ + 1) a = (_BitInt(sizeof (void *) * __CHAR_BIT__ + 1)) y;			/* { dg-warning "cast from pointer to integer of different size" } */
  unsigned _BitInt(sizeof (void *) * __CHAR_BIT__ + 1) b = (unsigned _BitInt(sizeof (void *) * __CHAR_BIT__ + 1)) y;	/* { dg-warning "cast from pointer to integer of different size" } */
  _BitInt(sizeof (void *) * __CHAR_BIT__ - 1) c = (_BitInt(sizeof (void *) * __CHAR_BIT__ - 1)) y;			/* { dg-warning "cast from pointer to integer of different size" } */
  unsigned _BitInt(sizeof (void *) * __CHAR_BIT__ - 1) d = (unsigned _BitInt(sizeof (void *) * __CHAR_BIT__ - 1)) y;	/* { dg-warning "cast from pointer to integer of different size" } */
  void *e = (void *) a;					/* { dg-warning "cast to pointer from integer of different size" } */
  void *f = (void *) b;					/* { dg-warning "cast to pointer from integer of different size" } */
  void *g = (void *) c;					/* { dg-warning "cast to pointer from integer of different size" } */
  void *h = (void *) d;					/* { dg-warning "cast to pointer from integer of different size" } */
  a = y;						/* { dg-warning "assignment to '_BitInt\\\(\[0-9]+\\\)' from 'long long int \\\*' makes integer from pointer without a cast" } */
  b = y;						/* { dg-warning "assignment to 'unsigned _BitInt\\\(\[0-9]+\\\)' from 'long long int \\\*' makes integer from pointer without a cast" } */
  c = y;						/* { dg-warning "assignment to '_BitInt\\\(\[0-9]+\\\)' from 'long long int \\\*' makes integer from pointer without a cast" } */
  d = y;						/* { dg-warning "assignment to 'unsigned _BitInt\\\(\[0-9]+\\\)' from 'long long int \\\*' makes integer from pointer without a cast" } */
  e = a;						/* { dg-warning "assignment to 'void \\\*' from '_BitInt\\\(\[0-9]+\\\)' makes pointer from integer without a cast" } */
  f = b;						/* { dg-warning "assignment to 'void \\\*' from 'unsigned _BitInt\\\(\[0-9]+\\\)' makes pointer from integer without a cast" } */
  g = c;						/* { dg-warning "assignment to 'void \\\*' from '_BitInt\\\(\[0-9]+\\\)' makes pointer from integer without a cast" } */
  h = d;						/* { dg-warning "assignment to 'void \\\*' from 'unsigned _BitInt\\\(\[0-9]+\\\)' makes pointer from integer without a cast" } */
  _BitInt(61) i = (1wb - 1152921504606846975wb) << 1;	/* { dg-warning "left shift of negative value" } */
							/* { dg-warning "result of '-1152921504606846974 << 1' requires 62 bits to represent, but '_BitInt\\\(61\\\)' only has 61 bits" "" { target *-*-* } .-1 } */
  _BitInt(61) j = i << (60 + 1);			/* { dg-warning "left shift count >= width of type" } */
  _BitInt(61) k = i >> (60 + 1);			/* { dg-warning "right shift count >= width of type" } */
  _BitInt(61) l = i / (51wb - 51wb);			/* { dg-warning "division by zero" } */
}

// SLATE-FILECHECK-ARGS -fpermissive
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
// DEFAULT-NEXT:     extern %[[VALUE_ax:[0-9]+]] ax: array<ptr<i8>, incomplete> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_assume_aligned:[0-9]+]] @__builtin_assume_aligned(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: u64, ...) -> ptr<void> [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i61b, %[[VALUE_y:[0-9]+]] y: ptr<i64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i61b>(shl<i61b, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i61b>(%[[VALUE_x]]), const<i32>(15)), const<i61b>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], from_bool<i32, reason=arg>(eq<i64b>(ptr_to_int<i64b, reason=explicit>(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(array_decay<ptr<ptr<i8>>, length=None>(%[[VALUE_ax]]), const<i32>(0))), widen<i64b, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], from_bool<i32, reason=arg>(eq<u64b>(ptr_to_int<u64b, reason=explicit>(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(array_decay<ptr<ptr<i8>>, length=None>(%[[VALUE_ax]]), const<i32>(1))), reinterpret<u64b, reason=usual_arith, fits=unknown>(widen<i64b, reason=usual_arith>(const<i32>(0))))));
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(ptr<const void>, u64, ...) -> ptr<void>>(%[[VALUE___builtin_assume_aligned]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<i64>>(%[[VALUE_y]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i7b>(32))), const<i5b>(8)));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%[[VALUE_z]])), truncate<i8, reason=assign, fits=always>(const<i32>(42)));
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i65b [storage=automatic] = ptr_to_int<i65b, reason=explicit>(read<ptr<i64>>(%[[VALUE_y]]));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: u65b [storage=automatic] = ptr_to_int<u65b, reason=explicit>(read<ptr<i64>>(%[[VALUE_y]]));
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i63b [storage=automatic] = ptr_to_int<i63b, reason=explicit>(read<ptr<i64>>(%[[VALUE_y]]));
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: u63b [storage=automatic] = ptr_to_int<u63b, reason=explicit>(read<ptr<i64>>(%[[VALUE_y]]));
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: ptr<void> [storage=automatic] = int_to_ptr<ptr<void>, reason=explicit>(read<i65b>(%[[VALUE_a]]));
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: ptr<void> [storage=automatic] = int_to_ptr<ptr<void>, reason=explicit>(read<u65b>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE_g:[0-9]+]] g: ptr<void> [storage=automatic] = int_to_ptr<ptr<void>, reason=explicit>(read<i63b>(%[[VALUE_c]]));
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: ptr<void> [storage=automatic] = int_to_ptr<ptr<void>, reason=explicit>(read<u63b>(%[[VALUE_d]]));
// DEFAULT-NEXT:         write<i65b>(%[[VALUE_a]], ptr_to_int<i65b, reason=assign>(read<ptr<i64>>(%[[VALUE_y]])));
// DEFAULT-NEXT:         write<u65b>(%[[VALUE_b]], ptr_to_int<u65b, reason=assign>(read<ptr<i64>>(%[[VALUE_y]])));
// DEFAULT-NEXT:         write<i63b>(%[[VALUE_c]], ptr_to_int<i63b, reason=assign>(read<ptr<i64>>(%[[VALUE_y]])));
// DEFAULT-NEXT:         write<u63b>(%[[VALUE_d]], ptr_to_int<u63b, reason=assign>(read<ptr<i64>>(%[[VALUE_y]])));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_e]], int_to_ptr<ptr<void>, reason=assign>(read<i65b>(%[[VALUE_a]])));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_f]], int_to_ptr<ptr<void>, reason=assign>(read<u65b>(%[[VALUE_b]])));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_g]], int_to_ptr<ptr<void>, reason=assign>(read<i63b>(%[[VALUE_c]])));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_h]], int_to_ptr<ptr<void>, reason=assign>(read<u63b>(%[[VALUE_d]])));
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i61b [storage=automatic] = shl<i61b, overflow=ub, amount_out_of_range=ub, negative_left=ub>(sub<i61b, overflow=ub>(widen<i61b, reason=usual_arith>(const<i2b>(1)), const<i61b>(1152921504606846975)), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i61b [storage=automatic] = shl<i61b, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i61b>(%[[VALUE_i]]), add<i32, overflow=ub>(const<i32>(60), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: i61b [storage=automatic] = shr<i61b, amount_out_of_range=ub, fill=sign_extend>(read<i61b>(%[[VALUE_i]]), add<i32, overflow=ub>(const<i32>(60), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_l:[0-9]+]] l: i61b [storage=automatic] = div<i61b, by_zero=ub, min_by_neg_one=ub>(read<i61b>(%[[VALUE_i]]), widen<i61b, reason=usual_arith>(sub<i7b, overflow=ub>(const<i7b>(51), const<i7b>(51))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
