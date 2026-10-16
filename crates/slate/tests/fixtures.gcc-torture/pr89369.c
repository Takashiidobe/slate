/* PR target/89369 */

#if __SIZEOF_INT__ == 4 && __SIZEOF_LONG_LONG__ == 8 && __CHAR_BIT__ == 8
struct S {
  unsigned int u[4];
};

static void foo(struct S *out, struct S const *in, int shift) {
  unsigned long long th, tl, oh, ol;
  th         = ((unsigned long long)in->u[3] << 32) | in->u[2];
  tl         = ((unsigned long long)in->u[1] << 32) | in->u[0];
  oh         = th >> (shift * 8);
  ol         = tl >> (shift * 8);
  ol        |= th << (64 - shift * 8);
  out->u[1]  = ol >> 32;
  out->u[0]  = ol;
  out->u[3]  = oh >> 32;
  out->u[2]  = oh;
}

static void bar(struct S *out, struct S const *in, int shift) {
  unsigned long long th, tl, oh, ol;
  th         = ((unsigned long long)in->u[3] << 32) | in->u[2];
  tl         = ((unsigned long long)in->u[1] << 32) | in->u[0];
  oh         = th << (shift * 8);
  ol         = tl << (shift * 8);
  oh        |= tl >> (64 - shift * 8);
  out->u[1]  = ol >> 32;
  out->u[0]  = ol;
  out->u[3]  = oh >> 32;
  out->u[2]  = oh;
}

__attribute__((noipa)) static void baz(struct S *r, struct S *a, struct S *b,
                                       struct S *c, struct S *d) {
  struct S x, y;
  bar(&x, a, 1);
  foo(&y, c, 1);
  r->u[0] = a->u[0] ^ x.u[0] ^ ((b->u[0] >> 11) & 0xdfffffefU) ^ y.u[0] ^
            (d->u[0] << 18);
  r->u[1] = a->u[1] ^ x.u[1] ^ ((b->u[1] >> 11) & 0xddfecb7fU) ^ y.u[1] ^
            (d->u[1] << 18);
  r->u[2] = a->u[2] ^ x.u[2] ^ ((b->u[2] >> 11) & 0xbffaffffU) ^ y.u[2] ^
            (d->u[2] << 18);
  r->u[3] = a->u[3] ^ x.u[3] ^ ((b->u[3] >> 11) & 0xbffffff6U) ^ y.u[3] ^
            (d->u[3] << 18);
}

