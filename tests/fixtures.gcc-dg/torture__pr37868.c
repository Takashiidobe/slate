/* { dg-do run } */
/* { dg-options "-fno-strict-aliasing" } */
/* { dg-skip-if "unaligned access" { arc*-*-* epiphany-*-* nds32*-*-* sparc*-*-* sh*-*-* tic6x-*-* } } */

extern void abort(void);
#if (__SIZEOF_INT__ <= 2)
struct X {
  unsigned char pad : 4;
  unsigned int  a   : 16;
  unsigned int  b   : 8;
  unsigned int  c   : 6;
} __attribute__((packed));
#else
struct X {
  unsigned char pad : 4;
  unsigned int  a   : 32;
  unsigned int  b   : 24;
  unsigned int  c   : 6;
} __attribute__((packed));

#endif

// @lowering-fn-begin
// @rewrite-fn-begin
int main(void) {
  struct X     x;
  unsigned int bad_bits;

  x.pad = -1;
  x.a   = -1;
  x.b   = -1;
  x.c   = -1;

  bad_bits = ((unsigned int)-1) ^ *(1 + (unsigned int *)&x);
  if (bad_bits != 0)
    abort();
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let mut x: X = X {
// LOWERING-DAG:         __bitfield_0: unsafe {
// LOWERING-DAG:             std::mem::transmute::<[u8; 9], __slate_bitfields::__SlateBitfield_X_0>([0; 9])
// LOWERING-DAG:         },
// LOWERING-DAG:     };
// LOWERING-DAG:     let mut bad_bits: u32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: u8 = 255;
// LOWERING-DAG:     let {{__v[0-9]+}}: u8 = ({{__v[0-9]+}} as u8) << 4 >> 4;
// LOWERING-DAG:     x.__bitfield_0.set_pad(({{__v[0-9]+}} as u8) << 4 >> 4);
// LOWERING-DAG:     let {{__v[0-9]+}}: u32 = 4294967295u32;
// LOWERING-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}};
// LOWERING-DAG:     x.__bitfield_0.set_a({{__v[0-9]+}});
// LOWERING-DAG:     let {{__v[0-9]+}}: u32 = 4294967295u32;
// LOWERING-DAG:     let {{__v[0-9]+}}: u32 = ({{__v[0-9]+}} as u32) << 8 >> 8;
// LOWERING-DAG:     x.__bitfield_0.set_b(({{__v[0-9]+}} as u32) << 8 >> 8);
// LOWERING-DAG:     let {{__v[0-9]+}}: u32 = 4294967295u32;
// LOWERING-DAG:     let {{__v[0-9]+}}: u32 = ({{__v[0-9]+}} as u32) << 26 >> 26;
// LOWERING-DAG:     x.__bitfield_0.set_c(({{__v[0-9]+}} as u32) << 26 >> 26);
// LOWERING-DAG:     let {{__v[0-9]+}}: u32 = 4294967295u32;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u32 = std::ptr::addr_of_mut!(x) as *mut u32;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut u32 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-DAG:     let {{__v[0-9]+}}: u32 = unsafe { std::ptr::read_unaligned({{__v[0-9]+}}) };
// LOWERING-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} ^ {{__v[0-9]+}};
// LOWERING-DAG:     bad_bits = {{__v[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: u32 = bad_bits;
// LOWERING-DAG:         let {{__v[0-9]+}}: u32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut x: X = X {
// REWRITES-DAG:         __bitfield_0: unsafe {
// REWRITES-DAG:             std::mem::transmute::<[u8; 9], __slate_bitfields::__SlateBitfield_X_0>([0; 9])
// REWRITES-DAG:         },
// REWRITES-DAG:     };
// REWRITES-DAG:     let mut bad_bits: u32 = 0;
// REWRITES-DAG:     x.__bitfield_0.set_pad((255 as u8) << 4 >> 4);
// REWRITES-DAG:     let {{__v[0-9]+}}: u32 = 4294967295u32;
// REWRITES-DAG:     x.__bitfield_0.set_a({{__v[0-9]+}});
// REWRITES-DAG:     let {{__v[0-9]+}}: u32 = 4294967295u32;
// REWRITES-DAG:     x.__bitfield_0.set_b(({{__v[0-9]+}} as u32) << 8 >> 8);
// REWRITES-DAG:     let {{__v[0-9]+}}: u32 = 4294967295u32;
// REWRITES-DAG:     x.__bitfield_0.set_c(({{__v[0-9]+}} as u32) << 26 >> 26);
// REWRITES-DAG:     let {{__v[0-9]+}}: u32 = 4294967295u32;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u32 = std::ptr::addr_of_mut!(x) as *mut u32;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut u32 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-DAG:     let {{__v[0-9]+}}: u32 = unsafe { std::ptr::read_unaligned({{__v[0-9]+}}) };
// REWRITES-DAG:     bad_bits = {{__v[0-9]+}} ^ {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = bad_bits != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
