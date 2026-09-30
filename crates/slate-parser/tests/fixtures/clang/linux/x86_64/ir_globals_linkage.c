// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir

_Thread_local int tls;
static __thread int tls_counter = 1;
extern __thread int tls_extern __attribute__((tls_model("initial-exec")));

static int helper(void);
int helper(void) { return tls_counter; }
int proto(int value);
int proto(int value);

int before_block;
int reads_block_externs(void) {
  extern int before_block;
  extern int after_block;
  extern int later_function(int);
  static _Thread_local int per_thread;
  return before_block + after_block + later_function(per_thread);
}
int after_block = 2;
int later_function(int value) { return value; }

int shadowed = 3;
int reads_through_shadow(void) {
  int shadowed = 4;
  {
    extern int shadowed;
    return shadowed;
  }
}

static int private_object;
int links_private(void) {
  extern int private_object;
  return private_object;
}

extern int hidden_object __attribute__((visibility("hidden")));
int hidden_object = 5;
__attribute__((weak)) int weak_object;
int placed __attribute__((section(".data.placed"), used, retain)) = 6;
extern int aliased __attribute__((alias("placed")));
extern int renamed __asm__("renamed_symbol");
__attribute__((visibility("protected"))) void exported(void);
void weak_function(void) __attribute__((weak));

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
// DEFAULT-NEXT:     global %[[VALUE_tls:[0-9]+]] tls: i32 [storage=thread] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_tls_counter:[0-9]+]] tls_counter: i32 [storage=thread] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     extern %[[VALUE_tls_extern:[0-9]+]] tls_extern: i32 [storage=thread] [linkage=external] [tls_model=initial-exec];
// DEFAULT-NEXT:     global %[[VALUE_before_block:[0-9]+]] before_block: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_after_block:[0-9]+]] after_block: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_per_thread:[0-9]+]] per_thread: i32 [storage=thread] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_shadowed:[0-9]+]] shadowed: i32 [storage=static] = const<i32>(3) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_private_object:[0-9]+]] private_object: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_hidden_object:[0-9]+]] hidden_object: i32 [storage=static] = const<i32>(5) [linkage=external] [visibility=hidden];
// DEFAULT-NEXT:     global %[[VALUE_weak_object:[0-9]+]] weak_object: i32 [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     global %[[VALUE_placed:[0-9]+]] placed: i32 [storage=static] = const<i32>(6) [linkage=external] [section=".data.placed"] [used] [retain];
// DEFAULT-NEXT:     global %[[VALUE_aliased:[0-9]+]] aliased: i32 [storage=static] [linkage=external] [alias="placed"];
// DEFAULT-NEXT:     extern %[[VALUE_renamed:[0-9]+]] renamed: i32 [storage=static] [linkage=external] [asm_name="renamed_symbol"];
// DEFAULT-NEXT:     fn %[[VALUE_helper:[0-9]+]] @helper() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_tls_counter]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_proto:[0-9]+]] @proto(%[[VALUE_value:[0-9]+]] value: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_later_function:[0-9]+]] @later_function(%[[VALUE_value_2:[0-9]+]] value: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_value_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_reads_block_externs:[0-9]+]] @reads_block_externs() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%[[VALUE_before_block]]), read<i32>(%[[VALUE_after_block]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_later_function]], read<i32>(%[[VALUE_per_thread]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_reads_through_shadow:[0-9]+]] @reads_through_shadow() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_shadowed_2:[0-9]+]] shadowed: i32 [storage=automatic] = const<i32>(4);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             return read<i32>(%[[VALUE_shadowed]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_links_private:[0-9]+]] @links_private() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_private_object]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_exported:[0-9]+]] @exported() -> void [linkage=external] [visibility=protected];
// DEFAULT-NEXT:     fn %[[VALUE_weak_function:[0-9]+]] @weak_function() -> void [linkage=external] [weak];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
