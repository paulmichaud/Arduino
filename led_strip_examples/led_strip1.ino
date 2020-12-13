// The following examples are how to control an LED string with WS2812B Neopixels using the FastLED library
// Install these two libaries
// Tools > Manage Libraries
// Search for "FastLED" and "SET datastructure" and install
#include <FastLED.h>
#include "set.h"

// hook up the LED strip data line to Arduino board on pin 7
#define LED_PIN 7
// Set the number of LEDs your strip has
// ******* Important: Using an incorrect value can cause the Arduino to crash/malfuction by accessing an array out-of-bounds ****
#define NUM_LEDS 10

CRGB leds[NUM_LEDS];

void setup() {
  // open the serial monitor to see which example is currently running
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  // set maximum brightness
  FastLED.setBrightness(50);
  // you can correct for the best looking color by determinging which of the following options
  // provides the best white. Try each correction with the string all white and look at
  // the light against a sheet of white paper.
  // options: UncorrectedColor, TypicalLEDStrip, TypicalPixelString
  FastLED.setCorrection(TypicalPixelString);
}

void loop() {

  Serial.println("Turn on the 10th LED");
  // turning on the LEDs by setting them to a value, then using the show command
  // turn on LED #10, you can use just about any "web color" or RGB value
  // Remember that they are zero-based indexed, so the first LED is at index 0 and 10 is at 9.
  leds[9] = CRGB::Crimson;
  FastLED.show();
  delay(2000);
  // turn off by setting to black, this time using RGB value
  leds[9] = CRGB(0,0,0);
  FastLED.show();
  delay(2000);
  
  // ================ Random color flood fill ================
  Serial.println("Flood fill");
  // random8 will pick a random 8-bit number, so it will be between 0 and 255
  int r = random8();
  int g = random8();
  int b = random8();
  for(int i = 0; i<=NUM_LEDS-1; i++)
  {
    leds[i] = CRGB(r,g,b);
    FastLED.show();
    delay(10);
  }
  
  // ================ All off and on ================
  Serial.println("Turn all on/off with fill_solid");
  // fill_solid(struct CRGB * pFirstLED, number of LEDs, color)
  // start at given location and turn on given number of LEDS
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  delay(1000);
  // turn them all on
  fill_solid(leds, NUM_LEDS, CRGB::HotPink);
  FastLED.show();
  delay(1000);
  // off
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  delay(1000);
  Serial.println("Use memory address with fill_solid");
  // & is used to reference the memory address at list location in the array
  fill_solid(&leds[NUM_LEDS/2], NUM_LEDS/4, CRGB::Yellow);
  FastLED.show();
  delay(1000);

  // ================ Fill gradient ================
  Serial.println("Fill gradient: Magenta to Yellow");
  fill_gradient_RGB(leds, NUM_LEDS, CRGB::Magenta, CRGB::Yellow);
  FastLED.show();
  delay(2000);
  Serial.println("Fill gradient: Red to Yellow, Green to Blue");
  // you can have up to 4 colors
  fill_gradient_RGB(leds, NUM_LEDS, CRGB::Red, CRGB::Yellow, CRGB::Green, CRGB::Blue );
  FastLED.show();
  delay(2000);
  // off
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  delay(1000);
  
  // ================ Random LED until all of them are on ================
  Serial.println("Random LED until all of them are on");
  Set litLeds; // This is the Set of indexes which we have already been turned on
  while(litLeds.count()<NUM_LEDS)
  {
    int index = random(NUM_LEDS+1);
    // if we haven't turned on this LED, then add to Set and turn it on
    if(!litLeds.has(index))
    {
      litLeds.add(index);
      // Random color
      int r = random8();
      int g = random8();
      int b = random8();
      leds[index] = CRGB(r,g,b);
      FastLED.show();
      delay(10);
    }
  }
  // Randomly turn off
  litLeds.clr();
  while(litLeds.count()<NUM_LEDS)
  {
    int index = random(NUM_LEDS);
    if(!litLeds.has(index))
    {
      litLeds.add(index);
      leds[index] = CRGB(0,0,0);
      FastLED.show();
      delay(10);
    }
  }

  // ================ Color Utils ================
  // color utils http://fastled.io/docs/3.1/group___colorutils.html
  Serial.println("fill_rainbow");
  // the rainbow API, similar to fill_gradient_RGB
  // fill_rainbow( struct CRGB * pFirstLED, int numToFill, uint8_t initialhue, uint8_t deltahue )
  fill_rainbow(leds, NUM_LEDS, 0, 5);
  FastLED.show();
  delay(1000);
  // off
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  delay(1000);

  // ================ Tracer ================
  Serial.println("Simple Tracer");
  fill_solid( leds, NUM_LEDS, CRGB::Black);
  for(int i = 1; i<=NUM_LEDS-1; i++)
  {
    leds[i] = CRGB(r,g,b);
    leds[i-1] = CRGB(0,0,0);
    FastLED.show();
    delay(8);
  }

  // ================ Zig-zag Tracer ================
  Serial.println("zig-zag Tracer");
  // more complex code for a tracer
  // Zig-zag back-and-forth until the random target index happens
  // to be near the end of the string.
  // Again, using a random color
  r = random8();
  g = random8();
  b = random8();
  int target = NUM_LEDS/2;
  int curIndex = 1;
  int offset = -1;
  int i;
  while(curIndex < NUM_LEDS-5)
  {
    leds[curIndex] = CRGB(r,g,b);
    leds[curIndex+offset] = CRGB(0,0,0);
    FastLED.show();
    delay(8);

    if(curIndex == target)
    {
      // new random destination index for the runner
      // random function is non-inclusive, so add +1 to include the last index
      target = random(NUM_LEDS+1);
    }

    if(target > curIndex)
    {
      offset = -1;
    }
    else
    {
      offset = 1;
    }
    curIndex -= offset;
  }
  leds[curIndex] = CRGB(0,0,0);
  FastLED.show();
  delay(15);

  // ================ Christmas lights ================
  Serial.println("Christmas lights - red and green alternating");
  // using the modulus operator you can determine if a number is even or odd
  for(int flash = 1; flash < 7; ++flash)
  {
    CRGB evenColor = CRGB::Red;
    CRGB oddColor = CRGB::Green;
    if(flash%2 != 0)
    {
      evenColor = CRGB::Green;
      oddColor = CRGB::Red;
    }
    for(int i = 0; i<=NUM_LEDS-1; i++)
    {
      if(i%2 == 0)
        leds[i] = evenColor;
      else
        leds[i] = oddColor;
    }
    FastLED.show();
    delay(1000);
  }

  // ================ Fading ================
  Serial.println("Fading in and out");
  int fadeAmount = 5;
  int fadeDirection = 1;
  int brightness = 0;
  int fadeCount = 0;
  while(fadeCount < 6)
  {
    for(int i = 0; i < NUM_LEDS; i++ )
    {
     leds[i].setRGB(0,255,250);  // you can set color like this too
     leds[i].fadeLightBy(brightness);
    }
    FastLED.show();
    brightness += fadeAmount * fadeDirection;
    // reverse the direction of the fading at the ends of the fade
    if(brightness < fadeAmount || brightness == 255)
    {
      fadeDirection = -fadeDirection ;
      fadeCount++;
    }    
    delay(20);
  }

  // turn them all off for our next example
  fill_solid( leds, NUM_LEDS, CRGB::Black);
  FastLED.show();

  // ================ Comet ================
  Serial.println("Comet");
  int hue = random(255) + 1; // (1-255) pick a color
  int saturation = 255; // (0-255)
  int maxBrightness = 250; // (0-255)
  int cometLength = 8;
  int cometsHead = cometLength; // Index that keeps track of the head position
  int curDirection = 1; // this is +/- 1
  int count = 0;
  // go up and down the string 4 times
  while(count < 4)
  {
    int brightness = maxBrightness;
    // set one more LED then the length of the comet
    // but set the last LED to 0 brightness (off)
    for(int i=0; i<=cometLength; i++)
    {
      int offset = i * curDirection; // make offset + or -, depending on direction of travel
      int index = cometsHead - offset; // minus because the tail is trailing behind
      // must perform sanity check to make sure the array index is not out-of-bounds
      if(index > 0 && index < NUM_LEDS)
      {
        // we don't dim the LED, because it is not on yet
        // the brightness is set when the LED color value is set
        leds[index] = CHSV(hue, saturation, brightness);
        if(i == cometLength-1)
          brightness = 0;
        else
          brightness = brightness - (maxBrightness/cometLength); // dim proportionally to the length
      }
    }

    // change direction upon reaching either end of the LED string
    if(cometsHead == 0 || cometsHead == NUM_LEDS-1)
    {
      count++;
      curDirection = curDirection * -1; // multiply by -1 to flip +/-
    }
    
    // increment the head position in whichever direction it is traveling
    cometsHead += 1 * curDirection;
    FastLED.show();
    delay(12);
  } // end while loop
  
  
  // ================ fadeToBlackBy ================
  Serial.println("fadeToBlack");
  // each time this is called it turns down the brightness
  fill_solid(leds, NUM_LEDS, CRGB::Purple);
  for(int i=0; i<255; i++)
  {
    // fade by 1, but could fade by a higher value to fade faster
    // typically, this is not done in a loop like this.
    fadeToBlackBy(leds, NUM_LEDS, 1);
    delay(2);
    FastLED.show();
  }

  // ================ Twinkle ================
  Serial.println("Twinkle");
  // Every 30 milliseconds an LED is turn on
  // All LEDs are faded by 1 each time through the loop
  // twinkle for 15 seconds
  unsigned long start = millis();
  while(millis() - start < 15000)
  {
    // EVERY_N_MILLISECONDS is part of the FastLED library
    EVERY_N_MILLISECONDS(30) {
      leds[random(NUM_LEDS)] = CRGB::Violet;
    }
    fadeToBlackBy(leds, NUM_LEDS, 1);
    FastLED.show();
  }

  // ================ Palettes ================
  // CRGBPalette16
  // DEFINE_GRADIENT_PALETTE()
  // sorry, no example

  // ================ Beats Sine Wave ================
  Serial.println("Beats Sine Wave");
  // Close the Serial Monitor, if you have it open
  // and open the Serial Plotter to see this plotting the wave.
  start = millis();
  while(millis() - start < 10000)
  {
    // 30 Beats Per Minutes
    // we use the whole string, from 0 to NUM_LEDS
    // no time base, no phase offset
    uint8_t beat = beatsin8(30, 0, NUM_LEDS, 0, 0);
    leds[beat] = CRGB::Blue;
    fadeToBlackBy(leds, NUM_LEDS, 10);
    EVERY_N_MILLISECONDS(30) {
        Serial.println(beat);
    }
      FastLED.show();
  }

  // ================ Beats Red, White & Blue ================
  Serial.println("Beats red, white and blue waves");
  start = millis();
  while(millis() - start < 10000)
  {
    // 30 Beats Per Minutes
    // we use the whole string, from 0 to NUM_LEDS
    // phase offsets. Period of wave = 60/bpm * 1000ms
    // phases are offset by 1/3
    uint8_t beat1 = beatsin8(30, 0, NUM_LEDS, 0, 0);
    uint8_t beat2 = beatsin8(30, 0, NUM_LEDS, 0, 85);
    uint8_t beat3 = beatsin8(30, 0, NUM_LEDS, 0, 170);
    
    leds[beat1] = CRGB::Red;
    leds[beat2] = CRGB::White;
    leds[beat3] = CRGB::Blue;
    fadeToBlackBy(leds, NUM_LEDS, 10);

    EVERY_N_MILLISECONDS(10) {
        Serial.print(beat1);
        Serial.print(",");
        Serial.print(beat2);
        Serial.print(",");
        Serial.println(beat3);
    }
    
    FastLED.show();
  }

  // turn them all off for our next example
  fill_solid( leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  Serial.println("Done with examples: Starting loop over...");
  Serial.println();
}
