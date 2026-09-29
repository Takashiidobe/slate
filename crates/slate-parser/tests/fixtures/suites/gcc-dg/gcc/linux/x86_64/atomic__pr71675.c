/* PR c++/71675 - __atomic_compare_exchange_n returns wrong type for typed
   enum  */
/* { dg-do compile } */
/* { dg-options "-std=c11" } */


#define Test(T)								\
  do {									\
    static T x;								\
    int r [_Generic (__atomic_compare_exchange_n (&x, &x, x, 0, 0, 0),	\
		     _Bool: 1, default: -1)];				\
    (void)&r;								\
  } while (0)

void f (void)
{
  /* __atomic_compare_exchange_n would fail to return _Bool when
     its arguments were one of the three character types.  */
  Test (char);
  Test (signed char);
  Test (unsigned char);

  Test (int);
  Test (unsigned int);

  Test (long);
  Test (unsigned long);

  Test (long long);
  Test (unsigned long long);

  typedef enum E { e } E;
  Test (E);
}

// SLATE-FILECHECK-STD DEFAULT c11
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
// DEFAULT-NEXT:     type @type[[TYPE_E:[0-9]+]] E = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_e:[0-9]+]] e = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_E_2:[0-9]+]] E = @type[[TYPE_E]];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_2:[0-9]+]] x: i8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_3:[0-9]+]] x: u8 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_4:[0-9]+]] x: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_5:[0-9]+]] x: u32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_6:[0-9]+]] x: i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_7:[0-9]+]] x: u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_8:[0-9]+]] x: i64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_9:[0-9]+]] x: u64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x_10:[0-9]+]] x: @type[[TYPE_E]] [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_e]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_r:[0-9]+]] r: array<i32, 1> [storage=automatic];
// DEFAULT-NEXT:                 addr_of<ptr<array<i32, 1>>>(%[[VALUE_r]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_r_2:[0-9]+]] r: array<i32, 1> [storage=automatic];
// DEFAULT-NEXT:                 addr_of<ptr<array<i32, 1>>>(%[[VALUE_r_2]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_r_3:[0-9]+]] r: array<i32, 1> [storage=automatic];
// DEFAULT-NEXT:                 addr_of<ptr<array<i32, 1>>>(%[[VALUE_r_3]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_r_4:[0-9]+]] r: array<i32, 1> [storage=automatic];
// DEFAULT-NEXT:                 addr_of<ptr<array<i32, 1>>>(%[[VALUE_r_4]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_r_5:[0-9]+]] r: array<i32, 1> [storage=automatic];
// DEFAULT-NEXT:                 addr_of<ptr<array<i32, 1>>>(%[[VALUE_r_5]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_r_6:[0-9]+]] r: array<i32, 1> [storage=automatic];
// DEFAULT-NEXT:                 addr_of<ptr<array<i32, 1>>>(%[[VALUE_r_6]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_r_7:[0-9]+]] r: array<i32, 1> [storage=automatic];
// DEFAULT-NEXT:                 addr_of<ptr<array<i32, 1>>>(%[[VALUE_r_7]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_r_8:[0-9]+]] r: array<i32, 1> [storage=automatic];
// DEFAULT-NEXT:                 addr_of<ptr<array<i32, 1>>>(%[[VALUE_r_8]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_r_9:[0-9]+]] r: array<i32, 1> [storage=automatic];
// DEFAULT-NEXT:                 addr_of<ptr<array<i32, 1>>>(%[[VALUE_r_9]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_r_10:[0-9]+]] r: array<i32, 1> [storage=automatic];
// DEFAULT-NEXT:                 addr_of<ptr<array<i32, 1>>>(%[[VALUE_r_10]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
