// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

struct Record {
  int  head;
  char name[10];
};

int next(void);

unsigned long folded(void) {
  struct Record record;
  return __builtin_object_size(&record.name[3], 0) +
         __builtin_object_size(&record.name[3], 1);
}

unsigned long member(struct Record *record) {
  return __builtin_object_size(&record->name, 1);
}

unsigned long deferred(char *p) {
  return __builtin_object_size(p, 0) + __builtin_dynamic_object_size(p, 2);
}

unsigned long lower_bound(char *p) { return __builtin_object_size(p, 3); }

unsigned long effects(char *p) {
  return __builtin_object_size(p + next(), 0);
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_Record:[0-9]+]] Record = struct {
// IR-NEXT:         field0 head: i32;
// IR-NEXT:         field1 name: array<i8, 10>;
// IR-NEXT:     } [size=16, align=4, offsets=[0, 4]];
// IR-NEXT:     fn %[[VALUE_next:[0-9]+]] @next() -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_folded:[0-9]+]] @folded() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_record:[0-9]+]] record: @type[[TYPE_Record]] [storage=automatic];
// IR-NEXT:         return add<u64>(const<u64>(9), const<u64>(7));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_member:[0-9]+]] @member(%[[VALUE_record_2:[0-9]+]] record: ptr<@type[[TYPE_Record]]>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return intrinsic<u64, llvm.objectsize>(addr_of<ptr<array<i8, 10>>>(field1(deref(read<ptr<@type[[TYPE_Record]]>>(%[[VALUE_record_2]])))), const<bool>(false), const<bool>(false), const<bool>(false));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_deferred:[0-9]+]] @deferred(%[[VALUE_p:[0-9]+]] p: ptr<i8>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<u64>(intrinsic<u64, llvm.objectsize>(read<ptr<i8>>(%[[VALUE_p]]), const<bool>(false), const<bool>(false), const<bool>(false)), intrinsic<u64, llvm.objectsize>(read<ptr<i8>>(%[[VALUE_p]]), const<bool>(true), const<bool>(false), const<bool>(true)));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_lower_bound:[0-9]+]] @lower_bound(%[[VALUE_p_2:[0-9]+]] p: ptr<i8>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(0);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_effects:[0-9]+]] @effects(%[[VALUE_p_3:[0-9]+]] p: ptr<i8>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(18446744073709551615);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
