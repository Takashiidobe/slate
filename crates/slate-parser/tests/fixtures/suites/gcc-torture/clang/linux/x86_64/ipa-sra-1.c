/* Trivially making sure IPA-SRA does not introduce segfaults where they should
   not be.  */

struct bovid {
  float red;
  int   green;
  void *blue;
};

static int __attribute__((noinline)) ox(int fail, struct bovid *cow) {
  int r;
  if (fail)
    r = cow->red;
  else
    r = 0;
  return r;
}

int main(int argc, char *argv[]) {
  int r;

  r = ox((argc > 2000), (void *)0);
  return r;
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
// DEFAULT-NEXT:     type @type[[TYPE_bovid:[0-9]+]] bovid = struct {
// DEFAULT-NEXT:         field0 red: f32;
// DEFAULT-NEXT:         field1 green: i32;
// DEFAULT-NEXT:         field2 blue: ptr<void>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     fn %[[VALUE_ox:[0-9]+]] @ox(%[[VALUE_fail:[0-9]+]] fail: i32, %[[VALUE_cow:[0-9]+]] cow: ptr<@type[[TYPE_bovid]]>) -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_fail]]), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_r]], float_to_int<i32, reason=assign, out_of_range=ub, exceptions=ignore>(read<f32>(field0(deref(read<ptr<@type[[TYPE_bovid]]>>(%[[VALUE_cow]]))))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%[[VALUE_r]], const<i32>(0));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_r]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_2]], call<i32, signature=fn(i32, ptr<@type[[TYPE_bovid]]>) -> i32>(%[[VALUE_ox]], from_bool<i32, reason=arg>(gt<i32>(read<i32>(%[[VALUE_argc]]), const<i32>(2000))), null<ptr<@type[[TYPE_bovid]]>>));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ptr<@type[[TYPE_bovid]]>) -> i32>(%[[VALUE_ox]], from_bool<i32, reason=arg>(gt<i32>(read<i32>(%[[VALUE_argc]]), const<i32>(2000))), null<ptr<@type[[TYPE_bovid]]>>);
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_r_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
