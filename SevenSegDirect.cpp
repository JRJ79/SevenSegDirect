/*
  SevenSegDirect v.1.0
*/

#include "Arduino.h"
#include "SevenSegDirect.h"
#define DEBUG true


// Constructor
SevenSegDirect::SevenSegDirect(){
  
  commonCathode(); // By default.
  _DP = -1; // no DP defined initially
  _numDigits = 1; // one digit by default
  _colonType = NO_COLON; // by default
  _colonDigit = -1;
  _colonSegment = -1;
  
  _onDelay = 2;
}

void SevenSegDirect::segmentPins(int segmentPins[]){
	
	_A = segmentPins[0];
	_B = segmentPins[1];
	_C = segmentPins[2];
	_D = segmentPins[3];
	_E = segmentPins[4];
	_F = segmentPins[5];
	_G = segmentPins[6];
	
	_segPins[0]=_A;
	_segPins[1]=_B;
	_segPins[2]=_C;
	_segPins[3]=_D;
	_segPins[4]=_E;
	_segPins[5]=_F;
	_segPins[6]=_G;
	_segPins[7]=_DP;

}
void SevenSegDirect::digitPins(int numdigits, int digitpins[]){
	_digitPins = digitpins;
	_numDigits = numdigits;

	_maxLong = 1;
	for (int i=1;i<_numDigits;i++){
		_maxLong *= 10;
	}
	_minLong = -_maxLong + 1;   // Ex: if 4 digits, ends up at -999
	_maxLong = _maxLong*10 - 1; // Ex: if 4 digits, ends up at 9999
	_nextDigit = 0;
}
void SevenSegDirect::setDPPin(int pin){
	_DP = pin;
	_segPins[7] = _DP;
	//_colonType = COLON_DP; // default to using DPs as colon when they're defined. Causes issues with order of setting DP and Colon types.
}

void SevenSegDirect::setColonType(int type){
	setColonType(type,0,0);
}
void SevenSegDirect::setColonType(int type, int dig){
	setColonType(type,dig,0);
}
void SevenSegDirect::setColonType(int type, int dig, int seg){
	if (type == NO_COLON) {
		_colonType = NO_COLON;
	}
	else if (type == COLON_DP) {
		_colonType = COLON_DP;
		_colonDigit = dig;
	}
	else if (type == COLON_SEGDIG) { // TODO
		_colonType = COLON_SEGDIG;
	}

} //TODO
void SevenSegDirect::commonAnode(){
  _commonCathode = false;
  _segOn = LOW;
  _segOff = HIGH;
  _digOn = HIGH;
  _digOff = LOW;
}  // Set the display to CommonAnode.
void SevenSegDirect::commonCathode(){
  _commonCathode = true;
  _segOn = HIGH;
  _segOff = LOW;
  _digOn = LOW;
  _digOff = HIGH;
}  // Set the display to CommonCathode 
void SevenSegDirect::invertDigits(){
  if (_digOn == HIGH){
    _digOn = LOW;
    _digOff = HIGH;
  } else {
    _digOn = HIGH;
    _digOff = LOW;
  }  
} // invert the digitpins, for instance with transistor drives configured to invert
void SevenSegDirect::invertSegments(){
  if (_segOn == HIGH){
    _segOn = LOW;
    _segOff = HIGH;
  } else {
    _segOn = HIGH;
    _segOff = LOW;
  }
} // invert segmentpins, likewise

void SevenSegDirect::init(){
	// set al the segpins as output, and off
	
	for (int i=0;i<8;i++){
		pinMode(_segPins[i], OUTPUT);
		digitalWrite(_segPins[i], _segOff);
	}
	// set all the digitpins as output, and off
	for (int i=0;i<_numDigits;i++){
		if (_digitPins[i] >= 0) {
			pinMode(_digitPins[i], OUTPUT);
			digitalWrite(_digitPins[i],_digOff);
		}	
	}
	// initialise displaycontents array
	for (int i=0;i<MAX_SUPPORTED_DIGITS;i++){
		if (DEBUG) _displaycontents[i] = B01010101; // useful pattern during debugging, looks like lower n with a strip above it
		else _displaycontents[i] = B0; 
	}
	_colon = false;
} // once all the parameters about the display are set, initialise the pins and blank it.

void SevenSegDirect::testLeds(int msecs){
    while (true){
		for (int i=0;i<_numDigits;i++){
			digitalWrite(_digitPins[i],_digOn);
			for (int i=0;i<8;i++){
				digitalWrite(_segPins[i],_segOn);
				delay(msecs);
			}
			for (int i=0;i<8;i++){
				digitalWrite(_segPins[i],_segOff);
				delay(msecs/4);
			}
			digitalWrite(_digitPins[i],_digOff);
		}
	}
} // super low level test function, parameter influences how quickly it runs

