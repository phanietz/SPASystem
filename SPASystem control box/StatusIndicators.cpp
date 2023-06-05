#include "StatusIndicators.h"
#include <FastLED.h>
#define NUM_LEDS 1
#define LED_PIN 8
CRGB leds[NUM_LEDS];
//https://github.com/FastLED/FastLED/wiki/Rgb-calibration

Colors::Colors(){ //Constructor  
  //delay(2000);  
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255); 
}

void Colors::testColor(){
  /*leds[0] = CRGB::FloralWhite; FastLED.show(); delay(4000);
  leds[0] = CRGB::Black; FastLED.show(); delay(800);
  leds[0] = CRGB::Cyan; FastLED.show(); delay(4000);
  leds[0] = CRGB::Blue; FastLED.show(); delay(4000);
  leds[0] = CRGB::DeepSkyBlue; FastLED.show(); delay(4000);
  leds[0] = CRGB::LightBlue; FastLED.show(); delay(4000);
  leds[0] = CRGB::LightSkyBlue; FastLED.show(); delay(4000);
  */
  
  leds[0] = CRGB::LightCoral; FastLED.show(); delay(4000);
  //leds[0] = CRGB::SaddleBrown; FastLED.show(); delay(4000);
  //leds[0] = CRGB::OrangeRed; FastLED.show(); delay(4000);
  //leds[0] = CRGB(147,11,168); FastLED.show(); delay(4000);
  //leds[0] = CRGB::Red; FastLED.show(); delay(4000); 
}

void Colors::WeatherTech(){
  leds[0] = CRGB::White; FastLED.show();
  //leds[1] = CRGB::Red; FastLED.show(); 
}

   
void Colors::Error(){
  leds[0] = CRGB::Red; FastLED.show(); 
  delay(300);
  leds[0] = CRGB::White; FastLED.show();
  delay(200);
  leds[0] = CRGB::Red; FastLED.show(); 
  delay(300);
  leds[0] = CRGB::White; FastLED.show();
  delay(200);
  leds[0] = CRGB::Red; FastLED.show(); 
  delay(300);
  leds[0] = CRGB::White; FastLED.show();
  delay(200);
  leds[0] = CRGB::Red; FastLED.show(); 
}

void Colors::Warning(){
  leds[0] = CRGB::OrangeRed; FastLED.show();
}

void Colors::Question(){
  leds[0] = CRGB::Yellow; FastLED.show();
}

void Colors::Successful(){
  
  leds[0] = CRGB::Green; FastLED.show();
  delay(400);
  Control();
  delay(200);
  leds[0] = CRGB::Green; FastLED.show();
  delay(400);
  Control();
  delay(200);
  leds[0] = CRGB::Green; FastLED.show();
  delay(600);   
  Control();
    
}

void Colors::Move(){
  leds[0] = CRGB::White; FastLED.show();
}
void Colors::PushButtom(){
  leds[0] = CRGB::Black; FastLED.show();
  delay(200); 
}

void Colors::Control(){
  if(control == 0){
    leds[0] = CRGB::Blue; FastLED.show();  
  }else if(control == 1){
    leds[0] = CRGB::Cyan; FastLED.show();
  }
}

void Colors::White(){
  leds[0] = CRGB::White; FastLED.show();
}

void Colors::ChangeMoving(){
  
  leds[0] = CRGB::Tomato; FastLED.show();
  delay(800);
  leds[0] = CRGB::OrangeRed; FastLED.show();
    
}

void Colors::info(){
  leds[0] = CRGB::LightCoral; FastLED.show();
}
