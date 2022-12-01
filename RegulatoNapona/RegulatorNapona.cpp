#include "mbed.h"
#include "RegulatorNapona.h"

Ticker zujalicaTicker;
PwmOut zujalica(PA_4);
bool isActive = false;
bool blink = true;
DigitalOut crvenaLedica(PC_0);
DigitalOut zutaLedica(PC_2);
DigitalOut zelenaLedica(PB_0);
Timer debounce;
void toggle(void);

void RegulatorNapona::Start()
{
    Serial pc(USBTX, USBRX);
    AnalogIn UlazniSignal(PC_1);
    float ADCdata;
    InterruptIn button(PC_3);
    debounce.start();
    button.rise(&toggle);
    button.mode(PullDown);

    while (1) {
        ADCdata=UlazniSignal;
        pc.printf("%f \n\r",ADCdata*5);

        zelenaLedica = ADCdata>=0.1;
        zutaLedica = ADCdata>=0.5;
        crvenaLedica= ADCdata>=0.7;
        if(ADCdata>0.85) {
            if(!isActive) {
                zujalicaTicker.attach(&zujalicaSound, 0.1);
                isActive = true;
            }
        }
        wait(0.1);
    }
}
void toggle()
{
    if (debounce.read_ms()>200) {
        zujalicaTicker.detach();
        zujalica =0;
        isActive = false;
        debounce.reset();
    }
}
void RegulatorNapona::zujalicaSound()
{
    zujalica.period(0.001);
    zujalica = 0.7;
    crvenaLedica = blink;
    blink = !blink;
}