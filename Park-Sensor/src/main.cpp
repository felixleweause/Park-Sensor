#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_VL53L0X.h"
#include <FastLED.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "painlessMesh.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
Adafruit_VL53L0X::VL53L0X_Sense_config_t long_range = Adafruit_VL53L0X::VL53L0X_SENSE_LONG_RANGE;

String range = ""; 

// LED
#define NUM_LEDS 16 

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 14
CRGB leds[16];


// Mesh
#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555



Scheduler userScheduler;

painlessMesh  mesh;


// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  String msg = range;
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}






void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();


}


void loop() {
  mesh.update();
  VL53L0X_RangingMeasurementData_t measure;
    
  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("Distance (mm): "); 
    Serial.println(measure.RangeMilliMeter);
    range = measure.RangeMilliMeter;
    if (measure.RangeMilliMeter>1000){  
    leds[0] = CRGB::Black; FastLED.show();
    leds[1] = CRGB::Black; FastLED.show();
    leds[2] = CRGB::Black; FastLED.show();
    leds[3] = CRGB::Black; FastLED.show();
    leds[4] = CRGB::Black; FastLED.show();
    leds[5] = CRGB::Black; FastLED.show(); 
    leds[6] = CRGB::Black; FastLED.show(); 
    leds[8] = CRGB::Black; FastLED.show(); 
    leds[9] = CRGB::Black; FastLED.show(); 
    leds[10] = CRGB::Black; FastLED.show(); 
    FastLED.show();
  } else if (measure.RangeMilliMeter<1000 && measure.RangeMilliMeter>900){  
    leds[0] = CRGB::White; FastLED.show();
    leds[1] = CRGB::White; FastLED.show();
    leds[2] = CRGB::White; FastLED.show();
    leds[3] = CRGB::White; FastLED.show();
    leds[4] = CRGB::White; FastLED.show();
    leds[5] = CRGB::White; FastLED.show(); 
    leds[6] = CRGB::White; FastLED.show(); 
    leds[8] = CRGB::White; FastLED.show(); 
    leds[9] = CRGB::White; FastLED.show(); 
    leds[10] = CRGB::White; FastLED.show(); 
    FastLED.show();
  } else if (measure.RangeMilliMeter<700 && measure.RangeMilliMeter>600){
    leds[0] = CRGB::Green; FastLED.show();
    leds[1] = CRGB::Green; FastLED.show();
    leds[2] = CRGB::Black; FastLED.show();
    leds[3] = CRGB::Black; FastLED.show();
    leds[4] = CRGB::Black; FastLED.show();
    leds[5] = CRGB::Black; FastLED.show(); 
    leds[6] = CRGB::Black; FastLED.show(); 
    leds[8] = CRGB::Black; FastLED.show(); 
    leds[9] = CRGB::Black; FastLED.show(); 
    leds[10] = CRGB::Black; FastLED.show(); 
    FastLED.show();
  } else if (measure.RangeMilliMeter<600 && measure.RangeMilliMeter>500){
    leds[0] = CRGB::Green; FastLED.show();
    leds[1] = CRGB::Green; FastLED.show();
    leds[2] = CRGB::Green; FastLED.show();
    leds[3] = CRGB::Green; FastLED.show();
    leds[4] = CRGB::Black; FastLED.show();
    leds[5] = CRGB::Black; FastLED.show(); 
    leds[6] = CRGB::Black; FastLED.show(); 
    leds[8] = CRGB::Black; FastLED.show(); 
    leds[9] = CRGB::Black; FastLED.show(); 
    leds[10] = CRGB::Black; FastLED.show(); 
    FastLED.show();
  } else if (measure.RangeMilliMeter<500 && measure.RangeMilliMeter>400){
    leds[0] = CRGB::Green; FastLED.show();
    leds[1] = CRGB::Green; FastLED.show();
    leds[2] = CRGB::Green; FastLED.show();
    leds[3] = CRGB::Green; FastLED.show();
    leds[4] = CRGB::Green; FastLED.show();
    leds[5] = CRGB::Green; FastLED.show(); 
    leds[6] = CRGB::Black; FastLED.show(); 
    leds[8] = CRGB::Black; FastLED.show(); 
    leds[9] = CRGB::Black; FastLED.show(); 
    leds[10] = CRGB::Black; FastLED.show(); 
    FastLED.show();
  } else if (measure.RangeMilliMeter<400 && measure.RangeMilliMeter>300){
    leds[0] = CRGB::Green; FastLED.show();
    leds[1] = CRGB::Green; FastLED.show();
    leds[2] = CRGB::Green; FastLED.show();
    leds[3] = CRGB::Green; FastLED.show();
    leds[4] = CRGB::Green; FastLED.show();
    leds[5] = CRGB::Green; FastLED.show(); 
    leds[6] = CRGB::Green; FastLED.show(); 
    leds[8] = CRGB::Green; FastLED.show(); 
    leds[9] = CRGB::Black; FastLED.show(); 
    leds[10] = CRGB::Black; FastLED.show(); 
    FastLED.show();
  } else if (measure.RangeMilliMeter<300 && measure.RangeMilliMeter>200){
    leds[0] = CRGB::Green; FastLED.show();
    leds[1] = CRGB::Green; FastLED.show();
    leds[2] = CRGB::Green; FastLED.show();
    leds[3] = CRGB::Green; FastLED.show();
    leds[4] = CRGB::Green; FastLED.show();
    leds[5] = CRGB::Green; FastLED.show(); 
    leds[6] = CRGB::Green; FastLED.show(); 
    leds[8] = CRGB::Green; FastLED.show(); 
    leds[9] = CRGB::Green; FastLED.show(); 
    leds[10] = CRGB::Green; FastLED.show(); 
    FastLED.show();
  } else if (measure.RangeMilliMeter<200){
    leds[0] = CRGB::Red; FastLED.show();
    leds[1] = CRGB::Red; FastLED.show();
    leds[2] = CRGB::Red; FastLED.show();
    leds[3] = CRGB::Red; FastLED.show();
    leds[4] = CRGB::Red; FastLED.show();
    leds[5] = CRGB::Red; FastLED.show(); 
    leds[6] = CRGB::Red; FastLED.show(); 
    leds[8] = CRGB::Red; FastLED.show(); 
    leds[9] = CRGB::Red; FastLED.show(); 
    leds[10] = CRGB::Red; FastLED.show(); 
    FastLED.show();
  } 
  } else {
    Serial.println(" out of range ");
  }
}