int main() {
  struct S a[] = {{0x000004d3, 0xbc5448db, 0xf22bde9f, 0xebb44f8f},
                  {0x03a32799, 0x60be8246, 0xa2d266ed, 0x7aa18536},
                  {0x15a38518, 0xcf655ce1, 0xf3e09994, 0x50ef69fe},
                  {0x88274b07, 0xe7c94866, 0xc0ea9f47, 0xb6a83c43},
                  {0xcd0d0032, 0x5d47f5d7, 0x5a0afbf6, 0xaea87b24},
                  {0, 0, 0, 0}};
  baz(&a[5], &a[0], &a[1], &a[2], &a[3]);
  if (a[4].u[0] != a[5].u[0] || a[4].u[1] != a[5].u[1] ||
      a[4].u[2] != a[5].u[2] || a[4].u[3] != a[5].u[3])
    __builtin_abort();
  return 0;
}
#else
int main() { return 0; }
#endif

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/89369
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     u: [u32; 4],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut out: &mut S, mut r#in: *const S, mut shift: i32) {
// CHECK-NEXT:     let mut th: u64 = ((unsafe {
// CHECK-NEXT:         *unsafe { unsafe { (*r#in).u.as_ptr() as *const u32 }.offset((3 as i32) as isize) }
// CHECK-NEXT:     }) as u64)
// CHECK-NEXT:         << (32 as i32)
// CHECK-NEXT:         | ((unsafe {
// CHECK-NEXT:             *unsafe { unsafe { (*r#in).u.as_ptr() as *const u32 }.offset((2 as i32) as isize) }
// CHECK-NEXT:         }) as u64);
// CHECK-NEXT:     let mut tl: u64 = ((unsafe {
// CHECK-NEXT:         *unsafe { unsafe { (*r#in).u.as_ptr() as *const u32 }.offset((1 as i32) as isize) }
// CHECK-NEXT:     }) as u64)
// CHECK-NEXT:         << (32 as i32)
// CHECK-NEXT:         | ((unsafe {
// CHECK-NEXT:             *unsafe { unsafe { (*r#in).u.as_ptr() as *const u32 }.offset((0 as i32) as isize) }
// CHECK-NEXT:         }) as u64);
// CHECK-NEXT:     let mut oh: u64 = th >> shift * (8 as i32);
// CHECK-NEXT:     let mut ol: u64 = tl >> shift * (8 as i32);
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u64 = ol;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u64 = [[V_0]] | th << (64 as i32) - shift * (8 as i32);
// CHECK-NEXT:     ol = [[V_1]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*(out as *mut S)).u.as_mut_ptr() as *mut u32 }.offset((1 as i32) as isize)
// CHECK-NEXT:         } = (ol >> (32 as i32)) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*(out as *mut S)).u.as_mut_ptr() as *mut u32 }.offset((0 as i32) as isize)
// CHECK-NEXT:         } = ol as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*(out as *mut S)).u.as_mut_ptr() as *mut u32 }.offset((3 as i32) as isize)
// CHECK-NEXT:         } = (oh >> (32 as i32)) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*(out as *mut S)).u.as_mut_ptr() as *mut u32 }.offset((2 as i32) as isize)
// CHECK-NEXT:         } = oh as u32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut out: &mut S, mut r#in: *const S, mut shift: i32) {
// CHECK-NEXT:     let mut th: u64 = ((unsafe {
// CHECK-NEXT:         *unsafe { unsafe { (*r#in).u.as_ptr() as *const u32 }.offset((3 as i32) as isize) }
// CHECK-NEXT:     }) as u64)
// CHECK-NEXT:         << (32 as i32)
// CHECK-NEXT:         | ((unsafe {
// CHECK-NEXT:             *unsafe { unsafe { (*r#in).u.as_ptr() as *const u32 }.offset((2 as i32) as isize) }
// CHECK-NEXT:         }) as u64);
// CHECK-NEXT:     let mut tl: u64 = ((unsafe {
// CHECK-NEXT:         *unsafe { unsafe { (*r#in).u.as_ptr() as *const u32 }.offset((1 as i32) as isize) }
// CHECK-NEXT:     }) as u64)
// CHECK-NEXT:         << (32 as i32)
// CHECK-NEXT:         | ((unsafe {
// CHECK-NEXT:             *unsafe { unsafe { (*r#in).u.as_ptr() as *const u32 }.offset((0 as i32) as isize) }
// CHECK-NEXT:         }) as u64);
// CHECK-NEXT:     let mut oh: u64 = th << shift * (8 as i32);
// CHECK-NEXT:     let mut ol: u64 = tl << shift * (8 as i32);
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u64 = oh;
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u64 = [[V_2]] | tl >> (64 as i32) - shift * (8 as i32);
// CHECK-NEXT:     oh = [[V_3]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*(out as *mut S)).u.as_mut_ptr() as *mut u32 }.offset((1 as i32) as isize)
// CHECK-NEXT:         } = (ol >> (32 as i32)) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*(out as *mut S)).u.as_mut_ptr() as *mut u32 }.offset((0 as i32) as isize)
// CHECK-NEXT:         } = ol as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*(out as *mut S)).u.as_mut_ptr() as *mut u32 }.offset((3 as i32) as isize)
// CHECK-NEXT:         } = (oh >> (32 as i32)) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*(out as *mut S)).u.as_mut_ptr() as *mut u32 }.offset((2 as i32) as isize)
// CHECK-NEXT:         } = oh as u32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz(mut r: *mut S, mut a: *mut S, mut b: *mut S, mut c: *mut S, mut d: *mut S) {
// CHECK-NEXT:     let mut x: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut y: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     bar(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(x)) },
// CHECK-NEXT:         a as *const S,
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     foo(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(y)) },
// CHECK-NEXT:         c as *const S,
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { (*r).u.as_mut_ptr() as *mut u32 }.offset((0 as i32) as isize) } = (unsafe {
// CHECK-NEXT:             *unsafe { unsafe { (*a).u.as_mut_ptr() as *mut u32 }.offset((0 as i32) as isize) }
// CHECK-NEXT:         }) ^ unsafe {
// CHECK-NEXT:             *unsafe { (x.u.as_mut_ptr() as *mut u32).offset((0 as i32) as isize) }
// CHECK-NEXT:         } ^ (unsafe {
// CHECK-NEXT:             *unsafe { unsafe { (*b).u.as_mut_ptr() as *mut u32 }.offset((0 as i32) as isize) }
// CHECK-NEXT:         })
// CHECK-NEXT:             >> (11 as i32)
// CHECK-NEXT:             & (3758096367 as u32)
// CHECK-NEXT:             ^ unsafe { *unsafe { (y.u.as_mut_ptr() as *mut u32).offset((0 as i32) as isize) } }
// CHECK-NEXT:             ^ (unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { (*d).u.as_mut_ptr() as *mut u32 }.offset((0 as i32) as isize) }
// CHECK-NEXT:             }) << (18 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { (*r).u.as_mut_ptr() as *mut u32 }.offset((1 as i32) as isize) } = (unsafe {
// CHECK-NEXT:             *unsafe { unsafe { (*a).u.as_mut_ptr() as *mut u32 }.offset((1 as i32) as isize) }
// CHECK-NEXT:         }) ^ unsafe {
// CHECK-NEXT:             *unsafe { (x.u.as_mut_ptr() as *mut u32).offset((1 as i32) as isize) }
// CHECK-NEXT:         } ^ (unsafe {
// CHECK-NEXT:             *unsafe { unsafe { (*b).u.as_mut_ptr() as *mut u32 }.offset((1 as i32) as isize) }
// CHECK-NEXT:         })
// CHECK-NEXT:             >> (11 as i32)
// CHECK-NEXT:             & (3724462975 as u32)
// CHECK-NEXT:             ^ unsafe { *unsafe { (y.u.as_mut_ptr() as *mut u32).offset((1 as i32) as isize) } }
// CHECK-NEXT:             ^ (unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { (*d).u.as_mut_ptr() as *mut u32 }.offset((1 as i32) as isize) }
// CHECK-NEXT:             }) << (18 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { (*r).u.as_mut_ptr() as *mut u32 }.offset((2 as i32) as isize) } = (unsafe {
// CHECK-NEXT:             *unsafe { unsafe { (*a).u.as_mut_ptr() as *mut u32 }.offset((2 as i32) as isize) }
// CHECK-NEXT:         }) ^ unsafe {
// CHECK-NEXT:             *unsafe { (x.u.as_mut_ptr() as *mut u32).offset((2 as i32) as isize) }
// CHECK-NEXT:         } ^ (unsafe {
// CHECK-NEXT:             *unsafe { unsafe { (*b).u.as_mut_ptr() as *mut u32 }.offset((2 as i32) as isize) }
// CHECK-NEXT:         })
// CHECK-NEXT:             >> (11 as i32)
// CHECK-NEXT:             & (3220897791 as u32)
// CHECK-NEXT:             ^ unsafe { *unsafe { (y.u.as_mut_ptr() as *mut u32).offset((2 as i32) as isize) } }
// CHECK-NEXT:             ^ (unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { (*d).u.as_mut_ptr() as *mut u32 }.offset((2 as i32) as isize) }
// CHECK-NEXT:             }) << (18 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { unsafe { (*r).u.as_mut_ptr() as *mut u32 }.offset((3 as i32) as isize) } = (unsafe {
// CHECK-NEXT:             *unsafe { unsafe { (*a).u.as_mut_ptr() as *mut u32 }.offset((3 as i32) as isize) }
// CHECK-NEXT:         }) ^ unsafe {
// CHECK-NEXT:             *unsafe { (x.u.as_mut_ptr() as *mut u32).offset((3 as i32) as isize) }
// CHECK-NEXT:         } ^ (unsafe {
// CHECK-NEXT:             *unsafe { unsafe { (*b).u.as_mut_ptr() as *mut u32 }.offset((3 as i32) as isize) }
// CHECK-NEXT:         })
// CHECK-NEXT:             >> (11 as i32)
// CHECK-NEXT:             & (3221225462 as u32)
// CHECK-NEXT:             ^ unsafe { *unsafe { (y.u.as_mut_ptr() as *mut u32).offset((3 as i32) as isize) } }
// CHECK-NEXT:             ^ (unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { (*d).u.as_mut_ptr() as *mut u32 }.offset((3 as i32) as isize) }
// CHECK-NEXT:             }) << (18 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: __SlateAlign16<[S; 6]> = __SlateAlign16([
// CHECK-NEXT:         S {
// CHECK-NEXT:             u: [
// CHECK-NEXT:                 (1235 as i32) as u32,
// CHECK-NEXT:                 3159640283 as u32,
// CHECK-NEXT:                 4062961311 as u32,
// CHECK-NEXT:                 3954462607 as u32,
// CHECK-NEXT:             ],
// CHECK-NEXT:         },
// CHECK-NEXT:         S {
// CHECK-NEXT:             u: [
// CHECK-NEXT:                 (61024153 as i32) as u32,
// CHECK-NEXT:                 (1623097926 as i32) as u32,
// CHECK-NEXT:                 2731697901 as u32,
// CHECK-NEXT:                 (2057405750 as i32) as u32,
// CHECK-NEXT:             ],
// CHECK-NEXT:         },
// CHECK-NEXT:         S {
// CHECK-NEXT:             u: [
// CHECK-NEXT:                 (363037976 as i32) as u32,
// CHECK-NEXT:                 3479526625 as u32,
// CHECK-NEXT:                 4091582868 as u32,
// CHECK-NEXT:                 (1357867518 as i32) as u32,
// CHECK-NEXT:             ],
// CHECK-NEXT:         },
// CHECK-NEXT:         S {
// CHECK-NEXT:             u: [
// CHECK-NEXT:                 2284276487 as u32,
// CHECK-NEXT:                 3888728166 as u32,
// CHECK-NEXT:                 3236601671 as u32,
// CHECK-NEXT:                 3064478787 as u32,
// CHECK-NEXT:             ],
// CHECK-NEXT:         },
// CHECK-NEXT:         S {
// CHECK-NEXT:             u: [
// CHECK-NEXT:                 3440181298 as u32,
// CHECK-NEXT:                 (1564997079 as i32) as u32,
// CHECK-NEXT:                 (1510669302 as i32) as u32,
// CHECK-NEXT:                 2930277156 as u32,
// CHECK-NEXT:             ],
// CHECK-NEXT:         },
// CHECK-NEXT:         S {
// CHECK-NEXT:             u: [
// CHECK-NEXT:                 (0 as i32) as u32,
// CHECK-NEXT:                 (0 as i32) as u32,
// CHECK-NEXT:                 (0 as i32) as u32,
// CHECK-NEXT:                 (0 as i32) as u32,
// CHECK-NEXT:             ],
// CHECK-NEXT:         },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     baz(
// CHECK-NEXT:         unsafe { (a.0.as_mut_ptr() as *mut S).offset((5 as i32) as isize) },
// CHECK-NEXT:         unsafe { (a.0.as_mut_ptr() as *mut S).offset((0 as i32) as isize) },
// CHECK-NEXT:         unsafe { (a.0.as_mut_ptr() as *mut S).offset((1 as i32) as isize) },
// CHECK-NEXT:         unsafe { (a.0.as_mut_ptr() as *mut S).offset((2 as i32) as isize) },
// CHECK-NEXT:         unsafe { (a.0.as_mut_ptr() as *mut S).offset((3 as i32) as isize) },
// CHECK-NEXT:     );
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe { (a.0.as_mut_ptr() as *mut S).offset((4 as i32) as isize) })
// CHECK-NEXT:                     .u
// CHECK-NEXT:                     .as_mut_ptr() as *mut u32
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset((0 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe { (a.0.as_mut_ptr() as *mut S).offset((5 as i32) as isize) })
// CHECK-NEXT:                     .u
// CHECK-NEXT:                     .as_mut_ptr() as *mut u32
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset((0 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     } || (unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe { (a.0.as_mut_ptr() as *mut S).offset((4 as i32) as isize) })
// CHECK-NEXT:                     .u
// CHECK-NEXT:                     .as_mut_ptr() as *mut u32
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset((1 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe { (a.0.as_mut_ptr() as *mut S).offset((5 as i32) as isize) })
// CHECK-NEXT:                     .u
// CHECK-NEXT:                     .as_mut_ptr() as *mut u32
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset((1 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     } || (unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe { (a.0.as_mut_ptr() as *mut S).offset((4 as i32) as isize) })
// CHECK-NEXT:                     .u
// CHECK-NEXT:                     .as_mut_ptr() as *mut u32
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset((2 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe { (a.0.as_mut_ptr() as *mut S).offset((5 as i32) as isize) })
// CHECK-NEXT:                     .u
// CHECK-NEXT:                     .as_mut_ptr() as *mut u32
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset((2 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     } || (unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe { (a.0.as_mut_ptr() as *mut S).offset((4 as i32) as isize) })
// CHECK-NEXT:                     .u
// CHECK-NEXT:                     .as_mut_ptr() as *mut u32
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset((3 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe { (a.0.as_mut_ptr() as *mut S).offset((5 as i32) as isize) })
// CHECK-NEXT:                     .u
// CHECK-NEXT:                     .as_mut_ptr() as *mut u32
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset((3 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     } {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
