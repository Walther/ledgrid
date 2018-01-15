#include <Arduino.h>

#define elements(x)  (sizeof(x) / sizeof((x)[0]))

// row in terms of led matrix pins, positive
unsigned int rows[] = {5, 2, 7, 1, 12, 8, 14, 9};
// column in terms of led matrix pins, ground
unsigned int columns[] = {13, 3, 4, 10, 6, 11, 15, 16};

const int rowCount = elements(rows);
const int columnCount = elements(columns);

unsigned int tick = 0;
unsigned int on;
unsigned int off;

void setup() {
  // "post-process" the pins:
  // pins 1-8 of matrix are at pins 1-8 on teensy
  // pins 9-16 of matrix are at pins 14-21 on teensy
  for (unsigned int i=0; i<elements(rows); i++){
    if(rows[i] >= 9) {
      rows[i] += 5;
    }
  }
  for (unsigned int i=0; i<elements(columns); i++){
    if(columns[i] >= 9) {
      columns[i] += 5;
    }
  }


  // initialize the "positive volt" pins
  for (unsigned int i=0; i<elements(rows);i++) {
    pinMode(rows[i], OUTPUT);
    digitalWrite(rows[i], LOW);
  }
  // initialize the "programmable ground" pins
  for (unsigned int i=0; i<elements(columns);i++) {
    pinMode(columns[i], OUTPUT);
    digitalWrite(columns[i], HIGH);
  }

}

const int heart[rowCount][columnCount] = {
  // note: upside down here in editor
  // because 0,0 is bottom left of the grid
  {0,0,0,0,0,0,0,0},
  {0,0,0,1,1,0,0,0},
  {0,0,1,1,1,1,0,0},
  {0,1,1,1,1,1,1,0},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {0,1,1,0,0,1,1,0},
  {0,0,0,0,0,0,0,0}
};

void drawPixel(int row, int column, int intensity) {
  // safety measure: make sure 0 < intensity < 64
  // TODO: proper intensity gradient
  intensity = max(0, intensity);
  intensity = min(64, intensity);

  // ASSUMPTION: 8x8 = 64 leds, aka single panel only
  // ASSUMPTION: 1 frame per second
  // time per pixel = 0.015625 seconds = 15625 microseconds
  // Factors of 15625: 5 x 3125, 25 x 625, 125 x 125
  // ASSUMPTION: 125 fps
  // time per pixel = 125 microseconds. Convenient!

  on = intensity;
  off = 125 - on;

  if(on > 0) {
    digitalWrite(rows[row], HIGH);
    digitalWrite(columns[column], LOW);
    delayMicroseconds(on);

    digitalWrite(rows[row], LOW);
    digitalWrite(columns[column], HIGH);
    delayMicroseconds(off);
  } else {
    delayMicroseconds(on+off);
  }
}

void loop() {
  // draw all pixels
  for(int row = 0; row < rowCount; row++) {
    for(int column = 0; column < columnCount; column++) {
      // draw a heart
      //drawPixel(row, column, heart[row][column]);
      // draw a gradient
      drawPixel(row, column, row*column +1);
    }
  }
  tick++;
}

