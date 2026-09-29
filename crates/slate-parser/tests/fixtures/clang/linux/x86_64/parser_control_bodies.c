void bodies(int x) {
  if (x) { x++; } else x--;
  if (x) x++; else { x--; }
  while (x) { x--; }
  while (x) x--;
  do { x++; } while (x);
  do x++; while (x);
  for (; x; x--) { x++; }
  for (; x; x--) x++;
  switch (x) { case 0: x++; break; }
  switch (x) default: x++;
  if (x) {} else ;
}
typedef int T;
int condition_typedef(void) {
  if (sizeof(enum { T = 1 })) (void)sizeof(T);
  return sizeof(T);
}
int body_typedef(void) {
  if (1) (void)sizeof(enum { T = 1 });
  return sizeof(T);
}
int do_typedef(void) {
  do (void)sizeof(enum { T = 1 }); while (sizeof(T));
  return sizeof(T);
}
int nested_typedef(void) {
  if (sizeof(enum { T = 1 })) {
    typedef int T;
    T x;
  }
  return sizeof(T);
}

// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-DEFINES C99
// SLATE-FILECHECK-STD C99 c99

// SLATE-FILECHECK-BEGIN C89
// C89: module {
// C89-NEXT:     target "x86_64-unknown-linux-gnu" {
// C89-NEXT:         endian = little;
// C89-NEXT:         pointer [size=8, align=8];
// C89-NEXT:         stack_alignment = 16;
// C89-NEXT:         long_double = f80;
// C89-NEXT:         storage bool [size=1, align=1];
// C89-NEXT:         storage i8, u8 [size=1, align=1];
// C89-NEXT:         storage i16, u16 [size=2, align=2];
// C89-NEXT:         storage i32, u32 [size=4, align=4];
// C89-NEXT:         storage i64, u64 [size=8, align=8];
// C89-NEXT:         storage i128, u128 [size=16, align=16];
// C89-NEXT:         storage bf16 [size=2, align=2];
// C89-NEXT:         storage f16 [size=2, align=2];
// C89-NEXT:         storage f32 [size=4, align=4];
// C89-NEXT:         storage f64 [size=8, align=8];
// C89-NEXT:         storage f80 [size=16, align=16];
// C89-NEXT:         storage f128 [size=16, align=16];
// C89-NEXT:         storage d32 [size=4, align=4];
// C89-NEXT:         storage d64 [size=8, align=8];
// C89-NEXT:         storage d128 [size=16, align=16];
// C89-NEXT:     }
// C89-NEXT:     type @type[[TYPE_T:[0-9]+]] T = i32;
// C89-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// C89-NEXT:         %[[VALUE_T:[0-9]+]] T = const<i32>(1);
// C89-NEXT:     } [size=4, align=4];
// C89-NEXT:     type @type[[TYPE1:[0-9]+]] = enum : u32 {
// C89-NEXT:         %[[VALUE_T]] T = const<i32>(1);
// C89-NEXT:     } [size=4, align=4];
// C89-NEXT:     type @type[[TYPE2:[0-9]+]] = enum : u32 {
// C89-NEXT:         %[[VALUE_T]] T = const<i32>(1);
// C89-NEXT:     } [size=4, align=4];
// C89-NEXT:     type @type[[TYPE3:[0-9]+]] = enum : u32 {
// C89-NEXT:         %[[VALUE_T]] T = const<i32>(1);
// C89-NEXT:     } [size=4, align=4];
// C89-NEXT:     type @type[[TYPE_T_2:[0-9]+]] T = i32;
// C89-NEXT:     fn %[[VALUE_T]] @bodies(%[[VALUE_x:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// C89-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// C89-NEXT:             {
// C89-NEXT:                 let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C89-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// C89-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE1]]));
// C89-NEXT:             }
// C89-NEXT:         else
// C89-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C89-NEXT:             let %[[VALUE3:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// C89-NEXT:             write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE3]]));
// C89-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// C89-NEXT:             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C89-NEXT:             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// C89-NEXT:             write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE5]]));
// C89-NEXT:         else
// C89-NEXT:             {
// C89-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C89-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// C89-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE7]]));
// C89-NEXT:             }
// C89-NEXT:         while %[[VALUE8:[0-9]+]] ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// C89-NEXT:             {
// C89-NEXT:                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C89-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// C89-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE10]]));
// C89-NEXT:             }
// C89-NEXT:         while %[[VALUE11:[0-9]+]] ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// C89-NEXT:             let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C89-NEXT:             let %[[VALUE13:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE12]]), const<i32>(1));
// C89-NEXT:             write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE13]]));
// C89-NEXT:         do %[[VALUE14:[0-9]+]]
// C89-NEXT:             {
// C89-NEXT:                 let %[[VALUE15:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C89-NEXT:                 let %[[VALUE16:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE15]]), const<i32>(1));
// C89-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE16]]));
// C89-NEXT:             }
// C89-NEXT:         while ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0));
// C89-NEXT:         do %[[VALUE17:[0-9]+]]
// C89-NEXT:             let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C89-NEXT:             let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), const<i32>(1));
// C89-NEXT:             write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE19]]));
// C89-NEXT:         while ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0));
// C89-NEXT:         for %[[VALUE20:[0-9]+]]
// C89-NEXT:             init:
// C89-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// C89-NEXT:             increment: {
// C89-NEXT:                 let %[[VALUE21:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C89-NEXT:                 let %[[VALUE22:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE21]]), const<i32>(1));
// C89-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE22]]));
// C89-NEXT:                 yield void;
// C89-NEXT:             }
// C89-NEXT:             body:
// C89-NEXT:                 {
// C89-NEXT:                     let %[[VALUE23:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C89-NEXT:                     let %[[VALUE24:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE23]]), const<i32>(1));
// C89-NEXT:                     write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE24]]));
// C89-NEXT:                 }
// C89-NEXT:         for %[[VALUE25:[0-9]+]]
// C89-NEXT:             init:
// C89-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// C89-NEXT:             increment: {
// C89-NEXT:                 let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C89-NEXT:                 let %[[VALUE27:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE26]]), const<i32>(1));
// C89-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE27]]));
// C89-NEXT:                 yield void;
// C89-NEXT:             }
// C89-NEXT:             body:
// C89-NEXT:                 let %[[VALUE28:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C89-NEXT:                 let %[[VALUE29:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE28]]), const<i32>(1));
// C89-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE29]]));
// C89-NEXT:         switch %[[VALUE30:[0-9]+]] read<i32>(%[[VALUE_x]])
// C89-NEXT:             {
// C89-NEXT:                 case %[[VALUE30]] const<i32>(0):
// C89-NEXT:                     let %[[VALUE31:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C89-NEXT:                     let %[[VALUE32:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE31]]), const<i32>(1));
// C89-NEXT:                     write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE32]]));
// C89-NEXT:                 break %[[VALUE30]];
// C89-NEXT:             }
// C89-NEXT:         switch %[[VALUE33:[0-9]+]] read<i32>(%[[VALUE_x]])
// C89-NEXT:             default %[[VALUE33]]:
// C89-NEXT:                 let %[[VALUE34:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C89-NEXT:                 let %[[VALUE35:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE34]]), const<i32>(1));
// C89-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE35]]));
// C89-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// C89-NEXT:             {
// C89-NEXT:             }
// C89-NEXT:         else
// C89-NEXT:             ;
// C89-NEXT:     }
// C89-NEXT:     fn %[[VALUE_condition_typedef:[0-9]+]] @condition_typedef() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// C89-NEXT:         if ne<u64>(const<u64>(4), const<u64>(0))
// C89-NEXT:             const<u64>(4);
// C89-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// C89-NEXT:     }
// C89-NEXT:     fn %[[VALUE_body_typedef:[0-9]+]] @body_typedef() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// C89-NEXT:         if ne<i32>(const<i32>(1), const<i32>(0))
// C89-NEXT:             const<u64>(4);
// C89-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// C89-NEXT:     }
// C89-NEXT:     fn %[[VALUE_do_typedef:[0-9]+]] @do_typedef() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// C89-NEXT:         do %[[VALUE36:[0-9]+]]
// C89-NEXT:             const<u64>(4);
// C89-NEXT:         while ne<u64>(const<u64>(4), const<u64>(0));
// C89-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// C89-NEXT:     }
// C89-NEXT:     fn %[[VALUE_nested_typedef:[0-9]+]] @nested_typedef() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// C89-NEXT:         if ne<u64>(const<u64>(4), const<u64>(0))
// C89-NEXT:             {
// C89-NEXT:                 let %[[VALUE_x_2:[0-9]+]] x: i32 [storage=automatic];
// C89-NEXT:             }
// C89-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// C89-NEXT:     }
// C89-NEXT: }
// SLATE-FILECHECK-END C89
// SLATE-FILECHECK-BEGIN C99
// C99: module {
// C99-NEXT:     target "x86_64-unknown-linux-gnu" {
// C99-NEXT:         endian = little;
// C99-NEXT:         pointer [size=8, align=8];
// C99-NEXT:         stack_alignment = 16;
// C99-NEXT:         long_double = f80;
// C99-NEXT:         storage bool [size=1, align=1];
// C99-NEXT:         storage i8, u8 [size=1, align=1];
// C99-NEXT:         storage i16, u16 [size=2, align=2];
// C99-NEXT:         storage i32, u32 [size=4, align=4];
// C99-NEXT:         storage i64, u64 [size=8, align=8];
// C99-NEXT:         storage i128, u128 [size=16, align=16];
// C99-NEXT:         storage bf16 [size=2, align=2];
// C99-NEXT:         storage f16 [size=2, align=2];
// C99-NEXT:         storage f32 [size=4, align=4];
// C99-NEXT:         storage f64 [size=8, align=8];
// C99-NEXT:         storage f80 [size=16, align=16];
// C99-NEXT:         storage f128 [size=16, align=16];
// C99-NEXT:         storage d32 [size=4, align=4];
// C99-NEXT:         storage d64 [size=8, align=8];
// C99-NEXT:         storage d128 [size=16, align=16];
// C99-NEXT:     }
// C99-NEXT:     type @type[[TYPE_T:[0-9]+]] T = i32;
// C99-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// C99-NEXT:         %[[VALUE_T:[0-9]+]] T = const<i32>(1);
// C99-NEXT:     } [size=4, align=4];
// C99-NEXT:     type @type[[TYPE1:[0-9]+]] = enum : u32 {
// C99-NEXT:         %[[VALUE_T]] T = const<i32>(1);
// C99-NEXT:     } [size=4, align=4];
// C99-NEXT:     type @type[[TYPE2:[0-9]+]] = enum : u32 {
// C99-NEXT:         %[[VALUE_T]] T = const<i32>(1);
// C99-NEXT:     } [size=4, align=4];
// C99-NEXT:     type @type[[TYPE3:[0-9]+]] = enum : u32 {
// C99-NEXT:         %[[VALUE_T]] T = const<i32>(1);
// C99-NEXT:     } [size=4, align=4];
// C99-NEXT:     type @type[[TYPE_T_2:[0-9]+]] T = i32;
// C99-NEXT:     fn %[[VALUE_T]] @bodies(%[[VALUE_x:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// C99-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// C99-NEXT:             {
// C99-NEXT:                 let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C99-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// C99-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE1]]));
// C99-NEXT:             }
// C99-NEXT:         else
// C99-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C99-NEXT:             let %[[VALUE3:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// C99-NEXT:             write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE3]]));
// C99-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// C99-NEXT:             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C99-NEXT:             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// C99-NEXT:             write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE5]]));
// C99-NEXT:         else
// C99-NEXT:             {
// C99-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C99-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// C99-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE7]]));
// C99-NEXT:             }
// C99-NEXT:         while %[[VALUE8:[0-9]+]] ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// C99-NEXT:             {
// C99-NEXT:                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C99-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// C99-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE10]]));
// C99-NEXT:             }
// C99-NEXT:         while %[[VALUE11:[0-9]+]] ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// C99-NEXT:             let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C99-NEXT:             let %[[VALUE13:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE12]]), const<i32>(1));
// C99-NEXT:             write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE13]]));
// C99-NEXT:         do %[[VALUE14:[0-9]+]]
// C99-NEXT:             {
// C99-NEXT:                 let %[[VALUE15:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C99-NEXT:                 let %[[VALUE16:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE15]]), const<i32>(1));
// C99-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE16]]));
// C99-NEXT:             }
// C99-NEXT:         while ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0));
// C99-NEXT:         do %[[VALUE17:[0-9]+]]
// C99-NEXT:             let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C99-NEXT:             let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), const<i32>(1));
// C99-NEXT:             write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE19]]));
// C99-NEXT:         while ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0));
// C99-NEXT:         for %[[VALUE20:[0-9]+]]
// C99-NEXT:             init:
// C99-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// C99-NEXT:             increment: {
// C99-NEXT:                 let %[[VALUE21:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C99-NEXT:                 let %[[VALUE22:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE21]]), const<i32>(1));
// C99-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE22]]));
// C99-NEXT:                 yield void;
// C99-NEXT:             }
// C99-NEXT:             body:
// C99-NEXT:                 {
// C99-NEXT:                     let %[[VALUE23:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C99-NEXT:                     let %[[VALUE24:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE23]]), const<i32>(1));
// C99-NEXT:                     write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE24]]));
// C99-NEXT:                 }
// C99-NEXT:         for %[[VALUE25:[0-9]+]]
// C99-NEXT:             init:
// C99-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// C99-NEXT:             increment: {
// C99-NEXT:                 let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C99-NEXT:                 let %[[VALUE27:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE26]]), const<i32>(1));
// C99-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE27]]));
// C99-NEXT:                 yield void;
// C99-NEXT:             }
// C99-NEXT:             body:
// C99-NEXT:                 let %[[VALUE28:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C99-NEXT:                 let %[[VALUE29:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE28]]), const<i32>(1));
// C99-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE29]]));
// C99-NEXT:         switch %[[VALUE30:[0-9]+]] read<i32>(%[[VALUE_x]])
// C99-NEXT:             {
// C99-NEXT:                 case %[[VALUE30]] const<i32>(0):
// C99-NEXT:                     let %[[VALUE31:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C99-NEXT:                     let %[[VALUE32:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE31]]), const<i32>(1));
// C99-NEXT:                     write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE32]]));
// C99-NEXT:                 break %[[VALUE30]];
// C99-NEXT:             }
// C99-NEXT:         switch %[[VALUE33:[0-9]+]] read<i32>(%[[VALUE_x]])
// C99-NEXT:             default %[[VALUE33]]:
// C99-NEXT:                 let %[[VALUE34:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// C99-NEXT:                 let %[[VALUE35:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE34]]), const<i32>(1));
// C99-NEXT:                 write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE35]]));
// C99-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// C99-NEXT:             {
// C99-NEXT:             }
// C99-NEXT:         else
// C99-NEXT:             ;
// C99-NEXT:     }
// C99-NEXT:     fn %[[VALUE_condition_typedef:[0-9]+]] @condition_typedef() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// C99-NEXT:         if ne<u64>(const<u64>(4), const<u64>(0))
// C99-NEXT:             const<u64>(4);
// C99-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// C99-NEXT:     }
// C99-NEXT:     fn %[[VALUE_body_typedef:[0-9]+]] @body_typedef() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// C99-NEXT:         if ne<i32>(const<i32>(1), const<i32>(0))
// C99-NEXT:             const<u64>(4);
// C99-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// C99-NEXT:     }
// C99-NEXT:     fn %[[VALUE_do_typedef:[0-9]+]] @do_typedef() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// C99-NEXT:         do %[[VALUE36:[0-9]+]]
// C99-NEXT:             const<u64>(4);
// C99-NEXT:         while ne<u64>(const<u64>(4), const<u64>(0));
// C99-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// C99-NEXT:     }
// C99-NEXT:     fn %[[VALUE_nested_typedef:[0-9]+]] @nested_typedef() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// C99-NEXT:         if ne<u64>(const<u64>(4), const<u64>(0))
// C99-NEXT:             {
// C99-NEXT:                 let %[[VALUE_x_2:[0-9]+]] x: i32 [storage=automatic];
// C99-NEXT:             }
// C99-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(4)));
// C99-NEXT:     }
// C99-NEXT: }
// SLATE-FILECHECK-END C99
