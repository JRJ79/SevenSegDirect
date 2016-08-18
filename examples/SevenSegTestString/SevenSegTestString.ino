#include <SevenSegDirect.h>
  int segmentpins[] = {8,7,12,13,A0,6,11};
  int digitpins[] = {5,4,3,2}; // use {-1} for a single hardwired digit without digitpins. Start with the rightmost digit.
  int numdigits = 4;
  
SevenSegDirect disp;
int DEBUG = 1;

void setup() {
  // put your setup code here, to run once:

  if (DEBUG) {Serial.begin(9600); delay(1000); Serial.println("Serial port opened.");}

  disp.segmentPins(segmentpins);
  disp.digitPins(numdigits,digitpins);

  disp.commonAnode();
  disp.init();
}

void loop() {
  String str = "abcdefghij_1234567890_ABCDEFGHIJ ~C ~K ~F "; // ~C 
  disp.write(str);
  while (true){
    for (int i=0;i<30;i++){
      disp.show();
      delay(1);
    }
    int len = str.length();
    char tmp = str.charAt(0);
    for (int i=0;i<len;i++){
      str.setCharAt(i, str.charAt((i+1)%len));
    }
    str.setCharAt(len-1, tmp);

    
    disp.write(str);
  }
  // infinite sidescroller through String object
}

