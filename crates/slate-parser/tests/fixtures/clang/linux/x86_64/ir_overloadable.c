// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu11

struct spinlock { int v; };
struct mutex { long v; };

#define lock_helpers(name)                                                     \
  static inline void acquire(const struct name *lock)                          \
      __attribute__((overloadable)) { }                                        \
  static inline _Bool try_acquire(const struct name *lock, _Bool ret)          \
      __attribute__((overloadable)) { return ret; }

lock_helpers(spinlock)
lock_helpers(mutex)

void lock_both(struct spinlock *s, struct mutex *m) {
  acquire(s);
  (acquire)(m);
  try_acquire(m, 1);
}

static int pick(int x) __attribute__((overloadable)) { return 1; }
static int pick(double x) __attribute__((overloadable)) { return 2; }
static int pick(void *p) __attribute__((overloadable)) { return 3; }

int pick_char(void) { return pick('c'); }
int pick_float(void) { return pick(1.0f); }
int pick_pointer(char *s) { return pick(s); }
int pick_null(void) { return pick(0); }

static int arity(int x, ...) __attribute__((overloadable)) { return 1; }
static int arity(int x, int y) __attribute__((overloadable)) { return 2; }

int pick_arity(void) { return arity(1, 2) + arity(1, 2, 3); }

int plain(int x);
static int plain(double x) __attribute__((overloadable)) { return 2; }

int pick_plain(void) { return plain(1) + plain(1.0); }

static int later(int x) __attribute__((overloadable)) { return 1; }
int before_second(void) { return later(1.0); }
static int later(double x) __attribute__((overloadable)) { return 2; }
int after_second(void) { return later(1.0); }

static int only(int x) __attribute__((overloadable)) { return x; }
int (*only_pointer)(int) = only;

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_spinlock:[0-9]+]] spinlock = struct {
// IR-NEXT:         field0 v: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_mutex:[0-9]+]] mutex = struct {
// IR-NEXT:         field0 v: i64;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     global %[[VALUE_only_pointer:[0-9]+]] only_pointer: ptr<fn(i32) -> i32> [storage=static] = function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_only:[0-9]+]]) [linkage=external];
// IR-NEXT:     fn %[[VALUE_acquire:[0-9]+]] @acquire(%[[VALUE_lock:[0-9]+]] lock: ptr<const @type[[TYPE_spinlock]]>) -> void [linkage=internal] [inline=hint] [definition=emitted] [overloadable] [fallthrough=ret_void] {
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_try_acquire:[0-9]+]] @try_acquire(%[[VALUE_lock_2:[0-9]+]] lock: ptr<const @type[[TYPE_spinlock]]>, %[[VALUE_ret:[0-9]+]] ret: bool) -> bool [linkage=internal] [inline=hint] [definition=emitted] [overloadable] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<bool>(%[[VALUE_ret]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_acquire_2:[0-9]+]] @acquire(%[[VALUE_lock_3:[0-9]+]] lock: ptr<const @type[[TYPE_mutex]]>) -> void [linkage=internal] [inline=hint] [definition=emitted] [overloadable] [fallthrough=ret_void] {
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_try_acquire_2:[0-9]+]] @try_acquire(%[[VALUE_lock_4:[0-9]+]] lock: ptr<const @type[[TYPE_mutex]]>, %[[VALUE_ret_2:[0-9]+]] ret: bool) -> bool [linkage=internal] [inline=hint] [definition=emitted] [overloadable] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<bool>(%[[VALUE_ret_2]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_lock_both:[0-9]+]] @lock_both(%[[VALUE_s:[0-9]+]] s: ptr<@type[[TYPE_spinlock]]>, %[[VALUE_m:[0-9]+]] m: ptr<@type[[TYPE_mutex]]>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         call<void, signature=fn(ptr<const @type[[TYPE_spinlock]]>) -> void>(%[[VALUE_acquire]], pointer_cast<ptr<const @type[[TYPE_spinlock]]>, reason=arg>(read<ptr<@type[[TYPE_spinlock]]>>(%[[VALUE_s]])));
// IR-NEXT:         call<void, signature=fn(ptr<const @type[[TYPE_mutex]]>) -> void>(%[[VALUE_acquire_2]], pointer_cast<ptr<const @type[[TYPE_mutex]]>, reason=arg>(read<ptr<@type[[TYPE_mutex]]>>(%[[VALUE_m]])));
// IR-NEXT:         call<bool, signature=fn(ptr<const @type[[TYPE_mutex]]>, bool) -> bool>(%[[VALUE_try_acquire_2]], pointer_cast<ptr<const @type[[TYPE_mutex]]>, reason=arg>(read<ptr<@type[[TYPE_mutex]]>>(%[[VALUE_m]])), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_pick:[0-9]+]] @pick(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=internal] [overloadable] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_pick_2:[0-9]+]] @pick(%[[VALUE_x_2:[0-9]+]] x: f64) -> i32 [linkage=internal] [overloadable] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(2);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_pick_3:[0-9]+]] @pick(%[[VALUE_p:[0-9]+]] p: ptr<void>) -> i32 [linkage=internal] [overloadable] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(3);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_pick_char:[0-9]+]] @pick_char() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32) -> i32>(%[[VALUE_pick]], const<i32>(99));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_pick_float:[0-9]+]] @pick_float() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(f64) -> i32>(%[[VALUE_pick_2]], float_widen<f64, reason=arg>(const<f32>(1.0)));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_pick_pointer:[0-9]+]] @pick_pointer(%[[VALUE_s_2:[0-9]+]] s: ptr<i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(ptr<void>) -> i32>(%[[VALUE_pick_3]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_s_2]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_pick_null:[0-9]+]] @pick_null() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32) -> i32>(%[[VALUE_pick]], const<i32>(0));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_arity:[0-9]+]] @arity(%[[VALUE_x_3:[0-9]+]] x: i32, ...) -> i32 [linkage=internal] [overloadable] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_arity_2:[0-9]+]] @arity(%[[VALUE_x_4:[0-9]+]] x: i32, %[[VALUE_y:[0-9]+]] y: i32) -> i32 [linkage=internal] [overloadable] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(2);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_pick_arity:[0-9]+]] @pick_arity() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_arity_2]], const<i32>(1), const<i32>(2)), call<i32, signature=fn(i32, ...) -> i32>(%[[VALUE_arity]], const<i32>(1), const<i32>(2), const<i32>(3)));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_plain:[0-9]+]] @plain(%[[VALUE_x_5:[0-9]+]] x: i32) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_plain_2:[0-9]+]] @plain(%[[VALUE_x_6:[0-9]+]] x: f64) -> i32 [linkage=internal] [overloadable] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(2);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_pick_plain:[0-9]+]] @pick_plain() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_plain]], const<i32>(1)), call<i32, signature=fn(f64) -> i32>(%[[VALUE_plain_2]], const<f64>(1.0)));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_later:[0-9]+]] @later(%[[VALUE_x_7:[0-9]+]] x: i32) -> i32 [linkage=internal] [overloadable] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_before_second:[0-9]+]] @before_second() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32) -> i32>(%[[VALUE_later]], float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(const<f64>(1.0)));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_later_2:[0-9]+]] @later(%[[VALUE_x_8:[0-9]+]] x: f64) -> i32 [linkage=internal] [overloadable] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(2);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_after_second:[0-9]+]] @after_second() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(f64) -> i32>(%[[VALUE_later_2]], const<f64>(1.0));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_only]] @only(%[[VALUE_x_9:[0-9]+]] x: i32) -> i32 [linkage=internal] [overloadable] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(%[[VALUE_x_9]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
