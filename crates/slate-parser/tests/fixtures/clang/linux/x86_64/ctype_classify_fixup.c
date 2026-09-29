#include <ctype.h>
#include <stdio.h>

int main(void) {
  char alpha = 'A';
  char digit = '5';
  char space = ' ';
  char vtab  = '\v';
  char punct = '!';

  if (isalpha(alpha)) {
    printf("alpha-yes\n");
  } else {
    printf("alpha-no\n");
  }
  if (!isalpha(digit)) {
    printf("not-alpha-yes\n");
  } else {
    printf("not-alpha-no\n");
  }
  if (isdigit(digit)) {
    printf("digit-yes\n");
  } else {
    printf("digit-no\n");
  }
  if (isupper(alpha)) {
    printf("upper-yes\n");
  } else {
    printf("upper-no\n");
  }
  if (islower(alpha)) {
    printf("lower-yes\n");
  } else {
    printf("lower-no\n");
  }
  if (isalnum(punct)) {
    printf("alnum-yes\n");
  } else {
    printf("alnum-no\n");
  }
  if (isxdigit(alpha)) {
    printf("xdigit-yes\n");
  } else {
    printf("xdigit-no\n");
  }
  if (ispunct(punct)) {
    printf("punct-yes\n");
  } else {
    printf("punct-no\n");
  }
  if (iscntrl(vtab)) {
    printf("cntrl-yes\n");
  } else {
    printf("cntrl-no\n");
  }
  if (isgraph(punct)) {
    printf("graph-yes\n");
  } else {
    printf("graph-no\n");
  }
  if (isprint(space)) {
    printf("print-yes\n");
  } else {
    printf("print-no\n");
  }
  if (isspace(space)) {
    printf("space-yes\n");
  } else {
    printf("space-no\n");
  }
  if (isspace(vtab)) {
    printf("vtab-space-yes\n");
  } else {
    printf("vtab-space-no\n");
  }

  printf("%d\n", isalpha(alpha));

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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE__ISupper:[0-9]+]] _ISupper = const<i32>(256);
// DEFAULT-NEXT:         %[[VALUE__ISlower:[0-9]+]] _ISlower = const<i32>(512);
// DEFAULT-NEXT:         %[[VALUE__ISalpha:[0-9]+]] _ISalpha = const<i32>(1024);
// DEFAULT-NEXT:         %[[VALUE__ISdigit:[0-9]+]] _ISdigit = const<i32>(2048);
// DEFAULT-NEXT:         %[[VALUE__ISxdigit:[0-9]+]] _ISxdigit = const<i32>(4096);
// DEFAULT-NEXT:         %[[VALUE__ISspace:[0-9]+]] _ISspace = const<i32>(8192);
// DEFAULT-NEXT:         %[[VALUE__ISprint:[0-9]+]] _ISprint = const<i32>(16384);
// DEFAULT-NEXT:         %[[VALUE__ISgraph:[0-9]+]] _ISgraph = const<i32>(32768);
// DEFAULT-NEXT:         %[[VALUE__ISblank:[0-9]+]] _ISblank = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE__IScntrl:[0-9]+]] _IScntrl = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE__ISpunct:[0-9]+]] _ISpunct = const<i32>(4);
// DEFAULT-NEXT:         %[[VALUE__ISalnum:[0-9]+]] _ISalnum = const<i32>(8);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([97, 108, 112, 104, 97, 45, 121, 101, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([97, 108, 112, 104, 97, 45, 110, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([110, 111, 116, 45, 97, 108, 112, 104, 97, 45, 121, 101, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([110, 111, 116, 45, 97, 108, 112, 104, 97, 45, 110, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([100, 105, 103, 105, 116, 45, 121, 101, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([100, 105, 103, 105, 116, 45, 110, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([117, 112, 112, 101, 114, 45, 121, 101, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([117, 112, 112, 101, 114, 45, 110, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([108, 111, 119, 101, 114, 45, 121, 101, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([108, 111, 119, 101, 114, 45, 110, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([97, 108, 110, 117, 109, 45, 121, 101, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([97, 108, 110, 117, 109, 45, 110, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([120, 100, 105, 103, 105, 116, 45, 121, 101, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([120, 100, 105, 103, 105, 116, 45, 110, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_15:[0-9]+]] .str[[VALUE_str_15]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([112, 117, 110, 99, 116, 45, 121, 101, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_16:[0-9]+]] .str[[VALUE_str_16]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([112, 117, 110, 99, 116, 45, 110, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_17:[0-9]+]] .str[[VALUE_str_17]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([99, 110, 116, 114, 108, 45, 121, 101, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_18:[0-9]+]] .str[[VALUE_str_18]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([99, 110, 116, 114, 108, 45, 110, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_19:[0-9]+]] .str[[VALUE_str_19]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([103, 114, 97, 112, 104, 45, 121, 101, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_20:[0-9]+]] .str[[VALUE_str_20]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([103, 114, 97, 112, 104, 45, 110, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_21:[0-9]+]] .str[[VALUE_str_21]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([112, 114, 105, 110, 116, 45, 121, 101, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_22:[0-9]+]] .str[[VALUE_str_22]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([112, 114, 105, 110, 116, 45, 110, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_23:[0-9]+]] .str[[VALUE_str_23]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([115, 112, 97, 99, 101, 45, 121, 101, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_24:[0-9]+]] .str[[VALUE_str_24]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([115, 112, 97, 99, 101, 45, 110, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_25:[0-9]+]] .str[[VALUE_str_25]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([118, 116, 97, 98, 45, 115, 112, 97, 99, 101, 45, 121, 101, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_26:[0-9]+]] .str[[VALUE_str_26]]: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([118, 116, 97, 98, 45, 115, 112, 97, 99, 101, 45, 110, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_27:[0-9]+]] .str[[VALUE_str_27]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE___ctype_b_loc:[0-9]+]] @__ctype_b_loc() -> ptr<ptr<const u16>> [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_alpha:[0-9]+]] alpha: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(65));
// DEFAULT-NEXT:         let %[[VALUE_digit:[0-9]+]] digit: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(53));
// DEFAULT-NEXT:         let %[[VALUE_space:[0-9]+]] space: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(32));
// DEFAULT-NEXT:         let %[[VALUE_vtab:[0-9]+]] vtab: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(11));
// DEFAULT-NEXT:         let %[[VALUE_punct:[0-9]+]] punct: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(33));
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(deref(call<ptr<ptr<const u16>>, signature=fn() -> ptr<ptr<const u16>>>(%[[VALUE___ctype_b_loc]]))), widen<i32, reason=explicit>(read<i8>(%[[VALUE_alpha]]))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1024)))))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_2]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if not<bool>(ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(deref(call<ptr<ptr<const u16>>, signature=fn() -> ptr<ptr<const u16>>>(%[[VALUE___ctype_b_loc]]))), widen<i32, reason=explicit>(read<i8>(%[[VALUE_digit]]))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1024)))))), const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(15)>(%[[VALUE_str_3]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%[[VALUE_str_4]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(deref(call<ptr<ptr<const u16>>, signature=fn() -> ptr<ptr<const u16>>>(%[[VALUE___ctype_b_loc]]))), widen<i32, reason=explicit>(read<i8>(%[[VALUE_digit]]))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2048)))))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_5]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_6]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(deref(call<ptr<ptr<const u16>>, signature=fn() -> ptr<ptr<const u16>>>(%[[VALUE___ctype_b_loc]]))), widen<i32, reason=explicit>(read<i8>(%[[VALUE_alpha]]))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(256)))))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_7]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_8]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(deref(call<ptr<ptr<const u16>>, signature=fn() -> ptr<ptr<const u16>>>(%[[VALUE___ctype_b_loc]]))), widen<i32, reason=explicit>(read<i8>(%[[VALUE_alpha]]))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(512)))))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_9]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_10]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(deref(call<ptr<ptr<const u16>>, signature=fn() -> ptr<ptr<const u16>>>(%[[VALUE___ctype_b_loc]]))), widen<i32, reason=explicit>(read<i8>(%[[VALUE_punct]]))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(8)))))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_11]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_12]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(deref(call<ptr<ptr<const u16>>, signature=fn() -> ptr<ptr<const u16>>>(%[[VALUE___ctype_b_loc]]))), widen<i32, reason=explicit>(read<i8>(%[[VALUE_alpha]]))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(4096)))))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_str_13]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_14]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(deref(call<ptr<ptr<const u16>>, signature=fn() -> ptr<ptr<const u16>>>(%[[VALUE___ctype_b_loc]]))), widen<i32, reason=explicit>(read<i8>(%[[VALUE_punct]]))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(4)))))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_15]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_16]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(deref(call<ptr<ptr<const u16>>, signature=fn() -> ptr<ptr<const u16>>>(%[[VALUE___ctype_b_loc]]))), widen<i32, reason=explicit>(read<i8>(%[[VALUE_vtab]]))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(2)))))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_17]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_18]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(deref(call<ptr<ptr<const u16>>, signature=fn() -> ptr<ptr<const u16>>>(%[[VALUE___ctype_b_loc]]))), widen<i32, reason=explicit>(read<i8>(%[[VALUE_punct]]))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(32768)))))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_19]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_20]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(deref(call<ptr<ptr<const u16>>, signature=fn() -> ptr<ptr<const u16>>>(%[[VALUE___ctype_b_loc]]))), widen<i32, reason=explicit>(read<i8>(%[[VALUE_space]]))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(16384)))))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_21]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_22]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(deref(call<ptr<ptr<const u16>>, signature=fn() -> ptr<ptr<const u16>>>(%[[VALUE___ctype_b_loc]]))), widen<i32, reason=explicit>(read<i8>(%[[VALUE_space]]))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(8192)))))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_23]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_24]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(deref(call<ptr<ptr<const u16>>, signature=fn() -> ptr<ptr<const u16>>>(%[[VALUE___ctype_b_loc]]))), widen<i32, reason=explicit>(read<i8>(%[[VALUE_vtab]]))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(8192)))))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str_25]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(15)>(%[[VALUE_str_26]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_27]])), and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(deref(call<ptr<ptr<const u16>>, signature=fn() -> ptr<ptr<const u16>>>(%[[VALUE___ctype_b_loc]]))), widen<i32, reason=explicit>(read<i8>(%[[VALUE_alpha]]))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1024)))))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
