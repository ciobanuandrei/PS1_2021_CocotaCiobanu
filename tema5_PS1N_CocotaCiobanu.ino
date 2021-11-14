#include <LiquidCrystal.h>
#define ROOM_TEMP 20
const int rs=12, en=11 ,d4=5,d5=4,d6=3,d7=2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int ora=23,min=59,sec=58,numar_CAN,voltageValue;
double temp=40;
float q=5000/1023.0;
double kp = 9.6, ki = 16, kd =5.44 ; // exemplu valori
int timp_inc=10;
int timp_men=10;
int timp_rac=10;
  double eroare= 0;

  double suma_erori= 0;

  double eroare_anterioara = 0;

  double derivativa = 0;

  double dt=1; // timp esantionare
	double output;
  double setpoint = 30;
void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A =15624 ;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12)|(1 << CS10);  
  TIMSK1 |= (1 << OCIE1A);
}
double moving_sp=ROOM_TEMP;
void afisare_timp()
{
  int min=0; int sec=0;
  int remaining=0;
  lcd.setCursor(0,1);
  lcd.print("P:");
  lcd.print(moving_sp);
  
  double now=millis();
  int total_seconds=now/1000;
  if(total_seconds<=timp_inc)
  {
   Serial.print("inc: ");
   Serial.println(total_seconds);
   moving_sp=ROOM_TEMP+(temp-ROOM_TEMP)*total_seconds/timp_inc;
   Serial.print("SP: ");
   Serial.println(moving_sp);
  }
  else if(total_seconds<=(timp_inc+timp_men))
  {
  	Serial.print("Men: ");
    Serial.println(total_seconds);
    moving_sp=temp;
  }
  else if(total_seconds<=(timp_inc+timp_men+timp_rac))
  {
   Serial.print("Rac: ");
   Serial.println(total_seconds);
   moving_sp=ROOM_TEMP+(temp-ROOM_TEMP)-(temp-ROOM_TEMP)*total_seconds/timp_rac;
   	
  }
  else
  {
    Serial.print("Oprit");
    Serial.println(total_seconds);            
  }
  lcd.print(" t:");
  lcd.print(total_seconds);
}
void loop()
{
  //adc_init();
  //numar_CAN=read_adc(0);
  //voltageValue=numar_CAN*q;
  //temp=(voltageValue-500)/10;
  //lcd.print("T ");
  //lcd.print(analogRead(0)/2-50);
  //lcd.print(" S ");
  afisare_timp();
  delay(1000);
  

  
}
ISR(TIMER1_COMPA_vect)
{
   
  /*PID();
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
  Serial.print(output);
  Serial.print("\n");
  */
  
  
}
void PID(){
 eroare = moving_sp - temp;
 

    suma_erori= suma_erori + eroare * dt;

    derivativa = (eroare - eroare_anterioara) / dt;

    output = (kp * eroare) + (ki * suma_erori ) + (kd * derivativa);

    eroare_anterioara = eroare;
 
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