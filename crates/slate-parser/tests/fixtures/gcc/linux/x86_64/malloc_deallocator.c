typedef __SIZE_TYPE__ size_t;
typedef struct stream stream;

void release(void *);
void close_at(int, stream *);

void *allocate(size_t) __attribute__((malloc, malloc(release)));
stream *open_stream(int) __attribute__((malloc(close_at, 2), malloc(__builtin_free)));
void *redeclared(size_t) __attribute__((malloc(release)));
void *redeclared(size_t) __attribute__((malloc(__builtin_free)));
void *ignored_index(size_t) __attribute__((malloc(close_at, 1)));
int not_a_pointer(void) __attribute__((malloc(release)));

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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_stream:[0-9]+]] stream = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_stream_2:[0-9]+]] stream = @type[[TYPE_stream]];
// DEFAULT-NEXT:     fn %[[VALUE_release:[0-9]+]] @release(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_close_at:[0-9]+]] @close_at(%[[VALUE1:[0-9]+]] <unnamed>: i32, %[[VALUE2:[0-9]+]] <unnamed>: ptr<@type[[TYPE_stream]]>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_allocate:[0-9]+]] @allocate(%[[VALUE3:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external] [deallocator=%[[VALUE_release]], argument=0];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_free:[0-9]+]] @__builtin_free(%[[VALUE4:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_open_stream:[0-9]+]] @open_stream(%[[VALUE5:[0-9]+]] <unnamed>: i32) -> ptr<@type[[TYPE_stream]]> [linkage=external] [deallocator=%[[VALUE_close_at]], argument=1] [deallocator=%[[VALUE___builtin_free]], argument=0];
// DEFAULT-NEXT:     fn %[[VALUE_redeclared:[0-9]+]] @redeclared(%[[VALUE6:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external] [deallocator=%[[VALUE_release]], argument=0] [deallocator=%[[VALUE___builtin_free]], argument=0];
// DEFAULT-NEXT:     fn %[[VALUE_ignored_index:[0-9]+]] @ignored_index(%[[VALUE7:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_not_a_pointer:[0-9]+]] @not_a_pointer() -> i32 [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