void SevenSegDirect::show(){
	for (int i=0;i<_numDigits;i++){
		showDigit(i);
	} // TODO: COLON_SEGDIG case
} // Call showDigit for every digit.
void SevenSegDirect::showDigit(int digit){
	for (int i=0;i<NUMSEGS;i++){
		digitalWrite(_segPins[i],_segOff);
	} // make sure segment pins are off

	byte currDigit = _displaycontents[digit];
	for (int i=0;i<NUMSEGS;i++){
		if (currDigit & B00000001 == B1){
			digitalWrite(_segPins[i],_segOn);
		}
		currDigit >>= 1;
	}
	if (_colon && _colonType == COLON_DP && digit == _colonDigit) digitalWrite(_DP,_segOn);
	digitalWrite(_digitPins[digit],_digOn);

	delay(_onDelay);

	digitalWrite(_digitPins[digit],_digOff);
} // Actually show given digit for a period of _onDelay
void SevenSegDirect::showNextDigit(){
	showDigit(_nextDigit);
	_nextDigit = (_nextDigit +1)% _numDigits;
} // call this from your timer to display one digit of the display TODO: separate Colon
void SevenSegDirect::showColon(){
	if (_colonType == NO_COLON || _colonType == COLON_DP) return;
	else if (_colonType == COLON_SEGDIG) {
	return;
	}
} // TODO

