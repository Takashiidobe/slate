#define ERROR_TOKEN expanded
#error unexpanded ERROR_TOKEN "quoted" C:\tmp

int main(void) { return 0; }

// COMMON: compile_error!("unexpanded ERROR_TOKEN \"quoted\" C:\\tmp");
