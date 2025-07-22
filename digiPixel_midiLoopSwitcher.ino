////////////////////////////////////////////////////////////////////////////////
// ARDUINO: Midi Audio Loop switcher using Digipixel Shield for the interface.
// can also be controlled by buttons on the interface.
// LEDS display the status and the current midi program #
// 
// by John Pollard - jp3.co
// john (at) jp3 [dot] co
// c. 2013
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <DigiPixel.h>
#include <avr/pgmspace.h>




#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define CLR 2
#define CLR2 4
#define CLR3 4
#define BD 25


#define LOOP1 A0
#define LOOP2 A5
#define LOOP3 A1
#define LOOP4 A2
#define SW1 A3
#define SW2 A4



#define PROGRAM_CHANGE_MIN    0xC0
#define PROGRAM_CHANGE_MAX    0xCF

//byte midiChannel=10;
int programNumber=0;

//typedef uint8_t PROGMEM prog_uint8_t;



typedef struct {    
bool loop1;
bool loop2;
bool loop3;
bool loop4;
bool sw1;
bool sw2;
bool sw3;
bool sw4;
} programState;

programState currentProgram;
//currentProgram= (programState){true,true,false,false,false,true,false,false};

//programState program[128];
byte program[128];

byte buttonDelay = BD;



DigiPixel digiPixel(5,2,6,4,3);  // LED Latch/Button Shift !load pin, LED/Button clock pin, LED Data Pin, LED Output Enable pin, Buttons data pin)


byte const numberTable[30]PROGMEM={0b01111110, 0b01000010, 0b01111110, 0b00100010, 0b01111110, 0b00000010, 0b01001110, 0b01001010, 0b01111010, 0b01000010, 0b01001010, 0b01111110, 0b01111000, 0b00001000, 0b01111110, 0b01111010, 0b01001010, 0b01001110, 0b01111110, 0b01001010, 0b01001110, 0b01000000, 0b01000000, 0b01111110, 0b01111110, 0b01001010, 0b01111110, 0b01111010, 0b01001010, 0b01111110};



// MIDI COPY PASTE

//PC LISTEN
byte midiListenChannel = 0xc5;

// CC LISTEN
byte ccListenChannel = 0xB5;

byte commandByte;
byte noteByte;
byte velocityByte;





void checkMIDI(){
  do{  
      commandByte = Serial.read();//read first byte
      noteByte = Serial.read();//read next byte
      
      //Serial.write(commandByte);
      //Serial.write(noteByte);
      

        if (byte(commandByte) < PROGRAM_CHANGE_MIN || byte(commandByte) > PROGRAM_CHANGE_MAX){
          velocityByte = Serial.read();//read final byte
          
          if (byte(commandByte) == 0xB5){
          	if (byte(noteByte) == 0x01) { if (byte(velocityByte > 0x40) ) {currentProgram.loop1=true;} else {currentProgram.loop1=false;} }
          	if (byte(noteByte) == 0x02) { if (byte(velocityByte > 0x40) ) {currentProgram.loop2=true;} else {currentProgram.loop2=false;} }
          	if (byte(noteByte) == 0x03) { if (byte(velocityByte > 0x40) ) {currentProgram.loop3=true;} else {currentProgram.loop3=false;} }
          	if (byte(noteByte) == 0x04) { if (byte(velocityByte > 0x40) ) {currentProgram.loop4=true;} else {currentProgram.loop4=false;} }
          	
          	if (byte(noteByte) == 0x05) { if (byte(velocityByte > 0x40) ) {currentProgram.sw1=true;} else {currentProgram.sw1=false;} }
          	if (byte(noteByte) == 0x06) { if (byte(velocityByte > 0x40) ) {currentProgram.sw2=true;} else {currentProgram.sw2=false;} }

          
          	runCurrentProgram();
          }
          
          //Serial.write(velocityByte);
          
        } else {
          if (byte(commandByte)==0xC5){
           loadProgram(int(noteByte)); 
             runCurrentProgram();
          }
          
        }
        
      Serial.write(commandByte);
      Serial.write(noteByte);
      
      
      if (byte(commandByte) < PROGRAM_CHANGE_MIN || byte(commandByte) > PROGRAM_CHANGE_MAX){Serial.write(velocityByte);}
    }

  while (Serial.available() > 1);//when at least three bytes available
}














