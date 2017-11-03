#include "HX711-multi.h"

// Pins to the load cell amp
#define CLK PA0      // clock pin to the load cell amp
#define DOUT1 PA1    // data pin to the first lca
#define DOUT2 PA2    // data pin to the second lca
#define DOUT3 PA3    // data pin to the third lca
//#define DOUT4 PA4
//#define DOUT5 PA5


#define TARE_TIMEOUT_SECONDS 4

byte DOUTS[3] = {DOUT1, DOUT2, DOUT3/*, DOUT4, DOUT5*/};

#define CHANNEL_COUNT sizeof(DOUTS)/sizeof(byte)

long int results[CHANNEL_COUNT];

HX711MULTI scales(CHANNEL_COUNT, DOUTS, CLK);

void setup() {
  Serial.begin(9600);
  Serial.flush();
  pinMode(PC13, OUTPUT);
  
  tare();
}


void tare() {
  bool tareSuccessful = false;

  unsigned long tareStartTime = millis();
  while (!tareSuccessful && millis()<(tareStartTime+TARE_TIMEOUT_SECONDS*1000)) {
    tareSuccessful = scales.tare(20,10000);  //reject 'tare' if still ringing
  }
}

void sendRawData() {
  scales.read(results);
char info[50];
   String str;
   sprintf(info,"%lu,%lu,%lu,0,0",-results[0],-results[1],-results[2]/*,-results[3],-results[4]*/);
   str += info;
   digitalWrite(PC13,HIGH);  
   Serial.println(str); 
  delay(100);
  digitalWrite(PC13,LOW);
}

void loop() {

  sendRawData(); 


  if (Serial.available()>0) {
    while (Serial.available()) {
      Serial.read();
    }
    tare();
  }
 }


