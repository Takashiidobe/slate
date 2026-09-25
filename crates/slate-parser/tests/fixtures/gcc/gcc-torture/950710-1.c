void abort(void);
void exit(int);

struct twelve {
  int a;
  int b;
  int c;
};

struct pair {
  int first;
  int second;
};

struct pair g() {
  struct pair p;
  return p;
}

static void f() {
  int i;
  for (i = 0; i < 1; i++) {
    int j;
    for (j = 0; j < 1; j++) {
      if (0) {
        int k;
        for (k = 0; k < 1; k++) {
          struct pair e = g();
        }
      } else {
        struct twelve a, b;
        if ((((char *)&b - (char *)&a) < 0
                 ? (-((char *)&b - (char *)&a))
                 : ((char *)&b - (char *)&a)) < sizeof(a))
          abort();
      }
    }
  }
}

int main(void) {
  f();
  exit(0);
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
// DEFAULT-NEXT:     type @type0 twelve = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type1 pair = struct {
// DEFAULT-NEXT:         field0 first: i32;
// DEFAULT-NEXT:         field1 second: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%14 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @g() -> @type1 [linkage=external] [abi=sysv64() -> coerce<i64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 p: @type1 [storage=automatic];
// DEFAULT-NEXT:         return copy<@type1, reason=return>(read<@type1>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %15
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%7), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%19));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %8 j: i32 [storage=automatic];
// DEFAULT-NEXT:                     for %16
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%8, const<i32>(0));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%8), const<i32>(1))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %20: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                             let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%8, read<i32>(%21));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 if ne<i32>(const<i32>(0), const<i32>(0))
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %9 k: i32 [storage=automatic];
// DEFAULT-NEXT:                                         for %17
// DEFAULT-NEXT:                                             init:
// DEFAULT-NEXT:                                                 write<i32>(%9, const<i32>(0));
// DEFAULT-NEXT:                                             condition: lt<i32>(read<i32>(%9), const<i32>(1))
// DEFAULT-NEXT:                                             increment: {
// DEFAULT-NEXT:                                                 let %22: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                                                 let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), const<i32>(1));
// DEFAULT-NEXT:                                                 write<i32>(%9, read<i32>(%23));
// DEFAULT-NEXT:                                                 yield void;
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:                                             body:
// DEFAULT-NEXT:                                                 {
// DEFAULT-NEXT:                                                     let %10 e: @type1 [storage=automatic] = copy<@type1, reason=assign>(call<@type1, signature=fn() -> @type1, abi=sysv64() -> coerce<i64>>(%4));
// DEFAULT-NEXT:                                                 }
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                 else
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %11 a: @type0 [storage=automatic];
// DEFAULT-NEXT:                                         let %12 b: @type0 [storage=automatic];
// DEFAULT-NEXT:                                         if lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(conditional<i64>(lt<i64>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type0>>(%12)), pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type0>>(%11))), widen<i64, reason=usual_arith>(const<i32>(0))), neg<i64, overflow=ub>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type0>>(%12)), pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type0>>(%11)))), ptr_diff<i64, element=i8, same_array=required, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type0>>(%12)), pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type0>>(%11))))), const<u64>(12))
// DEFAULT-NEXT:                                             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
