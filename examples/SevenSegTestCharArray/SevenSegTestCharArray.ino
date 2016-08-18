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
  
  char str[] = "abcdefghij_1234567890_ABCDEFGHIJ ~C ~F  ";
  char tmp;
  int len = sizeof(str);   
  disp.write(str);
  while (true){
    for (int i=0;i<30;i++){
      disp.show();
      delay(1);
    }
    str[len-1] = str[0];
    for (int i=0;i<len-1;i++){
      str[i] = str[i+1];
    }
    str[len-1]='\0';

    disp.write(str); 
  } // infinite sidescroller through character array

  
}