void setup() {


    Serial.begin(31250);  // MIDI BAUD RATE

  

   randomSeed(analogRead(0));
  // Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(LOOP1, OUTPUT);
  pinMode(LOOP2, OUTPUT);
  pinMode(LOOP3, OUTPUT);
  pinMode(LOOP4, OUTPUT);
  pinMode(SW1, OUTPUT);
  pinMode(SW2, OUTPUT);

  int i;
  for (i=0;i<=127;i++) {
    //program[i]= (programBank) {false,false,false ,false,false,false,false,false};
    program[i]=B0000000;
    //program[i]= (programState) {rb(),rb(), rb(),rb(), rb(), rb(), rb(), rb()};
    
  }

  
  currentProgram= (programState) {false,false,false ,false,false,false,false,false};
  program[0] =B00000000;
  program[1] =B00000001;
  program[2] =B00000010;
  program[3] =B00000011;
  program[4] =B00000100;
  program[5] =B00000101;
  program[6] =B00000110;
  program[7] =B00000111;
  program[8] =B00001000;
  program[9] =B00001001;
  program[10]=B00001010;
  program[11]=B00001011;
  program[12]=B00001100;
  program[13]=B00001101;
  program[14]=B00001110;
  program[15]=B00001111;
  
  program[16]=B00010000;
  program[17]=B00010001;
  program[18]=B00010010;
  program[19]=B00010011;
  program[20]=B00010100;
  program[21]=B00010101;
  program[22]=B00010110;
  program[23]=B00010111;
  program[24]=B00011000;
  program[25]=B00011001;
  program[26]=B00011010;
  program[27]=B00011011;
  program[28]=B00011100;
  program[29]=B00011101;
  program[30]=B00011110;
  program[31]=B00011111;

  program[32]=B00100000;
  program[33]=B00100001;
  program[34]=B00100010;
  program[35]=B00100011;
  program[36]=B00100100;
  program[37]=B00100101;
  program[38]=B00100110;
  program[39]=B00100111;
  program[40]=B00101000;
  program[41]=B00101001;
  program[42]=B00101010;
  program[43]=B00101011;
  program[44]=B00101100;
  program[45]=B00101101;
  program[46]=B00101110;
  program[47]=B00101111;

  program[48]=B00110000;
  program[49]=B00110001;
  program[50]=B00110010;
  program[51]=B00110011;
  program[52]=B00110100;
  program[53]=B00110101;
  program[54]=B00110110;
  program[55]=B00110111;
  program[56]=B00111000;
  program[57]=B00111001;
  program[58]=B00111010;
  program[59]=B00111011;
  program[60]=B00111100;
  program[61]=B00111101;
  program[62]=B00111110;
  program[63]=B00111111;

}

void loop() {
  checkMIDI();
  digiPixel.saveButtonStates();
  if(buttonDelay!=0) {
    buttonDelay--;
    //delay(4);
  } else {
     processButtonsNormal();
  }

  delay(5);
  runCurrentProgram();
  digiPixel.drawScreen();   


}


//=========================== FUNCTIONS ==========================================================

bool rb (){
  int randNumber;
  randNumber = random(300);
  if (randNumber > 150) {return true;} else {return false;}
}


void processButtonsNormal(){
  if(digiPixel.buttonLeftPressed == true){
    buttonDelay=BD;
    if (currentProgram.loop1==false){
      currentProgram.loop1=true;
    } else {
      currentProgram.loop1=false;
    }
  }
  
  if(digiPixel.buttonUpPressed == true){
    buttonDelay=BD;
    if (currentProgram.loop2==false){
      currentProgram.loop2=true;
    } else {
      currentProgram.loop2=false;
    }
    
  }
  
  if(digiPixel.buttonBPressed == true){
        buttonDelay=35;
    if (currentProgram.sw1==false && currentProgram.sw2 == false){
      currentProgram.sw1=true;

    } else if (currentProgram.sw1==true && currentProgram.sw2==false){
      currentProgram.sw1=false;
      currentProgram.sw2=true;

    } else if (currentProgram.sw1==false && currentProgram.sw2==true){
      currentProgram.sw1=true;
      currentProgram.sw2=true;

    } else if (currentProgram.sw1==true && currentProgram.sw2==true){
      currentProgram.sw1=false;
      currentProgram.sw2=false;

    } 
   
  }
  
  
  if(digiPixel.buttonRightPressed == true){
    buttonDelay=BD;
    if (currentProgram.loop3==false){
      currentProgram.loop3=true;
    } else {
      currentProgram.loop3=false;
    }
  }
if(digiPixel.buttonDownPressed == true){
    buttonDelay=BD;
    if (currentProgram.loop4==false){
      currentProgram.loop4=true;
    } else {
      currentProgram.loop4=false;
    }
  }
  
  if(digiPixel.buttonAPressed == true){
    buttonDelay=BD;
    programNumber++;
    if(programNumber >=127){programNumber=0;}
    loadProgram(programNumber);
    
  }
  
   /*
    * 
    if(digiPixel.buttonBPressed == true){
    buttonDelay=BD;
    programNumber--;
   if(programNumber < 0){programNumber=127;}
    loadProgram(programNumber);
    
  }
  */
}

