#include <SevenSegDirect.h>
  int segmentpins[] = {8,7,12,13,A0,6,11}; // segment pins a through g in order
  int digitpins[] = {5,4,3,2}; // use {-1} for a single hardwired digit without digitpins. Start with the rightmost digit.
  int numdigits = 4;
  
SevenSegDirect disp;

void setup() {
  disp.segmentPins(segmentpins);
  disp.digitPins(numdigits,digitpins);
  disp.setDPPin(10); 
 
  disp.commonAnode();
  disp.init();
}

void loop() {
  int hh = 1;
  int mm = 0;
  int ss = 10;

  long curr_secs = 0;
  int disp_ss, disp_mm, disp_hh = 0;
  int curr_colon = 0;

  long totalmillis = (hh*3600L+mm*60L+ss)*1000L;
  
  unsigned long currentmillis = millis();
  totalmillis += currentmillis;

  while (currentmillis<=totalmillis){
    currentmillis = millis();

    curr_secs = (totalmillis-currentmillis)/500;
    curr_colon = curr_secs % 2;         // curr_colon is set and not set every half second
    curr_secs /= 2;                     // now curr_secs holds the current remaining time in secs

    disp_hh = curr_secs/(60*60);
    disp_mm = (curr_secs/60)%60;
    disp_ss = curr_secs%60;
    
    if (curr_colon) disp.writeClock(disp_hh,disp_mm,disp_ss,'.');
    else disp.writeClock(disp_hh,disp_mm,disp_ss,' '); // sets and unsets colon properly
    disp.show();
    delay(10);  
  }
}

