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
// DEFAULT-NEXT:     type @type[[TYPE__zend_compiler_globals:[0-9]+]] _zend_compiler_globals = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type[[TYPE__zend_executor_globals:[0-9]+]] _zend_executor_globals = struct {
// DEFAULT-NEXT:         field0 uninitialized_zval_ptr: ptr<i32>;
// DEFAULT-NEXT:         field1 argument_stack: @type[[TYPE__zend_ptr_stack:[0-9]+]];
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_zend_executor_globals:[0-9]+]] zend_executor_globals = @type[[TYPE__zend_executor_globals]];
// DEFAULT-NEXT:     type @type[[TYPE__zend_ptr_stack]] _zend_ptr_stack = struct {
// DEFAULT-NEXT:         field0 top: i32;
// DEFAULT-NEXT:         field1 top_element: ptr<ptr<void>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_zend_ptr_stack:[0-9]+]] zend_ptr_stack = @type[[TYPE__zend_ptr_stack]];
// DEFAULT-NEXT:     extern %[[VALUE_compiler_globals:[0-9]+]] compiler_globals: @type[[TYPE__zend_compiler_globals]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_executor_globals:[0-9]+]] executor_globals: @type[[TYPE__zend_executor_globals]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_safe_free_zval_ptr:[0-9]+]] @safe_free_zval_ptr(%[[VALUE_p:[0-9]+]] p: ptr<i32>) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_p]]), read<ptr<i32>>(field0(%[[VALUE_executor_globals]])))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_zend_ptr_stack_clear_multiple:[0-9]+]] @zend_ptr_stack_clear_multiple() -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(field0(field1(%[[VALUE_executor_globals]])));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(field0(field1(%[[VALUE_executor_globals]])), read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
