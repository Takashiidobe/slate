extern void abort(void);

int stab_xcoff_builtin_type(int typenum) {
  const char *name;
  if (typenum >= 0 || typenum < -34) {
    return 0;
  }
  switch (-typenum) {
  case 1:
    name = "int";
    break;
  case 2:
    name = "char";
  case 3:
    name = "short";
    break;
  case 4:
    name = "long";
  case 5:
    name = "unsigned char";
  case 6:
    name = "signed char";
  case 7:
    name = "unsigned short";
  case 8:
    name = "unsigned int";
  case 9:
    name = "unsigned";
  case 10:
    name = "unsigned long";
  case 11:
    name = "void";
  case 12:
    name = "float";
  case 13:
    name = "double";
  case 14:
    name = "long double";
  case 15:
    name = "integer";
  case 16:
    name = "boolean";
  case 17:
    name = "short real";
  case 18:
    name = "real";
  case 19:
    name = "stringptr";
  case 20:
    name = "character";
  case 21:
    name = "logical*1";
  case 22:
    name = "logical*2";
  case 23:
    name = "logical*4";
  case 24:
    name = "logical";
  case 25:
    name = "complex";
  case 26:
    name = "double complex";
  case 27:
    name = "integer*1";
  case 28:
    name = "integer*2";
  case 29:
    name = "integer*4";
  case 30:
    name = "wchar";
  case 31:
    name = "long long";
  case 32:
    name = "unsigned long long";
  case 33:
    name = "logical*8";
  case 34:
    name = "integer*8";
  }
  return name[0];
}

int main() {
  int i;
  if (stab_xcoff_builtin_type(0) != 0)
    abort();
  if (stab_xcoff_builtin_type(-1) != 'i')
    abort();
  if (stab_xcoff_builtin_type(-2) != 's')
    abort();
  if (stab_xcoff_builtin_type(-3) != 's')
    abort();
  for (i = -4; i >= -34; --i)
    if (stab_xcoff_builtin_type(i) != 'i')
      abort();
  if (stab_xcoff_builtin_type(-35) != 0)
    abort();
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 104, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 104, 111, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 99, 104, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([115, 105, 103, 110, 101, 100, 32, 99, 104, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 115, 104, 111, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([117, 110, 115, 105, 103, 110, 101, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([118, 111, 105, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([102, 108, 111, 97, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([100, 111, 117, 98, 108, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([108, 111, 110, 103, 32, 100, 111, 117, 98, 108, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_15:[0-9]+]] .str[[VALUE_str_15]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([105, 110, 116, 101, 103, 101, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_16:[0-9]+]] .str[[VALUE_str_16]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([98, 111, 111, 108, 101, 97, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_17:[0-9]+]] .str[[VALUE_str_17]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([115, 104, 111, 114, 116, 32, 114, 101, 97, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_18:[0-9]+]] .str[[VALUE_str_18]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([114, 101, 97, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_19:[0-9]+]] .str[[VALUE_str_19]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([115, 116, 114, 105, 110, 103, 112, 116, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_20:[0-9]+]] .str[[VALUE_str_20]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([99, 104, 97, 114, 97, 99, 116, 101, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_21:[0-9]+]] .str[[VALUE_str_21]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([108, 111, 103, 105, 99, 97, 108, 42, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_22:[0-9]+]] .str[[VALUE_str_22]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([108, 111, 103, 105, 99, 97, 108, 42, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_23:[0-9]+]] .str[[VALUE_str_23]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([108, 111, 103, 105, 99, 97, 108, 42, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_24:[0-9]+]] .str[[VALUE_str_24]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([108, 111, 103, 105, 99, 97, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_25:[0-9]+]] .str[[VALUE_str_25]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([99, 111, 109, 112, 108, 101, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_26:[0-9]+]] .str[[VALUE_str_26]]: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([100, 111, 117, 98, 108, 101, 32, 99, 111, 109, 112, 108, 101, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_27:[0-9]+]] .str[[VALUE_str_27]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([105, 110, 116, 101, 103, 101, 114, 42, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_28:[0-9]+]] .str[[VALUE_str_28]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([105, 110, 116, 101, 103, 101, 114, 42, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_29:[0-9]+]] .str[[VALUE_str_29]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([105, 110, 116, 101, 103, 101, 114, 42, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_30:[0-9]+]] .str[[VALUE_str_30]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([119, 99, 104, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_31:[0-9]+]] .str[[VALUE_str_31]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([108, 111, 110, 103, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_32:[0-9]+]] .str[[VALUE_str_32]]: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([117, 110, 115, 105, 103, 110, 101, 100, 32, 108, 111, 110, 103, 32, 108, 111, 110, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_33:[0-9]+]] .str[[VALUE_str_33]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([108, 111, 103, 105, 99, 97, 108, 42, 56, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_34:[0-9]+]] .str[[VALUE_str_34]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([105, 110, 116, 101, 103, 101, 114, 42, 56, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_stab_xcoff_builtin_type:[0-9]+]] @stab_xcoff_builtin_type(%[[VALUE_typenum:[0-9]+]] typenum: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_name:[0-9]+]] name: ptr<const i8> [storage=automatic];
// DEFAULT-NEXT:         if logical_or<bool>(ge<i32>(read<i32>(%[[VALUE_typenum]]), const<i32>(0)), lt<i32>(read<i32>(%[[VALUE_typenum]]), neg<i32, overflow=ub>(const<i32>(34))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         switch %[[VALUE0:[0-9]+]] neg<i32, overflow=ub>(read<i32>(%[[VALUE_typenum]]))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(1):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(2):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_2]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(3):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_3]])));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(4):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_4]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(5):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(14)>(%[[VALUE_str_5]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(6):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_str_6]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(7):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(15)>(%[[VALUE_str_7]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(8):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str_8]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(9):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_9]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(10):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(14)>(%[[VALUE_str_10]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(11):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_11]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(12):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_12]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(13):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_13]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(14):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_str_14]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(15):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str_15]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(16):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str_16]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(17):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_17]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(18):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_18]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(19):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_19]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(20):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_20]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(21):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_21]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(22):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_22]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(23):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_23]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(24):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str_24]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(25):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str_25]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(26):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(15)>(%[[VALUE_str_26]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(27):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_27]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(28):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_28]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(29):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_29]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(30):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_30]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(31):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_31]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(32):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(19)>(%[[VALUE_str_32]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(33):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_33]])));
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(34):
// DEFAULT-NEXT:                     write<ptr<const i8>>(%[[VALUE_name]], pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_34]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_name]]), const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_stab_xcoff_builtin_type]], const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_stab_xcoff_builtin_type]], neg<i32, overflow=ub>(const<i32>(1))), const<i32>(105))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_stab_xcoff_builtin_type]], neg<i32, overflow=ub>(const<i32>(2))), const<i32>(115))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_stab_xcoff_builtin_type]], neg<i32, overflow=ub>(const<i32>(3))), const<i32>(115))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], neg<i32, overflow=ub>(const<i32>(4)));
// DEFAULT-NEXT:             condition: ge<i32>(read<i32>(%[[VALUE_i]]), neg<i32, overflow=ub>(const<i32>(34)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_stab_xcoff_builtin_type]], read<i32>(%[[VALUE_i]])), const<i32>(105))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_stab_xcoff_builtin_type]], neg<i32, overflow=ub>(const<i32>(35))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
