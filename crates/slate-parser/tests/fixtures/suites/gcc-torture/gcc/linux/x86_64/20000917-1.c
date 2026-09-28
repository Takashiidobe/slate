/* This bug exists in gcc-2.95, egcs-1.1.2, gcc-2.7.2 and probably
   every other version as well.  */

void abort(void);
void exit(int);

typedef struct int3 {
  int a, b, c;
} int3;

int3 one(void) { return (int3){1, 1, 1}; }

int3 zero(void) { return (int3){0, 0, 0}; }

int main(void) {
  int3 a;

  /* gcc allocates a temporary for the inner expression statement
     to store the return value of `one'.

     gcc frees the temporaries for the inner expression statement.

     gcc realloates the same temporary slot to store the return
     value of `zero'.

     gcc expands the call to zero ahead of the expansion of the
     statement expressions.  The temporary gets the value of `zero'.

     gcc expands statement expressions and the stale temporary is
     clobbered with the value of `one'.  The bad value is copied from
     the temporary into *&a.  */

  *({
    ({
      one();
      &a;
    });
  }) = zero();
  if (a.a && a.b && a.c)
    abort();
  exit(0);
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
// DEFAULT-NEXT:     type @type0 int3 = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type1 int3 = @type0;
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%8 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @one() -> @type0 [linkage=external] [abi=sysv64() -> coerce<i64, i32>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(compound_literal %9 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(1), field2 = const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @zero() -> @type0 [linkage=external] [abi=sysv64() -> coerce<i64, i32>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(compound_literal %10 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(0), field2 = const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 a: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %11: ptr<@type0> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %12: ptr<@type0> [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<@type0, signature=fn() -> @type0, abi=sysv64() -> coerce<i64, i32>>(%4);
// DEFAULT-NEXT:                 write<ptr<@type0>>(%12, addr_of<ptr<@type0>>(%7));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<ptr<@type0>>(%11, read<ptr<@type0>>(%12));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<@type0>(deref(read<ptr<@type0>>(%11)), copy<@type0, reason=assign>(call<@type0, signature=fn() -> @type0, abi=sysv64() -> coerce<i64, i32>>(%5)));
// DEFAULT-NEXT:         copy<@type0, reason=assign>(call<@type0, signature=fn() -> @type0, abi=sysv64() -> coerce<i64, i32>>(%5));
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(field0(%7)), const<i32>(0)), ne<i32>(read<i32>(field1(%7)), const<i32>(0))), ne<i32>(read<i32>(field2(%7)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