void runCurrentProgram(){
  

    if (currentProgram.loop1==true){
      digiPixel.setPixel(0,0, 3);
      digitalWrite(LOOP1,HIGH);
    } else {
      digitalWrite(LOOP1, LOW);
      digiPixel.setPixel(0,0, black);
    }
 
 
    if (currentProgram.loop2==true){
      digitalWrite(LOOP2,HIGH);
      digiPixel.setPixel(1,0,5);
    } else {
      digitalWrite(LOOP2,LOW);
      digiPixel.setPixel(1,0, black);
    }


    if (currentProgram.sw1==true){
      digitalWrite(SW1,HIGH);
      digiPixel.setPixel(6,0,4);
    } else {
      digitalWrite(SW1,LOW);
      digiPixel.setPixel(6,0, black);
    }
    
    
    if (currentProgram.sw2==true){
      digitalWrite(SW2,HIGH);
      digiPixel.setPixel(7,0,4);
    } else {
      digitalWrite(SW2,LOW);
      digiPixel.setPixel(7,0, black);
    }

    if (currentProgram.loop3==true){
      digitalWrite(LOOP3,HIGH);
      digiPixel.setPixel(2,0, 2);
    } else {
      digitalWrite(LOOP3,LOW);
      digiPixel.setPixel(2,0, black);
    }

    if (currentProgram.loop4==true){
      digitalWrite(LOOP4,HIGH);
      digiPixel.setPixel(3,0, 1);
    } else {
      digitalWrite(LOOP4,LOW);
      digiPixel.setPixel(3,0, black);
    }

    //if (debug){ }
    
  }
  
  
  
  
void loadProgram(int numb){
 drawDigits(numb);

  bool holder[8];
  byte data =  program[numb];   // fetch data from program memory
  int m;
  m=0;

  
    for (byte y=0; y<7; ++y) {
        if (data & (1<<y)) {
            holder[m]=true;
            // turn on the LED at location (x,y)
        } else {
          holder[m]=false;
            // turn off the LED at location (x,y)
        }
        m++;
    }
//*/

 currentProgram.loop1=holder[0];
 currentProgram.loop2=holder[1];
 currentProgram.loop3=holder[2];
 currentProgram.loop4=holder[3];
 
 currentProgram.sw1=holder[4];
 currentProgram.sw2=holder[5];

 /*
 currentProgram.loop1=program[numb].loop1;
 currentProgram.loop2=program[numb].loop2;
 currentProgram.loop3=program[numb].loop3;
 currentProgram.loop4=program[numb].loop4;
 
 currentProgram.sw1=program[numb].sw1;
 currentProgram.sw2=program[numb].sw2;
 currentProgram.sw3=program[numb].sw3;
 currentProgram.sw4=program[numb].sw4;
 */
}


