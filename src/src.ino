#define elements(x)  (sizeof(x) / sizeof((x)[0]))
// array element counter

// row in terms of led matrix pins, positive
int rows[] = {5, 2, 7, 1, 12, 8, 14, 9};
// column in terms of led matrix pins, ground
int columns[] = {13, 3, 4, 10, 6, 11, 15, 16};

const int rowCount = elements(rows);
const int columnCount = elements(columns);

int tick = 0;
int on;
int off;

int slowdown = 1;
// low slowdown = entire grid looks "on",
// higher slowdown = individual led progression visible
// try values between 1 and 1000

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
  // currently intensity is 0 or 1
  // safety measure: make sure 0 < intensity < 1
  // TODO: proper intensity gradient
  intensity = max(0, intensity);
  intensity = min(intensity, 1);

  // safety measure: PWM 25%, don't burn the leds or the teensy
  on = slowdown * intensity;
  off = 4 * on;


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

  // TODO: make sure each drawPixel always takes the same time
  // so that each full frame takes a constant time
}

void loop() {
  // draw all pixels
  for(int row = 0; row < rowCount; row++) {
    for(int column = 0; column < columnCount; column++) {
      // draw the figure
      drawPixel(row, column, heart[row][column]);
    }
  }
  tick++;
}

