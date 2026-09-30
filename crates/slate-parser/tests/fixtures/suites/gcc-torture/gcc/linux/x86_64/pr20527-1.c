/* PR rtl-optimization/20527
   Mishandled postincrement.  This test-case is derived from the
   function BZ2_hbCreateDecodeTables in the file huffman.c from
   bzip2-1.0.2, hence requiring the following disclaimer copied here:  */

/*--
  This file is a part of bzip2 and/or libbzip2, a program and
  library for lossless, block-sorting data compression.

  Copyright (C) 1996-2002 Julian R Seward.  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

  2. The origin of this software must not be misrepresented; you must
     not claim that you wrote the original software.  If you use this
     software in a product, an acknowledgment in the product
     documentation would be appreciated but is not required.

  3. Altered source versions must be plainly marked as such, and must
     not be misrepresented as being the original software.

  4. The name of the author may not be used to endorse or promote
     products derived from this software without specific prior written
     permission.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  Julian Seward, Cambridge, UK.
  jseward@acm.org
  bzip2/libbzip2 version 1.0 of 21 March 2000

  This program is based on (at least) the work of:
     Mike Burrows
     David Wheeler
     Peter Fenwick
     Alistair Moffat
     Radford Neal
     Ian H. Witten
     Robert Sedgewick
     Jon L. Bentley

  For more information on these sources, see the manual.
--*/

void f(long *limit, long *base, long minLen, long maxLen)
    __attribute__((__noinline__));
void f(long *limit, long *base, long minLen, long maxLen) {
  long i;
  long vec;
  vec = 0;
  for (i = minLen; i <= maxLen; i++) {
    vec      += (base[i + 1] - base[i]);
    limit[i]  = vec - 1;
  }
}
extern void abort(void);
extern void exit(int);
long        b[] = {1, 5, 11, 23};
int         main(void) {
  long l[3];
  f(l, b, 0, 2);
  if (l[0] != 3 || l[1] != 9 || l[2] != 21)
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
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: array<i64, 4> [storage=static] [align=16] = aggregate<array<i64, 4>, zero_fill=false>(index0 = widen<i64, reason=assign>(const<i32>(1)), index1 = widen<i64, reason=assign>(const<i32>(5)), index2 = widen<i64, reason=assign>(const<i32>(11)), index3 = widen<i64, reason=assign>(const<i32>(23))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_limit:[0-9]+]] limit: ptr<i64>, %[[VALUE_base:[0-9]+]] base: ptr<i64>, %[[VALUE_minLen:[0-9]+]] minLen: i64, %[[VALUE_maxLen:[0-9]+]] maxLen: i64) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_vec:[0-9]+]] vec: i64 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(%[[VALUE_vec]], widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_i]], read<i64>(%[[VALUE_minLen]]));
// DEFAULT-NEXT:             condition: le<i64>(read<i64>(%[[VALUE_i]]), read<i64>(%[[VALUE_maxLen]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE1]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_i]], read<i64>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_vec]]);
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE3]]), sub<i64, overflow=ub>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%[[VALUE_base]]), add<i64, overflow=ub>(read<i64>(%[[VALUE_i]]), widen<i64, reason=usual_arith>(const<i32>(1)))))), read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%[[VALUE_base]]), read<i64>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:                     write<i64>(%[[VALUE_vec]], read<i64>(%[[VALUE4]]));
// DEFAULT-NEXT:                     write<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%[[VALUE_limit]]), read<i64>(%[[VALUE_i]]))), sub<i64, overflow=ub>(read<i64>(%[[VALUE_vec]]), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE5:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_l:[0-9]+]] l: array<i64, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i64>, ptr<i64>, i64, i64) -> void>(%[[VALUE_f]], array_decay<ptr<i64>, length=Some(3)>(%[[VALUE_l]]), array_decay<ptr<i64>, length=Some(4)>(%[[VALUE_b]]), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(2)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(3)>(%[[VALUE_l]]), const<i32>(0)))), widen<i64, reason=usual_arith>(const<i32>(3))), ne<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(3)>(%[[VALUE_l]]), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(9)))), ne<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(3)>(%[[VALUE_l]]), const<i32>(2)))), widen<i64, reason=usual_arith>(const<i32>(21))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
