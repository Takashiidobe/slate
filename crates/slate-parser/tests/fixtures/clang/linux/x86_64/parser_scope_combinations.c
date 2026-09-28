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
// DEFAULT-NEXT:     type @type0 T = i32;
// DEFAULT-NEXT:     type @type1 T = i32;
// DEFAULT-NEXT:     type @type2 T = i32;
// DEFAULT-NEXT:     type @type3 T = i32;
// DEFAULT-NEXT:     type @type4 T = i32;
// DEFAULT-NEXT:     type @type5 = enum : u32 {
// DEFAULT-NEXT:         %0 T = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type6 T = i32;
// DEFAULT-NEXT:     type @type7 = enum : u32 {
// DEFAULT-NEXT:         %0 T = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type8 T = i32;
// DEFAULT-NEXT:     type @type9 = enum : u32 {
// DEFAULT-NEXT:         %0 T = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type10 T = i32;
// DEFAULT-NEXT:     type @type11 = enum : u32 {
// DEFAULT-NEXT:         %0 T = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type12 T = i32;
// DEFAULT-NEXT:     type @type13 T = i64;
// DEFAULT-NEXT:     type @type14 T = i32;
// DEFAULT-NEXT:     type @type15 T = i64;
// DEFAULT-NEXT:     type @type16 T = i32;
// DEFAULT-NEXT:     type @type17 T = i64;
// DEFAULT-NEXT:     type @type18 T = i32;
// DEFAULT-NEXT:     type @type19 T = i64;
// DEFAULT-NEXT:     type @type20 T = i32;
// DEFAULT-NEXT:     fn %1 @object_sizeof() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %3 T: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:             let %123: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:             let %124: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%123))), const<u64>(4))));
// DEFAULT-NEXT:             write<i32>(%2, read<i32>(%124));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %125: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                 let %126: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%125))), const<u64>(4))));
// DEFAULT-NEXT:                 write<i32>(%2, read<i32>(%126));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %127: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:             let %128: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%127))), const<u64>(4))));
// DEFAULT-NEXT:             write<i32>(%2, read<i32>(%128));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%2))), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @object_cast() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %7 T: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:             let %129: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:             let %130: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%129), add<i32, overflow=ub>(read<i32>(%7), const<i32>(1)));
// DEFAULT-NEXT:             write<i32>(%6, read<i32>(%130));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %131: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %132: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%131), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%132));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %133: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:             let %134: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%133), add<i32, overflow=ub>(read<i32>(%7), const<i32>(1)));
// DEFAULT-NEXT:             write<i32>(%6, read<i32>(%134));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%6), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @object_generic() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %11 T: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:             let %135: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:             let %136: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%135), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%10, read<i32>(%136));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %137: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %138: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%137), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%138));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %139: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:             let %140: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%139), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%10, read<i32>(%140));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @object_bound() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %14 result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %15 T: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:             let %141: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:             let %97: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(add<i32, overflow=ub>(read<i32>(%15), const<i32>(1))));
// DEFAULT-NEXT:             let %142: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%141))), mul<u64, overflow=wrap>(read<u64>(%97), const<u64>(4)))));
// DEFAULT-NEXT:             write<i32>(%14, read<i32>(%142));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %143: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:                 let %144: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%143))), const<u64>(4))));
// DEFAULT-NEXT:                 write<i32>(%14, read<i32>(%144));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %145: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:             let %98: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(add<i32, overflow=ub>(read<i32>(%15), const<i32>(1))));
// DEFAULT-NEXT:             let %146: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%145))), mul<u64, overflow=wrap>(read<u64>(%98), const<u64>(4)))));
// DEFAULT-NEXT:             write<i32>(%14, read<i32>(%146));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%14))), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @enumerator_sizeof() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %18 result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %147: i32 [synthetic] = read<i32>(%18);
// DEFAULT-NEXT:             let %148: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%147))), const<u64>(4))));
// DEFAULT-NEXT:             write<i32>(%18, read<i32>(%148));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %149: i32 [synthetic] = read<i32>(%18);
// DEFAULT-NEXT:                 let %150: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%149))), const<u64>(4))));
// DEFAULT-NEXT:                 write<i32>(%18, read<i32>(%150));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %151: i32 [synthetic] = read<i32>(%18);
// DEFAULT-NEXT:             let %152: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%151))), const<u64>(4))));
// DEFAULT-NEXT:             write<i32>(%18, read<i32>(%152));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%18))), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @enumerator_cast() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %23 result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %153: i32 [synthetic] = read<i32>(%23);
// DEFAULT-NEXT:             let %154: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%153), add<i32, overflow=ub>(const<i32>(2), const<i32>(1)));
// DEFAULT-NEXT:             write<i32>(%23, read<i32>(%154));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %155: i32 [synthetic] = read<i32>(%23);
// DEFAULT-NEXT:                 let %156: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%155), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%23, read<i32>(%156));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %157: i32 [synthetic] = read<i32>(%23);
// DEFAULT-NEXT:             let %158: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%157), add<i32, overflow=ub>(const<i32>(2), const<i32>(1)));
// DEFAULT-NEXT:             write<i32>(%23, read<i32>(%158));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%23), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @enumerator_generic() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %28 result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %159: i32 [synthetic] = read<i32>(%28);
// DEFAULT-NEXT:             let %160: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%159), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%28, read<i32>(%160));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %161: i32 [synthetic] = read<i32>(%28);
// DEFAULT-NEXT:                 let %162: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%161), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%28, read<i32>(%162));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %163: i32 [synthetic] = read<i32>(%28);
// DEFAULT-NEXT:             let %164: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%163), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%28, read<i32>(%164));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%28), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @enumerator_bound() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %33 result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %165: i32 [synthetic] = read<i32>(%33);
// DEFAULT-NEXT:             let %166: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%165))), const<u64>(12))));
// DEFAULT-NEXT:             write<i32>(%33, read<i32>(%166));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %167: i32 [synthetic] = read<i32>(%33);
// DEFAULT-NEXT:                 let %168: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%167))), const<u64>(4))));
// DEFAULT-NEXT:                 write<i32>(%33, read<i32>(%168));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %169: i32 [synthetic] = read<i32>(%33);
// DEFAULT-NEXT:             let %170: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%169))), const<u64>(12))));
// DEFAULT-NEXT:             write<i32>(%33, read<i32>(%170));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%33))), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %37 @typedef_sizeof() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %38 result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %171: i32 [synthetic] = read<i32>(%38);
// DEFAULT-NEXT:             let %172: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%171))), const<u64>(8))));
// DEFAULT-NEXT:             write<i32>(%38, read<i32>(%172));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %173: i32 [synthetic] = read<i32>(%38);
// DEFAULT-NEXT:                 let %174: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%173))), const<u64>(4))));
// DEFAULT-NEXT:                 write<i32>(%38, read<i32>(%174));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %175: i32 [synthetic] = read<i32>(%38);
// DEFAULT-NEXT:             let %176: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%175))), const<u64>(8))));
// DEFAULT-NEXT:             write<i32>(%38, read<i32>(%176));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%38))), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @typedef_cast() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %42 result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %177: i32 [synthetic] = read<i32>(%42);
// DEFAULT-NEXT:             let %178: i32 [synthetic] = truncate<i32, reason=assign, fits=unknown>(add<i64, overflow=ub>(widen<i64, reason=usual_arith>(read<i32>(%177)), widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:             write<i32>(%42, read<i32>(%178));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %179: i32 [synthetic] = read<i32>(%42);
// DEFAULT-NEXT:                 let %180: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%179), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%42, read<i32>(%180));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %181: i32 [synthetic] = read<i32>(%42);
// DEFAULT-NEXT:             let %182: i32 [synthetic] = truncate<i32, reason=assign, fits=unknown>(add<i64, overflow=ub>(widen<i64, reason=usual_arith>(read<i32>(%181)), widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:             write<i32>(%42, read<i32>(%182));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%42), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %45 @typedef_generic() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %46 result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %183: i32 [synthetic] = read<i32>(%46);
// DEFAULT-NEXT:             let %184: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%183), const<i32>(0));
// DEFAULT-NEXT:             write<i32>(%46, read<i32>(%184));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %185: i32 [synthetic] = read<i32>(%46);
// DEFAULT-NEXT:                 let %186: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%185), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%46, read<i32>(%186));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %187: i32 [synthetic] = read<i32>(%46);
// DEFAULT-NEXT:             let %188: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%187), const<i32>(0));
// DEFAULT-NEXT:             write<i32>(%46, read<i32>(%188));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%46), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %49 @typedef_bound() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %50 result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %189: i32 [synthetic] = read<i32>(%50);
// DEFAULT-NEXT:             let %190: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%189))), const<u64>(4))));
// DEFAULT-NEXT:             write<i32>(%50, read<i32>(%190));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %191: i32 [synthetic] = read<i32>(%50);
// DEFAULT-NEXT:                 let %192: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%191))), const<u64>(4))));
// DEFAULT-NEXT:                 write<i32>(%50, read<i32>(%192));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %193: i32 [synthetic] = read<i32>(%50);
// DEFAULT-NEXT:             let %194: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%193))), const<u64>(4))));
// DEFAULT-NEXT:             write<i32>(%50, read<i32>(%194));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%50))), const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %53 @loop_sizeof() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %99
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %54 T: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %55 value: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:             condition: ne<u64>(const<u64>(4), const<u64>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %195: i32 [synthetic] = read<i32>(%54);
// DEFAULT-NEXT:                 let %196: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%195))), const<u64>(4))));
// DEFAULT-NEXT:                 write<i32>(%54, read<i32>(%196));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %197: i32 [synthetic] = read<i32>(%55);
// DEFAULT-NEXT:                 let %198: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%197))), const<u64>(4))));
// DEFAULT-NEXT:                 write<i32>(%55, read<i32>(%198));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %58 @parameter_sizeof(%100 T: i32, %101 a: ptr<i32> [array=*]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %63 @nested_sizeof(%102 callback: ptr<fn(i32, ptr<i32>) -> i32>, %103 b: ptr<i32> [array=4]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %64 @loop_cast() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %104
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %65 T: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %66 value: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%65), const<i32>(1));
// DEFAULT-NEXT:             condition: ne<i32>(add<i32, overflow=ub>(read<i32>(%65), const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %199: i32 [synthetic] = read<i32>(%65);
// DEFAULT-NEXT:                 let %200: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%199), add<i32, overflow=ub>(read<i32>(%65), const<i32>(1)));
// DEFAULT-NEXT:                 write<i32>(%65, read<i32>(%200));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %201: i32 [synthetic] = read<i32>(%66);
// DEFAULT-NEXT:                 let %202: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%201), add<i32, overflow=ub>(read<i32>(%65), const<i32>(1)));
// DEFAULT-NEXT:                 write<i32>(%66, read<i32>(%202));
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %69 @parameter_cast(%105 T: i32, %106 a: ptr<i32> [array=*]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %74 @nested_cast(%107 callback: ptr<fn(i32, ptr<i32>) -> i32>, %108 b: ptr<i32> [array=1]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %75 @loop_generic() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %109
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %76 T: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %77 value: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:             condition: ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %203: i32 [synthetic] = read<i32>(%76);
// DEFAULT-NEXT:                 let %204: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%203), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%76, read<i32>(%204));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %205: i32 [synthetic] = read<i32>(%77);
// DEFAULT-NEXT:                 let %206: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%205), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%77, read<i32>(%206));
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %80 @parameter_generic(%110 T: i32, %111 a: ptr<i32> [array=*]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %85 @nested_generic(%112 callback: ptr<fn(i32, ptr<i32>) -> i32>, %113 b: ptr<i32> [array=1]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %86 @loop_bound() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %114
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %87 T: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:                 let %88 value: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %115: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(add<i32, overflow=ub>(read<i32>(%87), const<i32>(1))));
// DEFAULT-NEXT:                 write<i32>(%88, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(mul<u64, overflow=wrap>(read<u64>(%115), const<u64>(4)))));
// DEFAULT-NEXT:             condition: {
// DEFAULT-NEXT:                 let %116: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(add<i32, overflow=ub>(read<i32>(%87), const<i32>(1))));
// DEFAULT-NEXT:                 yield ne<u64>(mul<u64, overflow=wrap>(read<u64>(%116), const<u64>(4)), const<u64>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %207: i32 [synthetic] = read<i32>(%87);
// DEFAULT-NEXT:                 let %117: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(add<i32, overflow=ub>(read<i32>(%87), const<i32>(1))));
// DEFAULT-NEXT:                 let %208: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%207))), mul<u64, overflow=wrap>(read<u64>(%117), const<u64>(4)))));
// DEFAULT-NEXT:                 write<i32>(%87, read<i32>(%208));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %209: i32 [synthetic] = read<i32>(%88);
// DEFAULT-NEXT:                 let %118: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(add<i32, overflow=ub>(read<i32>(%87), const<i32>(1))));
// DEFAULT-NEXT:                 let %210: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%209))), mul<u64, overflow=wrap>(read<u64>(%118), const<u64>(4)))));
// DEFAULT-NEXT:                 write<i32>(%88, read<i32>(%210));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %91 @parameter_bound(%119 T: i32, %120 a: ptr<i32> [array=*]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %96 @nested_bound(%121 callback: ptr<fn(i32, ptr<i32>) -> i32>, %122 b: ptr<i32> [array=4]) -> i32 [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
