/* Test callback attribute error checking. */

/* { dg-do compile } */
/* { dg-options "-std=gnu17 -Wattributes" } */

[[gnu::callback_only(1, 2)]]
void
correct_1(void (*)(int*), int*);

[[gnu::callback_only(1, 2, 3)]]
void
correct_2(void (*)(int*, double*), int*, double*);

[[gnu::callback_only(1, 2, 3), gnu::callback_only(4, 5)]]
void
correct_3(void (*)(int*, double*), int*, double*, int (*)(void*), void*);

[[gnu::callback_only(1, 0)]]
void
unknown_1(void (*)(int*));

[[gnu::callback_only(1, 2, 0)]]
void
unknown_2(void (*)(int*, double*), int*, double*, char*);

[[gnu::callback_only(1, 0, 3, 3)]]
void
too_many(void (*)(int*, double*), int*, double*); /* { dg-warning "argument number mismatch, 2 expected, got 3" }*/

[[gnu::callback_only(1, 2)]]
void
too_few_1(void (*)(int*, double*), int*, double*); /* { dg-warning "argument number mismatch, 2 expected, got 1" }*/

[[gnu::callback_only(1)]]
void
too_few_2(void (*)(int*, double*), int*, double*); /* { dg-warning "argument number mismatch, 2 expected, got 0" }*/

[[gnu::callback_only(3, 1)]]
void
promotion(char*, float, int (*)(int*));

[[gnu::callback_only(2, 3)]]
void
downcast(char*, void* (*)(float*), double*);

[[gnu::callback_only(1, 2, 5)]]
void
out_of_range_1(char (*)(float*, double*), float*, double*, int*); /* { dg-warning "exceeds the number of function parameters" } */

[[gnu::callback_only(1, -2, 3)]]
void
out_of_range_2(char (*)(float*, double*), float*, double*, int*); /* { dg-warning "exceeds the number of function parameters" } */

[[gnu::callback_only(-1, 2, 3)]]
void
out_of_range_3(char (*)(float*, double*), float*, double*, int*);  /* { dg-warning "exceeds the number of function parameters" } */

[[gnu::callback_only(67, 2, 3)]]
void
out_of_range_4(char (*)(float*, double*), float*, double*, int*); /* { dg-warning "exceeds the number of function parameters" } */

[[gnu::callback_only(0, 2, 3)]]
void
unknown_fn(char (*)(float*, double*), float*, double*, int*); /* { dg-warning "callback function position cannot be marked as unknown" } */

[[gnu::callback_only(1, 2)]]
void
not_a_fn(int, int); /* { dg-warning "refers to" } */

[[gnu::callback_only(1, 2)]]
void
vararg_1(void (*)(int*), int*, ...); /* { dg-warning "cannot be used on variadic functions" } */

[[gnu::callback_only(1, 2)]]
void
vararg_2(void (*)(int*, ...), int*); /* { dg-warning "callback function cannot be variadic" } */

void
not_used_on_fn_1 ()
{
  __attribute__ ((callback_only (1))) int a = 1; /* { dg-warning "attribute can only be used on functions" } */
}

/* This warning is not issued by the attribute handler, rather by
   decl_attributes in attribs.cc.  Test it anyway.  */
struct __attribute__ ((callback_only (1))) not_used_on_fn_2
{
  int x;
}; /* { dg-warning "attribute does not apply to types" } */

struct S
{
  int x;
};

static struct S placeholder;

static int one = 1;

static const int const_one = 1;

[[gnu::callback_only(1, 2)]]
void
incompatible_types_1(void (*)(struct S*), struct S); /* { dg-warning "refers to" } */

[[gnu::callback_only(1, 3, 2)]]
void
incompatible_types_2(void (*)(struct S*, int*), int*, double); /* { dg-warning "refers to" } */

[[gnu::callback_only(1, "2")]]
void
wrong_arg_type_1(void (*)(void*), void*); /* { dg-warning "argument no. 1 is not an integer constant" } */

[[gnu::callback_only("not a number", 2, 2)]]
void
wrong_arg_type_2(void (*)(void*, void*), void*); /* { dg-warning "has type" } */

[[gnu::callback_only(placeholder, 2, 2)]]
void
wrong_arg_type_3(void (*)(void*, void*), void*); /* { dg-warning "has type" } */

[[gnu::callback_only(one, 2, 2)]]
void
int_identifier(void (*)(void*, void*), void*); /* { dg-warning "is not an integer constant" } */

[[gnu::callback_only(one, 2, 2)]]
void
int_identifier_1(void (*)(void*, void*), void*); /* { dg-warning "is not an integer constant" } */

[[gnu::callback_only(1, 2), gnu::callback_only(1, 3)]]
void
multiple_single_fn(void (*)(int*), int*, int*); /* { dg-warning "function declaration has multiple callback attributes describing argument no. 1" } */

/* Check that the attribute won't resolve outside of our namespace.  */

[[callback(1, 2)]] /* { dg-warning "ignored" } */
void
ignore_1(void (*)(int*), int*);

[[gnu::callback(1, 2)]]
void
ignore_2(void (*)(int*), int*); /* { dg-warning "ignored" } */

