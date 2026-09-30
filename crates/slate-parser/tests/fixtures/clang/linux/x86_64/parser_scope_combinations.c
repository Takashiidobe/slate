typedef int T;

int object_sizeof(void) {
  int result = 0;
  {
    int T = 2;
    result += sizeof(T);
    {
      typedef int T;
      result += sizeof(T);
    }
    result += sizeof(T);
  }
  return result + sizeof(T);
}

int object_cast(void) {
  int result = 0;
  {
    int T = 2;
    result += (T) + 1;
    {
      typedef int T;
      result += (T) + 1;
    }
    result += (T) + 1;
  }
  return result + (T) + 1;
}

int object_generic(void) {
  int result = 0;
  {
    int T = 2;
    result += _Generic((T) + 1, int: 1, default: 0);
    {
      typedef int T;
      result += _Generic((T) + 1, int: 1, default: 0);
    }
    result += _Generic((T) + 1, int: 1, default: 0);
  }
  return result + _Generic((T) + 1, int: 1, default: 0);
}

int object_bound(void) {
  int result = 0;
  {
    int T = 2;
    result += sizeof(int[(T) + 1]);
    {
      typedef int T;
      result += sizeof(int[(T) + 1]);
    }
    result += sizeof(int[(T) + 1]);
  }
  return result + sizeof(int[(T) + 1]);
}

int enumerator_sizeof(void) {
  int result = 0;
  {
    enum { T = 2 };
    result += sizeof(T);
    {
      typedef int T;
      result += sizeof(T);
    }
    result += sizeof(T);
  }
  return result + sizeof(T);
}

int enumerator_cast(void) {
  int result = 0;
  {
    enum { T = 2 };
    result += (T) + 1;
    {
      typedef int T;
      result += (T) + 1;
    }
    result += (T) + 1;
  }
  return result + (T) + 1;
}

int enumerator_generic(void) {
  int result = 0;
  {
    enum { T = 2 };
    result += _Generic((T) + 1, int: 1, default: 0);
    {
      typedef int T;
      result += _Generic((T) + 1, int: 1, default: 0);
    }
    result += _Generic((T) + 1, int: 1, default: 0);
  }
  return result + _Generic((T) + 1, int: 1, default: 0);
}

int enumerator_bound(void) {
  int result = 0;
  {
    enum { T = 2 };
    result += sizeof(int[(T) + 1]);
    {
      typedef int T;
      result += sizeof(int[(T) + 1]);
    }
    result += sizeof(int[(T) + 1]);
  }
  return result + sizeof(int[(T) + 1]);
}

int typedef_sizeof(void) {
  int result = 0;
  {
    typedef long T;
    result += sizeof(T);
    {
      typedef int T;
      result += sizeof(T);
    }
    result += sizeof(T);
  }
  return result + sizeof(T);
}

int typedef_cast(void) {
  int result = 0;
  {
    typedef long T;
    result += (T) + 1;
    {
      typedef int T;
      result += (T) + 1;
    }
    result += (T) + 1;
  }
  return result + (T) + 1;
}

int typedef_generic(void) {
  int result = 0;
  {
    typedef long T;
    result += _Generic((T) + 1, int: 1, default: 0);
    {
      typedef int T;
      result += _Generic((T) + 1, int: 1, default: 0);
    }
    result += _Generic((T) + 1, int: 1, default: 0);
  }
  return result + _Generic((T) + 1, int: 1, default: 0);
}

int typedef_bound(void) {
  int result = 0;
  {
    typedef long T;
    result += sizeof(int[(T) + 1]);
    {
      typedef int T;
      result += sizeof(int[(T) + 1]);
    }
    result += sizeof(int[(T) + 1]);
  }
  return result + sizeof(int[(T) + 1]);
}

int loop_sizeof(void) {
  for (int T = 2, value = sizeof(T); sizeof(T); T += sizeof(T))
    value += sizeof(T);
  return sizeof(T);
}

int parameter_sizeof(int T, int a[sizeof(T)]);
int nested_sizeof(int (*callback)(int T, int a[sizeof(T)]), int b[sizeof(T)]);

