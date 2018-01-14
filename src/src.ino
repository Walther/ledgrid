#define NELEMS(x)  (sizeof(x) / sizeof((x)[0])) // array element counter

const int ledPins[] = {0,1,2,3,4,5,6,7}; // volt pins are y-axis aka rows
const int groundPins[] = {14,15,16,17,18,19,20,21}; // groundpins are x-axis aka columns
const int ledPinCount = NELEMS(ledPins);
const int groundPinCount = NELEMS(groundPins);

int tick = 0;
int on;
int off;

int slowdown = 100;
// low slowdown = entire grid looks "on",
// higher slowdown = individual led progression visible

void setup() {
  // initialize the "positive volt" pins
  for(int i=0; i<NELEMS(ledPins);i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  // initialize the "programmable ground" pins
  for(int i=0; i<NELEMS(groundPins);i++) {
    pinMode(groundPins[i], OUTPUT);
    digitalWrite(groundPins[i], HIGH);
  }

}

void loop() {
  for(int ledPin = 0; ledPin < ledPinCount; ledPin++) {
      for(int groundPin = 0; groundPin < groundPinCount; groundPin++) {
        // PWM 10%
        on = slowdown * 10;
        off = slowdown * 100 - on;
        
        digitalWrite(ledPin, HIGH);   // set the LED on
        digitalWrite(groundPins[groundPin], LOW);
        delayMicroseconds(on);
        digitalWrite(ledPin, LOW);    // set the LED off
        delayMicroseconds(off);
        digitalWrite(groundPins[groundPin], HIGH);
      }
    }
    tick++;
}