void SevenSegDirect::clearDisp(){
	for (int i=0;i<MAX_SUPPORTED_DIGITS;i++){
		_displaycontents[i] = B0; 
	}
	_colon=false;
}
void SevenSegDirect::setLed(int digit,char segment){
	switch (segment){
		case 'a':
			_displaycontents[digit] |= B00000001;
			break;
		case 'b':
			_displaycontents[digit] |= B00000010;
			break;
		case 'c':
			_displaycontents[digit] |= B00000100;
			break;
		case 'd':
			_displaycontents[digit] |= B00001000;
			break;
		case 'e':
			_displaycontents[digit] |= B00010000;
			break;
		case 'f':
			_displaycontents[digit] |= B00100000;
			break;
		case 'g':
			_displaycontents[digit] |= B01000000;
			break;
		case 'p':
			_displaycontents[digit] |= B10000000;
			break;
	}
} // set an individual segment of the whole display
void SevenSegDirect::unsetLed(int digit,char segment){
	switch (segment){
		case 'a':
			_displaycontents[digit] &= B11111110;
			break;
		case 'b':
			_displaycontents[digit] &= B11111101;
			break;
		case 'c':
			_displaycontents[digit] &= B11111011;
			break;
		case 'd':
			_displaycontents[digit] &= B11110111;
			break;
		case 'e':
			_displaycontents[digit] &= B11101111;
			break;
		case 'f':
			_displaycontents[digit] &= B11011111;
			break;
		case 'g':
			_displaycontents[digit] &= B10111111;
			break;
		case 'p':
			_displaycontents[digit] &= B01111111;
			break;
	}
} // unset an individual segment
void SevenSegDirect::setDigit(int digit,byte pattern){
	_displaycontents[digit] = pattern;
} // set a whole digit to the given bitpattern
void SevenSegDirect::writeDigit(int digit, int value){
	switch (value){
		case 0:
			_displaycontents[digit] = B00111111 | (_displaycontents[digit] & B10000000);
			break;
		case 1:
			_displaycontents[digit] = B00000110 | (_displaycontents[digit] & B10000000);
			break;
		case 2:
			_displaycontents[digit] = B01011011 | (_displaycontents[digit] & B10000000);
			break;
		case 3:
			_displaycontents[digit] = B01001111 | (_displaycontents[digit] & B10000000);
			break;
		case 4:
			_displaycontents[digit] = B01100110 | (_displaycontents[digit] & B10000000);
			break;
		case 5:
			_displaycontents[digit] = B01101101 | (_displaycontents[digit] & B10000000);
			break;
		case 6:
			_displaycontents[digit] = B01111101 | (_displaycontents[digit] & B10000000);
			break;
		case 7:
			_displaycontents[digit] = B00000111 | (_displaycontents[digit] & B10000000);
			break;
		case 8:
			_displaycontents[digit] = B01111111 | (_displaycontents[digit] & B10000000);
			break;
		case 9:
			_displaycontents[digit] = B01101111 | (_displaycontents[digit] & B10000000);
			break;
	}
} // write the given digit with the given number
void SevenSegDirect::writeDigit(int digit, char value){
	if(value>=48&&value<=57){
		writeDigit(digit, value-48); //write numbers using the (int) version
		return;
	}
	switch (value){ // for most of these small and large fall through to the same shape, because, well, 7 seg.
		case '-':
			_displaycontents[digit] = B01000000 | (_displaycontents[digit] & B10000000);
			break;			
		case 'a':
			_displaycontents[digit] = B01011111 | (_displaycontents[digit] & B10000000);
			break;
		case 'A':
			_displaycontents[digit] = B01110111 | (_displaycontents[digit] & B10000000);
			break;
		case 'b':
		case 'B':
			_displaycontents[digit] = B01111100 | (_displaycontents[digit] & B10000000);
			break;
		case 'c':
			_displaycontents[digit] = B01011000 | (_displaycontents[digit] & B10000000);
			break;
		case 'C':
			_displaycontents[digit] = B00111001| (_displaycontents[digit] & B10000000);
			break;
		case 'd':
		case 'D':
			_displaycontents[digit] = B01011110 | (_displaycontents[digit] & B10000000);
			break;
		case 'e':
		case 'E':
			_displaycontents[digit] = B01111001 | (_displaycontents[digit] & B10000000);
			break;
		case 'f':
		case 'F':
			_displaycontents[digit] = B01110001 | (_displaycontents[digit] & B10000000);
			break;
		case 'g':
		case 'G':
			_displaycontents[digit] = B00111101 | (_displaycontents[digit] & B10000000);
			break;
		case 'h':
		case 'H':
			_displaycontents[digit] = B01110100 | (_displaycontents[digit] & B10000000);
			break;
		case 'k':
		case 'K':
		case 'x':
		case 'X':
			_displaycontents[digit] = B01110110 | (_displaycontents[digit] & B10000000);
			break;
		case 'i':
			_displaycontents[digit] = B00010000 | (_displaycontents[digit] & B10000000);
			break;
		case 'I':
			_displaycontents[digit] = B00110000 | (_displaycontents[digit] & B10000000);
			break;
		case 'j':
		case 'J':
			_displaycontents[digit] = B00001110 | (_displaycontents[digit] & B10000000);
			break;
		case 'l':
		case 'L':
			_displaycontents[digit] = B00111000 | (_displaycontents[digit] & B10000000);
			break;
		case 'm':
		case 'M':
			_displaycontents[digit] = B00010101 | (_displaycontents[digit] & B10000000);
			break;
		case 'n':
		case 'N':
			_displaycontents[digit] = B01010100 | (_displaycontents[digit] & B10000000);
			break;
		case 'o':
		case 'O':
			_displaycontents[digit] = B01011100 | (_displaycontents[digit] & B10000000);
			break;
		case 'p':
		case 'P':
			_displaycontents[digit] = B01110011 | (_displaycontents[digit] & B10000000);
			break;
		case 'q':
		case 'Q':
			_displaycontents[digit] = B01100111 | (_displaycontents[digit] & B10000000);
			break;
		case 'r':
		case 'R':
			_displaycontents[digit] = B01010000 | (_displaycontents[digit] & B10000000);
			break;
		case 's':
		case 'S': // conflicts with 5
			_displaycontents[digit] = B01101101 | (_displaycontents[digit] & B10000000);
			break;
		case 't':
		case 'T':
			_displaycontents[digit] = B01111000 | (_displaycontents[digit] & B10000000);
			break;
		case 'u':
		case 'U':
			_displaycontents[digit] = B00011100 | (_displaycontents[digit] & B10000000);
			break;
		case 'v':
		case 'V':
			_displaycontents[digit] = B00111110 | (_displaycontents[digit] & B10000000);
			break;
		case 'w':
		case 'W':
			_displaycontents[digit] = B00111100 | (_displaycontents[digit] & B10000000);
			break;
		case 'y':
		case 'Y':
			_displaycontents[digit] = B01101110 | (_displaycontents[digit] & B10000000);
			break;
		case 'z':
		case 'Z': // conflicts with 2
			_displaycontents[digit] = B01011011 | (_displaycontents[digit] & B10000000);
			break;
		case '~': // 'degrees' symbol
			_displaycontents[digit] = B01100011 | (_displaycontents[digit] & B10000000);
			break;
		case ' ':
			_displaycontents[digit] = B00000000 | (_displaycontents[digit] & B10000000);
			break;
		case '_':
			_displaycontents[digit] = B00001000 | (_displaycontents[digit] & B10000000);
			break;
		default:
			_displaycontents[digit] = B00000001 | (_displaycontents[digit] & B10000000);
			break;
	}
} // write the given digit with the given character
void SevenSegDirect::posOL(){
	clearDisp();
	if (_numDigits >1) write("0L");
	else write("L");
	_overLoad = true;
} // display positive overload indication
void SevenSegDirect::negOL(){
	clearDisp();
	if (_numDigits>2) write("-0L"); 
	else if (_numDigits==2) write("0L"); 
	else write("L");
	_overLoad = true;
}

