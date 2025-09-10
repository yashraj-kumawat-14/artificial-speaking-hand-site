#include <SD.h>
#include <TMRpcm.h>
#include <SPI.h>

TMRpcm audio;

// Pin definitions
#define SD_CS 4       // Chip select for SD card
#define SPEAKER_PIN 9 // Speaker output (PWM)

// Flex sensor pins
#define FLEX1 A0
#define FLEX2 A1
#define FLEX3 A2
#define FLEX4 A3

// Threshold values (adjust after calibration)
int th1 = 130;
int th2 = 130;
int th3 = 130;
int th4 = 130;

// State tracking to avoid repeating same sound
int lastGesture = 0;

void setup() {
  Serial.begin(9600);

  // Setup SD card
  if (!SD.begin(SD_CS)) {
    Serial.println("SD initialization failed!");
    while (1);
  }
  Serial.println("SD OK");

  // Setup audio
  audio.speakerPin = SPEAKER_PIN;
  audio.setVolume(5);  // 0–7 (lower = quieter, cleaner)

  // Flex sensors as inputs
  pinMode(FLEX1, INPUT);
  pinMode(FLEX2, INPUT);
  pinMode(FLEX3, INPUT);
  pinMode(FLEX4, INPUT);

  Serial.println("Smart Speaking Hand Ready");
}

void loop() {
  // Read flex sensors
  int f1 = analogRead(FLEX1);
  int f2 = analogRead(FLEX2);
  int f3 = analogRead(FLEX3);
  int f4 = analogRead(FLEX4);

  // Debugging
  Serial.print("F1:"); Serial.print(f1);
  Serial.print(" F2:"); Serial.print(f2);
  Serial.print(" F3:"); Serial.print(f3);
  Serial.print(" F4:"); Serial.println(f4);

  int gesture = detectGesture(f1, f2, f3, f4);

  if (gesture != 0 && gesture != lastGesture) {
    playGesture(gesture);
    lastGesture = gesture;
  }

  delay(200); // small debounce delay
}

// Detect gesture based on flex thresholds
int detectGesture(int f1, int f2, int f3, int f4) {
  if (f1 > th1 && f2 < th2 && f3 < th3 && f4 < th4) {
    return 1; // Gesture 1
  }
  else if (f1 < th1 && f2 > th2 && f3 < th3 && f4 < th4) {
    return 2; // Gesture 2
  }
  else if (f1 < th1 && f2 < th2 && f3 > th3 && f4 < th4) {
    return 3; // Gesture 3
  }
  else if (f1 < th1 && f2 < th2 && f3 < th3 && f4 > th4) {
    return 4; // Gesture 4
  }
  else {
    return 0; // No valid gesture
  }
}

// Play the corresponding audio file
void playGesture(int gesture) {
  switch (gesture) {
    case 1:
      audio.play("1.wav"); // Please give me water
      break;
    case 2:
      audio.play("2.wav"); // I need help
      break;
    case 3:
      audio.play("3.wav"); // Call my family
      break;
    case 4:
      audio.play("4.wav"); // Thank you
      break;
  }
}
