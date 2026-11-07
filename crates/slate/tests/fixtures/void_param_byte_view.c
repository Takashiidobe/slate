#include <stdio.h>

typedef struct {
  unsigned char i, j, s[256];
} Rc4;

// @check-fn-begin
static void xor_block(void *vblk, int len) {
  unsigned char *blk = (unsigned char *)vblk;
  unsigned       k;
  for (k = 0; (int)k < len; k++)
    blk[k] ^= (unsigned char)(k * 7 + 1);
}
// @check-fn-end

// @check-fn-begin
static void rc4_block(void *handle, void *vblk, int len) {
  unsigned char *blk = (unsigned char *)vblk;
  Rc4           *ctx = (Rc4 *)handle;
  unsigned       k;
  unsigned char  tmp, i, j, *s;

  s = ctx->s;
  i = ctx->i;
  j = ctx->j;
  for (k = 0; (int)k < len; k++) {
    i      = (i + 1) & 0xff;
    j      = (j + s[i]) & 0xff;
    tmp    = s[i];
    s[i]   = s[j];
    s[j]   = tmp;
    blk[k] ^= s[(s[i] + s[j]) & 0xff];
  }
  ctx->i = i;
  ctx->j = j;
}
// @check-fn-end

int main(void) {
  unsigned char buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  Rc4           ctx;
  ctx.i = ctx.j = 0;
  for (int i = 0; i < 256; i++)
    ctx.s[i] = (unsigned char)(255 - i);
  xor_block(buf, 8);
  rc4_block(&ctx, buf, 8);
  for (int i = 0; i < 8; i++)
    printf("%u ", buf[i]);
  printf("%u %u\n", ctx.i, ctx.j);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn xor_block(vblk: *mut (), len: i32) {
// CHECK-NEXT:     let __vblk_view = unsafe {
// CHECK-NEXT:         std::slice::from_raw_parts_mut(vblk as *mut u8, len as usize)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut k: u32 = 0;
// CHECK-NEXT:     while (k as i32) < len {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __vblk_view[k as usize] = ((__vblk_view[k as usize] as i32)
// CHECK-NEXT:                 ^ ((k.wrapping_mul(7).wrapping_add(1) as u8) as i32)) as u8;
// CHECK-NEXT:         }
// CHECK-NEXT:         k = k.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn rc4_block(handle: &mut (), vblk: *mut (), len: i32) {
// CHECK-NEXT:     let __vblk_view = unsafe {
// CHECK-NEXT:         std::slice::from_raw_parts_mut(vblk as *mut u8, len as usize)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut k: u32 = 0;
// CHECK-NEXT:     let mut tmp: u8 = 0;
// CHECK-NEXT:     let mut i: u8 = unsafe {
// CHECK-NEXT:         (*((handle as *mut ()) as *mut __SlateRecord_void_param_byte_view_c_5)).i
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut j: u8 = unsafe {
// CHECK-NEXT:         (*((handle as *mut ()) as *mut __SlateRecord_void_param_byte_view_c_5)).j
// CHECK-NEXT:     };
// CHECK-NEXT:     let s: *mut u8 = unsafe {
// CHECK-NEXT:         (*((handle as *mut ()) as *mut __SlateRecord_void_param_byte_view_c_5))
// CHECK-NEXT:             .s
// CHECK-NEXT:             .as_mut_ptr() as *mut u8
// CHECK-NEXT:     };
// CHECK-NEXT:     while (k as i32) < len {
// CHECK-NEXT:         i = ((i as i32) + 1 & 255) as u8;
// CHECK-NEXT:         j = ((j as i32) + ((unsafe { *s.offset((i as i32) as isize) }) as i32) & 255)
// CHECK-NEXT:             as u8;
// CHECK-NEXT:         tmp = unsafe { *s.offset((i as i32) as isize) };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *s.offset((i as i32) as isize) = *s.offset((j as i32) as isize);
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *s.offset((j as i32) as isize) = tmp;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __vblk_view[k as usize] = ((__vblk_view[k as usize] as i32)
// CHECK-NEXT:                 ^ (*s
// CHECK-NEXT:                     .offset(
// CHECK-NEXT:                         ((*s.offset((i as i32) as isize) as i32)
// CHECK-NEXT:                             + (*s.offset((j as i32) as isize) as i32) & 255) as isize,
// CHECK-NEXT:                     ) as i32)) as u8;
// CHECK-NEXT:         }
// CHECK-NEXT:         k = k.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*((handle as *mut ()) as *mut __SlateRecord_void_param_byte_view_c_5)).i = i;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*((handle as *mut ()) as *mut __SlateRecord_void_param_byte_view_c_5)).j = j;
// CHECK-NEXT:     }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
