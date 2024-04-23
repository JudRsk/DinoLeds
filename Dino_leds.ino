#include "SoftPWM.h"

const int SOUND_PIN = A5;
const int BUTTON_PIN = 12;
const int POT_PIN = A6;

const int number_leds = 15;
const int yellow_led_1 = A3;
const int yellow_led_2 = A4;
const int yellow_led_3 = 4;
const int yellow_led_4 = 2;
const int yellow_led_5 = 3;
const int red_led_1 = 8;
const int red_led_2 = A2;
const int red_led_3 = A1;
const int red_led_4 = A0;
const int red_led_5 = 7;
const int blue_led_1 = 5;
const int blue_led_2 = 6;
const int blue_led_3 = 10;
const int blue_led_4 = 9;
const int blue_led_5 = 11;

int ledPins[] = { yellow_led_1, yellow_led_2, yellow_led_3, yellow_led_4, yellow_led_5, red_led_1, red_led_2, red_led_3, red_led_4, red_led_5, blue_led_1, blue_led_2, blue_led_3, blue_led_4, blue_led_5 };
int ledRandom[] = { yellow_led_1, yellow_led_2, yellow_led_3, yellow_led_4, yellow_led_5, red_led_1, red_led_2, red_led_3, red_led_4, red_led_5, blue_led_1, blue_led_2, blue_led_3, blue_led_4, blue_led_5 };

int mode = 0;
int pot_value = 0;


void setup() {
  Serial.begin(9600);

  SoftPWMBegin();

  for (int i = 0; i < number_leds; i++) {
    SoftPWMSet(ledPins[i], 0);
    SoftPWMSetFadeTime(ledPins[i], 100, 1000);
  }
  pinMode(POT_PIN, INPUT);
  pinMode(SOUND_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void randomize(int arr[], int n) {
  randomSeed(analogRead(0));
  for (int i = 0; i < n - 1; i++) {
    int j = random(0, n - i);
    int t = arr[j];
    arr[j] = arr[i];
    arr[i] = t;
  }
}

bool changeButton() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(1);
    if (digitalRead(BUTTON_PIN) == LOW) {
      mode++;
      if (mode == 4) {
        mode = 0;
      }
      reset();
      return true;
    }
  }
  return false;
}

void blinking(int arr[], int n) {
  int count = 0;

  for (int i = 0; i < n; i++) {
    SoftPWMSetPercent(arr[i], 50);
    SoftPWMSetFadeTime(arr[i], 100, 1000);
    pot_value = analogRead(POT_PIN);
    if (changeButton()) {
      count = 1;
      break;
    };

    if (mode == 1 || mode == 2) {
      delay(50 + pot_value);
    }
  }
  if (mode == 3){
    delay(100);
  }
  if (mode != 3) {
    delay(100 + pot_value);
  }

  if (count != 1) {
    for (int i = 0; i < n; i++) {
      SoftPWMSetPercent(arr[i], 0);
      SoftPWMSetFadeTime(arr[i], 0, 0);

      pot_value = analogRead(POT_PIN);

      if (changeButton()) {
        break;
      };

      if (mode == 1 || mode == 2) {
        delay(50 + pot_value);
      }
    }
  }

  if (mode == 3){
    delay(100);
  }

  if (mode != 3) {
    delay(100 + pot_value);
  }
}

void reset() {
  int ledPins[] = { yellow_led_1, yellow_led_2, yellow_led_3, yellow_led_4, yellow_led_5, red_led_1, red_led_2, red_led_3, red_led_4, red_led_5, blue_led_1, blue_led_2, blue_led_3, blue_led_4, blue_led_5 };
  for (int i = 0; i < number_leds; i++) {
    SoftPWMSetFadeTime(ledPins[i], 0, 0);
    SoftPWMSetPercent(ledPins[i], 0);
  }
}


void loop() {

  //blinking at same time
  if (mode == 0) {
    reset();
    blinking(ledPins, number_leds);
  }

  //randomly blinking
  else if (mode == 1) {
    reset();
    randomize(ledRandom, number_leds);
    blinking(ledRandom, number_leds);
  }

  //led ring
  else if (mode == 2) {
    reset();
    blinking(ledPins, number_leds);
  }

  else if (mode == 3) {
    //240-280
    //reset();
    int val = map(analogRead(SOUND_PIN) * 50, 13900, 14500, 0, 16);
    blinking(ledPins, val);
    Serial.println(analogRead(SOUND_PIN) * 50);
  }
}
