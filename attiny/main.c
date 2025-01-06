#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 9600000UL  // Define the CPU frequency as 9.6 MHz
#include <util/delay.h>

#define LED_PIN PB0

int main(void) {

    // Disable interrupts during the change
    cli();
    // Enable changes to the clock prescaler
    CLKPR = (1 << CLKPCE);
    // Set the clock prescaler to 1 (no division)
    CLKPR = 0;
    // Re-enable interrupts
    sei();

    // Set PB0 as output
    DDRB |= (1 << LED_PIN);

    while (1) {
        // Toggle LED
        PORTB ^= (1 << LED_PIN);
        _delay_ms(500); // 500ms delay
    }

    return 0;
}

