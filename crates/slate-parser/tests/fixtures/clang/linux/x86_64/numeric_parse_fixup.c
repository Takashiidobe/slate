#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char  whole[]          = "42";
  char  whole_long[]     = "-12345";
  char  whole_unsigned[] = "77";
  char  leading[]        = "  -17tail";
  char  empty[]          = "";
  char  large[]          = "999999999999999999999999999999";
  char  flt[]            = "  -3.5e2rest";
  char  end_source[]     = "12tail";
  char *end              = 0;

  printf("%d %ld %lu %ld %ld %lu %.1f\n", atoi(whole),
         strtol(whole_long, 0, 10), strtoul(whole_unsigned, 0, 10),
         atol(leading), strtol(large, 0, 10), strtoul(empty, 0, 10),
         strtod(flt, 0));

  long raw = strtol(end_source, &end, 10);
  printf("%ld %c\n", raw, *end);
  return 0;
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 29> [storage=static] = code_units<array<i8, 29>>([37, 100, 32, 37, 108, 100, 32, 37, 108, 117, 32, 37, 108, 100, 32, 37, 108, 100, 32, 37, 108, 117, 32, 37, 46, 49, 102, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([37, 108, 100, 32, 37, 99, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atoi:[0-9]+]] @atoi(%[[VALUE___nptr:[0-9]+]] __nptr: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_atol:[0-9]+]] @atol(%[[VALUE___nptr_2:[0-9]+]] __nptr: ptr<const i8>) -> i64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_strtod:[0-9]+]] @strtod(%[[VALUE___nptr_3:[0-9]+]] __nptr: ptr<const i8> [restrict], %[[VALUE___endptr:[0-9]+]] __endptr: ptr<ptr<i8>> [restrict]) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strtol:[0-9]+]] @strtol(%[[VALUE___nptr_4:[0-9]+]] __nptr: ptr<const i8> [restrict], %[[VALUE___endptr_2:[0-9]+]] __endptr: ptr<ptr<i8>> [restrict], %[[VALUE___base:[0-9]+]] __base: i32) -> i64 [linkage=external] [asm_name="__isoc23_strtol"];
// DEFAULT-NEXT:     fn %[[VALUE_strtoul:[0-9]+]] @strtoul(%[[VALUE___nptr_5:[0-9]+]] __nptr: ptr<const i8> [restrict], %[[VALUE___endptr_3:[0-9]+]] __endptr: ptr<ptr<i8>> [restrict], %[[VALUE___base_2:[0-9]+]] __base: i32) -> u64 [linkage=external] [asm_name="__isoc23_strtoul"];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_whole:[0-9]+]] whole: array<i8, 3> [storage=automatic] = code_units<array<i8, 3>>([52, 50, 0]);
// DEFAULT-NEXT:         let %[[VALUE_whole_long:[0-9]+]] whole_long: array<i8, 7> [storage=automatic] = code_units<array<i8, 7>>([45, 49, 50, 51, 52, 53, 0]);
// DEFAULT-NEXT:         let %[[VALUE_whole_unsigned:[0-9]+]] whole_unsigned: array<i8, 3> [storage=automatic] = code_units<array<i8, 3>>([55, 55, 0]);
// DEFAULT-NEXT:         let %[[VALUE_leading:[0-9]+]] leading: array<i8, 10> [storage=automatic] = code_units<array<i8, 10>>([32, 32, 45, 49, 55, 116, 97, 105, 108, 0]);
// DEFAULT-NEXT:         let %[[VALUE_empty:[0-9]+]] empty: array<i8, 1> [storage=automatic] = code_units<array<i8, 1>>([0]);
// DEFAULT-NEXT:         let %[[VALUE_large:[0-9]+]] large: array<i8, 31> [storage=automatic] [align=16] = code_units<array<i8, 31>>([57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 0]);
// DEFAULT-NEXT:         let %[[VALUE_flt:[0-9]+]] flt: array<i8, 13> [storage=automatic] = code_units<array<i8, 13>>([32, 32, 45, 51, 46, 53, 101, 50, 114, 101, 115, 116, 0]);
// DEFAULT-NEXT:         let %[[VALUE_end_source:[0-9]+]] end_source: array<i8, 7> [storage=automatic] = code_units<array<i8, 7>>([49, 50, 116, 97, 105, 108, 0]);
// DEFAULT-NEXT:         let %[[VALUE_end:[0-9]+]] end: ptr<i8> [storage=automatic] = null<ptr<i8>>;
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>,
// DEFAULT-SAME: length=Some(29)>(%[[VALUE_str]])), call<i32, signature=fn(ptr<const i8>) ->
// DEFAULT-SAME: i32>(%[[VALUE_atoi]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>,
// DEFAULT-SAME: length=Some(3)>(%[[VALUE_whole]]))), call<i64, signature=fn(ptr<const i8>, ptr<ptr<i8>>, i32) ->
// DEFAULT-SAME: i64>(%[[VALUE_strtol]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>,
// DEFAULT-SAME: length=Some(7)>(%[[VALUE_whole_long]])), null<ptr<ptr<i8>>>, const<i32>(10)), call<u64, signature=fn(ptr<const i8>, ptr<ptr<i8>>, i32) ->
// DEFAULT-SAME: u64>(%[[VALUE_strtoul]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>,
// DEFAULT-SAME: length=Some(3)>(%[[VALUE_whole_unsigned]])), null<ptr<ptr<i8>>>, const<i32>(10)), call<i64, signature=fn(ptr<const i8>) ->
// DEFAULT-SAME: i64>(%[[VALUE_atol]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>,
// DEFAULT-SAME: length=Some(10)>(%[[VALUE_leading]]))), call<i64, signature=fn(ptr<const i8>, ptr<ptr<i8>>, i32) ->
// DEFAULT-SAME: i64>(%[[VALUE_strtol]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>,
// DEFAULT-SAME: length=Some(31)>(%[[VALUE_large]])), null<ptr<ptr<i8>>>, const<i32>(10)), call<u64, signature=fn(ptr<const i8>, ptr<ptr<i8>>, i32) ->
// DEFAULT-SAME: u64>(%[[VALUE_strtoul]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>,
// DEFAULT-SAME: length=Some(1)>(%[[VALUE_empty]])), null<ptr<ptr<i8>>>, const<i32>(10)), call<f64, signature=fn(ptr<const i8>, ptr<ptr<i8>>) ->
// DEFAULT-SAME: f64>(%[[VALUE_strtod]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>,
// DEFAULT-SAME: length=Some(13)>(%[[VALUE_flt]])), null<ptr<ptr<i8>>>));
// DEFAULT-NEXT:         let %[[VALUE_raw:[0-9]+]] raw: i64 [storage=automatic] = call<i64, signature=fn(ptr<const i8>, ptr<ptr<i8>>, i32) -> i64>(%[[VALUE_strtol]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_end_source]])), addr_of<ptr<ptr<i8>>>(%[[VALUE_end]]), const<i32>(10));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str_2]])), read<i64>(%[[VALUE_raw]]), widen<i32, reason=vararg>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_end]])))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
