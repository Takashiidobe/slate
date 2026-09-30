extern void *malloc(__SIZE_TYPE__);
extern void  abort(void);
extern void  free(void *);

typedef struct SEntry {
  unsigned char num;
} TEntry;

typedef struct STable {
  TEntry data[2];
} TTable;

TTable *init() { return malloc(sizeof(TTable)); }

void expect_func(int a, unsigned char *b) __attribute__((noinline));

static inline void inlined_wrong(TEntry *entry_p, int flag);

void inlined_wrong(TEntry *entry_p, int flag) {
  unsigned char index;
  entry_p->num = 0;

  if (flag == 0)
    abort();

  for (index = 0; index < 1; index++)
    entry_p->num++;

  if (!entry_p->num) {
    abort();
  }
}

void expect_func(int a, unsigned char *b) {
  if (__builtin_abs((a == 0)))
    abort();
  if (__builtin_abs((b == 0)))
    abort();
}

int main() {
  unsigned char index   = 0;
  TTable       *table_p = init();
  TEntry        work;

  inlined_wrong(&(table_p->data[1]), 1);
  expect_func(1, &index);
  inlined_wrong(&work, 1);

  free(table_p);

  return 0;
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
// DEFAULT-NEXT:     type @type[[TYPE_SEntry:[0-9]+]] SEntry = struct {
// DEFAULT-NEXT:         field0 num: u8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_TEntry:[0-9]+]] TEntry = @type[[TYPE_SEntry]];
// DEFAULT-NEXT:     type @type[[TYPE_STable:[0-9]+]] STable = struct {
// DEFAULT-NEXT:         field0 data: array<@type[[TYPE_SEntry]], 2>;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_TTable:[0-9]+]] TTable = @type[[TYPE_STable]];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE0:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_free:[0-9]+]] @free(%[[VALUE1:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_init:[0-9]+]] @init() -> ptr<@type[[TYPE_STable]]> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<@type[[TYPE_STable]]>, reason=return>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], const<u64>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_expect_func:[0-9]+]] @expect_func(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: ptr<u8>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_abs:[0-9]+]], from_bool<i32, reason=arg>(eq<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_abs]], from_bool<i32, reason=arg>(eq<ptr<u8>>(read<ptr<u8>>(%[[VALUE_b]]), null<ptr<u8>>))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_inlined_wrong:[0-9]+]] @inlined_wrong(%[[VALUE_entry_p:[0-9]+]] entry_p: ptr<@type[[TYPE_SEntry]]>, %[[VALUE_flag:[0-9]+]] flag: i32) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_index:[0-9]+]] index: u8 [storage=automatic];
// DEFAULT-NEXT:         write<u8>(field0(deref(read<ptr<@type[[TYPE_SEntry]]>>(%[[VALUE_entry_p]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_flag]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         for %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u8>(%[[VALUE_index]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:             condition: lt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_index]]))), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: u8 [synthetic] = read<u8>(%[[VALUE_index]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE3]]))), const<i32>(1))));
// DEFAULT-NEXT:                 write<u8>(%[[VALUE_index]], read<u8>(%[[VALUE4]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: ptr<@type[[TYPE_SEntry]]> [synthetic] = read<ptr<@type[[TYPE_SEntry]]>>(%[[VALUE_entry_p]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: u8 [synthetic] = read<u8>(field0(deref(read<ptr<@type[[TYPE_SEntry]]>>(%[[VALUE5]]))));
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE6]]))), const<i32>(1))));
// DEFAULT-NEXT:                 write<u8>(field0(deref(read<ptr<@type[[TYPE_SEntry]]>>(%[[VALUE5]]))), read<u8>(%[[VALUE7]]));
// DEFAULT-NEXT:         if not<bool>(ne<u8>(read<u8>(field0(deref(read<ptr<@type[[TYPE_SEntry]]>>(%[[VALUE_entry_p]])))), const<u8>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abs]] @__builtin_abs(%[[VALUE8:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_index_2:[0-9]+]] index: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_table_p:[0-9]+]] table_p: ptr<@type[[TYPE_STable]]> [storage=automatic] = call<ptr<@type[[TYPE_STable]]>, signature=fn() -> ptr<@type[[TYPE_STable]]>>(%[[VALUE_init]]);
// DEFAULT-NEXT:         let %[[VALUE_work:[0-9]+]] work: @type[[TYPE_SEntry]] [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_SEntry]]>, i32) -> void>(%[[VALUE_inlined_wrong]], addr_of<ptr<@type[[TYPE_SEntry]]>>(deref(ptr_offset<ptr<@type[[TYPE_SEntry]]>, subtract=false, element=@type[[TYPE_SEntry]], overflow=ub>(array_decay<ptr<@type[[TYPE_SEntry]]>, length=Some(2)>(field0(deref(read<ptr<@type[[TYPE_STable]]>>(%[[VALUE_table_p]])))), const<i32>(1)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<u8>) -> void>(%[[VALUE_expect_func]], const<i32>(1), addr_of<ptr<u8>>(%[[VALUE_index_2]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_SEntry]]>, i32) -> void>(%[[VALUE_inlined_wrong]], addr_of<ptr<@type[[TYPE_SEntry]]>>(%[[VALUE_work]]), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_STable]]>>(%[[VALUE_table_p]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
