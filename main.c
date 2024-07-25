#include <xc.h>
#define _XTAL_FREQ 20000000  // Define oscillator frequency for delay
#define LCD_RS  RB4
#define LCD_EN  RB5
#define LCD_D4  RB0
#define LCD_D5  RB1
#define LCD_D6  RB2
#define LCD_D7  RB3

#define LCD_RS_Direction TRISB4
#define LCD_EN_Direction  TRISB5
#define LCD_D4_Direction  TRISB0
#define LCD_D5_Direction  TRISB1
#define LCD_D6_Direction  TRISB2
#define LCD_D7_Direction  TRISB3
void ADC_Init(){
    ADCON0 = 0x41;  // Turn ON ADC and select Fosc/8 as the conversion clock
    ADCON1 = 0x80;  // Configure ADC voltage reference and result format (right justified)
}

unsigned int ADC_Read(unsigned char channel){
    if(channel > 7) return 0;  // ADC has 8 channels, 0-7
    ADCON0 &= 0xC5;  // Clear existing channel selection bits
    ADCON0 |= channel << 3;  // Set new channel
    __delay_ms(2);  // Acquisition time to charge the hold capacitor
    GO_nDONE = 1;  // Start ADC conversion
    while(GO_nDONE);  // Wait for conversion to complete
    return ((ADRESH << 8) + ADRESL);  // Return result
}

void main(){
    unsigned int adc_value;
    float voltage, input_voltage;

    TRISA = 0xFF;  // Configure PORTA as input
    ADC_Init();  // Initialize ADC

    while(1){
        adc_value = ADC_Read(0);  // Read ADC value from channel 0
        voltage = (adc_value * 5.0) / 1023.0;  // Assuming Vref+ is 5V and 10-bit ADC resolution
        input_voltage = voltage * ((100.0 + 3.3) / 3.3);  // Convert to input voltage

        // Now you can use input_voltage as needed
        __delay_ms(500);  // Delay for a while
    }
}
