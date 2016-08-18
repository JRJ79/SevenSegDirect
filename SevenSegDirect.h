/*
  SevenSeg_Jasper v1.0
*/

#ifndef SevenSegDirect_h
#define SevenSegDirect_h
#define MAX_SUPPORTED_DIGITS 8
#define NUMSEGS 8

#define NO_COLON 0
#define COLON_DP 1
#define COLON_SEGDIG 2

#include "Arduino.h"

class SevenSegDirect
{

  public:

    // Constructor
    SevenSegDirect();
    void testLeds(int);
    void segmentPins(int []);
    void digitPins(int,int []);
    void setDPPin(int);
    void setColonType(int);
    void setColonType(int,int);
    void setColonType(int,int,int);

    void commonAnode();
    void commonCathode();
    void invertDigits();
    void invertSegments();
    void init();
    void show();
    void showDigit(int);
    void showNextDigit();
    void showColon();
    void clearDisp();
    void setLed(int,char);
    void unsetLed(int, char);
	void setDigit(int, byte);
	void writeDigit(int, int);
	void writeDigit(int, char);
	void posOL();
	void negOL();
	
	void write(char[]);
	void write(String);
	void write(int);
	void write(int,int);
	void write(long);
	void write(long,int);
	void write(long, int, bool);
	void write(float);
	void write(float,int);
	void write(double);
	void write(double,int);
	
	void writeClock(int,int,char);
	void writeClock(int,int);
	void writeClock(int,int,int,char);
	void writeClock(int,int,int);
	
  private:
    bool 	_commonCathode;
    bool 	_segOn;
    bool 	_segOff;
    bool 	_digOn;
    bool 	_digOff;
    int		_onDelay;
  
    int 	_A;
    int 	_B;
    int 	_C;
    int 	_D;
    int 	_E;
    int 	_F;
    int 	_G;
    int 	_DP;
    int 	*_digitPins;
    int 	_numDigits;
    int 	_segPins[NUMSEGS];
    int		_colonType;
	int		_colonDigit;
	int		_colonSegment;
    
    byte 	_displaycontents[MAX_SUPPORTED_DIGITS];
    bool 	_colon;
    
    long	_maxLong;
    long 	_minLong;
    bool	_overLoad;
    int		_nextDigit;
    

};



#endif
