/* PR middle-end/44843 */
/* Verify that we don't use the alignment of struct S for inner accesses.  */

struct S {
  double for_alignment;
  struct {
    int x, y, z;
  } a[16];
};

void f(struct S *s) __attribute__((noinline));

void f(struct S *s) {
  unsigned int i;

  for (i = 0; i < 16; ++i) {
    s->a[i].x = 0;
    s->a[i].y = 0;
    s->a[i].z = 0;
  }
}

int main(void) {
  struct S s;
  f(&s);
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
// DEFAULT-NEXT:         field0 for_alignment: f64;
// DEFAULT-NEXT:         field1 a: array<@type1, 16>;
// DEFAULT-NEXT:     } [size=200, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type1 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:         field2 z: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     fn %3 @f(%4 s: ptr<@type0>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 i: u32 [storage=automatic];
// DEFAULT-NEXT:         for %9
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%5, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%5), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(16)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %10: u32 [synthetic] = read<u32>(%5);
// DEFAULT-NEXT:                 let %11: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%10), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%5, read<u32>(%11));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(field0(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(16)>(field1(deref(read<ptr<@type0>>(%4)))), read<u32>(%5)))), const<i32>(0));
// DEFAULT-NEXT:                     write<i32>(field1(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(16)>(field1(deref(read<ptr<@type0>>(%4)))), read<u32>(%5)))), const<i32>(0));
// DEFAULT-NEXT:                     write<i32>(field2(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(16)>(field1(deref(read<ptr<@type0>>(%4)))), read<u32>(%5)))), const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 s: @type0 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%3, addr_of<ptr<@type0>>(%7));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
