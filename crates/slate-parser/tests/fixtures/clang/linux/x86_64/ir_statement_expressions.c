// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu17

struct Pair { int a, b; };

int twice(int x) { return ({ int y = x + 1; y * 2; }); }

int nested(int x) { return ({ int y = ({ x * 3; }); y; }) + 1; }

void discarded(int *p) { ({ *p = 1; (void)0; }); }

void no_value(int *p) { ({ *p = 2; if (*p) *p = 3; }); }

int guarded(int x, int *p) { return x && ({ *p = x; 1; }); }

struct Pair pair(int x) { return ({ struct Pair t = { x, x }; t; }); }

int early(int x) {
  int y = ({
    if (x < 0)
      return -1;
    x;
  });
  return y;
}

int loop(int n) {
  int total = 0;
  while (({ n--; }) > 0)
    total += ({ int k = n; k * k; });
  return total;
}

int labeled_result(int x) {
  return ({
    __label__ failed, done;
    int result;
    if (!x)
      goto failed;
    result = 17;
    goto done;
  failed:
    result = -5;
  done:
    result;
  });
}

int nested_labels(int x) { return ({ outer: inner: x + 1; }); }

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 Pair = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     fn %1 @twice(%2 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %34: i32 [synthetic];
// IR-NEXT:         {
// IR-NEXT:             let %3 y: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%2), const<i32>(1));
// IR-NEXT:             write<i32>(%34, mul<i32, overflow=ub>(read<i32>(%3), const<i32>(2)));
// IR-NEXT:         }
// IR-NEXT:         return read<i32>(%34);
// IR-NEXT:     }
// IR-NEXT:     fn %4 @nested(%5 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %35: i32 [synthetic];
// IR-NEXT:         {
// IR-NEXT:             let %6 y: i32 [storage=automatic];
// IR-NEXT:             let %36: i32 [synthetic];
// IR-NEXT:             {
// IR-NEXT:                 write<i32>(%36, mul<i32, overflow=ub>(read<i32>(%5), const<i32>(3)));
// IR-NEXT:             }
// IR-NEXT:             write<i32>(%6, read<i32>(%36));
// IR-NEXT:             write<i32>(%35, read<i32>(%6));
// IR-NEXT:         }
// IR-NEXT:         return add<i32, overflow=ub>(read<i32>(%35), const<i32>(1));
// IR-NEXT:     }
// IR-NEXT:     fn %7 @discarded(%8 p: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         {
// IR-NEXT:             write<i32>(deref(read<ptr<i32>>(%8)), const<i32>(1));
// IR-NEXT:             const<i32>(0);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %9 @no_value(%10 p: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         {
// IR-NEXT:             write<i32>(deref(read<ptr<i32>>(%10)), const<i32>(2));
// IR-NEXT:             if ne<i32>(read<i32>(deref(read<ptr<i32>>(%10))), const<i32>(0))
// IR-NEXT:                 write<i32>(deref(read<ptr<i32>>(%10)), const<i32>(3));
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %11 @guarded(%12 x: i32, %13 p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %37: bool [synthetic];
// IR-NEXT:         if ne<i32>(read<i32>(%12), const<i32>(0))
// IR-NEXT:             let %38: i32 [synthetic];
// IR-NEXT:             {
// IR-NEXT:                 write<i32>(deref(read<ptr<i32>>(%13)), read<i32>(%12));
// IR-NEXT:                 write<i32>(%38, const<i32>(1));
// IR-NEXT:             }
// IR-NEXT:             write<bool>(%37, ne<i32>(read<i32>(%38), const<i32>(0)));
// IR-NEXT:         else
// IR-NEXT:             write<bool>(%37, const<bool>(false));
// IR-NEXT:         return from_bool<i32, reason=return>(read<bool>(%37));
// IR-NEXT:     }
// IR-NEXT:     fn %14 @pair(%15 x: i32) -> @type0 [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         let %39: @type0 [synthetic];
// IR-NEXT:         {
// IR-NEXT:             let %16 t: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = read<i32>(%15), field1 = read<i32>(%15));
// IR-NEXT:             write<@type0>(%39, read<@type0>(%16));
// IR-NEXT:         }
// IR-NEXT:         return copy<@type0, reason=return>(read<@type0>(%39));
// IR-NEXT:     }
// IR-NEXT:     fn %17 @early(%18 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %19 y: i32 [storage=automatic];
// IR-NEXT:         let %40: i32 [synthetic];
// IR-NEXT:         {
// IR-NEXT:             if lt<i32>(read<i32>(%18), const<i32>(0))
// IR-NEXT:                 return neg<i32, overflow=ub>(const<i32>(1));
// IR-NEXT:             write<i32>(%40, read<i32>(%18));
// IR-NEXT:         }
// IR-NEXT:         write<i32>(%19, read<i32>(%40));
// IR-NEXT:         return read<i32>(%19);
// IR-NEXT:     }
// IR-NEXT:     fn %20 @loop(%21 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %22 total: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:         while %33 {
// IR-NEXT:             let %41: i32 [synthetic];
// IR-NEXT:             {
// IR-NEXT:                 let %42: i32 [synthetic] = read<i32>(%21);
// IR-NEXT:                 let %43: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%42), const<i32>(1));
// IR-NEXT:                 write<i32>(%21, read<i32>(%43));
// IR-NEXT:                 write<i32>(%41, read<i32>(%42));
// IR-NEXT:             }
// IR-NEXT:             yield gt<i32>(read<i32>(%41), const<i32>(0));
// IR-NEXT:         }
// IR-NEXT:             let %44: i32 [synthetic] = read<i32>(%22);
// IR-NEXT:             let %45: i32 [synthetic];
// IR-NEXT:             {
// IR-NEXT:                 let %23 k: i32 [storage=automatic] = read<i32>(%21);
// IR-NEXT:                 write<i32>(%45, mul<i32, overflow=ub>(read<i32>(%23), read<i32>(%23)));
// IR-NEXT:             }
// IR-NEXT:             let %46: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%44), read<i32>(%45));
// IR-NEXT:             write<i32>(%22, read<i32>(%46));
// IR-NEXT:         return read<i32>(%22);
// IR-NEXT:     }
// IR-NEXT:     fn %24 @labeled_result(%27 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %47: i32 [synthetic];
// IR-NEXT:         {
// IR-NEXT:             let %28 result: i32 [storage=automatic];
// IR-NEXT:             if not<bool>(ne<i32>(read<i32>(%27), const<i32>(0)))
// IR-NEXT:                 goto %25;
// IR-NEXT:             write<i32>(%28, const<i32>(17));
// IR-NEXT:             goto %26;
// IR-NEXT:             label %25 failed:
// IR-NEXT:                 write<i32>(%28, neg<i32, overflow=ub>(const<i32>(5)));
// IR-NEXT:             label %26 done:
// IR-NEXT:                 ;
// IR-NEXT:             write<i32>(%47, read<i32>(%28));
// IR-NEXT:         }
// IR-NEXT:         return read<i32>(%47);
// IR-NEXT:     }
// IR-NEXT:     fn %29 @nested_labels(%32 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %48: i32 [synthetic];
// IR-NEXT:         {
// IR-NEXT:             label %30 outer:
// IR-NEXT:                 label %31 inner:
// IR-NEXT:                     ;
// IR-NEXT:             write<i32>(%48, add<i32, overflow=ub>(read<i32>(%32), const<i32>(1)));
// IR-NEXT:         }
// IR-NEXT:         return read<i32>(%48);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
