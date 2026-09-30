/* If some target has a Max alignment less than 32, please create
   a #ifdef around the alignment and add your alignment.  */
#ifdef __pdp11__
#define alignment 2
#else
#define alignment 32
#endif

void abort(void);
void exit(int);

typedef struct x {
  int a;
  int b;
} __attribute__((aligned(alignment))) X;
typedef struct y {
  X   x;
  X   y[31];
  int c;
} Y;

Y y[2];

int main(void) {
  if (((char *)&y[1] - (char *)&y[0]) & 31)
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
// DEFAULT-NEXT:     type @type[[TYPE_x:[0-9]+]] x = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_X:[0-9]+]] X = @type[[TYPE_x]];
// DEFAULT-NEXT:     type @type[[TYPE_y:[0-9]+]] y = struct {
// DEFAULT-NEXT:         field0 x: @type[[TYPE_x]];
// DEFAULT-NEXT:         field1 y: array<@type[[TYPE_x]], 31>;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:     } [size=1056, align=32, offsets=[0, 32, 1024]];
// DEFAULT-NEXT:     type @type[[TYPE_Y:[0-9]+]] Y = @type[[TYPE_y]];
// DEFAULT-NEXT:     global %[[VALUE_y:[0-9]+]] y: array<@type[[TYPE_y]], 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i64>(and<i64>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_y]]>>(deref(ptr_offset<ptr<@type[[TYPE_y]]>, subtract=false, element=@type[[TYPE_y]], overflow=ub>(array_decay<ptr<@type[[TYPE_y]]>, length=Some(2)>(%[[VALUE_y]]), const<i32>(1))))), pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_y]]>>(deref(ptr_offset<ptr<@type[[TYPE_y]]>, subtract=false, element=@type[[TYPE_y]], overflow=ub>(array_decay<ptr<@type[[TYPE_y]]>, length=Some(2)>(%[[VALUE_y]]), const<i32>(0)))))), widen<i64, reason=usual_arith>(const<i32>(31))), const<i64>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
