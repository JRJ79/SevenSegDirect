#include <SevenSegDirect.h>
  int segmentpins[] = {8,7,12,13,A0,6,11};
  int digitpins[] = {5,4,3,2}; // use {-1} for a single hardwired digit without digitpins. Start with the rightmost digit.
  int numdigits = 4;
  
SevenSegDirect disp;

void setup() {
  // put your setup code here, to run once:

  disp.segmentPins(segmentpins);
  disp.digitPins(numdigits,digitpins);

  disp.commonAnode();
  disp.init();
  disp.testLeds(200);
}

void loop() {
}

