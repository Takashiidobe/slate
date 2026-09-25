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
// DEFAULT-NEXT:     type @type0 SEntry = struct {
// DEFAULT-NEXT:         field0 num: u8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type1 TEntry = @type0;
// DEFAULT-NEXT:     type @type2 STable = struct {
// DEFAULT-NEXT:         field0 data: array<@type0, 2>;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type3 TTable = @type2;
// DEFAULT-NEXT:     fn %0 @malloc(%19 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @free(%20 <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @init() -> ptr<@type2> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<@type2>, reason=return>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%0, const<u64>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @expect_func(%13 a: i32, %14 b: ptr<u8>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(__builtin_abs, from_bool<i32, reason=arg>(eq<i32>(read<i32>(%13), const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(__builtin_abs, from_bool<i32, reason=arg>(eq<ptr<u8>>(read<ptr<u8>>(%14), null<ptr<u8>>))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @inlined_wrong(%10 entry_p: ptr<@type0>, %11 flag: i32) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %12 index: u8 [storage=automatic];
// DEFAULT-NEXT:         write<u8>(field0(deref(read<ptr<@type0>>(%10))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%11), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         for %25
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u8>(%12, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:             condition: lt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%12))), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %26: u8 [synthetic] = read<u8>(%12);
// DEFAULT-NEXT:                 let %27: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%26))), const<i32>(1))));
// DEFAULT-NEXT:                 write<u8>(%12, read<u8>(%27));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %28: ptr<@type0> [synthetic] = read<ptr<@type0>>(%10);
// DEFAULT-NEXT:                 let %29: u8 [synthetic] = read<u8>(field0(deref(read<ptr<@type0>>(%28))));
// DEFAULT-NEXT:                 let %30: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%29))), const<i32>(1))));
// DEFAULT-NEXT:                 write<u8>(field0(deref(read<ptr<@type0>>(%28))), read<u8>(%30));
// DEFAULT-NEXT:         if not<bool>(ne<u8>(read<u8>(field0(deref(read<ptr<@type0>>(%10)))), const<u8>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %16 index: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %17 table_p: ptr<@type2> [storage=automatic] = call<ptr<@type2>, signature=fn() -> ptr<@type2>>(%7);
// DEFAULT-NEXT:         let %18 work: @type0 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%9, addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(field0(deref(read<ptr<@type2>>(%17)))), const<i32>(1)))), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<u8>) -> void>(%8, const<i32>(1), addr_of<ptr<u8>>(%16));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%9, addr_of<ptr<@type0>>(%18), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%2, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type2>>(%17)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
