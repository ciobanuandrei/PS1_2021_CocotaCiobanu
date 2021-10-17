#include <LiquidCrystal.h>
const int rs=12, en=11 ,d4=5,d5=4,d6=3,d7=2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int ora=23,min=59,sec=58,temp,numar_CAN,voltageValue;
float q=5000/1023.0;

void setup()
{
  lcd.begin(16, 2);
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A =15624 ;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12)|(1 << CS10);  
  TIMSK1 |= (1 << OCIE1A);
}

void loop()
{
  adc_init();
  numar_CAN=read_adc(0);
  voltageValue=numar_CAN*q;
  temp=(voltageValue-500)/10;

}
ISR(TIMER1_COMPA_vect)
{
  sec++;
  if(sec==60)
  {
    sec=0;
    min++;
  }
  if(min==60)
  {
    min=0;
    if(ora==23)
    	ora=0;
    else
    	ora++;
  }
  lcd.clear();
  lcd.print("Temp = ");
  lcd.print(temp);
  lcd.print("'C");
  lcd.setCursor(0,1);
  lcd.print("Ora ");
  if(ora<10)
    lcd.print("0");
  lcd.print(ora);
  lcd.print(":");
  if(min<10)
    lcd.print("0");
  lcd.print(min);
  lcd.print(":");
  if(sec<10)
    lcd.print("0");
  lcd.print(sec);
}
void adc_init()//adc initialization
{
ADCSRA |= ((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0));//set division factor between system clock frequency and the input clock to the ADC- 128
ADMUX |= (1<<REFS0);//AVcc with external capacitor at Aref pin 
ADCSRA |= (1<<ADEN);//enable ADC
ADCSRA |= (1<<ADSC);//ADC start conversion
}

uint16_t read_adc(uint8_t channel)
{
ADMUX &= 0xF0;//set input AO to A5
ADMUX |= channel;//select chanel AO to A5
ADCSRA |= (1<<ADSC);//start conversion
while(ADCSRA & (1<<ADSC));//wait while adc conversion are not updated 
return ADCW;//read and return voltage
}