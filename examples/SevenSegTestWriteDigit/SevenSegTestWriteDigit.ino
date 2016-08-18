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
}

void loop() {
  
  disp.writeDigit(3,2);
  disp.writeDigit(2,7);
  disp.setDigit(1,B1100011);
  disp.setDigit(0,B0001101);
  disp.setLed(0,'e');
  disp.setLed(0,'f');
  disp.unsetLed(0,'c');// 27 degrees C | tests integers and lowlevel

/*
  disp.writeDigit(3,'a');
  disp.writeDigit(2,'b');
  disp.writeDigit(1,'c');
  disp.writeDigit(0,'d'); // tests characters
*/

  while (true){
    disp.show();
    delay(1);
  }
}

