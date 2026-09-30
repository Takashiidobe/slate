/* PR 50584 - No warning for passing small array to C99 static array declarator
   Exercise interaction between explicit attribute access and VLA parameters.
   { dg-do compile }
   { dg-options "-Wall" } */

#define RW(...) __attribute__ ((access (read_write, __VA_ARGS__)))


void f1 (int n, int[n], int);               // { dg-message "designating the bound of variable length array argument 2" "note" }

// Verify that a redundant attribute access doesn't trigger a warning.
RW (2, 1) void f1 (int n, int[n], int);

RW (2, 3) void f1 (int n, int[n], int);     // { dg-warning "attribute 'access\\\(read_write, 2, 3\\\)' positional argument 2 conflicts with previous designation by argument 1" }


/* Verify that applying the attribute to a VLA with an unspecified bound
   doesn't trigger any warnings, both with and without a size operand.  */
          void f2 (int, int[*], int);
RW (2)    void f2 (int, int[*], int);
RW (2, 3) void f2 (int, int[*], int);

/* Designating a parameter that comes before the VLA is the same as
   using the standard VLA int[n] syntax.  It might be worth issuing
   a portability warning suggesting to prefer the standard syntax.  */
          void f3 (int, int[*], int);
RW (2, 1) void f3 (int, int[*], int);

/* Designating a parameter that comes after the VLA cannot be expressed
   using the standard VLA int[n] syntax.  Verify it doesn't trigger
   a warning.  */
          void f4 (int, int[*], int);
RW (2, 3) void f4 (int, int[*], int);

/* Also verify the same on the same declaration.  */
          void f5 (int[*], int) RW (1, 2);
RW (1, 2) void f5 (int[*], int);
RW (1, 2) void f5 (int[*], int) RW (1, 2);


/* Verify that designating a VLA parameter with an explicit bound without
   also designating the same bound parameter triggers a warning (it has
   a different meaning).  */
       void f7 (int n, int[n]);         // { dg-message "21:note: designating the bound of variable length array argument 2" "note" }
RW (2) void f7 (int n, int[n]);         // { dg-warning "attribute 'access\\\(read_write, 2\\\)' missing positional argument 2 provided in previous designation by argument 1" }

          void f8 (int n, int[n]);
RW (2, 1) void f8 (int n, int[n]);


          void f9 (int, char[]);        // { dg-message "25:note: previously declared as an ordinary array 'char\\\[]'" note" }
RW (2)    void f9 (int n, char a[n])    // { dg-warning "argument 2 of type 'char\\\[n]' declared as a variable length array" }
                                        // { dg-warning "attribute 'access *\\\(read_write, 2\\\)' positional argument 2 missing in previous designation" "" { target *-*-* } .-1 }
                                        // { dg-message "24:note: designating the bound of variable length array argument 2" "note" { target *-*-* } .-2 }
{ (void)&n; (void)&a; }


          void f10 (int, char[]);       // { dg-message "26:note: previously declared as an ordinary array 'char\\\[]'" "note" }
RW (2, 1) void f10 (int n, char a[n])   // { dg-warning "attribute 'access *\\\(read_write, 2, 1\\\)' positional argument 2 missing in previous designation" "pr????" { xfail *-*-* } }
                                        // { dg-warning "argument 2 of type 'char\\\[n]' declared as a variable length array"  "" { target *-*-* } .-1 }
{ (void)&n; (void)&a; }

/* Verify that redeclaring a function with attribute access applying
   to an array parameter of any form is not diagnosed.  */
          void f12__ (int, int[]) RW (2, 1);
RW (2, 1) void f12__ (int, int[]);

          void f12_3 (int, int[3]) RW (2, 1);
RW (2, 1) void f12_3 (int, int[3]);

          void f12_n (int n, int[n]) RW (2, 1);
RW (2, 1) void f12_n (int n, int[n]);

          void f12_x (int, int[*]) RW (2, 1);
RW (2, 1) void f12_x (int, int[*]);

          void f13__ (int, int[]);
RW (2, 1) void f13__ (int, int[]);

          void f13_5 (int, int[5]);
RW (2, 1) void f13_5 (int, int[5]);

          void f13_n (int n, int[n]);
RW (2, 1) void f13_n (int n, int[n]);

          void f13_x (int, int[*]);
RW (2, 1) void f13_x (int, int[*]);

RW (2, 1) void f14__ (int, int[]);
          void f14__ (int, int[]);

RW (2, 1) void f14_7 (int, int[7]);
          void f14_7 (int, int[7]);

RW (2, 1) void f14_n (int n, int[n]);
          void f14_n (int n, int[n]);

RW (2, 1) void f14_x (int, int[*]);
          void f14_x (int, int[*]);

typedef void G1 (int n, int[n], int);

G1 g1;

/* The warning is about the attribute positional argument 2 which refers
   to the last function argument.  Ideally, the caret would be under
   the corresponding function argument, i.e., the last one here) but
   that location isn't available yet.  Verify that the caret doesn't
   point to function argument 1 which is the VLA bound (that's what
   the caret in the note points to).  */
