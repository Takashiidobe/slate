
typedef struct st {
  int a;
} ST;

int __attribute__((noinline, noclone)) foo(ST *s, int c) {
  int first = 1;
  int count = c;
  ST *item  = s;
  int a     = s->a;
  int x;

  while (count--) {
    x = item->a;
    if (first)
      first = 0;
    else if (x >= a)
      return 1;
    a = x;
    item++;
  }
  return 0;
}

extern void abort(void);

int main() {
  ST _1[2] = {{2}, {1}};
  if (foo(_1, 2) != 0)
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
// DEFAULT-NEXT:     type @type0 st = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 ST = @type0;
// DEFAULT-NEXT:     fn %2 @foo(%3 s: ptr<@type0>, %4 c: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 first: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %6 count: i32 [storage=automatic] = read<i32>(%4);
// DEFAULT-NEXT:         let %7 item: ptr<@type0> [storage=automatic] = read<ptr<@type0>>(%3);
// DEFAULT-NEXT:         let %8 a: i32 [storage=automatic] = read<i32>(field0(deref(read<ptr<@type0>>(%3))));
// DEFAULT-NEXT:         let %9 x: i32 [storage=automatic];
// DEFAULT-NEXT:         while %13 {
// DEFAULT-NEXT:             let %14: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:             let %15: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%6, read<i32>(%15));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%14), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%9, read<i32>(field0(deref(read<ptr<@type0>>(%7)))));
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%5), const<i32>(0))
// DEFAULT-NEXT:                     write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if ge<i32>(read<i32>(%9), read<i32>(%8))
// DEFAULT-NEXT:                         return const<i32>(1);
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%9));
// DEFAULT-NEXT:                 let %16: ptr<@type0> [synthetic] = read<ptr<@type0>>(%7);
// DEFAULT-NEXT:                 let %17: ptr<@type0> [synthetic] = ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%16), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<@type0>>(%7, read<ptr<@type0>>(%17));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 _1: array<@type0, 2> [storage=automatic] = aggregate<array<@type0, 2>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(2)), index1 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type0>, i32) -> i32>(%2, array_decay<ptr<@type0>, length=Some(2)>(%12), const<i32>(2)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
