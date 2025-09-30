//-------------------------
// Title: microphone
//-------------------------
// Program Details:
//-------------------------
// Purpose: Reads microphone analog data. If the room is quite enough, it will turn on the speaker. 
//          Also, during certain thresholds the leds will light up. 
//          Lastly, the speaker will turn on if the user enters a letter B.
// Inputs:  Microphone(analog), user input(keyboard)
// Outputs: 2 LEDs, 1 active speaker
// Date:  9/29/25
// Compiler:  3.1.0-gcc10.3-e5f9fec
// Author:  Isaac Rodriguez
// Versions:
//            V1 - uses sound intensity from enviornment 
//-------------------------
// File Dependancies:
//                    None
//-------------------------

#include <Arduino.h>

//-------------------------
// Main Program
//-------------------------

#define LED1 D8
#define LED2 D7
#define BUZZER D6
const int sampleWindow = 500;  
int const AMP_PIN = A0;       // Preamp output pin connected to A0
unsigned int sample;
char receivedChar;
boolean newData = false;

void setup()
{
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}

void loop()
{
  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level
  unsigned int dB_val = 0;
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 50 mS and then plot data
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(AMP_PIN);
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  dB_val = 10.744*log(peakToPeak) + 8.6451;
  Serial.print("dB reading: ");
  Serial.println(dB_val);

  if (dB_val < 45)
  {
    digitalWrite(BUZZER, HIGH); // buzzer
    digitalWrite(LED1, LOW); // some color
    digitalWrite(LED2, LOW); // some other color
  }

  if ((dB_val > 50) & (dB_val < 70))
  {
    digitalWrite(LED1, HIGH); // some color
    digitalWrite(BUZZER, LOW); // buzzer
    digitalWrite(LED2, LOW); // some other color
  }
  else if (dB_val >= 70)
  {
    digitalWrite(LED2, HIGH); // some other color
    digitalWrite(LED1, LOW); // some color
    digitalWrite(BUZZER, LOW); // buzzer
  }
   

  if (Serial.available() > 0) {   // read user input data
    receivedChar = Serial.read();
    newData = true;
  }
  if (newData == true) {
    Serial.println(receivedChar);
    newData = false;
    if (receivedChar == 'B'){
      digitalWrite(BUZZER, HIGH);
      delay(5000);
      digitalWrite(BUZZER, LOW);
    }
  }

}
