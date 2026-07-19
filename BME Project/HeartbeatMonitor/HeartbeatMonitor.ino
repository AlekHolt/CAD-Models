#define USE_ARDUINO_INTERRUPTS true


#include <Wire.h>
#include <PulseSensorPlayground.h>


// OLED screen settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C


Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);


// Pulse sensor settings
const int PULSE_SENSOR_PIN = A0;
const int LED_PIN = LED_BUILTIN;
const int THRESHOLD = 550;


PulseSensorPlayground pulseSensor;


int bpm = 0;


void setup()
{
  Serial.begin(9600);


  // Start OLED screen
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS))
  {
    Serial.println("OLED screen not found.");


    while (true)
    {
      // Stop here if OLED is not detected
    }
  }


  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);


  display.setTextSize(1);
  display.setCursor(22, 10);
  display.println("PULSE MONITOR");


  display.setTextSize(2);
  display.setCursor(15, 32);
  display.println("Starting...");


  display.display();


  // Start pulse sensor
  pulseSensor.analogInput(PULSE_SENSOR_PIN);
  pulseSensor.blinkOnPulse(LED_PIN);
  pulseSensor.setThreshold(THRESHOLD);


  if (pulseSensor.begin())
  {
    Serial.println("Pulse sensor started.");


    display.clearDisplay();


    display.setTextSize(1);
    display.setCursor(22, 8);
    display.println("PULSE MONITOR");


    display.setTextSize(1);
    display.setCursor(25, 30);
    display.println("Place finger");


    display.display();
  }
  else
  {
    Serial.println("Pulse sensor failed.");


    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(10, 25);
    display.println("Sensor error");
    display.display();
  }
}


void loop()
{
  // Check for a heartbeat
  if (pulseSensor.sawStartOfBeat())
  {
    bpm = pulseSensor.getBeatsPerMinute();


    Serial.print("Heart rate: ");
    Serial.print(bpm);
    Serial.println(" BPM");


    // Only display reasonable readings
    if (bpm >= 30 && bpm <= 220)
    {
      display.clearDisplay();


      display.setTextSize(1);
      display.setCursor(30, 5);
      display.println("HEART RATE");


      display.setTextSize(3);


      if (bpm < 100)
      {
        display.setCursor(38, 22);
      }
      else
      {
        display.setCursor(20, 22);
      }


      display.print(bpm);


      display.setTextSize(1);
      display.setCursor(52, 54);
      display.println("BPM");


      display.display();
    }
  }


  delay(20);
}
/*
* HeartbeatMonitor.ino
* Uses the PulseSensor Playground library to detect heartbeats from a
* PulseSensor (pulsesensor.com) and print each beat + BPM to Serial.
*
* Install first: Sketch > Include Library > Manage Libraries...
*   search "PulseSensor Playground" by World Famous Electronics, install.
*
* Wiring:
*   Signal (purple wire) -> A0
*   +      (red wire)    -> 5V (or 3.3V, per your board)
*   -      (black wire)  -> GND
*
* Open Tools > Serial Monitor at 115200 baud, rest a finger on the
* sensor, and hold still.
*/


#include <PulseSensorPlayground.h>


const int PULSE_PIN = A0;
const int THRESHOLD = 560; // raise/lower if beats are missed or over-triggered


PulseSensorPlayground pulseSensor;


void setup() {
 Serial.begin(115200);


 pulseSensor.analogInput(PULSE_PIN);
 pulseSensor.setThreshold(THRESHOLD);


 if (pulseSensor.begin()) {
   Serial.println("Heartbeat monitor started. Place finger on sensor...");
 }
}


void loop() {
 if (pulseSensor.sawStartOfBeat()) {
   Serial.print("Beat!  BPM: ");
   Serial.println(pulseSensor.getBeatsPerMinute());
   delay(200);
 }
}
