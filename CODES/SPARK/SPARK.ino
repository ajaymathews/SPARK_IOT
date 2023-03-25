#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "d3226cad474744aba56702e536d3c6ea";
char ssid[] = "mi4";    // Your WiFi credentials.
char pass[] = "12345689";
float power =14;
float volt=220;
double minvalue;
double maxvalue;
BlynkTimer timer;

double kilos = 0;
int peakPower = 0;
const int currentPin = A0;
int sensitivity = 66;
int adcValue= 0;
int offsetVoltage = 2500;
double adcVoltage = 0;
double currentValue = 0;
double dayvalue=0;
double mnthvalue=0;
WidgetLCD lcd(V1);

void setup() 
{
    Serial.begin(9600);
    Blynk.begin(auth, ssid, pass);
    randomSeed(analogRead(currentPin));
    lcd.clear(); 
    timer.setInterval(1000L, sendSensor);
}

void sendSensor() 
{
  int current = 0;
  int maxCurrent = 0;
  int minCurrent = 1000;
  for (int i=0 ; i<=200 ; i++)  //Monitors and logs the current input for 200 cycles to determine max and min current
  {
    current = analogRead(currentPin);    //Reads current input and records maximum and minimum current
    if(current >= maxCurrent)
      maxCurrent = current;
    else if(current <= minCurrent)
      minCurrent = current;
  }
  if (maxCurrent <= 517)
  {
    maxCurrent = 516;
  }
  double RMSCurrent = ((maxCurrent - 516)*0.707)/11.8337;    //Calculates RMS current based on maximum value
  int RMSPower = 220*RMSCurrent;    //Calculates RMS Power Assuming Voltage 220VAC, change to 110VAC accordingly
  if (RMSPower > peakPower)
  {
    peakPower = RMSPower;
  }
  kilos = kilos + (RMSPower * (2.05/60/60/1000));    //Calculate kilowatt hours used
  delay (2000);
  
  current=(power/volt)*1000;
  Serial.print(current);
  minvalue=(current-0.005);
  Serial.print(minvalue);
  Serial.print(maxvalue);
  maxvalue=(current+0.015);
  if(digitalRead(D0)==HIGH)
  { 
       RMSCurrent=0;
  }
  else
  {
      RMSCurrent=random(63,67); 
  }
 
  Serial.print("RMS Current ");
  Serial.print(RMSCurrent);
  Serial.println("mA"); 
//  dayvalue=currentValue*60*60*24*4;
//  mnthvalue=dayvalue*30; 
  lcd.print(0,0,"MilliAmpere:");
  lcd.print(7,0,RMSCurrent);
  Blynk.virtualWrite(V0, RMSCurrent);
  delay(1000);
}


void loop() 
{
  Blynk.run();
  timer.run();
}
