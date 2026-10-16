extern void abort(void);

struct delay_block {
  struct delay_block *succ;
};

static struct delay_block Timer_Queue;

struct delay_block *time_enqueue(struct delay_block *d) {
  struct delay_block *q = Timer_Queue.succ;
  d->succ               = (void *)0;
  return Timer_Queue.succ;
}

int main(void) {
  Timer_Queue.succ = &Timer_Queue;
  if (time_enqueue(&Timer_Queue) != (void *)0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct delay_block {
// CHECK-NEXT:     succ: *mut delay_block,
// CHECK-NEXT: }
// CHECK-NEXT: static mut Timer_Queue: delay_block = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn time_enqueue(mut d: *mut delay_block) -> *mut delay_block {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*d).succ = std::ptr::null_mut::<delay_block>();
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { Timer_Queue.succ }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         Timer_Queue.succ = unsafe { std::ptr::addr_of_mut!(Timer_Queue) };
// CHECK-NEXT:     }
// CHECK-NEXT:     if time_enqueue(unsafe { std::ptr::addr_of_mut!(Timer_Queue) })
// CHECK-NEXT:         != std::ptr::null_mut::<delay_block>()
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
