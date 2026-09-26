/* PR tree-optimization/27285 */

extern void abort(void);

struct S {
  unsigned char a, b, c, d[16];
};

void __attribute__((noinline)) foo(struct S *x, struct S *y) {
  int           a, b;
  unsigned char c, *d, *e;

  b = x->b;
  d = x->d;
  e = y->d;
  a = 0;
  while (b) {
    if (b >= 8) {
      c  = 0xff;
      b -= 8;
    } else {
      c = 0xff << (8 - b);
      b = 0;
    }

    e[a] = d[a] & c;
    a++;
  }
}

int main(void) {
  struct S x = {0, 25, 0, {0xaa, 0xbb, 0xcc, 0xdd}};
  struct S y = {0, 0, 0, {0}};

  foo(&x, &y);
  if (x.d[0] != y.d[0] || x.d[1] != y.d[1] || x.d[2] != y.d[2] ||
      (x.d[3] & 0x80) != y.d[3])
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: u8;
// DEFAULT-NEXT:         field1 b: u8;
// DEFAULT-NEXT:         field2 c: u8;
// DEFAULT-NEXT:         field3 d: array<u8, 16>;
// DEFAULT-NEXT:     } [size=19, align=1, offsets=[0, 1, 2, 3]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 x: ptr<@type0>, %4 y: ptr<@type0>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 a: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 b: i32 [storage=automatic];
// DEFAULT-NEXT:         let %7 c: u8 [storage=automatic];
// DEFAULT-NEXT:         let %8 d: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %9 e: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%6, reinterpret<i32, reason=assign, fits=unknown>(widen<u32, reason=assign>(read<u8>(field1(deref(read<ptr<@type0>>(%3)))))));
// DEFAULT-NEXT:         write<ptr<u8>>(%8, array_decay<ptr<u8>, length=Some(16)>(field3(deref(read<ptr<@type0>>(%3)))));
// DEFAULT-NEXT:         write<ptr<u8>>(%9, array_decay<ptr<u8>, length=Some(16)>(field3(deref(read<ptr<@type0>>(%4)))));
// DEFAULT-NEXT:         write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:         while %13 ne<i32>(read<i32>(%6), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ge<i32>(read<i32>(%6), const<i32>(8))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<u8>(%7, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(255))));
// DEFAULT-NEXT:                         let %14: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                         let %15: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%14), const<i32>(8));
// DEFAULT-NEXT:                         write<i32>(%6, read<i32>(%15));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<u8>(%7, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(255), sub<i32, overflow=ub>(const<i32>(8), read<i32>(%6))))));
// DEFAULT-NEXT:                         write<i32>(%6, const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%9), read<i32>(%5))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%8), read<i32>(%5)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%7)))))));
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%17));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 x: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))), field1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(25))), field2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))), field3 = aggregate<array<u8, 16>, zero_fill=true>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(170))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(187))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(204))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(221)))));
// DEFAULT-NEXT:         let %12 y: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))), field1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))), field2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))), field3 = aggregate<array<u8, 16>, zero_fill=true>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, ptr<@type0>) -> void>(%2, addr_of<ptr<@type0>>(%11), addr_of<ptr<@type0>>(%12));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(16)>(field3(%11)), const<i32>(0)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(16)>(field3(%12)), const<i32>(0))))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(16)>(field3(%11)), const<i32>(1)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(16)>(field3(%12)), const<i32>(1)))))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(16)>(field3(%11)), const<i32>(2)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(16)>(field3(%12)), const<i32>(2)))))))), ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(16)>(field3(%11)), const<i32>(3)))))), const<i32>(128)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(16)>(field3(%12)), const<i32>(3))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
