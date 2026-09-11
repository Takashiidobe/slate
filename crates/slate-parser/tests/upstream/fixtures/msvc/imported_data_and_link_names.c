__attribute__((dllimport)) extern int g_counter;

extern int renamed_target(int) asm("actual_symbol");

int use_imported_data_and_link_name(void) {
  return g_counter + renamed_target(3);
}

int main(void) { return use_imported_data_and_link_name(); }


