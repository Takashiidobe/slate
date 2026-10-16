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

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/20527
// CHECK-NEXT: //! Mishandled postincrement.  This test-case is derived from the
// CHECK-NEXT: //! function BZ2_hbCreateDecodeTables in the file huffman.c from
// CHECK-NEXT: //! bzip2-1.0.2, hence requiring the following disclaimer copied here:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: // --
// CHECK-NEXT: //   This file is a part of bzip2 and/or libbzip2, a program and
// CHECK-NEXT: //   library for lossless, block-sorting data compression.
// CHECK-NEXT: //
// CHECK-NEXT: //   Copyright (C) 1996-2002 Julian R Seward.  All rights reserved.
// CHECK-NEXT: //
// CHECK-NEXT: //   Redistribution and use in source and binary forms, with or without
// CHECK-NEXT: //   modification, are permitted provided that the following conditions
// CHECK-NEXT: //   are met:
// CHECK-NEXT: //
// CHECK-NEXT: //   1. Redistributions of source code must retain the above copyright
// CHECK-NEXT: //      notice, this list of conditions and the following disclaimer.
// CHECK-NEXT: //
// CHECK-NEXT: //   2. The origin of this software must not be misrepresented; you must
// CHECK-NEXT: //      not claim that you wrote the original software.  If you use this
// CHECK-NEXT: //      software in a product, an acknowledgment in the product
// CHECK-NEXT: //      documentation would be appreciated but is not required.
// CHECK-NEXT: //
// CHECK-NEXT: //   3. Altered source versions must be plainly marked as such, and must
// CHECK-NEXT: //      not be misrepresented as being the original software.
// CHECK-NEXT: //
// CHECK-NEXT: //   4. The name of the author may not be used to endorse or promote
// CHECK-NEXT: //      products derived from this software without specific prior written
// CHECK-NEXT: //      permission.
// CHECK-NEXT: //
// CHECK-NEXT: //   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
// CHECK-NEXT: //   OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// CHECK-NEXT: //   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// CHECK-NEXT: //   ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
// CHECK-NEXT: //   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// CHECK-NEXT: //   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// CHECK-NEXT: //   GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// CHECK-NEXT: //   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// CHECK-NEXT: //   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// CHECK-NEXT: //   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// CHECK-NEXT: //   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// CHECK-NEXT: //
// CHECK-NEXT: //   Julian Seward, Cambridge, UK.
// CHECK-NEXT: //   jseward@acm.org
// CHECK-NEXT: //   bzip2/libbzip2 version 1.0 of 21 March 2000
// CHECK-NEXT: //
// CHECK-NEXT: //   This program is based on (at least) the work of:
// CHECK-NEXT: //      Mike Burrows
// CHECK-NEXT: //      David Wheeler
// CHECK-NEXT: //      Peter Fenwick
// CHECK-NEXT: //      Alistair Moffat
// CHECK-NEXT: //      Radford Neal
// CHECK-NEXT: //      Ian H. Witten
// CHECK-NEXT: //      Robert Sedgewick
// CHECK-NEXT: //      Jon L. Bentley
// CHECK-NEXT: //
// CHECK-NEXT: //   For more information on these sources, see the manual.
// CHECK-NEXT: // --
// CHECK-NEXT: fn f(mut limit: *mut i64, mut base: *mut i64, mut i: i64, mut maxLen: i64) {
// CHECK-NEXT:     let mut vec: i64 = (0 as i32) as i64;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i <= maxLen {
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i64 = vec;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i64 = [[V_1]]
// CHECK-NEXT:             + ((unsafe { *unsafe { base.offset((i + ((1 as i32) as i64)) as isize) } })
// CHECK-NEXT:                 - unsafe { *unsafe { base.offset(i as isize) } });
// CHECK-NEXT:         vec = [[V_2]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { limit.offset(i as isize) } = vec - ((1 as i32) as i64);
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i64 = i;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i64 = [[V_3]] + ((1 as i32) as i64);
// CHECK-NEXT:         i = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: __SlateAlign16<[i64; 4]> = __SlateAlign16([
// CHECK-NEXT:     (1 as i32) as i64,
// CHECK-NEXT:     (5 as i32) as i64,
// CHECK-NEXT:     (11 as i32) as i64,
// CHECK-NEXT:     (23 as i32) as i64,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut l: __SlateAlign16<[i64; 3]> = __SlateAlign16([0 as i64; 3]);
// CHECK-NEXT:     f(
// CHECK-NEXT:         l.0.as_mut_ptr() as *mut i64,
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(b.0) as *mut i64 },
// CHECK-NEXT:         (0 as i32) as i64,
// CHECK-NEXT:         (2 as i32) as i64,
// CHECK-NEXT:     );
// CHECK-NEXT:     if (unsafe { *unsafe { (l.0.as_mut_ptr() as *mut i64).offset((0 as i32) as isize) } })
// CHECK-NEXT:         != ((3 as i32) as i64)
// CHECK-NEXT:         || (unsafe { *unsafe { (l.0.as_mut_ptr() as *mut i64).offset((1 as i32) as isize) } })
// CHECK-NEXT:             != ((9 as i32) as i64)
// CHECK-NEXT:         || (unsafe { *unsafe { (l.0.as_mut_ptr() as *mut i64).offset((2 as i32) as isize) } })
// CHECK-NEXT:             != ((21 as i32) as i64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