[[clang::callback_only(1, 2)]]
void
ignore_3(void (*)(int*), int*); /* { dg-warning "ignored" } */

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT gnu17
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
// DEFAULT-NEXT:     type @type0 not_used_on_fn_2 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 S = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %22 placeholder: @type1 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %23 one: i32 [storage=static] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     global %24 const_one: i32 [storage=static] [const] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @correct_1(%36 <unnamed>: ptr<fn(ptr<i32>) -> void>, %37 <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @correct_2(%38 <unnamed>: ptr<fn(ptr<i32>, ptr<f64>) -> void>, %39 <unnamed>: ptr<i32>, %40 <unnamed>: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @correct_3(%41 <unnamed>: ptr<fn(ptr<i32>, ptr<f64>) -> void>, %42 <unnamed>: ptr<i32>, %43 <unnamed>: ptr<f64>, %44 <unnamed>: ptr<fn(ptr<void>) -> i32>, %45 <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @unknown_1(%46 <unnamed>: ptr<fn(ptr<i32>) -> void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @unknown_2(%47 <unnamed>: ptr<fn(ptr<i32>, ptr<f64>) -> void>, %48 <unnamed>: ptr<i32>, %49 <unnamed>: ptr<f64>, %50 <unnamed>: ptr<i8>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @too_many(%51 <unnamed>: ptr<fn(ptr<i32>, ptr<f64>) -> void>, %52 <unnamed>: ptr<i32>, %53 <unnamed>: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @too_few_1(%54 <unnamed>: ptr<fn(ptr<i32>, ptr<f64>) -> void>, %55 <unnamed>: ptr<i32>, %56 <unnamed>: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @too_few_2(%57 <unnamed>: ptr<fn(ptr<i32>, ptr<f64>) -> void>, %58 <unnamed>: ptr<i32>, %59 <unnamed>: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %8 @promotion(%60 <unnamed>: ptr<i8>, %61 <unnamed>: f32, %62 <unnamed>: ptr<fn(ptr<i32>) -> i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %9 @downcast(%63 <unnamed>: ptr<i8>, %64 <unnamed>: ptr<fn(ptr<f32>) -> ptr<void>>, %65 <unnamed>: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %10 @out_of_range_1(%66 <unnamed>: ptr<fn(ptr<f32>, ptr<f64>) -> i8>, %67 <unnamed>: ptr<f32>, %68 <unnamed>: ptr<f64>, %69 <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %11 @out_of_range_2(%70 <unnamed>: ptr<fn(ptr<f32>, ptr<f64>) -> i8>, %71 <unnamed>: ptr<f32>, %72 <unnamed>: ptr<f64>, %73 <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %12 @out_of_range_3(%74 <unnamed>: ptr<fn(ptr<f32>, ptr<f64>) -> i8>, %75 <unnamed>: ptr<f32>, %76 <unnamed>: ptr<f64>, %77 <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %13 @out_of_range_4(%78 <unnamed>: ptr<fn(ptr<f32>, ptr<f64>) -> i8>, %79 <unnamed>: ptr<f32>, %80 <unnamed>: ptr<f64>, %81 <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %14 @unknown_fn(%82 <unnamed>: ptr<fn(ptr<f32>, ptr<f64>) -> i8>, %83 <unnamed>: ptr<f32>, %84 <unnamed>: ptr<f64>, %85 <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %15 @not_a_fn(%86 <unnamed>: i32, %87 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %16 @vararg_1(%88 <unnamed>: ptr<fn(ptr<i32>) -> void>, %89 <unnamed>: ptr<i32>, ...) -> void [linkage=external];
// DEFAULT-NEXT:     fn %17 @vararg_2(%90 <unnamed>: ptr<fn(ptr<i32>, ...) -> void>, %91 <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %18 @not_used_on_fn_1(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %19 a: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @incompatible_types_1(%92 <unnamed>: ptr<fn(ptr<@type1>) -> void>, %93 <unnamed>: @type1) -> void [linkage=external] [abi=sysv64(scalar, coerce<i32>) -> void];
// DEFAULT-NEXT:     fn %26 @incompatible_types_2(%94 <unnamed>: ptr<fn(ptr<@type1>, ptr<i32>) -> void>, %95 <unnamed>: ptr<i32>, %96 <unnamed>: f64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %27 @wrong_arg_type_1(%97 <unnamed>: ptr<fn(ptr<void>) -> void>, %98 <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %28 @wrong_arg_type_2(%99 <unnamed>: ptr<fn(ptr<void>, ptr<void>) -> void>, %100 <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %29 @wrong_arg_type_3(%101 <unnamed>: ptr<fn(ptr<void>, ptr<void>) -> void>, %102 <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %30 @int_identifier(%103 <unnamed>: ptr<fn(ptr<void>, ptr<void>) -> void>, %104 <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %31 @int_identifier_1(%105 <unnamed>: ptr<fn(ptr<void>, ptr<void>) -> void>, %106 <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %32 @multiple_single_fn(%107 <unnamed>: ptr<fn(ptr<i32>) -> void>, %108 <unnamed>: ptr<i32>, %109 <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %33 @ignore_1(%110 <unnamed>: ptr<fn(ptr<i32>) -> void>, %111 <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %34 @ignore_2(%112 <unnamed>: ptr<fn(ptr<i32>) -> void>, %113 <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %35 @ignore_3(%114 <unnamed>: ptr<fn(ptr<i32>) -> void>, %115 <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
