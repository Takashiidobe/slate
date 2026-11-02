// SLATE-FILECHECK-DEFINES DEFAULT

extern int __dummy (void *__preg, const char *__string);
extern int rpmatch (const char *response);

int
rpmatch (const char *response)
{
  auto inline int try (void *re);

  inline int try (void *re)
    {
      return __dummy (re, response);
    }
  static void *yesre;
  return (try (&yesre));
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
// DEFAULT-NEXT:     global %[[VALUE_yesre:[0-9]+]] yesre: ptr<void> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE___dummy:[0-9]+]] @__dummy(%[[VALUE___preg:[0-9]+]] __preg: ptr<void>, %[[VALUE___string:[0-9]+]] __string: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_rpmatch:[0-9]+]] @rpmatch(%[[VALUE_response:[0-9]+]] response: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         nested_fn %[[VALUE_try:[0-9]+]] @try(%[[VALUE_re:[0-9]+]] re: ptr<void>) -> i32 [captures=%[[VALUE_response]]: ptr<const i8>] {
// DEFAULT-NEXT:             return call<i32, signature=fn(ptr<void>, ptr<const i8>) -> i32>(%[[VALUE___dummy]], read<ptr<void>>(%[[VALUE_re]]), read<ptr<const i8>>(%[[VALUE_response]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<void>) -> i32>(%[[VALUE_try]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<ptr<void>>>(%[[VALUE_yesre]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
