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
// DEFAULT-NEXT:     type @type[[TYPE_delay_block:[0-9]+]] delay_block = struct {
// DEFAULT-NEXT:         field0 succ: ptr<@type[[TYPE_delay_block]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_Timer_Queue:[0-9]+]] Timer_Queue: @type[[TYPE_delay_block]] [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_time_enqueue:[0-9]+]] @time_enqueue(%[[VALUE_d:[0-9]+]] d: ptr<@type[[TYPE_delay_block]]>) -> ptr<@type[[TYPE_delay_block]]> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<@type[[TYPE_delay_block]]> [storage=automatic] = read<ptr<@type[[TYPE_delay_block]]>>(field0(%[[VALUE_Timer_Queue]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_delay_block]]>>(field0(deref(read<ptr<@type[[TYPE_delay_block]]>>(%[[VALUE_d]]))), null<ptr<@type[[TYPE_delay_block]]>>);
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_delay_block]]>>(field0(%[[VALUE_Timer_Queue]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_delay_block]]>>(field0(%[[VALUE_Timer_Queue]]), addr_of<ptr<@type[[TYPE_delay_block]]>>(%[[VALUE_Timer_Queue]]));
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE_delay_block]]>>(call<ptr<@type[[TYPE_delay_block]]>, signature=fn(ptr<@type[[TYPE_delay_block]]>) -> ptr<@type[[TYPE_delay_block]]>>(%[[VALUE_time_enqueue]], addr_of<ptr<@type[[TYPE_delay_block]]>>(%[[VALUE_Timer_Queue]])), null<ptr<@type[[TYPE_delay_block]]>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
