// SLATE-FILECHECK-DEFINES DEFAULT

/* PR 10201 */

extern struct _zend_compiler_globals compiler_globals;
typedef struct _zend_executor_globals zend_executor_globals;
extern zend_executor_globals executor_globals;

typedef struct _zend_ptr_stack {
        int top;
        void **top_element;
} zend_ptr_stack;
struct _zend_compiler_globals {
};
struct _zend_executor_globals {
        int *uninitialized_zval_ptr;
        zend_ptr_stack argument_stack;
};

static inline void safe_free_zval_ptr(int *p)
{
        if (p!=(executor_globals.uninitialized_zval_ptr)) {
        }
}
zend_executor_globals executor_globals;
static inline void zend_ptr_stack_clear_multiple(void)
{
        executor_globals.argument_stack.top -= 2;
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
// DEFAULT-NEXT:     type @type0 _zend_compiler_globals = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type1 _zend_executor_globals = struct {
// DEFAULT-NEXT:         field0 uninitialized_zval_ptr: ptr<i32>;
// DEFAULT-NEXT:         field1 argument_stack: @type3;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type2 zend_executor_globals = @type1;
// DEFAULT-NEXT:     type @type3 _zend_ptr_stack = struct {
// DEFAULT-NEXT:         field0 top: i32;
// DEFAULT-NEXT:         field1 top_element: ptr<ptr<void>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type4 zend_ptr_stack = @type3;
// DEFAULT-NEXT:     extern %1 compiler_globals: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 executor_globals: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %7 @safe_free_zval_ptr(%8 p: ptr<i32>) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<ptr<i32>>(read<ptr<i32>>(%8), read<ptr<i32>>(field0(%4)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @zend_ptr_stack_clear_multiple() -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %10: i32 [synthetic] = read<i32>(field0(field1(%4)));
// DEFAULT-NEXT:         let %11: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%10), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(field0(field1(%4)), read<i32>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
