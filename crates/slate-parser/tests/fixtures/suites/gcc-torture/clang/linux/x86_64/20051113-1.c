extern void  abort(void);
extern void *malloc(__SIZE_TYPE__);
extern void *memset(void *, int, __SIZE_TYPE__);
typedef struct {
  short              a;
  unsigned short     b;
  unsigned short     c;
  unsigned long long Count;
  long long          Count2;
} __attribute__((packed)) Struct1;

typedef struct {
  short              a;
  unsigned short     b;
  unsigned short     c;
  unsigned long long d;
  long long          e;
  long long          f;
} __attribute__((packed)) Struct2;

typedef union {
  Struct1 a;
  Struct2 b;
} Union;

typedef struct {
  int   Count;
  Union List[0];
} __attribute__((packed)) Struct3;

unsigned long long Sum(Struct3 *instrs) __attribute__((noinline));
unsigned long long Sum(Struct3 *instrs) {
  unsigned long long count = 0;
  int                i;

  for (i = 0; i < instrs->Count; i++) {
    count += instrs->List[i].a.Count;
  }
  return count;
}
long long Sum2(Struct3 *instrs) __attribute__((noinline));
long long Sum2(Struct3 *instrs) {
  long long count = 0;
  int       i;

  for (i = 0; i < instrs->Count; i++) {
    count += instrs->List[i].a.Count2;
  }
  return count;
}
int main(void) {
  Struct3 *p = malloc(sizeof(int) + 3 * sizeof(Union));
  memset(p, 0, sizeof(int) + 3 * sizeof(Union));
  p->Count            = 3;
  p->List[0].a.Count  = 555;
  p->List[1].a.Count  = 999;
  p->List[2].a.Count  = 0x101010101ULL;
  p->List[0].a.Count2 = 555;
  p->List[1].a.Count2 = 999;
  p->List[2].a.Count2 = 0x101010101LL;
  if (Sum(p) != 555 + 999 + 0x101010101ULL)
    abort();
  if (Sum2(p) != 555 + 999 + 0x101010101LL)
    abort();
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 a: i16;
// DEFAULT-NEXT:         field1 b: u16;
// DEFAULT-NEXT:         field2 c: u16;
// DEFAULT-NEXT:         field3 Count: u64;
// DEFAULT-NEXT:         field4 Count2: i64;
// DEFAULT-NEXT:     } [size=22, align=1, offsets=[0, 2, 4, 6, 14]];
// DEFAULT-NEXT:     type @type[[TYPE_Struct1:[0-9]+]] Struct1 = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 a: i16;
// DEFAULT-NEXT:         field1 b: u16;
// DEFAULT-NEXT:         field2 c: u16;
// DEFAULT-NEXT:         field3 d: u64;
// DEFAULT-NEXT:         field4 e: i64;
// DEFAULT-NEXT:         field5 f: i64;
// DEFAULT-NEXT:     } [size=30, align=1, offsets=[0, 2, 4, 6, 14, 22]];
// DEFAULT-NEXT:     type @type[[TYPE_Struct2:[0-9]+]] Struct2 = @type[[TYPE1]];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 a: @type[[TYPE0]];
// DEFAULT-NEXT:         field1 b: @type[[TYPE1]];
// DEFAULT-NEXT:     } [size=30, align=1, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_Union:[0-9]+]] Union = @type[[TYPE2]];
// DEFAULT-NEXT:     type @type[[TYPE3:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 Count: i32;
// DEFAULT-NEXT:         field1 List: array<@type[[TYPE2]], 0>;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_Struct3:[0-9]+]] Struct3 = @type[[TYPE3]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE0:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memset:[0-9]+]] @memset(%[[VALUE1:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE2:[0-9]+]] <unnamed>: i32, %[[VALUE3:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_Sum:[0-9]+]] @Sum(%[[VALUE_instrs:[0-9]+]] instrs: ptr<@type[[TYPE3]]>) -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_count:[0-9]+]] count: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(field0(deref(read<ptr<@type[[TYPE3]]>>(%[[VALUE_instrs]])))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE7:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_count]]);
// DEFAULT-NEXT:                     let %[[VALUE8:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE7]]), read<u64>(field3(field0(deref(ptr_offset<ptr<@type[[TYPE2]]>, subtract=false, element=@type[[TYPE2]], overflow=ub>(array_decay<ptr<@type[[TYPE2]]>, length=Some(0)>(field1(deref(read<ptr<@type[[TYPE3]]>>(%[[VALUE_instrs]])))), read<i32>(%[[VALUE_i]])))))));
// DEFAULT-NEXT:                     write<u64>(%[[VALUE_count]], read<u64>(%[[VALUE8]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_count]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Sum2:[0-9]+]] @Sum2(%[[VALUE_instrs_2:[0-9]+]] instrs: ptr<@type[[TYPE3]]>) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_count_2:[0-9]+]] count: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), read<i32>(field0(deref(read<ptr<@type[[TYPE3]]>>(%[[VALUE_instrs_2]])))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE12:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_count_2]]);
// DEFAULT-NEXT:                     let %[[VALUE13:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE12]]), read<i64>(field4(field0(deref(ptr_offset<ptr<@type[[TYPE2]]>, subtract=false, element=@type[[TYPE2]], overflow=ub>(array_decay<ptr<@type[[TYPE2]]>, length=Some(0)>(field1(deref(read<ptr<@type[[TYPE3]]>>(%[[VALUE_instrs_2]])))), read<i32>(%[[VALUE_i_2]])))))));
// DEFAULT-NEXT:                     write<i64>(%[[VALUE_count_2]], read<i64>(%[[VALUE13]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i64>(%[[VALUE_count_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE3]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE3]]>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], add<u64, overflow=wrap>(const<u64>(4), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), const<u64>(30)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE3]]>>(%[[VALUE_p]])), const<i32>(0), add<u64, overflow=wrap>(const<u64>(4), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), const<u64>(30))));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE3]]>>(%[[VALUE_p]]))), const<i32>(3));
// DEFAULT-NEXT:         write<u64>(field3(field0(deref(ptr_offset<ptr<@type[[TYPE2]]>, subtract=false, element=@type[[TYPE2]], overflow=ub>(array_decay<ptr<@type[[TYPE2]]>, length=Some(0)>(field1(deref(read<ptr<@type[[TYPE3]]>>(%[[VALUE_p]])))), const<i32>(0))))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(555))));
// DEFAULT-NEXT:         write<u64>(field3(field0(deref(ptr_offset<ptr<@type[[TYPE2]]>, subtract=false, element=@type[[TYPE2]], overflow=ub>(array_decay<ptr<@type[[TYPE2]]>, length=Some(0)>(field1(deref(read<ptr<@type[[TYPE3]]>>(%[[VALUE_p]])))), const<i32>(1))))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(999))));
// DEFAULT-NEXT:         write<u64>(field3(field0(deref(ptr_offset<ptr<@type[[TYPE2]]>, subtract=false, element=@type[[TYPE2]], overflow=ub>(array_decay<ptr<@type[[TYPE2]]>, length=Some(0)>(field1(deref(read<ptr<@type[[TYPE3]]>>(%[[VALUE_p]])))), const<i32>(2))))), const<u64>(4311810305));
// DEFAULT-NEXT:         write<i64>(field4(field0(deref(ptr_offset<ptr<@type[[TYPE2]]>, subtract=false, element=@type[[TYPE2]], overflow=ub>(array_decay<ptr<@type[[TYPE2]]>, length=Some(0)>(field1(deref(read<ptr<@type[[TYPE3]]>>(%[[VALUE_p]])))), const<i32>(0))))), widen<i64, reason=assign>(const<i32>(555)));
// DEFAULT-NEXT:         write<i64>(field4(field0(deref(ptr_offset<ptr<@type[[TYPE2]]>, subtract=false, element=@type[[TYPE2]], overflow=ub>(array_decay<ptr<@type[[TYPE2]]>, length=Some(0)>(field1(deref(read<ptr<@type[[TYPE3]]>>(%[[VALUE_p]])))), const<i32>(1))))), widen<i64, reason=assign>(const<i32>(999)));
// DEFAULT-NEXT:         write<i64>(field4(field0(deref(ptr_offset<ptr<@type[[TYPE2]]>, subtract=false, element=@type[[TYPE2]], overflow=ub>(array_decay<ptr<@type[[TYPE2]]>, length=Some(0)>(field1(deref(read<ptr<@type[[TYPE3]]>>(%[[VALUE_p]])))), const<i32>(2))))), const<i64>(4311810305));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<@type[[TYPE3]]>) -> u64>(%[[VALUE_Sum]], read<ptr<@type[[TYPE3]]>>(%[[VALUE_p]])), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(add<i32, overflow=ub>(const<i32>(555), const<i32>(999)))), const<u64>(4311810305)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(ptr<@type[[TYPE3]]>) -> i64>(%[[VALUE_Sum2]], read<ptr<@type[[TYPE3]]>>(%[[VALUE_p]])), add<i64, overflow=ub>(widen<i64, reason=usual_arith>(add<i32, overflow=ub>(const<i32>(555), const<i32>(999))), const<i64>(4311810305)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
