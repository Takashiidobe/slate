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
// DEFAULT-NEXT:     type @type[[TYPE_not_used_on_fn_2:[0-9]+]] not_used_on_fn_2 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_placeholder:[0-9]+]] placeholder: @type[[TYPE_S]] [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_one:[0-9]+]] one: i32 [storage=static] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_const_one:[0-9]+]] const_one: i32 [storage=static] [const] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_correct_1:[0-9]+]] @correct_1(%[[VALUE0:[0-9]+]] <unnamed>: ptr<fn(ptr<i32>) -> void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_correct_2:[0-9]+]] @correct_2(%[[VALUE2:[0-9]+]] <unnamed>: ptr<fn(ptr<i32>, ptr<f64>) -> void>, %[[VALUE3:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE4:[0-9]+]] <unnamed>: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_correct_3:[0-9]+]] @correct_3(%[[VALUE5:[0-9]+]] <unnamed>: ptr<fn(ptr<i32>, ptr<f64>) -> void>, %[[VALUE6:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE7:[0-9]+]] <unnamed>: ptr<f64>, %[[VALUE8:[0-9]+]] <unnamed>: ptr<fn(ptr<void>) -> i32>, %[[VALUE9:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_unknown_1:[0-9]+]] @unknown_1(%[[VALUE10:[0-9]+]] <unnamed>: ptr<fn(ptr<i32>) -> void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_unknown_2:[0-9]+]] @unknown_2(%[[VALUE11:[0-9]+]] <unnamed>: ptr<fn(ptr<i32>, ptr<f64>) -> void>, %[[VALUE12:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE13:[0-9]+]] <unnamed>: ptr<f64>, %[[VALUE14:[0-9]+]] <unnamed>: ptr<i8>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_too_many:[0-9]+]] @too_many(%[[VALUE15:[0-9]+]] <unnamed>: ptr<fn(ptr<i32>, ptr<f64>) -> void>, %[[VALUE16:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE17:[0-9]+]] <unnamed>: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_too_few_1:[0-9]+]] @too_few_1(%[[VALUE18:[0-9]+]] <unnamed>: ptr<fn(ptr<i32>, ptr<f64>) -> void>, %[[VALUE19:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE20:[0-9]+]] <unnamed>: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_too_few_2:[0-9]+]] @too_few_2(%[[VALUE21:[0-9]+]] <unnamed>: ptr<fn(ptr<i32>, ptr<f64>) -> void>, %[[VALUE22:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE23:[0-9]+]] <unnamed>: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_promotion:[0-9]+]] @promotion(%[[VALUE24:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE25:[0-9]+]] <unnamed>: f32, %[[VALUE26:[0-9]+]] <unnamed>: ptr<fn(ptr<i32>) -> i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_downcast:[0-9]+]] @downcast(%[[VALUE27:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE28:[0-9]+]] <unnamed>: ptr<fn(ptr<f32>) -> ptr<void>>, %[[VALUE29:[0-9]+]] <unnamed>: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_out_of_range_1:[0-9]+]] @out_of_range_1(%[[VALUE30:[0-9]+]] <unnamed>: ptr<fn(ptr<f32>, ptr<f64>) -> i8>, %[[VALUE31:[0-9]+]] <unnamed>: ptr<f32>, %[[VALUE32:[0-9]+]] <unnamed>: ptr<f64>, %[[VALUE33:[0-9]+]] <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_out_of_range_2:[0-9]+]] @out_of_range_2(%[[VALUE34:[0-9]+]] <unnamed>: ptr<fn(ptr<f32>, ptr<f64>) -> i8>, %[[VALUE35:[0-9]+]] <unnamed>: ptr<f32>, %[[VALUE36:[0-9]+]] <unnamed>: ptr<f64>, %[[VALUE37:[0-9]+]] <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_out_of_range_3:[0-9]+]] @out_of_range_3(%[[VALUE38:[0-9]+]] <unnamed>: ptr<fn(ptr<f32>, ptr<f64>) -> i8>, %[[VALUE39:[0-9]+]] <unnamed>: ptr<f32>, %[[VALUE40:[0-9]+]] <unnamed>: ptr<f64>, %[[VALUE41:[0-9]+]] <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_out_of_range_4:[0-9]+]] @out_of_range_4(%[[VALUE42:[0-9]+]] <unnamed>: ptr<fn(ptr<f32>, ptr<f64>) -> i8>, %[[VALUE43:[0-9]+]] <unnamed>: ptr<f32>, %[[VALUE44:[0-9]+]] <unnamed>: ptr<f64>, %[[VALUE45:[0-9]+]] <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_unknown_fn:[0-9]+]] @unknown_fn(%[[VALUE46:[0-9]+]] <unnamed>: ptr<fn(ptr<f32>, ptr<f64>) -> i8>, %[[VALUE47:[0-9]+]] <unnamed>: ptr<f32>, %[[VALUE48:[0-9]+]] <unnamed>: ptr<f64>, %[[VALUE49:[0-9]+]] <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_not_a_fn:[0-9]+]] @not_a_fn(%[[VALUE50:[0-9]+]] <unnamed>: i32, %[[VALUE51:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_vararg_1:[0-9]+]] @vararg_1(%[[VALUE52:[0-9]+]] <unnamed>: ptr<fn(ptr<i32>) -> void>, %[[VALUE53:[0-9]+]] <unnamed>: ptr<i32>, ...) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_vararg_2:[0-9]+]] @vararg_2(%[[VALUE54:[0-9]+]] <unnamed>: ptr<fn(ptr<i32>, ...) -> void>, %[[VALUE55:[0-9]+]] <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_not_used_on_fn_1:[0-9]+]] @not_used_on_fn_1(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_incompatible_types_1:[0-9]+]] @incompatible_types_1(%[[VALUE56:[0-9]+]] <unnamed>: ptr<fn(ptr<@type[[TYPE_S]]>) -> void>, %[[VALUE57:[0-9]+]] <unnamed>: @type[[TYPE_S]]) -> void [linkage=external] [abi=sysv64(scalar, native_c) -> void];
// DEFAULT-NEXT:     fn %[[VALUE_incompatible_types_2:[0-9]+]] @incompatible_types_2(%[[VALUE58:[0-9]+]] <unnamed>: ptr<fn(ptr<@type[[TYPE_S]]>, ptr<i32>) -> void>, %[[VALUE59:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE60:[0-9]+]] <unnamed>: f64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_wrong_arg_type_1:[0-9]+]] @wrong_arg_type_1(%[[VALUE61:[0-9]+]] <unnamed>: ptr<fn(ptr<void>) -> void>, %[[VALUE62:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_wrong_arg_type_2:[0-9]+]] @wrong_arg_type_2(%[[VALUE63:[0-9]+]] <unnamed>: ptr<fn(ptr<void>, ptr<void>) -> void>, %[[VALUE64:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_wrong_arg_type_3:[0-9]+]] @wrong_arg_type_3(%[[VALUE65:[0-9]+]] <unnamed>: ptr<fn(ptr<void>, ptr<void>) -> void>, %[[VALUE66:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_int_identifier:[0-9]+]] @int_identifier(%[[VALUE67:[0-9]+]] <unnamed>: ptr<fn(ptr<void>, ptr<void>) -> void>, %[[VALUE68:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_int_identifier_1:[0-9]+]] @int_identifier_1(%[[VALUE69:[0-9]+]] <unnamed>: ptr<fn(ptr<void>, ptr<void>) -> void>, %[[VALUE70:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_multiple_single_fn:[0-9]+]] @multiple_single_fn(%[[VALUE71:[0-9]+]] <unnamed>: ptr<fn(ptr<i32>) -> void>, %[[VALUE72:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE73:[0-9]+]] <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ignore_1:[0-9]+]] @ignore_1(%[[VALUE74:[0-9]+]] <unnamed>: ptr<fn(ptr<i32>) -> void>, %[[VALUE75:[0-9]+]] <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ignore_2:[0-9]+]] @ignore_2(%[[VALUE76:[0-9]+]] <unnamed>: ptr<fn(ptr<i32>) -> void>, %[[VALUE77:[0-9]+]] <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ignore_3:[0-9]+]] @ignore_3(%[[VALUE78:[0-9]+]] <unnamed>: ptr<fn(ptr<i32>) -> void>, %[[VALUE79:[0-9]+]] <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
