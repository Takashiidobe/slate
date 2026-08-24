#include <stdint.h>

#define UART_STATUS (*(volatile uint32_t *)0x40001000u)
#define UART_DATA   (*(volatile uint32_t *)0x40001004u)

void send_byte(uint8_t byte) {
  while ((UART_STATUS & 1u) == 0)
    ;

  UART_DATA = byte;
}
// LOWERING-LABEL: {{^}}pub extern "C" fn send_byte(
// LOWERING-DAG: std::ptr::read_volatile
// LOWERING-DAG: std::ptr::write_volatile
// LOWERING: {{^}}}
