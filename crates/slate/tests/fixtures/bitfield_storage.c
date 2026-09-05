union U {
  struct {
    unsigned first  : 1;
    unsigned second : 1;
  } bits;
  unsigned raw;
};

int main(void) {
  union U value = {0};
  // @lowering-begin
  // @rewrite-begin
  value.bits.second = 1;
  // @rewrite-end
  // @lowering-end
  return value.bits.second != 1;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{__v[0-9]+}}: u32 = 1;
// LOWERING-DAG: let {{__v[0-9]+}}: u32 = ({{__v[0-9]+}} as u32) << 31 >> 31;
// LOWERING-DAG: unsafe {
// LOWERING-DAG:     value.bits.__bitfield_0.set_second(({{__v[0-9]+}} as u32) << 31 >> 31);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     value.bits.__bitfield_0.set_second((1 as u32) << 31 >> 31);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
