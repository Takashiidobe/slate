/* PR 17252.  When a char * pointer P takes its own address, storing
   into *P changes P itself.  */

void abort(void);

char *a;

int main(void) {
  /* Make 'a' point to itself.  */
  a = (char *)&a;

  /* Change what 'a' is pointing to.  */
  a[0]++;

  /* If a's memory tag does not contain 'a' in its alias set, we will
     think that this predicate is superfluous and change it to
     'if (1)'.  */
  if (a == (char *)&a)
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_a]], pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<ptr<i8>>>(%[[VALUE_a]])));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_a]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i8 [synthetic] = read<i8>(deref(read<ptr<i8>>(%[[VALUE0]])));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE1]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%[[VALUE0]])), read<i8>(%[[VALUE2]]));
// DEFAULT-NEXT:         if eq<ptr<i8>>(read<ptr<i8>>(%[[VALUE_a]]), pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<ptr<i8>>>(%[[VALUE_a]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
