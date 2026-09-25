// SLATE-FILECHECK-DEFINES DEFAULT

struct { char *m; long n; } a[20];
int b = 20, c;
void bar(void) __attribute__((__noreturn__));

int
foo(int x)
{
  int i;

  for (i = 0; i < x; i++)
    {
      a[0].m = "a"; a[0].n = 10; c=1;
      a[c].m = "b"; a[c].n = 32; c++;
      if (c >= b) bar ();
      a[c].m = "c"; a[c].n = 80; c++;
      if (c >= b) bar ();
    }
  return 0;
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 m: ptr<i8>;
// DEFAULT-NEXT:         field1 n: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %1 a: array<@type0, 20> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 b: i32 [storage=static] = const<i32>(20) [linkage=external];
// DEFAULT-NEXT:     global %3 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %11 .str11: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([99, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %4 @bar() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @foo(%6 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %8
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%7), read<i32>(%6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %12: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%13));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<i8>>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(20)>(%1), const<i32>(0)))), array_decay<ptr<i8>, length=Some(2)>(%9));
// DEFAULT-NEXT:                     write<i64>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(20)>(%1), const<i32>(0)))), widen<i64, reason=assign>(const<i32>(10)));
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<i8>>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(20)>(%1), read<i32>(%3)))), array_decay<ptr<i8>, length=Some(2)>(%10));
// DEFAULT-NEXT:                     write<i64>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(20)>(%1), read<i32>(%3)))), widen<i64, reason=assign>(const<i32>(32)));
// DEFAULT-NEXT:                     let %14: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                     let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%3, read<i32>(%15));
// DEFAULT-NEXT:                     if ge<i32>(read<i32>(%3), read<i32>(%2))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:                     write<ptr<i8>>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(20)>(%1), read<i32>(%3)))), array_decay<ptr<i8>, length=Some(2)>(%11));
// DEFAULT-NEXT:                     write<i64>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(20)>(%1), read<i32>(%3)))), widen<i64, reason=assign>(const<i32>(80)));
// DEFAULT-NEXT:                     let %16: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                     let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%3, read<i32>(%17));
// DEFAULT-NEXT:                     if ge<i32>(read<i32>(%3), read<i32>(%2))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