RW (2, 3) void g1 (int n, int[n], int);     // { dg-warning "16: attribute 'access *\\\(read_write, 2, 3\\\)' positional argument 2 conflicts with previous designation by argument 3" }
// { dg-message "24:designating the bound of variable length array argument 2" "note" { target *-*-* } .-1 }

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type[[TYPE_G1:[0-9]+]] G1 = fn(i32, ptr<i32>, i32) -> void;
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_n:[0-9]+]] n: i32, %[[VALUE0:[0-9]+]] <unnamed>: ptr<i32> [array=*], %[[VALUE1:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE2:[0-9]+]] <unnamed>: i32, %[[VALUE3:[0-9]+]] <unnamed>: ptr<i32> [array=*], %[[VALUE4:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE5:[0-9]+]] <unnamed>: i32, %[[VALUE6:[0-9]+]] <unnamed>: ptr<i32> [array=*], %[[VALUE7:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE8:[0-9]+]] <unnamed>: i32, %[[VALUE9:[0-9]+]] <unnamed>: ptr<i32> [array=*], %[[VALUE10:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5(%[[VALUE11:[0-9]+]] <unnamed>: ptr<i32> [array=*], %[[VALUE12:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f7:[0-9]+]] @f7(%[[VALUE_n_2:[0-9]+]] n: i32, %[[VALUE13:[0-9]+]] <unnamed>: ptr<i32> [array=*]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f8:[0-9]+]] @f8(%[[VALUE_n_3:[0-9]+]] n: i32, %[[VALUE14:[0-9]+]] <unnamed>: ptr<i32> [array=*]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f9:[0-9]+]] @f9(%[[VALUE_n_4:[0-9]+]] n: i32, %[[VALUE_a:[0-9]+]] a: ptr<i8> [array=%[[VALUE15:[0-9]+]]]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE15]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_4]])));
// DEFAULT-NEXT:         addr_of<ptr<i32>>(%[[VALUE_n_4]]);
// DEFAULT-NEXT:         addr_of<ptr<ptr<i8>>>(%[[VALUE_a]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f10:[0-9]+]] @f10(%[[VALUE_n_5:[0-9]+]] n: i32, %[[VALUE_a_2:[0-9]+]] a: ptr<i8> [array=%[[VALUE16:[0-9]+]]]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE16]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_5]])));
// DEFAULT-NEXT:         addr_of<ptr<i32>>(%[[VALUE_n_5]]);
// DEFAULT-NEXT:         addr_of<ptr<ptr<i8>>>(%[[VALUE_a_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f12__:[0-9]+]] @f12__(%[[VALUE17:[0-9]+]] <unnamed>: i32, %[[VALUE18:[0-9]+]] <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f12_3:[0-9]+]] @f12_3(%[[VALUE19:[0-9]+]] <unnamed>: i32, %[[VALUE20:[0-9]+]] <unnamed>: ptr<i32> [array=3]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f12_n:[0-9]+]] @f12_n(%[[VALUE_n_6:[0-9]+]] n: i32, %[[VALUE21:[0-9]+]] <unnamed>: ptr<i32> [array=*]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f12_x:[0-9]+]] @f12_x(%[[VALUE22:[0-9]+]] <unnamed>: i32, %[[VALUE23:[0-9]+]] <unnamed>: ptr<i32> [array=*]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f13__:[0-9]+]] @f13__(%[[VALUE24:[0-9]+]] <unnamed>: i32, %[[VALUE25:[0-9]+]] <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f13_5:[0-9]+]] @f13_5(%[[VALUE26:[0-9]+]] <unnamed>: i32, %[[VALUE27:[0-9]+]] <unnamed>: ptr<i32> [array=5]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f13_n:[0-9]+]] @f13_n(%[[VALUE_n_7:[0-9]+]] n: i32, %[[VALUE28:[0-9]+]] <unnamed>: ptr<i32> [array=*]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f13_x:[0-9]+]] @f13_x(%[[VALUE29:[0-9]+]] <unnamed>: i32, %[[VALUE30:[0-9]+]] <unnamed>: ptr<i32> [array=*]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f14__:[0-9]+]] @f14__(%[[VALUE31:[0-9]+]] <unnamed>: i32, %[[VALUE32:[0-9]+]] <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f14_7:[0-9]+]] @f14_7(%[[VALUE33:[0-9]+]] <unnamed>: i32, %[[VALUE34:[0-9]+]] <unnamed>: ptr<i32> [array=7]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f14_n:[0-9]+]] @f14_n(%[[VALUE_n_8:[0-9]+]] n: i32, %[[VALUE35:[0-9]+]] <unnamed>: ptr<i32> [array=*]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f14_x:[0-9]+]] @f14_x(%[[VALUE36:[0-9]+]] <unnamed>: i32, %[[VALUE37:[0-9]+]] <unnamed>: ptr<i32> [array=*]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_g1:[0-9]+]] @g1(%[[VALUE38:[0-9]+]] <unnamed>: i32, %[[VALUE39:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE40:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
