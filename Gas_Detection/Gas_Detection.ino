#include <Servo.h>

#define POTENTIOMETER_PIN A2
#define LPG_PIN A1
#define SERVO_PIN 2

#define LPG_THRESHOLD 150
#define FLAME_THRESHOLD 50

#define RT0 10000
#define B 3977
#define VCC 5
#define R 10000
float RT, VR, ln, TX,  T0, VRT;

int lpgValue;
int flameValue;
int knobValue;
bool gasDetected = false;
bool flameDetected = false;

Servo servo;

void setup() {
  pinMode(POTENTIOMETER_PIN, INPUT);
  servo.attach(SERVO_PIN);
  Serial.begin(9600);
  T0 = 25 + 273.15;
}

void loop() {
  servo.detach();

  VRT = analogRead(A0);              //Acquisition analog value of VRT
  VRT  = (5.00 / 1023.00) * VRT;      //Conversion to voltage
  VR = VCC - VRT;
  RT = VRT / (VR / R);               //Resistance of RT
  ln = log(RT / RT0);
  TX = (1 / ((ln / B) + (1 / T0))); //Temperature from thermistor
  TX =  TX - 273.15;

  lpgValue = analogRead(LPG_PIN);
  flameValue = TX;
  knobValue = analogRead(POTENTIOMETER_PIN);

  if (lpgValue > LPG_THRESHOLD) {
    gasDetected = true;
  } else {
    gasDetected = false;
  }

  if (flameValue > FLAME_THRESHOLD) {
    flameDetected = true;
  } else {
    flameDetected = false;
  }

  Serial.print("LPG Value: ");
  Serial.println(lpgValue);
  Serial.print("Flame Value: ");
  Serial.println(flameValue);
  Serial.print("Knob Value: ");
  Serial.println(knobValue);
  Serial.println();

  if (gasDetected && !flameDetected) {
    servo.attach(SERVO_PIN);
    servo.write(180);

    while ((analogRead(POTENTIOMETER_PIN)) > 500) {
      delay(100);
    }

   if (knobValue < 500) {
      Serial.println("Knob Closed Successfully");
   }
  }

  delay(1000);
}