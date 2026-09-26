void abort(void);
void exit(int);

int    i;
double d;

/* Make sure we return a constant.  */
float rootbeer[__builtin_types_compatible_p(int, typeof(i))];

typedef enum { hot, dog, poo, bear } dingos;
typedef enum { janette, laura, amanda } cranberry;

typedef float same1;
typedef float same2;

int main(void);

int main(void) {
  /* Compatible types.  */
  if (!(__builtin_types_compatible_p(int, const int) &&
        __builtin_types_compatible_p(typeof(hot), int) &&
        __builtin_types_compatible_p(typeof(hot), typeof(laura)) &&
        __builtin_types_compatible_p(int[5], int[]) &&
        __builtin_types_compatible_p(same1, same2)))
    abort();

  /* Incompatible types.  */
  if (__builtin_types_compatible_p(char *, int) ||
      __builtin_types_compatible_p(char *, const char *) ||
      __builtin_types_compatible_p(long double, double) ||
      __builtin_types_compatible_p(typeof(i), typeof(d)) ||
      __builtin_types_compatible_p(typeof(dingos), typeof(cranberry)) ||
      __builtin_types_compatible_p(char, int) ||
      __builtin_types_compatible_p(char *, char **))
    abort();
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
// DEFAULT-NEXT:     type @type0 = enum : u32 {
// DEFAULT-NEXT:         %0 hot = const<i32>(0);
// DEFAULT-NEXT:         %1 dog = const<i32>(1);
// DEFAULT-NEXT:         %2 poo = const<i32>(2);
// DEFAULT-NEXT:         %3 bear = const<i32>(3);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 dingos = @type0;
// DEFAULT-NEXT:     type @type2 = enum : u32 {
// DEFAULT-NEXT:         %0 janette = const<i32>(0);
// DEFAULT-NEXT:         %1 laura = const<i32>(1);
// DEFAULT-NEXT:         %2 amanda = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type3 cranberry = @type2;
// DEFAULT-NEXT:     type @type4 same1 = f32;
// DEFAULT-NEXT:     type @type5 same2 = f32;
// DEFAULT-NEXT:     global %2 i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 d: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 rootbeer: array<f32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%19 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %18 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if not<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<i32>(const<i32>(1), const<i32>(0)), ne<i32>(const<i32>(1), const<i32>(0))), ne<i32>(const<i32>(1), const<i32>(0))), ne<i32>(const<i32>(1), const<i32>(0))), ne<i32>(const<i32>(1), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(const<i32>(0), const<i32>(0)), ne<i32>(const<i32>(0), const<i32>(0))), ne<i32>(const<i32>(0), const<i32>(0))), ne<i32>(const<i32>(0), const<i32>(0))), ne<i32>(const<i32>(0), const<i32>(0))), ne<i32>(const<i32>(0), const<i32>(0))), ne<i32>(const<i32>(0), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
