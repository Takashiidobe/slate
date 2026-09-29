// SLATE-FILECHECK-DEFINES DEFAULT

/* Test call to static variable.  */

/* { dg-require-effective-target indirect_calls } */

typedef struct
{
  long long a[10];
} A;
  
void bar (A *);
  
typedef int (*B)(int);
  
void foo (void)
{
  static A a;
  bar (&a);
  (*(B)&a) (1);
}

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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 a: array<i64, 10>;
// DEFAULT-NEXT:     } [size=80, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE_B:[0-9]+]] B = ptr<fn(i32) -> i32>;
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: @type[[TYPE0]] [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE0:[0-9]+]] <unnamed>: ptr<@type[[TYPE0]]>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE0]]>) -> void>(%[[VALUE_bar]], addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_a]]));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(pointer_cast<ptr<fn(i32) -> i32>, reason=explicit>(addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_a]])), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
