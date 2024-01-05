#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif

#define blueSerial Serial1
#include <SoftwareSerial.h>
SoftwareSerial HM10{ 2, 3 };  // RX, TX pins on Arduino (connect TX of Bluetooth to 3, RX to 2)

char appData;
String inData = "";

#define PIN_NEO_PIXEL 6    // Arduino pin that connects to NeoPixel
#define NUM_PIXELS 300     // The number of LEDs (pixels) on NeoPixel
#define DELAY_INTERVAL 50  // 50ms pause between each pixel

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);

  NeoPixel.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  NeoPixel.clear();  // set all pixel colors to 'off'. It only takes effect if pixels.show() is called
  NeoPixel.show();

  Serial.println("HM10 serial started at 9600");
  HM10.begin(9600);

  int pixel = 0;

  for (int y = 0; y < 300; y++) {
    NeoPixel.setPixelColor(y, NeoPixel.Color(253, 244, 220));
    NeoPixel.show();        // send the updated pixel colors to the NeoPixel hardware.
    delay(DELAY_INTERVAL);  // pause between each pixel
  }
}

void loop() {
  if (HM10.available()) {
    // Read data from HM10
    String receivedData = HM10.readStringUntil('\n');

    // Process the received data
    processBluetoothData(receivedData);
  }
}

void processBluetoothData(String data) {

  // Convert the received data to an integer
  int heartRate = data.toInt();
  int pixel = 0;  //indicates which pixel it is

  //CALM MODE: below code will be put to trigger when heartrate is above 110 bpm
  if (heartRate > 100) {
    // 1st color: turns lightest blue
    for (int x = 0; x < 100; x++) {  //x is num pixels that turn this color
      NeoPixel.setPixelColor(pixel, NeoPixel.Color(33, 150, 243));
      NeoPixel.show();
      pixel++;
      delay(DELAY_INTERVAL);  // pause between each pixel
    }
    // 2nd color
    for (int x = 0; x < 100; x++) {
      NeoPixel.setPixelColor(pixel, NeoPixel.Color(25, 41, 255));
      NeoPixel.show();
      pixel++;
      delay(DELAY_INTERVAL);
    }
    // 3rd color
    for (int x = 0; x < 100; x++) {
      NeoPixel.setPixelColor(pixel, NeoPixel.Color(0, 0, 139));
      NeoPixel.show();
      pixel++;
      delay(DELAY_INTERVAL);
    }
  } else {
    //Regular MODE: below code will be put to trigger when heartrate is below 110BPM
    for (int y = 300; y >= 0; y--) {
      NeoPixel.setPixelColor(y, NeoPixel.Color(0, 0, 0));
      NeoPixel.show();
      NeoPixel.setPixelColor(y, NeoPixel.Color(253, 244, 220));
      NeoPixel.show();        // send the updated pixel colors to the NeoPixel hardware.
      delay(DELAY_INTERVAL);  // pause between each pixel
    }
  }
}