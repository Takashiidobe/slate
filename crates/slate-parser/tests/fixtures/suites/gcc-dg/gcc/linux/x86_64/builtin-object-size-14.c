/* { dg-do run } */
/* { dg-options "-O2" } */

extern void abort (void);
extern char *strncpy(char *, const char *, __SIZE_TYPE__);

union u {
    struct {
	char vi[8];
	char pi[16];
    };
    char all[8+16+4];
};

void __attribute__((noinline,noclone))
f(union u *u)
{
  char vi[8+1];
  __builtin_strncpy(vi, u->vi, sizeof(u->vi));
  if (__builtin_object_size (u->all, 1) != -1)
    abort ();
}
int main()
{
  union u u;
  f (&u);
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
// DEFAULT-NEXT:     type @type[[TYPE_u:[0-9]+]] u = union {
// DEFAULT-NEXT:         field0 <anonymous>: @type[[TYPE0:[0-9]+]];
// DEFAULT-NEXT:         field1 all: array<i8, 28>;
// DEFAULT-NEXT:     } [size=28, align=1, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE0]] = struct {
// DEFAULT-NEXT:         field0 vi: array<i8, 8>;
// DEFAULT-NEXT:         field1 pi: array<i8, 16>;
// DEFAULT-NEXT:     } [size=24, align=1, offsets=[0, 8]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_strncpy:[0-9]+]] @strncpy(%[[VALUE0:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strncpy:[0-9]+]] @__builtin_strncpy(%[[VALUE3:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE4:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE5:[0-9]+]] <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_object_size:[0-9]+]] @__builtin_object_size(%[[VALUE6:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE7:[0-9]+]] <unnamed>: i32) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_u:[0-9]+]] u: ptr<@type[[TYPE_u]]>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_vi:[0-9]+]] vi: array<i8, 9> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE___builtin_strncpy]], array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_vi]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(field0(field0(deref(read<ptr<@type[[TYPE_u]]>>(%[[VALUE_u]])))))), const<u64>(8));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const void>, i32) -> u64>(%[[VALUE___builtin_object_size]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(28)>(field1(deref(read<ptr<@type[[TYPE_u]]>>(%[[VALUE_u]]))))), const<i32>(1)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_u_2:[0-9]+]] u: @type[[TYPE_u]] [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_u]]>) -> void>(%[[VALUE_f]], addr_of<ptr<@type[[TYPE_u]]>>(%[[VALUE_u_2]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
