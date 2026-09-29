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
// IR-NEXT:     type @type[[TYPE_Pair:[0-9]+]] Pair = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     fn %[[VALUE_twice:[0-9]+]] @twice(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic];
// IR-NEXT:         {
// IR-NEXT:             let %[[VALUE_y:[0-9]+]] y: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_x]]), const<i32>(1));
// IR-NEXT:             write<i32>(%[[VALUE0]], mul<i32, overflow=ub>(read<i32>(%[[VALUE_y]]), const<i32>(2)));
// IR-NEXT:         }
// IR-NEXT:         return read<i32>(%[[VALUE0]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_nested:[0-9]+]] @nested(%[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic];
// IR-NEXT:         {
// IR-NEXT:             let %[[VALUE_y_2:[0-9]+]] y: i32 [storage=automatic];
// IR-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic];
// IR-NEXT:             {
// IR-NEXT:                 write<i32>(%[[VALUE2]], mul<i32, overflow=ub>(read<i32>(%[[VALUE_x_2]]), const<i32>(3)));
// IR-NEXT:             }
// IR-NEXT:             write<i32>(%[[VALUE_y_2]], read<i32>(%[[VALUE2]]));
// IR-NEXT:             write<i32>(%[[VALUE1]], read<i32>(%[[VALUE_y_2]]));
// IR-NEXT:         }
// IR-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_discarded:[0-9]+]] @discarded(%[[VALUE_p:[0-9]+]] p: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         {
// IR-NEXT:             write<i32>(deref(read<ptr<i32>>(%[[VALUE_p]])), const<i32>(1));
// IR-NEXT:             const<i32>(0);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_no_value:[0-9]+]] @no_value(%[[VALUE_p_2:[0-9]+]] p: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         {
// IR-NEXT:             write<i32>(deref(read<ptr<i32>>(%[[VALUE_p_2]])), const<i32>(2));
// IR-NEXT:             if ne<i32>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_p_2]]))), const<i32>(0))
// IR-NEXT:                 write<i32>(deref(read<ptr<i32>>(%[[VALUE_p_2]])), const<i32>(3));
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_guarded:[0-9]+]] @guarded(%[[VALUE_x_3:[0-9]+]] x: i32, %[[VALUE_p_3:[0-9]+]] p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: bool [synthetic];
// IR-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x_3]]), const<i32>(0))
// IR-NEXT:             let %[[VALUE4:[0-9]+]]: i32 [synthetic];
// IR-NEXT:             {
// IR-NEXT:                 write<i32>(deref(read<ptr<i32>>(%[[VALUE_p_3]])), read<i32>(%[[VALUE_x_3]]));
// IR-NEXT:                 write<i32>(%[[VALUE4]], const<i32>(1));
// IR-NEXT:             }
// IR-NEXT:             write<bool>(%[[VALUE3]], ne<i32>(read<i32>(%[[VALUE4]]), const<i32>(0)));
// IR-NEXT:         else
// IR-NEXT:             write<bool>(%[[VALUE3]], const<bool>(false));
// IR-NEXT:         return from_bool<i32, reason=return>(read<bool>(%[[VALUE3]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_pair:[0-9]+]] @pair(%[[VALUE_x_4:[0-9]+]] x: i32) -> @type[[TYPE_Pair]] [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: @type[[TYPE_Pair]] [synthetic];
// IR-NEXT:         {
// IR-NEXT:             let %[[VALUE_t:[0-9]+]] t: @type[[TYPE_Pair]] [storage=automatic] = aggregate<@type[[TYPE_Pair]], zero_fill=false>(field0 = read<i32>(%[[VALUE_x_4]]), field1 = read<i32>(%[[VALUE_x_4]]));
// IR-NEXT:             write<@type[[TYPE_Pair]]>(%[[VALUE5]], read<@type[[TYPE_Pair]]>(%[[VALUE_t]]));
// IR-NEXT:         }
// IR-NEXT:         return copy<@type[[TYPE_Pair]], reason=return>(read<@type[[TYPE_Pair]]>(%[[VALUE5]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_early:[0-9]+]] @early(%[[VALUE_x_5:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_y_3:[0-9]+]] y: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic];
// IR-NEXT:         {
// IR-NEXT:             if lt<i32>(read<i32>(%[[VALUE_x_5]]), const<i32>(0))
// IR-NEXT:                 return neg<i32, overflow=ub>(const<i32>(1));
// IR-NEXT:             write<i32>(%[[VALUE6]], read<i32>(%[[VALUE_x_5]]));
// IR-NEXT:         }
// IR-NEXT:         write<i32>(%[[VALUE_y_3]], read<i32>(%[[VALUE6]]));
// IR-NEXT:         return read<i32>(%[[VALUE_y_3]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_loop:[0-9]+]] @loop(%[[VALUE_n:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:         while %[[VALUE7:[0-9]+]] {
// IR-NEXT:             let %[[VALUE8:[0-9]+]]: i32 [synthetic];
// IR-NEXT:             {
// IR-NEXT:                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n]]);
// IR-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE10]]));
// IR-NEXT:                 write<i32>(%[[VALUE8]], read<i32>(%[[VALUE9]]));
// IR-NEXT:             }
// IR-NEXT:             yield gt<i32>(read<i32>(%[[VALUE8]]), const<i32>(0));
// IR-NEXT:         }
// IR-NEXT:             let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// IR-NEXT:             let %[[VALUE12:[0-9]+]]: i32 [synthetic];
// IR-NEXT:             {
// IR-NEXT:                 let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic] = read<i32>(%[[VALUE_n]]);
// IR-NEXT:                 write<i32>(%[[VALUE12]], mul<i32, overflow=ub>(read<i32>(%[[VALUE_k]]), read<i32>(%[[VALUE_k]])));
// IR-NEXT:             }
// IR-NEXT:             let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE11]]), read<i32>(%[[VALUE12]]));
// IR-NEXT:             write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE13]]));
// IR-NEXT:         return read<i32>(%[[VALUE_total]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_labeled_result:[0-9]+]] @labeled_result(%[[VALUE_x_6:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE14:[0-9]+]]: i32 [synthetic];
// IR-NEXT:         {
// IR-NEXT:             let %[[VALUE_result:[0-9]+]] result: i32 [storage=automatic];
// IR-NEXT:             if not<bool>(ne<i32>(read<i32>(%[[VALUE_x_6]]), const<i32>(0)))
// IR-NEXT:                 goto %[[VALUE_failed:[0-9]+]];
// IR-NEXT:             write<i32>(%[[VALUE_result]], const<i32>(17));
// IR-NEXT:             goto %[[VALUE_done:[0-9]+]];
// IR-NEXT:             label %[[VALUE_failed]] failed:
// IR-NEXT:                 write<i32>(%[[VALUE_result]], neg<i32, overflow=ub>(const<i32>(5)));
// IR-NEXT:             label %[[VALUE_done]] done:
// IR-NEXT:                 ;
// IR-NEXT:             write<i32>(%[[VALUE14]], read<i32>(%[[VALUE_result]]));
// IR-NEXT:         }
// IR-NEXT:         return read<i32>(%[[VALUE14]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_nested_labels:[0-9]+]] @nested_labels(%[[VALUE_x_7:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE15:[0-9]+]]: i32 [synthetic];
// IR-NEXT:         {
// IR-NEXT:             label %[[VALUE_outer:[0-9]+]] outer:
// IR-NEXT:                 label %[[VALUE_inner:[0-9]+]] inner:
// IR-NEXT:                     ;
// IR-NEXT:             write<i32>(%[[VALUE15]], add<i32, overflow=ub>(read<i32>(%[[VALUE_x_7]]), const<i32>(1)));
// IR-NEXT:         }
// IR-NEXT:         return read<i32>(%[[VALUE15]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
