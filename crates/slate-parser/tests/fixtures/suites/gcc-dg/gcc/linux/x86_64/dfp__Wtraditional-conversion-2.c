/* Test messages for -Wtraditional-conversion 
   (based on gcc.dg/Wtraditional-conversion-2.c).  */

/* { dg-do compile } */
/* { dg-options "-Wtraditional-conversion" } */

void fsi(signed int);
void fd32(_Decimal32);
void fd64(_Decimal64);
void fd128(_Decimal128);

struct s {
  void (*fsi)(signed int);
  void (*fd32)(_Decimal32);
  void (*fd64)(_Decimal64);
  void (*fd128)(_Decimal128);
} x;

signed int si;
unsigned int ui;
_Decimal32 d32;
_Decimal64 d64;
_Decimal128 d128;

void
g (void)
{
  fsi(d32); /* { dg-warning "passing argument 1 of 'fsi' as integer rather than floating due to prototype" } */
  x.fsi(d32); /* { dg-warning "passing argument 1 of 'x.fsi' as integer rather than floating due to prototype" } */
  fsi(d64); /* { dg-warning "passing argument 1 of 'fsi' as integer rather than floating due to prototype" } */
  x.fsi(d64); /* { dg-warning "passing argument 1 of 'x.fsi' as integer rather than floating due to prototype" } */
  fsi(d128); /* { dg-warning "passing argument 1 of 'fsi' as integer rather than floating due to prototype" } */
  x.fsi(d128); /* { dg-warning "passing argument 1 of 'x.fsi' as integer rather than floating due to prototype" } */
  fd32(si); /* { dg-warning "passing argument 1 of 'fd32' as floating rather than integer due to prototype" } */
  x.fd32(si); /* { dg-warning "passing argument 1 of 'x.fd32' as floating rather than integer due to prototype" } */  
  fd64(ui); /* { dg-warning "passing argument 1 of 'fd64' as floating rather than integer due to prototype" } */
  x.fd64(ui); /* { dg-warning "passing argument 1 of 'x.fd64' as floating rather than integer due to prototype" } */
  fd128(si); /* { dg-warning "passing argument 1 of 'fd128' as floating rather than integer due to prototype" } */
  x.fd128(ui); /* { dg-warning "passing argument 1 of 'x.fd128' as floating rather than integer due to prototype" } */  
  fd32(1.0); /* { dg-warning "passing argument 1 of 'fd32' as '_Decimal32' rather than 'double' due to prototype" } */
  x.fd32(1.0); /* { dg-warning "passing argument 1 of 'x.fd32' as '_Decimal32' rather than 'double' due to prototype" } */
  fd64(1.0); /* { dg-warning "passing argument 1 of 'fd64' as '_Decimal64' rather than 'double' due to prototype" } */
  x.fd64(1.0); /* { dg-warning "passing argument 1 of 'x.fd64' as '_Decimal64' rather than 'double' due to prototype" } */
  fd128(1.0); /* { dg-warning "passing argument 1 of 'fd128' as '_Decimal128' rather than 'double' due to prototype" } */
  x.fd128(1.0); /* { dg-warning "passing argument 1 of 'x.fd128' as '_Decimal128' rather than 'double' due to prototype" } */
}

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 fsi: ptr<fn(i32) -> void>;
// DEFAULT-NEXT:         field1 fd32: ptr<fn(d32) -> void>;
// DEFAULT-NEXT:         field2 fd64: ptr<fn(d64) -> void>;
// DEFAULT-NEXT:         field3 fd128: ptr<fn(d128) -> void>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: @type[[TYPE_s]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_si:[0-9]+]] si: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ui:[0-9]+]] ui: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d32:[0-9]+]] d32: d32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d64:[0-9]+]] d64: d64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d128:[0-9]+]] d128: d128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fsi:[0-9]+]] @fsi(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fd32:[0-9]+]] @fd32(%[[VALUE1:[0-9]+]] <unnamed>: d32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fd64:[0-9]+]] @fd64(%[[VALUE2:[0-9]+]] <unnamed>: d64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fd128:[0-9]+]] @fd128(%[[VALUE3:[0-9]+]] <unnamed>: d128) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_fsi]], float_to_int<i32, reason=arg, out_of_range=ub, exceptions=observable>(read<d32>(%[[VALUE_d32]])));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(read<ptr<fn(i32) -> void>>(field0(%[[VALUE_x]])), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=observable>(read<d32>(%[[VALUE_d32]])));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_fsi]], float_to_int<i32, reason=arg, out_of_range=ub, exceptions=observable>(read<d64>(%[[VALUE_d64]])));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(read<ptr<fn(i32) -> void>>(field0(%[[VALUE_x]])), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=observable>(read<d64>(%[[VALUE_d64]])));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_fsi]], float_to_int<i32, reason=arg, out_of_range=ub, exceptions=observable>(read<d128>(%[[VALUE_d128]])));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(read<ptr<fn(i32) -> void>>(field0(%[[VALUE_x]])), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=observable>(read<d128>(%[[VALUE_d128]])));
// DEFAULT-NEXT:         call<void, signature=fn(d32) -> void>(%[[VALUE_fd32]], int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(read<i32>(%[[VALUE_si]])));
// DEFAULT-NEXT:         call<void, signature=fn(d32) -> void>(read<ptr<fn(d32) -> void>>(field1(%[[VALUE_x]])), int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(read<i32>(%[[VALUE_si]])));
// DEFAULT-NEXT:         call<void, signature=fn(d64) -> void>(%[[VALUE_fd64]], int_to_float<d64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(read<u32>(%[[VALUE_ui]])));
// DEFAULT-NEXT:         call<void, signature=fn(d64) -> void>(read<ptr<fn(d64) -> void>>(field2(%[[VALUE_x]])), int_to_float<d64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(read<u32>(%[[VALUE_ui]])));
// DEFAULT-NEXT:         call<void, signature=fn(d128) -> void>(%[[VALUE_fd128]], int_to_float<d128, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(read<i32>(%[[VALUE_si]])));
// DEFAULT-NEXT:         call<void, signature=fn(d128) -> void>(read<ptr<fn(d128) -> void>>(field3(%[[VALUE_x]])), int_to_float<d128, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(read<u32>(%[[VALUE_ui]])));
// DEFAULT-NEXT:         call<void, signature=fn(d32) -> void>(%[[VALUE_fd32]], float_convert<d32, reason=arg, rounding=nearest_even, exceptions=observable>(const<f64>(1.0)));
// DEFAULT-NEXT:         call<void, signature=fn(d32) -> void>(read<ptr<fn(d32) -> void>>(field1(%[[VALUE_x]])), float_convert<d32, reason=arg, rounding=nearest_even, exceptions=observable>(const<f64>(1.0)));
// DEFAULT-NEXT:         call<void, signature=fn(d64) -> void>(%[[VALUE_fd64]], float_convert<d64, reason=arg, rounding=nearest_even, exceptions=observable>(const<f64>(1.0)));
// DEFAULT-NEXT:         call<void, signature=fn(d64) -> void>(read<ptr<fn(d64) -> void>>(field2(%[[VALUE_x]])), float_convert<d64, reason=arg, rounding=nearest_even, exceptions=observable>(const<f64>(1.0)));
// DEFAULT-NEXT:         call<void, signature=fn(d128) -> void>(%[[VALUE_fd128]], float_convert<d128, reason=arg, rounding=nearest_even, exceptions=observable>(const<f64>(1.0)));
// DEFAULT-NEXT:         call<void, signature=fn(d128) -> void>(read<ptr<fn(d128) -> void>>(field3(%[[VALUE_x]])), float_convert<d128, reason=arg, rounding=nearest_even, exceptions=observable>(const<f64>(1.0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
