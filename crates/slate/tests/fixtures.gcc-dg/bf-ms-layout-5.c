/* PR target/52991 */
/* { dg-do run { target i?86-*-* x86_64-*-* } } */

struct S {
  int                              a : 2;
  __attribute__((aligned(8))) int  b : 2;
  int                              c : 28;
  __attribute__((aligned(16))) int d : 2;
  int                              e : 30;
} __attribute__((ms_struct));

struct S s;

int
// @lowering-fn-begin
// @rewrite-fn-begin
main() {
  int i;
  if (sizeof(s) != 32)
    __builtin_abort();
  s.a = -1;
  for (i = 0; i < 32; ++i)
    if (((char *)&s)[i] != (i ? 0 : 3))
      __builtin_abort();
  s.a = 0;
  s.b = -1;
  for (i = 0; i < 32; ++i)
    if (((char *)&s)[i] != (i ? 0 : 12))
      __builtin_abort();
  s.b = 0;
  s.c = -1;
  for (i = 0; i < 32; ++i)
    if (((signed char *)&s)[i] != (i > 3 ? 0 : (i ? -1 : -16)))
      __builtin_abort();
  s.c = 0;
  s.d = -1;
  for (i = 0; i < 32; ++i)
    if (((signed char *)&s)[i] != (i == 16 ? 3 : 0))
      __builtin_abort();
  s.d = 0;
  s.e = -1;
  for (i = 0; i < 32; ++i)
    if (((signed char *)&s)[i] !=
        ((i < 16 || i > 19) ? 0 : (i == 16 ? -4 : -1)))
      __builtin_abort();
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let mut i: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: u64 = 32;
// LOWERING-DAG:         let {{__v[0-9]+}}: u64 = 32;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) << 30 >> 30;
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         (*(&raw mut (*s)))
// LOWERING-DAG:             .__bitfield_0
// LOWERING-DAG:             .set_a(({{__v[0-9]+}} as i32) << 30 >> 30);
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         i = {{__v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:             if !{{__v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-DAG:             {
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-X86_64-GNU-DAG:                 let {{__v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!(*s) }) as *mut i8;
// LOWERING-X86_64-GNU-DAG:                 let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-X86_64-GNU-DAG:                 let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-DAG:                 let {{__v[0-9]+}}: *mut u8 = (unsafe { std::ptr::addr_of_mut!(*s) }) as *mut u8;
// LOWERING-AARCH64-GNU-DAG:                 let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-AARCH64-GNU-DAG:                 let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                 if {{__v[0-9]+}} {
// LOWERING-DAG:                     unsafe { abort() };
// LOWERING-DAG:                 }
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:             i = {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) << 30 >> 30;
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         (*(&raw mut (*s)))
// LOWERING-DAG:             .__bitfield_0
// LOWERING-DAG:             .set_a(({{__v[0-9]+}} as i32) << 30 >> 30);
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) << 30 >> 30;
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         (*(&raw mut (*s)))
// LOWERING-DAG:             .__bitfield_0
// LOWERING-DAG:             .set_b(({{__v[0-9]+}} as i32) << 30 >> 30);
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         i = {{__v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:             if !{{__v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-DAG:             {
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-X86_64-GNU-DAG:                 let {{__v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!(*s) }) as *mut i8;
// LOWERING-X86_64-GNU-DAG:                 let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-X86_64-GNU-DAG:                 let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-DAG:                 let {{__v[0-9]+}}: *mut u8 = (unsafe { std::ptr::addr_of_mut!(*s) }) as *mut u8;
// LOWERING-AARCH64-GNU-DAG:                 let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-AARCH64-GNU-DAG:                 let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 12;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                 if {{__v[0-9]+}} {
// LOWERING-DAG:                     unsafe { abort() };
// LOWERING-DAG:                 }
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:             i = {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) << 30 >> 30;
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         (*(&raw mut (*s)))
// LOWERING-DAG:             .__bitfield_0
// LOWERING-DAG:             .set_b(({{__v[0-9]+}} as i32) << 30 >> 30);
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) << 4 >> 4;
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         (*(&raw mut (*s)))
// LOWERING-DAG:             .__bitfield_0
// LOWERING-DAG:             .set_c(({{__v[0-9]+}} as i32) << 4 >> 4);
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         i = {{__v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:             if !{{__v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-DAG:             {
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!(*s) }) as *mut i8;
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                     {{__v[0-9]+}}
// LOWERING-DAG:                 } else {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = -16;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:                     {{__v[0-9]+}}
// LOWERING-DAG:                 };
// LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                 if {{__v[0-9]+}} {
// LOWERING-DAG:                     unsafe { abort() };
// LOWERING-DAG:                 }
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:             i = {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) << 4 >> 4;
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         (*(&raw mut (*s)))
// LOWERING-DAG:             .__bitfield_0
// LOWERING-DAG:             .set_c(({{__v[0-9]+}} as i32) << 4 >> 4);
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) << 30 >> 30;
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         (*(&raw mut (*s)))
// LOWERING-DAG:             .__bitfield_2
// LOWERING-DAG:             .set_d(({{__v[0-9]+}} as i32) << 30 >> 30);
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         i = {{__v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:             if !{{__v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-DAG:             {
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!(*s) }) as *mut i8;
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 16;
// LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                 if {{__v[0-9]+}} {
// LOWERING-DAG:                     unsafe { abort() };
// LOWERING-DAG:                 }
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:             i = {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) << 30 >> 30;
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         (*(&raw mut (*s)))
// LOWERING-DAG:             .__bitfield_2
// LOWERING-DAG:             .set_d(({{__v[0-9]+}} as i32) << 30 >> 30);
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) << 2 >> 2;
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         (*(&raw mut (*s)))
// LOWERING-DAG:             .__bitfield_2
// LOWERING-DAG:             .set_e(({{__v[0-9]+}} as i32) << 2 >> 2);
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         i = {{__v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:             if !{{__v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-DAG:             {
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!(*s) }) as *mut i8;
// LOWERING-DAG:                 let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 16;
// LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:                 let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:                     {{__v[0-9]+}}
// LOWERING-DAG:                 } else {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 19;
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// LOWERING-DAG:                     {{__v[0-9]+}}
// LOWERING-DAG:                 };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                     {{__v[0-9]+}}
// LOWERING-DAG:                 } else {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 16;
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = -4;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:                     {{__v[0-9]+}}
// LOWERING-DAG:                 };
// LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                 if {{__v[0-9]+}} {
// LOWERING-DAG:                     unsafe { abort() };
// LOWERING-DAG:                 }
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-DAG:             i = {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut i: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: u64 = 32;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = 32 != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { std::process::abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         (*(&raw mut (*s)))
// REWRITES-DAG:             .__bitfield_0
// REWRITES-DAG:             .set_a((-1 as i32) << 30 >> 30);
// REWRITES-DAG:     }
// REWRITES-DAG:     while i < 32 {
// REWRITES-DAG:         let {{__v[0-9]+}}: i64 = i as i64;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!(*s) }) as *mut i8;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: *mut u8 = (unsafe { std::ptr::addr_of_mut!(*s) }) as *mut u8;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 3;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = ((unsafe { *{{__v[0-9]+}} }) as i32) != if i != 0 { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// REWRITES-DAG:         if {{__v[0-9]+}} {
// REWRITES-DAG:             unsafe { std::process::abort() };
// REWRITES-DAG:         }
// REWRITES-DAG:         i += 1;
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         (*(&raw mut (*s)))
// REWRITES-DAG:             .__bitfield_0
// REWRITES-DAG:             .set_a((0 as i32) << 30 >> 30);
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         (*(&raw mut (*s)))
// REWRITES-DAG:             .__bitfield_0
// REWRITES-DAG:             .set_b((-1 as i32) << 30 >> 30);
// REWRITES-DAG:     }
// REWRITES-DAG:     i = 0;
// REWRITES-DAG:     while i < 32 {
// REWRITES-DAG:         let {{__v[0-9]+}}: i64 = i as i64;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!(*s) }) as *mut i8;
// REWRITES-X86_64-GNU-DAG:         let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: *mut u8 = (unsafe { std::ptr::addr_of_mut!(*s) }) as *mut u8;
// REWRITES-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 12;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = ((unsafe { *{{__v[0-9]+}} }) as i32) != if i != 0 { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// REWRITES-DAG:         if {{__v[0-9]+}} {
// REWRITES-DAG:             unsafe { std::process::abort() };
// REWRITES-DAG:         }
// REWRITES-DAG:         i += 1;
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         (*(&raw mut (*s)))
// REWRITES-DAG:             .__bitfield_0
// REWRITES-DAG:             .set_b((0 as i32) << 30 >> 30);
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         (*(&raw mut (*s))).__bitfield_0.set_c((-1 as i32) << 4 >> 4);
// REWRITES-DAG:     }
// REWRITES-DAG:     i = 0;
// REWRITES-DAG:     while i < 32 {
// REWRITES-DAG:         let {{__v[0-9]+}}: i64 = i as i64;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!(*s) }) as *mut i8;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = (unsafe { *{{__v[0-9]+}} }) as i32;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = if i > 3 {
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = -1;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = -16;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = if i != 0 { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         if {{__v[0-9]+}} {
// REWRITES-DAG:             unsafe { std::process::abort() };
// REWRITES-DAG:         }
// REWRITES-DAG:         i += 1;
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         (*(&raw mut (*s))).__bitfield_0.set_c((0 as i32) << 4 >> 4);
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         (*(&raw mut (*s)))
// REWRITES-DAG:             .__bitfield_2
// REWRITES-DAG:             .set_d((-1 as i32) << 30 >> 30);
// REWRITES-DAG:     }
// REWRITES-DAG:     i = 0;
// REWRITES-DAG:     while i < 32 {
// REWRITES-DAG:         let {{__v[0-9]+}}: i64 = i as i64;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!(*s) }) as *mut i8;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 3;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = ((unsafe { *{{__v[0-9]+}} }) as i32) != if i == 16 { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// REWRITES-DAG:         if {{__v[0-9]+}} {
// REWRITES-DAG:             unsafe { std::process::abort() };
// REWRITES-DAG:         }
// REWRITES-DAG:         i += 1;
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         (*(&raw mut (*s)))
// REWRITES-DAG:             .__bitfield_2
// REWRITES-DAG:             .set_d((0 as i32) << 30 >> 30);
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         (*(&raw mut (*s))).__bitfield_2.set_e((-1 as i32) << 2 >> 2);
// REWRITES-DAG:     }
// REWRITES-DAG:     i = 0;
// REWRITES-DAG:     while i < 32 {
// REWRITES-DAG:         let {{__v[0-9]+}}: i64 = i as i64;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!(*s) }) as *mut i8;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = (unsafe { *{{__v[0-9]+}} }) as i32;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = if i < 16 || i > 19 {
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = -4;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = -1;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = if i == 16 { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         if {{__v[0-9]+}} {
// REWRITES-DAG:             unsafe { std::process::abort() };
// REWRITES-DAG:         }
// REWRITES-DAG:         i += 1;
// REWRITES-DAG:     }
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
