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
// DEFAULT-NEXT:     type @type[[TYPE_int3:[0-9]+]] int3 = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_int3_2:[0-9]+]] int3 = @type[[TYPE_int3]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_one:[0-9]+]] @one() -> @type[[TYPE_int3]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_int3]], reason=return>(read<@type[[TYPE_int3]]>(compound_literal %[[VALUE1:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_int3]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(1), field2 = const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_zero:[0-9]+]] @zero() -> @type[[TYPE_int3]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_int3]], reason=return>(read<@type[[TYPE_int3]]>(compound_literal %[[VALUE2:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_int3]], zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(0), field2 = const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: @type[[TYPE_int3]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<@type[[TYPE_int3]]> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: ptr<@type[[TYPE_int3]]> [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<@type[[TYPE_int3]], signature=fn() -> @type[[TYPE_int3]], abi=sysv64() -> native_c>(%[[VALUE_one]]);
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_int3]]>>(%[[VALUE4]], addr_of<ptr<@type[[TYPE_int3]]>>(%[[VALUE_a]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_int3]]>>(%[[VALUE3]], read<ptr<@type[[TYPE_int3]]>>(%[[VALUE4]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<@type[[TYPE_int3]]>(deref(read<ptr<@type[[TYPE_int3]]>>(%[[VALUE3]])), copy<@type[[TYPE_int3]], reason=assign>(call<@type[[TYPE_int3]], signature=fn() -> @type[[TYPE_int3]], abi=sysv64() -> native_c>(%[[VALUE_zero]])));
// DEFAULT-NEXT:         copy<@type[[TYPE_int3]], reason=assign>(call<@type[[TYPE_int3]], signature=fn() -> @type[[TYPE_int3]], abi=sysv64() -> native_c>(%[[VALUE_zero]]));
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(field0(%[[VALUE_a]])), const<i32>(0)), ne<i32>(read<i32>(field1(%[[VALUE_a]])), const<i32>(0))), ne<i32>(read<i32>(field2(%[[VALUE_a]])), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
