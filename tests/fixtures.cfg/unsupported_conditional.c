#ifdef PACKED_LAYOUT
#pragma pack(push, 1)
#endif

int main(void) { return 0; }
// DIRECTIVES-DAG: #[cfg(feature = "packed_layout")]
// DIRECTIVES-DAG: compile_error!("unsupported semantic directive #pragma at line 2: pack(push, 1)");
