/*
From: niles@fan745.gsfc.nasa.gov
To: fortran@gnu.org
Subject: Re: Scary problems in g77 for RedHat 6.0. (glibc-2.1)
Date: Sun, 06 Jun 1999 23:37:23 -0400
X-UIDL: 9c1e40c572e3b306464f703461764cd5
*/

/* { dg-xfail-if "Can not call system libm.a with -msoft-float" { powerpc-*-aix*
 * rs6000-*-aix* } { "-msoft-float" } { "" } } */

#include <math.h>
#include <stdio.h>

void abort(void);

int main() {
  if (floor(0.1) != 0.)
    abort();
  return 0;
}

/*
It will result in 36028797018963968.000000 on Alpha RedHat Linux 6.0
using glibc-2.1 at least on my 21064.  This may result in g77 bug
reports concerning the INT() function, just so you know.

        Thanks,
        Rick Niles.
*/

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! From: niles@fan745.gsfc.nasa.gov
// CHECK-NEXT: //! To: fortran@gnu.org
// CHECK-NEXT: //! Subject: Re: Scary problems in g77 for RedHat 6.0. (glibc-2.1)
// CHECK-NEXT: //! Date: Sun, 06 Jun 1999 23:37:23 -0400
// CHECK-NEXT: //! X-UIDL: 9c1e40c572e3b306464f703461764cd5
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn floor(__x: f64) -> f64;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-xfail-if "Can not call system libm.a with -msoft-float" { powerpc-*-aix*
// CHECK-NEXT: // rs6000-*-aix* } { "-msoft-float" } { "" } }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (unsafe { floor(0.1f64) }) != 0.0f64 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // It will result in 36028797018963968.000000 on Alpha RedHat Linux 6.0
// CHECK-NEXT: // using glibc-2.1 at least on my 21064.  This may result in g77 bug
// CHECK-NEXT: // reports concerning the INT() function, just so you know.
// CHECK-NEXT: //
// CHECK-NEXT: //         Thanks,
// CHECK-NEXT: //         Rick Niles.
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
