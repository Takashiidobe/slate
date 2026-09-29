/* PR tree-optimization/80669 - Bad -Wstringop-overflow warnings for stpncpy
   { dg-do compile }
   { dg-options "-O2 -Wall -Wno-array-bounds -Wno-restrict -Wno-stringop-truncation" } */

#define SIZE_MAX __SIZE_MAX__

typedef __SIZE_TYPE__ size_t;

void sink (char*);

#define stpncpy (d, s, n)  sink (__builtin_stpncpy (d, s, n))

size_t value (void);

static size_t range (size_t min, size_t max)
{
  size_t val = value ();
  return val < min || max < val ? min : val;
}

/* Verify that no -Wstringop-overflow warning is issued for stpncpy
   with constant size.  (Some tests cause -Wstringop-truncation and
   that's expected).  */
void test_cst (char *d)
{
  __builtin_stpncpy (d, "123", 0);
  __builtin_stpncpy (d, "123", 1);
  __builtin_stpncpy (d, "123", 2);
  __builtin_stpncpy (d, "123", 3);
  __builtin_stpncpy (d, "123", 4);
  __builtin_stpncpy (d, "123", 5);
  __builtin_stpncpy (d, "123", 999);

  size_t n = SIZE_MAX / 2;

  __builtin_stpncpy (d, "123", n);

  __builtin_stpncpy (d, "123", n + 1);    /* { dg-warning "specified bound \[0-9\]+ exceeds maximum object size \[0-9\]+" } */
}


/* Verify that no -Wstringop-overflow warning is issued for stpncpy
   with size in some range.  */
void test_rng (char *d)
{
#define R(min, max) range (min, max)

  __builtin_stpncpy (d, "123", R (0, 1));
  __builtin_stpncpy (d, "123", R (0, 2));
  __builtin_stpncpy (d, "123", R (0, 3));
  __builtin_stpncpy (d, "123", R (0, 4));
  __builtin_stpncpy (d, "123", R (0, 5));

  __builtin_stpncpy (d, "123", R (1, 2));
  __builtin_stpncpy (d, "123", R (1, 3));
  __builtin_stpncpy (d, "123", R (1, 4));
  __builtin_stpncpy (d, "123", R (1, 5));

  __builtin_stpncpy (d, "123", R (2, 3));
  __builtin_stpncpy (d, "123", R (2, 4));
  __builtin_stpncpy (d, "123", R (2, 5));

  __builtin_stpncpy (d, "123", R (3, 4));
  __builtin_stpncpy (d, "123", R (3, 5));

  __builtin_stpncpy (d, "123", R (4, 5));

  __builtin_stpncpy (d, "123", R (5, 6));

  __builtin_stpncpy (d, "123", R (12345, 23456));

  size_t n = SIZE_MAX / 2;

  __builtin_stpncpy (d, "123", R (n - 1, n + 1));

  __builtin_stpncpy (d, "123", R (n + 1, n + 2));   /* { dg-warning "specified bound between \[0-9\]+ and \[0-9\]+ exceeds maximum object size \[0-9\]+" } */
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_15:[0-9]+]] .str[[VALUE_str_15]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_16:[0-9]+]] .str[[VALUE_str_16]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_17:[0-9]+]] .str[[VALUE_str_17]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_18:[0-9]+]] .str[[VALUE_str_18]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_19:[0-9]+]] .str[[VALUE_str_19]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_20:[0-9]+]] .str[[VALUE_str_20]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_21:[0-9]+]] .str[[VALUE_str_21]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_22:[0-9]+]] .str[[VALUE_str_22]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_23:[0-9]+]] .str[[VALUE_str_23]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_24:[0-9]+]] .str[[VALUE_str_24]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_25:[0-9]+]] .str[[VALUE_str_25]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_26:[0-9]+]] .str[[VALUE_str_26]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_27:[0-9]+]] .str[[VALUE_str_27]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_28:[0-9]+]] .str[[VALUE_str_28]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_sink:[0-9]+]] @sink(%[[VALUE0:[0-9]+]] <unnamed>: ptr<i8>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_value:[0-9]+]] @value() -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_range:[0-9]+]] @range(%[[VALUE_min:[0-9]+]] min: u64, %[[VALUE_max:[0-9]+]] max: u64) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_val:[0-9]+]] val: u64 [storage=automatic] = call<u64, signature=fn() -> u64>(%[[VALUE_value]]);
// DEFAULT-NEXT:         return conditional<u64>(logical_or<bool>(lt<u64>(read<u64>(%[[VALUE_val]]), read<u64>(%[[VALUE_min]])), lt<u64>(read<u64>(%[[VALUE_max]]), read<u64>(%[[VALUE_val]]))), read<u64>(%[[VALUE_min]]), read<u64>(%[[VALUE_val]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_stpncpy:[0-9]+]] @__builtin_stpncpy(%[[VALUE1:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE3:[0-9]+]] <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_cst:[0-9]+]] @test_cst(%[[VALUE_d:[0-9]+]] d: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_3]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_4]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_5]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_6]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_7]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(999))));
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: u64 [storage=automatic] = div<u64, by_zero=ub>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_8]])), read<u64>(%[[VALUE_n]]));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_9]])), add<u64, overflow=wrap>(read<u64>(%[[VALUE_n]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_rng:[0-9]+]] @test_rng(%[[VALUE_d_2:[0-9]+]] d: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_10]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1)))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_11]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_12]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_13]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_14]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_15]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_16]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_17]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_18]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_19]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_20]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_21]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_22]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_23]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_24]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5)))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_25]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_26]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12345))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(23456)))));
// DEFAULT-NEXT:         let %[[VALUE_n_2:[0-9]+]] n: u64 [storage=automatic] = div<u64, by_zero=ub>(const<u64>(18446744073709551615), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_27]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], sub<u64, overflow=wrap>(read<u64>(%[[VALUE_n_2]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), add<u64, overflow=wrap>(read<u64>(%[[VALUE_n_2]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_stpncpy]], read<ptr<i8>>(%[[VALUE_d_2]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_28]])), call<u64, signature=fn(u64, u64) -> u64>(%[[VALUE_range]], add<u64, overflow=wrap>(read<u64>(%[[VALUE_n_2]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), add<u64, overflow=wrap>(read<u64>(%[[VALUE_n_2]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
