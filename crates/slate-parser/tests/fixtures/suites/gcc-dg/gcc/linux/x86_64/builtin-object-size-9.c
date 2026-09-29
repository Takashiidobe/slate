/* { dg-do run } */
/* { dg-options "-O2" } */

typedef __SIZE_TYPE__ size_t;
extern void *malloc (size_t);
extern void free (void *);
extern void abort (void);

union U
{
  struct S { int a; int b; } s;
  int t;
};

struct T
{
  int c;
  char d[1];
};

int
main (void)
{
  union U *u = malloc (sizeof (struct S) + sizeof (struct T) + 6);
  struct T *t = (struct T *) (&u->s + 1);
  if (__builtin_object_size (t->d, 1)
      != sizeof (struct T) + 6 - __builtin_offsetof (struct T, d))
    abort ();
  free (u);
  return 0;
}

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type[[TYPE_U:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 s: @type[[TYPE_S:[0-9]+]];
// DEFAULT-NEXT:         field1 t: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_S]] S = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = struct {
// DEFAULT-NEXT:         field0 c: i32;
// DEFAULT-NEXT:         field1 d: array<i8, 1>;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE0:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_free:[0-9]+]] @free(%[[VALUE1:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_object_size:[0-9]+]] @__builtin_object_size(%[[VALUE2:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE3:[0-9]+]] <unnamed>: i32) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: ptr<@type[[TYPE_U]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_U]]>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), const<u64>(8)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6))))));
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: ptr<@type[[TYPE_T]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_T]]>, reason=explicit>(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(addr_of<ptr<@type[[TYPE_S]]>>(field0(deref(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_u]])))), const<i32>(1)));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(deref(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_t]]))))), const<i32>(1)), sub<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6)))), const<u64>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_U]]>>(%[[VALUE_u]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