void drawDigits(int numb){
  
  digiPixel.drawBox(0,1,7,7,black);
   //hundreds
  if (numb >= 100){
    digiPixel.drawVerticalLine(0,3,7, CLR);
    numb=numb-100;
  }
  int tens =int( (numb/10) % 10);
  int ones =int (numb%10);
  
    Serial.print(tens);
    Serial.println(ones);
  
  //tens
  digiPixel.drawVerticalLine(3,3,7, CLR2);
  switch (tens) {
    

    case 1:
      break;
    case 2:
       digiPixel.drawHorizontalLine(7,1,3,CLR2);
       digiPixel.drawHorizontalLine(5,1,3,CLR2);
       digiPixel.drawHorizontalLine(3,1,3,CLR2);
       digiPixel.setPixel(1,4,CLR2);
       digiPixel.setPixel(3,4,black);
       break;
    case 3:
      digiPixel.drawHorizontalLine(7,1,3,CLR2);
      digiPixel.drawHorizontalLine(5,2,3,CLR2);
      digiPixel.drawHorizontalLine(3,1,3,CLR2);
      break;
    case 4:
      digiPixel.drawHorizontalLine(5,1,3,CLR2);
      digiPixel.drawVerticalLine(1,5,7, CLR2);
      break;
    case 5:
       digiPixel.drawHorizontalLine(7,1,3,CLR2);
       digiPixel.drawHorizontalLine(5,1,3,CLR2);
       digiPixel.drawHorizontalLine(3,1,3,CLR2);
       digiPixel.setPixel(1,6,CLR2);
       digiPixel.setPixel(3,6,black);
       break;
     case 6:
       digiPixel.drawVerticalLine(1,3,7, CLR2);
       digiPixel.drawHorizontalLine(7,1,3,CLR2);
       digiPixel.drawHorizontalLine(5,1,3,CLR2);
       digiPixel.drawHorizontalLine(3,1,3,CLR2);
       digiPixel.setPixel(3,6,black);
       break;
     case 7:
       digiPixel.drawHorizontalLine(7,1,3,CLR2);
       digiPixel.setPixel(1,6,CLR2);
       break;
     case 8:
       digiPixel.drawVerticalLine(1,3,7, CLR2);
       digiPixel.drawHorizontalLine(7,1,3,CLR2);
       digiPixel.drawHorizontalLine(5,1,3,CLR2);
       digiPixel.drawHorizontalLine(3,1,3,CLR2);
       break;
      case 9:
       digiPixel.drawVerticalLine(1,5,7, CLR2);
       digiPixel.drawHorizontalLine(7,1,3,CLR2);
       digiPixel.drawHorizontalLine(5,1,3,CLR2);
       //digiPixel.drawHorizontalLine(3,1,3,CLR2);
       break;
      case 0:
      default:
        digiPixel.drawVerticalLine(1,3,7, CLR2);
        digiPixel.setPixel(2,3,CLR2);
        digiPixel.setPixel(2,7,CLR2);
      break;
  }
      
 digiPixel.drawVerticalLine(7,3,7, CLR3);      
 switch (ones) {
   
    case 1:
      break;
    case 2:
       digiPixel.drawHorizontalLine(7,5,7,CLR3);
       digiPixel.drawHorizontalLine(5,5,7,CLR3);
       digiPixel.drawHorizontalLine(3,5,7,CLR3);
       digiPixel.setPixel(5,4,CLR3);
       digiPixel.setPixel(7,4,black);
       break;
    case 3:
      digiPixel.drawHorizontalLine(7,5,7,CLR3);
      digiPixel.drawHorizontalLine(5,6,7,CLR3);
      digiPixel.drawHorizontalLine(3,5,7,CLR3);
      break;
    case 4:
      digiPixel.drawHorizontalLine(5,5,7,CLR3);
      digiPixel.drawVerticalLine(5,5,7, CLR3);
      break;
    case 5:
       digiPixel.drawHorizontalLine(7,5,7,CLR3);
       digiPixel.drawHorizontalLine(5,5,7,CLR3);
       digiPixel.drawHorizontalLine(3,5,7,CLR3);
       digiPixel.setPixel(5,6,CLR3);
       digiPixel.setPixel(7,6,black);
       break;
     case 6:
       digiPixel.drawVerticalLine(5,3,7, CLR3);
       digiPixel.drawHorizontalLine(7,5,7,CLR3);
       digiPixel.drawHorizontalLine(5,5,7,CLR3);
       digiPixel.drawHorizontalLine(3,5,7,CLR3);
       digiPixel.setPixel(7,6,black);
       break;
     case 7:
       digiPixel.drawHorizontalLine(7,5,7,CLR3);
       digiPixel.setPixel(5,6,CLR3);
       break;
     case 8:
       digiPixel.drawVerticalLine(5,3,7, CLR3);
       digiPixel.drawHorizontalLine(7,5,7,CLR3);
       digiPixel.drawHorizontalLine(5,5,7,CLR3);
       digiPixel.drawHorizontalLine(3,5,7,CLR3);
       break;
      case 9:
       digiPixel.drawVerticalLine(5,5,7, CLR3);
       digiPixel.drawHorizontalLine(7,5,7,CLR3);
       digiPixel.drawHorizontalLine(5,5,7,CLR3);
       //digiPixel.drawHorizontalLine(3,1,3,CLR2);
       break;
       
   default: 
   case 0:
      digiPixel.drawVerticalLine(5,3,7, CLR3);
      digiPixel.setPixel(6,3,CLR3);
      digiPixel.setPixel(6,7,CLR3);
      break;
  }
  
digiPixel.drawScreen(); 
  
  
}