void SevenSegDirect::write(String value){ 
	int len = _numDigits;
	if (value.length() < len) {
	  len = value.length();
	  clearDisp();
	}
	for (int i=0;i<len;i++){
		writeDigit(i,char(value.charAt(len-1-i)));
	}
} // when string is longer than display, display characters starting from the start of string
void SevenSegDirect::write(char value[]){
	int len = _numDigits;
	if (strlen(value)<len){
	  len = strlen(value);
	  clearDisp();
	}
	for (int i=0;i<len;i++){
  		writeDigit(i,value[len-1-i]);
  	}
}  // when shorter, start display with blanks.

void SevenSegDirect::write(int value){
	write(value, 0);
}
void SevenSegDirect::write(int value, int dp){
	write ((long) value, dp);
}
void SevenSegDirect::write(long value){
	write(value, 0);
}
void SevenSegDirect::write(long value, int dp){
	if (value >= 0){
		write(value, dp, false);
	} else {
		write(-value, dp, true);
	}
} // these four are shims for the full method below
void SevenSegDirect::write(long value, int dp, bool neg){
	clearDisp();

	if (!neg){
		if (value > _maxLong) {posOL(); return;}
		_overLoad = false;
		for (int i=0;i<_numDigits;i++){
			writeDigit(i, int(value%10L));
			value /=10L;
			if (dp==0 && i>0) setLed(i, 'p');
			if (value==0 && dp <= 0 ) break;
			dp -=1;
		}
	} else {
		if (value > -_minLong) {negOL(); return;}
		_overLoad = false;
		int i;
		for (i=0;i<_numDigits;i++){
			writeDigit(i, int(value%10L));
			value /=10L;
			if (dp==0 && i>0 ) setLed(i, 'p');
			if (value==0 && dp <= 0 ) break;
			dp -=1;
		}
		writeDigit(i+1,'-');
	}
}

void SevenSegDirect::write(float value){
	write((double) value);
}
void SevenSegDirect::write(float value, int dp){
	write((double) value,dp);
} // more shims
void SevenSegDirect::write(double value){
	int dp = _numDigits-1;
	double temp = value;
	if (temp < 0) {dp -= 1; temp = -temp;}
	while (true){
		if ( temp < 10 ) break;
		temp /= 10;
		dp -= 1;
	}
	write(value, dp);
} // determine best (most accurate) position of DP
void SevenSegDirect::write(double value, int dp){
	for (int i = 0;i<dp;i++){
		value *= 10;
	}
	long int intValue = (long int) value;
    double remainder = value - intValue;
    if(remainder>=0.5 && value>0) intValue++;
    if(remainder<=0.5 && value<0) intValue--;
	write (intValue, dp);
} // handle floating point numbers with fixed DP
// TODO: ENG/SCI display like 1.2345 E10

void SevenSegDirect::writeClock(int mm, int ss){
	if (_colonType == COLON_SEGDIG) writeClock(mm,ss,':');
	else if (_colonType == COLON_DP) writeClock(mm,ss,'.');
	else writeClock(mm,ss,' ');
}
void SevenSegDirect::writeClock(int hh, int mm, int ss){
	if (_colonType == COLON_SEGDIG) writeClock(hh,mm,ss,':');
	else if (_colonType == COLON_DP) writeClock(hh,mm,ss,'.');
	else writeClock(hh,mm,ss,' ');
} // shims again
void SevenSegDirect::writeClock(int mm, int ss, char colon){
	clearDisp();
	write(mm*100+ss);
	if (colon=='.') setLed(2,'p');
	else if (colon == ':') {
		_colon = true;
	}
} // the simple case
void SevenSegDirect::writeClock(int hh, int mm, int ss, char colon){
	if (_numDigits >= 6){
		if (hh==0) writeClock(mm,ss,colon);
		else {
			clearDisp();
			write(hh*10000L+mm*100L+ss);
			if (colon=='.') {setLed(2,'p'); setLed(4,'p');} // 
			else if (colon == ':') {
				_colon = true;
			}
		}
	} else if (_numDigits == 5){
		if (hh==0) writeClock(mm,ss,colon);
		else if (hh<=9) {
			clearDisp();
			write(hh*10000L+mm*100L+ss);
			if (colon=='.') {setLed(2,'p'); setLed(4,'p');}
			else if (colon == ':') {
				_colon = true;
			}
		} else writeClock (hh,mm,colon); // let the seconds drop off the display
	} else if (_numDigits == 4){
		if (hh==0) writeClock(mm,ss,colon);
		else writeClock(hh,mm,colon); // lose the seconds when necessary
	} 
} // and the overblown complexity case