int loop_cast(void) {
  for (int T = 2, value = (T) + 1; (T) + 1; T += (T) + 1)
    value += (T) + 1;
  return (T) + 1;
}

int parameter_cast(int T, int a[(T) + 1]);
int nested_cast(int (*callback)(int T, int a[(T) + 1]), int b[(T) + 1]);

int loop_generic(void) {
  for (int T = 2, value = _Generic((T) + 1, int: 1, default: 0); _Generic((T) + 1, int: 1, default: 0); T += _Generic((T) + 1, int: 1, default: 0))
    value += _Generic((T) + 1, int: 1, default: 0);
  return _Generic((T) + 1, int: 1, default: 0);
}

int parameter_generic(int T, int a[_Generic((T) + 1, int: 1, default: 0)]);
int nested_generic(int (*callback)(int T, int a[_Generic((T) + 1, int: 1, default: 0)]), int b[_Generic((T) + 1, int: 1, default: 0)]);

int loop_bound(void) {
  for (int T = 2, value = sizeof(int[(T) + 1]); sizeof(int[(T) + 1]); T += sizeof(int[(T) + 1]))
    value += sizeof(int[(T) + 1]);
  return sizeof(int[(T) + 1]);
}

int parameter_bound(int T, int a[sizeof(int[(T) + 1])]);
int nested_bound(int (*callback)(int T, int a[sizeof(int[(T) + 1])]), int b[sizeof(int[(T) + 1])]);

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
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = i32;
// DEFAULT-NEXT:     type @type[[TYPE_T_2:[0-9]+]] T = i32;
// DEFAULT-NEXT:     type @type[[TYPE_T_3:[0-9]+]] T = i32;
// DEFAULT-NEXT:     type @type[[TYPE_T_4:[0-9]+]] T = i32;
// DEFAULT-NEXT:     type @type[[TYPE_T_5:[0-9]+]] T = i32;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_T:[0-9]+]] T = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_T_6:[0-9]+]] T = i32;
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_T]] T = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_T_7:[0-9]+]] T = i32;
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_T]] T = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_T_8:[0-9]+]] T = i32;
// DEFAULT-NEXT:     type @type[[TYPE3:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_T]] T = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_T_9:[0-9]+]] T = i32;
// DEFAULT-NEXT:     type @type[[TYPE_T_10:[0-9]+]] T = i64;
// DEFAULT-NEXT:     type @type[[TYPE_T_11:[0-9]+]] T = i32;
// DEFAULT-NEXT:     type @type[[TYPE_T_12:[0-9]+]] T = i64;
// DEFAULT-NEXT:     type @type[[TYPE_T_13:[0-9]+]] T = i32;
// DEFAULT-NEXT:     type @type[[TYPE_T_14:[0-9]+]] T = i64;
// DEFAULT-NEXT:     type @type[[TYPE_T_15:[0-9]+]] T = i32;
// DEFAULT-NEXT:     type @type[[TYPE_T_16:[0-9]+]] T = i64;
// DEFAULT-NEXT:     type @type[[TYPE_T_17:[0-9]+]] T = i32;
// DEFAULT-NEXT:     fn %[[VALUE_object_sizeof:[0-9]+]] @object_sizeof() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result:[0-9]+]] result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_T_2:[0-9]+]] T: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:             let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result]]);
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE0]]))), const<u64>(4))));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE2]]))), const<u64>(4))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_result]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result]]);
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE4]]))), const<u64>(4))));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_result]]))), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_object_cast:[0-9]+]] @object_cast() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result_2:[0-9]+]] result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_T_3:[0-9]+]] T: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:             let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_2]]);
// DEFAULT-NEXT:             let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_T_3]]), const<i32>(1)));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result_2]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_2]]);
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_result_2]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_2]]);
// DEFAULT-NEXT:             let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_T_3]]), const<i32>(1)));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result_2]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_result_2]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_object_generic:[0-9]+]] @object_generic() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result_3:[0-9]+]] result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_T_4:[0-9]+]] T: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:             let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_3]]);
// DEFAULT-NEXT:             let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result_3]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_3]]);
// DEFAULT-NEXT:                 let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE14]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_result_3]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_3]]);
// DEFAULT-NEXT:             let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result_3]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_result_3]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_object_bound:[0-9]+]] @object_bound() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result_4:[0-9]+]] result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_T_5:[0-9]+]] T: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:             let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_4]]);
// DEFAULT-NEXT:             let %[[VALUE19:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(add<i32, overflow=ub>(read<i32>(%[[VALUE_T_5]]), const<i32>(1))));
// DEFAULT-NEXT:             let %[[VALUE20:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE18]]))), mul<u64, overflow=wrap>(read<u64>(%[[VALUE19]]), const<u64>(4)))));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result_4]], read<i32>(%[[VALUE20]]));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE21:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_4]]);
// DEFAULT-NEXT:                 let %[[VALUE22:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE21]]))), const<u64>(4))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_result_4]], read<i32>(%[[VALUE22]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %[[VALUE23:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_4]]);
// DEFAULT-NEXT:             let %[[VALUE24:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(add<i32, overflow=ub>(read<i32>(%[[VALUE_T_5]]), const<i32>(1))));
// DEFAULT-NEXT:             let %[[VALUE25:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE23]]))), mul<u64, overflow=wrap>(read<u64>(%[[VALUE24]]), const<u64>(4)))));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result_4]], read<i32>(%[[VALUE25]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_result_4]]))), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_enumerator_sizeof:[0-9]+]] @enumerator_sizeof() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result_5:[0-9]+]] result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_5]]);
// DEFAULT-NEXT:             let %[[VALUE27:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE26]]))), const<u64>(4))));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result_5]], read<i32>(%[[VALUE27]]));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE28:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_5]]);
// DEFAULT-NEXT:                 let %[[VALUE29:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE28]]))), const<u64>(4))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_result_5]], read<i32>(%[[VALUE29]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %[[VALUE30:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_5]]);
// DEFAULT-NEXT:             let %[[VALUE31:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE30]]))), const<u64>(4))));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result_5]], read<i32>(%[[VALUE31]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_result_5]]))), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_enumerator_cast:[0-9]+]] @enumerator_cast() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result_6:[0-9]+]] result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE32:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_6]]);
// DEFAULT-NEXT:             let %[[VALUE33:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE32]]), add<i32, overflow=ub>(const<i32>(2), const<i32>(1)));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result_6]], read<i32>(%[[VALUE33]]));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE34:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_6]]);
// DEFAULT-NEXT:                 let %[[VALUE35:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE34]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_result_6]], read<i32>(%[[VALUE35]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %[[VALUE36:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_6]]);
// DEFAULT-NEXT:             let %[[VALUE37:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE36]]), add<i32, overflow=ub>(const<i32>(2), const<i32>(1)));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result_6]], read<i32>(%[[VALUE37]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_result_6]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_enumerator_generic:[0-9]+]] @enumerator_generic() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result_7:[0-9]+]] result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE38:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_7]]);
// DEFAULT-NEXT:             let %[[VALUE39:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE38]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result_7]], read<i32>(%[[VALUE39]]));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE40:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_7]]);
// DEFAULT-NEXT:                 let %[[VALUE41:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE40]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_result_7]], read<i32>(%[[VALUE41]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %[[VALUE42:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_7]]);
// DEFAULT-NEXT:             let %[[VALUE43:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE42]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result_7]], read<i32>(%[[VALUE43]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_result_7]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_enumerator_bound:[0-9]+]] @enumerator_bound() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result_8:[0-9]+]] result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE44:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_8]]);
// DEFAULT-NEXT:             let %[[VALUE45:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE44]]))), const<u64>(12))));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result_8]], read<i32>(%[[VALUE45]]));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE46:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_8]]);
// DEFAULT-NEXT:                 let %[[VALUE47:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE46]]))), const<u64>(4))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_result_8]], read<i32>(%[[VALUE47]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %[[VALUE48:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_8]]);
// DEFAULT-NEXT:             let %[[VALUE49:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE48]]))), const<u64>(12))));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result_8]], read<i32>(%[[VALUE49]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_result_8]]))), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_typedef_sizeof:[0-9]+]] @typedef_sizeof() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result_9:[0-9]+]] result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE50:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_9]]);
// DEFAULT-NEXT:             let %[[VALUE51:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE50]]))), const<u64>(8))));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result_9]], read<i32>(%[[VALUE51]]));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE52:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_9]]);
// DEFAULT-NEXT:                 let %[[VALUE53:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE52]]))), const<u64>(4))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_result_9]], read<i32>(%[[VALUE53]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %[[VALUE54:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_9]]);
// DEFAULT-NEXT:             let %[[VALUE55:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE54]]))), const<u64>(8))));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result_9]], read<i32>(%[[VALUE55]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_result_9]]))), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_typedef_cast:[0-9]+]] @typedef_cast() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result_10:[0-9]+]] result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE56:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_10]]);
// DEFAULT-NEXT:             let %[[VALUE57:[0-9]+]]: i32 [synthetic] = truncate<i32, reason=assign, fits=unknown>(add<i64, overflow=ub>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE56]])), widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result_10]], read<i32>(%[[VALUE57]]));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE58:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_10]]);
// DEFAULT-NEXT:                 let %[[VALUE59:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE58]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_result_10]], read<i32>(%[[VALUE59]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %[[VALUE60:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_10]]);
// DEFAULT-NEXT:             let %[[VALUE61:[0-9]+]]: i32 [synthetic] = truncate<i32, reason=assign, fits=unknown>(add<i64, overflow=ub>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE60]])), widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result_10]], read<i32>(%[[VALUE61]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_result_10]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_typedef_generic:[0-9]+]] @typedef_generic() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result_11:[0-9]+]] result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE62:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_11]]);
// DEFAULT-NEXT:             let %[[VALUE63:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE62]]), const<i32>(0));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result_11]], read<i32>(%[[VALUE63]]));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE64:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_11]]);
// DEFAULT-NEXT:                 let %[[VALUE65:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE64]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_result_11]], read<i32>(%[[VALUE65]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %[[VALUE66:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_11]]);
// DEFAULT-NEXT:             let %[[VALUE67:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE66]]), const<i32>(0));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result_11]], read<i32>(%[[VALUE67]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_result_11]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_typedef_bound:[0-9]+]] @typedef_bound() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result_12:[0-9]+]] result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE68:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_12]]);
// DEFAULT-NEXT:             let %[[VALUE69:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE68]]))), const<u64>(4))));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result_12]], read<i32>(%[[VALUE69]]));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE70:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_12]]);
// DEFAULT-NEXT:                 let %[[VALUE71:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE70]]))), const<u64>(4))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_result_12]], read<i32>(%[[VALUE71]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %[[VALUE72:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_12]]);
// DEFAULT-NEXT:             let %[[VALUE73:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE72]]))), const<u64>(4))));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_result_12]], read<i32>(%[[VALUE73]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_result_12]]))), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_loop_sizeof:[0-9]+]] @loop_sizeof() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %[[VALUE74:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_T_6:[0-9]+]] T: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %[[VALUE_value:[0-9]+]] value: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:             condition: ne<u64>(const<u64>(4), const<u64>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE75:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_T_6]]);
// DEFAULT-NEXT:                 let %[[VALUE76:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE75]]))), const<u64>(4))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_T_6]], read<i32>(%[[VALUE76]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE77:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_value]]);
// DEFAULT-NEXT:                 let %[[VALUE78:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE77]]))), const<u64>(4))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_value]], read<i32>(%[[VALUE78]]));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_parameter_sizeof:[0-9]+]] @parameter_sizeof(%[[VALUE_T_7:[0-9]+]] T: i32, %[[VALUE_a:[0-9]+]] a: ptr<i32> [array=*]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nested_sizeof:[0-9]+]] @nested_sizeof(%[[VALUE_callback:[0-9]+]] callback: ptr<fn(i32, ptr<i32>) -> i32>, %[[VALUE_b:[0-9]+]] b: ptr<i32> [array=4]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_loop_cast:[0-9]+]] @loop_cast() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %[[VALUE79:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_T_8:[0-9]+]] T: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %[[VALUE_value_2:[0-9]+]] value: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_T_8]]), const<i32>(1));
// DEFAULT-NEXT:             condition: ne<i32>(add<i32, overflow=ub>(read<i32>(%[[VALUE_T_8]]), const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE80:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_T_8]]);
// DEFAULT-NEXT:                 let %[[VALUE81:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE80]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_T_8]]), const<i32>(1)));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_T_8]], read<i32>(%[[VALUE81]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE82:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_value_2]]);
// DEFAULT-NEXT:                 let %[[VALUE83:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE82]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_T_8]]), const<i32>(1)));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_value_2]], read<i32>(%[[VALUE83]]));
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_parameter_cast:[0-9]+]] @parameter_cast(%[[VALUE_T_9:[0-9]+]] T: i32, %[[VALUE_a_2:[0-9]+]] a: ptr<i32> [array=*]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nested_cast:[0-9]+]] @nested_cast(%[[VALUE_callback_2:[0-9]+]] callback: ptr<fn(i32, ptr<i32>) -> i32>, %[[VALUE_b_2:[0-9]+]] b: ptr<i32> [array=1]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_loop_generic:[0-9]+]] @loop_generic() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %[[VALUE84:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_T_10:[0-9]+]] T: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %[[VALUE_value_3:[0-9]+]] value: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:             condition: ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE85:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_T_10]]);
// DEFAULT-NEXT:                 let %[[VALUE86:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE85]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_T_10]], read<i32>(%[[VALUE86]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE87:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_value_3]]);
// DEFAULT-NEXT:                 let %[[VALUE88:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE87]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_value_3]], read<i32>(%[[VALUE88]]));
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_parameter_generic:[0-9]+]] @parameter_generic(%[[VALUE_T_11:[0-9]+]] T: i32, %[[VALUE_a_3:[0-9]+]] a: ptr<i32> [array=*]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nested_generic:[0-9]+]] @nested_generic(%[[VALUE_callback_3:[0-9]+]] callback: ptr<fn(i32, ptr<i32>) -> i32>, %[[VALUE_b_3:[0-9]+]] b: ptr<i32> [array=1]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_loop_bound:[0-9]+]] @loop_bound() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %[[VALUE89:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_T_12:[0-9]+]] T: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %[[VALUE_value_4:[0-9]+]] value: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE90:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(add<i32, overflow=ub>(read<i32>(%[[VALUE_T_12]]), const<i32>(1))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_value_4]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(mul<u64, overflow=wrap>(read<u64>(%[[VALUE90]]), const<u64>(4)))));
// DEFAULT-NEXT:             condition: {
// DEFAULT-NEXT:                 let %[[VALUE91:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(add<i32, overflow=ub>(read<i32>(%[[VALUE_T_12]]), const<i32>(1))));
// DEFAULT-NEXT:                 yield ne<u64>(mul<u64, overflow=wrap>(read<u64>(%[[VALUE91]]), const<u64>(4)), const<u64>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE92:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_T_12]]);
// DEFAULT-NEXT:                 let %[[VALUE93:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(add<i32, overflow=ub>(read<i32>(%[[VALUE_T_12]]), const<i32>(1))));
// DEFAULT-NEXT:                 let %[[VALUE94:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE92]]))), mul<u64, overflow=wrap>(read<u64>(%[[VALUE93]]), const<u64>(4)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_T_12]], read<i32>(%[[VALUE94]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE95:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_value_4]]);
// DEFAULT-NEXT:                 let %[[VALUE96:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(add<i32, overflow=ub>(read<i32>(%[[VALUE_T_12]]), const<i32>(1))));
// DEFAULT-NEXT:                 let %[[VALUE97:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE95]]))), mul<u64, overflow=wrap>(read<u64>(%[[VALUE96]]), const<u64>(4)))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_value_4]], read<i32>(%[[VALUE97]]));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_parameter_bound:[0-9]+]] @parameter_bound(%[[VALUE_T_13:[0-9]+]] T: i32, %[[VALUE_a_4:[0-9]+]] a: ptr<i32> [array=*]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nested_bound:[0-9]+]] @nested_bound(%[[VALUE_callback_4:[0-9]+]] callback: ptr<fn(i32, ptr<i32>) -> i32>, %[[VALUE_b_4:[0-9]+]] b: ptr<i32> [array=4]) -> i32 [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
