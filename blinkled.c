/* Header is in /usr/avr/include. For more, see http://bit.ly/avrgcc-headers */
#include <avr/io.h>
/* #include <avr/iom328p.h> -- *bad* non-portable alternative                */
#include <stdint.h>
static int seed = 1;
#define M 2147483647
#define A 16807
#define Q ( M / A )
#define R ( M % A )
/**
 * soft_delay() - wastes CPU time crunching cycle to achieve delay
 * @N:	number of outer "while" steps.
 *
 * This is very inefficient in terms of CPU and energy usage.
 * Better way is to use timer to count time and put CPU into sleep mode to save
 * energy.
 * But soft delays are useful for very precise timings (i.e. software USB
 * implementation, 1-Wire interface, etc.)
 * See <util/delay.h> for alternative implementation
 */
static void soft_delay(volatile uint16_t N)
{
	/* If volatile is not used, AVR-GCC will optimize this stuff out     */
        /* making our function completely empty                              */
	volatile uint8_t inner = 0xFF;
	while (N--) {
		while (inner--);
	}
}

/**
 * soft_delay() - linear congruential generator function
 * Generate random numbers.
 * for more http://www.eternallyconfuzzled.com/tuts/algorithms/jsw_tut_rand.aspx
 */
int jsw_rand(void)
{
    seed = A * (seed % Q) - R * (seed / Q);

    if (seed <= 0)
    {
        seed += M;
    }

    return seed;
}

int main(void)
{
	/* Configure GPIO */
	DDRB |=  (1 << PB1);	//set OC1A as output PORTB1
	// use 8-bit mode PWM by setting the WGM10 and the WGM12 bits
	TCCR1A |= (1 << COM1A1) | (1 << WGM10);
	TCCR1B |= (1 << WGM12) | (1 << CS11);	//set clock / 8 prescaler; 
	OCR1A = 0;	// OCR1A - comparasion register A (16 bits)

	while(1) {
      OCR1A = jsw_rand() % 65535 + 0;
      soft_delay(50);
   }
	return 0;
}