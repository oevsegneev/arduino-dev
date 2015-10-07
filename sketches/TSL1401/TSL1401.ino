#include <avr/io.h>
#include <avr/interrupt.h>
//#include <SerialFlow.h>

#define F_CPU 16000000UL
#define L_ARRAY 128
#define L_ARRAY_1 129

//static const byte PACKET_SIZE = 128;
//static const byte VALUE_SIZE = 1;
//static const boolean SEPARATE_VALUES = false;

//global vars
uint8_t CLKcycleCounter = 0; // counts the number of clock cycles expired
volatile uint8_t pixels[L_ARRAY] = {0}; // pixel buffer - holds data from ADC
uint8_t i, m, pix; // i is an index for loops, m is the mimimum pixel value
uint8_t mi; // index of darkest pixel on the array

char ptxt[L_ARRAY+1];

//SerialFlow rd(&Serial); // RX, TX

// ADC interrupt routine - this happens when the ADC is finished taking a sample
ISR(ADC_vect) {
    // if AO is sending pixel data (first 128 clock cycles of each frame)
    // then do an ADC and put it in the pixel buffer
    if (CLKcycleCounter < L_ARRAY) {
        // start next CLK cycle
        __asm__("nop\n\t"); // delay one CPU clock cycle = 62.5 nanosec
        __asm__("nop\n\t"); // delay one CPU clock cycle = 62.5 nanosec
        __asm__("nop\n\t"); // delay one CPU clock cycle = 62.5 nanosec
        PORTB |= (1 << PINB3); // CLK = 5V
        __asm__("nop\n\t"); // delay one CPU clock cycle = 62.5 nanosec
        __asm__("nop\n\t"); // delay one CPU clock cycle = 62.5 nanosec
        __asm__("nop\n\t"); // delay one CPU clock cycle = 62.5 nanosec
        PORTB &= ~(1 << PINB3); // CLK = 0V
        pixels[CLKcycleCounter] = ADCH; // high byte of ADC data
        CLKcycleCounter++; // iterate clock cycle counter
    } else if (CLKcycleCounter == L_ARRAY || CLKcycleCounter == L_ARRAY_1) { 
        // there is an extra clock cycle at the end (129) with no data
        // do a second extra clock for charge transfer
        // | CLKcycleCounter == L_ARRAY_1
        // start next CLK cycle
        __asm__("nop\n\t"); // delay one CPU clock cycle = 62.5 nanosec
        __asm__("nop\n\t"); // delay one CPU clock cycle = 62.5 nanosec
        __asm__("nop\n\t"); // delay one CPU clock cycle = 62.5 nanosec
        PORTB |= (1 << PINB3); // CLK = 5V
        __asm__("nop\n\t"); // delay one CPU clock cycle = 62.5 nanosec
        __asm__("nop\n\t"); // delay one CPU clock cycle = 62.5 nanosec
        __asm__("nop\n\t"); // delay one CPU clock cycle = 62.5 nanosec
        PORTB &= ~(1 << PINB3); // CLK = 0V
        CLKcycleCounter++; // iterate clock cycle counter
    } else { 
        // start next frame by sending a pulse on SI
        PORTB |= (1 << PINB0); // SI = 5V
        __asm__("nop\n\t"); // delay one CPU clock cycle = 62.5 nanosec
        __asm__("nop\n\t"); // delay one CPU clock cycle = 62.5 nanosec
        PORTB |= (1 << PINB3); // CLK = 5V
        __asm__("nop\n\t"); // delay one CPU clock cycle = 62.5 nanosec
        __asm__("nop\n\t"); // delay one CPU clock cycle = 62.5 nanosec
        PORTB &= ~(1 << PINB0); // SI = 0V, must go low before next clock rise!!!
        __asm__("nop\n\t"); // delay one CPU clock cycle = 62.5 nanosec
        __asm__("nop\n\t"); // delay one CPU clock cycle = 62.5 nanosec
        PORTB &= ~(1 << PINB3); // CLK = 0V
        CLKcycleCounter = 0; // reset clock cycle counter to zero  
    }
    ADCSRA |= (1 << ADSC); // start next conversion
}

int main(void) {
    // use serial library for communication with MATLAB
    Serial.begin(115200);
    //rd.setPacketFormat(VALUE_SIZE, PACKET_SIZE, SEPARATE_VALUES);
    //rd.begin(115200);
    
    // set up digital output pins on port B; CLK = B3 (pin 11) and SI = B0 (pin 8)
    DDRB = 0; // zero the register
    PORTB = 0; // zero the register
    DDRB |= (1 << PINB3) | (1 << PINB0); // CLK(B3) and SI(B0)
    PORTB &= ~(1 << PINB0); // CLK = 0V
    PORTB &= ~(1 << PINB3); // SI = 0V
  
    // ADC set-up; ADC will read the analog output (AO) voltage from the sensor
    sei(); // enable global interrupts
    ADCSRA = 0; // zero the register
    ADCSRA |= (1 << ADEN); // turn on ADC
    ADCSRA |= (1 << ADPS2); // set clock to F_CPU/16 = 1Mhz
    ADCSRA |= (1 << ADIE); // enable ADC interrupt
    ADMUX |= (1 << ADLAR) | (1 << REFS0) | (1 << MUX2) | (1 << MUX0); // ADC is on pin 5;
  
    // start first frame
    PORTB |= (1 << PINB0); // SI = 5V
    __asm__("nop\n\t"); // delay one CPU clock cycle = 62.5 nanosec
    __asm__("nop\n\t"); // delay one CPU clock cycle = 62.5 nanosec
    PORTB |= (1 << PINB3); // CLK = 5V
    __asm__("nop\n\t"); // delay one CPU clock cycle = 62.5 nanosec
    __asm__("nop\n\t"); // delay one CPU clock cycle = 62.5 nanosec
    PORTB &= ~(1 << PINB0); // SI = 0V, must go low before next clock rise!!!
    __asm__("nop\n\t"); // delay one CPU clock cycle = 62.5 nanosec
    __asm__("nop\n\t"); // delay one CPU clock cycle = 62.5 nanosec
    PORTB &= ~(1 << PINB3); // CLK = 0V
  
    // first conversion
    ADCSRA |= (1 << ADSC);

    while (1) {
        // find the minimum pixel value, indicating tail position
        /*m = 30;
        mi = 0;
        for (i = 1; i < L_ARRAY; ++i) {
            if (pixels[i] < m) {
                mi = i;
                m = pixels[i];
            }
        }
        */
        for (i = 1; i < L_ARRAY; i+=1) {
            if(pixels[i]==0)
//            if(pixels[i]>128)
                ptxt[i] = 'o';
            else
                ptxt[i] = ' ';
        }
        ptxt[0] = ' ';
        ptxt[L_ARRAY] = '\0';
        
        //for(int i=0; i<L_ARRAY; i++){
        //    rd.setPacketValue(pixels[i]);
        //}
        //rd.sendPacket();
        Serial.println(ptxt);
        //delay(100);
        
        //Serial.println(mi);
    } 
    return 0;
}




