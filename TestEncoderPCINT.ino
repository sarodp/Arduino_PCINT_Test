/*  Arduino Rotary Encoder with PinCode Interrupt
 *      
 *  PCINT tutorial
 *  https://www.electrosoftcloud.com/en/pcint-interrupts-on-arduino/
 *  
 *  Rotary Encoder Swithc Tutorial by Dejan Nedelkovski 
 *  www.HowToMechatronics.com
 *  
 *  combined coding  by psarod@gmail.com  4aug2022
 */

//***** Wiring ***********************************
//  NANO              Rotary Encoder Switch  
//   +5V -----R10K--+---encoderA  
//   D7-------------| 
//
//   +5V -----R10K--+---encoderB
//   D6-------------| 
//
//   +5V -----R10K--+---encoderSW 
//   D5-------------| 
// 
//   GND----------------commonSW,commonAB
//
// 
//  Note:
//  The Three R10K pullup resisters are optional.
//*************************************************

 
 #define encoderA 7
 #define encoderB 6
 #define encoderSW 5
 
 int counter = 0; 
 int aLastState;  

//===============================================
void setup() { 
 //---init PD port interrupt
 //PCICR  |= B00000001; // Enable interrupts on PB port
 //PCICR  |= B00000010; // Enable interrupts on PC port
   PCICR  |= B00000100; // Enable interrupts on PD port
 
 //PCMSK0 |= B00000100; // Masking interrupts on PB D13..D8 pins=D10
 //PCMSK1 |= B00001000; // Masking interrupts on PC A5...A0 pins=A3
   PCMSK2 |= B11100000; // Masking interrupts on PD D7..D0 pins=D7,D6,D5
 
   pinMode (encoderSW,INPUT_PULLUP);
   pinMode (encoderA,INPUT_PULLUP);
   pinMode (encoderB,INPUT_PULLUP);
   //pinMode (encoderSW,INPUT);
   //pinMode (encoderA,INPUT);
   //pinMode (encoderB,INPUT);
   
   Serial.begin (74800);
   Serial.print("Test Encoder Switch");
   // Reads the initial state of the encoderA
   aLastState = digitalRead(encoderA);   
} 

//===============================================
void loop() { 
  delay(100);
  //--print Counter
  Serial.print("Counter =  ");
  Serial.println(counter);
}


//===============================================
// PCINT0_vect -> PB ->PINB =pins D13..D8
// PCINT1_vect -> PC ->PINC =pins A5..A0
// PCINT2_vect -> PD ->PIND =pins D7..D0
//================================================

ISR (PCINT2_vect) {
  //1--read PIND
  int aState,bState,swState;
   //slower --> auto debounce --> OKAY
   swState = digitalRead(encoderSW);
   aState = digitalRead(encoderA); 
   bState = digitalRead(encoderB); 

   //too fast --> bstate with bounce --> BUG!!!
   // swState = (PIND & B00100000); 
   // aState = (PIND & B10000000); 
   // bState = (PIND & B01000000); 
 
  //2--encoderA,B state change 
  if (aState != aLastState){
     if (bState != aState) { 
       counter ++;  // encoder move CW
     } else {
       counter --;  // encoder move CCW
     }
  }
  //3--save encoderA state
   aLastState = aState; 

  //4--encoderSW => Reset Switch
    if (swState == LOW) {
      counter = 0;  
      Serial.println("====Reset Position=====");
    }
}
