// Arduino Code for ESP32. The code recieves input via bluetooth and use it to control led strips.
// Code made for ESDA 2 Project for group Hiv3mind. 


#include "BluetoothSerial.h" 
#include <FastLED.h>

FASTLED_USING_NAMESPACE

// init Class:
BluetoothSerial ESP_BT; 

// init PINs: assign any pin on ESP32
#define led_pin_1 4

#define DATA_PIN    13
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    15
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120
// Parameters for Bluetooth interface
int incoming;
char arrayTest[20];
String testString;
char *strings[6];
char *ptr = NULL;

void setup() {
  Serial.begin(115200);
  ESP_BT.begin("ESP32_Control"); //Name of your Bluetooth interface -> will show up on your phone

  pinMode (led_pin_1, OUTPUT);
  
  FastLED.addLeds<LED_TYPE,led_pin_1,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);  
}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {test, sinelon, test1, sinelon_colour, rainbowWithGlitter, confetti, juggle, bpm}; //, , , , ,  };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint8_t varying_hue = 0;
//uint8_t i = 0; 
bool pattern_sweep = false;

void loop() {
  gPatterns[gCurrentPatternNumber]();

  FastLED.show();  
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { varying_hue++; } // slowly cycle the "base color" through the rainbow
  if(pattern_sweep == true){
    EVERY_N_SECONDS( 4 ) { nextPattern();} // change patterns periodically
  }
  //
 // EVERY_N_MILLISECONDS(20){i++;}
  
  // -------------------- Receive Bluetooth signal ----------------------
  if (ESP_BT.available()) 
  {

    testString = ESP_BT.readString();
    testString.toCharArray(arrayTest, 20);
  
    byte index = 0;
    ptr = strtok(arrayTest, ",");

    while(ptr != NULL ){
      strings[index] = ptr;
      index++;
      ptr = strtok(NULL, ",");
    }
    
    for (int n = 0; n < index; n++){
      Serial.print(n);
      Serial.print("  ");
      Serial.println(strings[n]);
    }

      }
    //incoming = ESP_BT.read(); //Read what we receive 

    // separate button ID from button value -> button ID is 10, 20, 30, etc, value is 1 or 0

    
    int button = floor(incoming / 10);
    int value = incoming % 10;
    
    switch (button) {
      case 1:  
        Serial.print("Button 1:"); Serial.println(value);
        gCurrentPatternNumber = 0;
        pattern_sweep = false;

        break;
      case 2:  
        Serial.print("Button 2:"); Serial.println(value);
        gCurrentPatternNumber = 1;
        pattern_sweep = false;

        break;
      case 3:  
        Serial.print("Button 3:"); Serial.println(value);
        gCurrentPatternNumber = 2;
        pattern_sweep = false;

        break;

      case 4:  
        Serial.print("Button 4:"); Serial.println(value);
        gCurrentPatternNumber = 3;
        pattern_sweep = false;

        break;   

      case 5:  
        Serial.print("Button 5:"); Serial.println(value);
        gCurrentPatternNumber = 4;
        pattern_sweep = false;

        break;


      case 6:  
        Serial.print("Button 6:"); Serial.println(value);
        gCurrentPatternNumber = 5;
        pattern_sweep = false;

        break;

      case 7:  
        Serial.print("Button 7:"); Serial.println(value);
        gCurrentPatternNumber = 6;
        pattern_sweep = false;

        break;


      case 8:  
        Serial.print("Button 8:"); Serial.println(value);
        gCurrentPatternNumber = 7;
        pattern_sweep = false;
        break;

      case 9:  
        Serial.print("Button 9:"); Serial.println(value);
        //gCurrentPatternNumber = 8;
        pattern_sweep = false;
        //pattern_sweep = true;
        break;

          
  
  }
}


#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
  
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
  
}

void sinelon_colour()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( varying_hue, 255, 192);
  
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void test(){


  leds[1] =  CHSV(gHue, 255, 255);
  leds[3] =  CHSV(gHue, 255, 255);
  leds[5] =  CHSV(gHue, 255, 255);
  leds[7] =  CHSV(gHue, 255, 255);
  leds[9] =  CHSV(gHue, 255, 255);
  leds[11] =  CHSV(gHue, 255, 255);
  leds[13] =  CHSV(gHue, 255, 255);
  leds[0] =  CHSV(gHue, 0, 0);
  leds[2] =  CHSV(gHue, 0, 0);
  leds[4] =  CHSV(gHue, 0, 0);
  leds[6] =  CHSV(gHue, 0, 0);
  leds[8] =  CHSV(gHue, 0, 0);
  leds[10] =  CHSV(gHue, 0, 0);
  leds[12] = CHSV(gHue, 0, 0);
  leds[14] =  CHSV(gHue, 0, 0);
}

void test1(){


  leds[1] =  CHSV(varying_hue, 255, 255);
  leds[3] =  CHSV(varying_hue, 255, 255);
  leds[5] =  CHSV(varying_hue, 255, 255);
  leds[7] =  CHSV(varying_hue, 255, 255);
  leds[9] =  CHSV(varying_hue, 255, 255);
  leds[11] =  CHSV(varying_hue, 255, 255);
  leds[13] =  CHSV(varying_hue, 255, 255);
  leds[0] =  CHSV(varying_hue, 255, 255);
  leds[2] =  CHSV(varying_hue, 255, 255);
  leds[4] =  CHSV(varying_hue, 255, 255);
  leds[6] =  CHSV(varying_hue, 255, 255);
  leds[8] =  CHSV(varying_hue, 255, 255);
  leds[10] =  CHSV(varying_hue, 255, 255);
  leds[12] = CHSV(varying_hue, 255, 255);
  leds[14] =  CHSV(varying_hue, 255, 255);
}
