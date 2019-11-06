

#include <Servo.h>

//HERE HANDLE THE ADAFRUIT BOARD
//include adafruit library
#include "Adafruit_TLC5947.h"  // TLC driver library
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define NUM_TLC5974 1 // number of TLC drivers linked togother
// data, clock and latch pins connect from tlc to Arduino
#define data   8
#define clock   9
#define latch   10
#define oe      11  // output enable

Adafruit_TLC5947 tlc = Adafruit_TLC5947(NUM_TLC5974, clock, data, latch);

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x43);

///HERE HANDLE LED ARRAYS N SHIET
//LEDmatrix definition
const byte lednum = 14;//number of LEDs. Start with 1. Up to 255 LEDs
const word stages = 27; //number of column vectors in the LEDmatrix. e.g. 20Hz modulation * 1s = 20 states. 0th column is preadaptation
const word maxled = 4094; //maximum pwm frequency = maximum LED brightness

const byte nloops = 3;//actual loops is this number +1. only up to 255 loops
volatile byte nloop = 0; //loop counter. DONT TOUCH
//rows = LEDs, columns = stages. elements of the array correspond to the pwm frequency. First element both here and in the timing array must stay as 0s and -1, respectively.
//otherwise the LEDs will be ON when no program is running.


//circumvent motormatrix
const word Xposition = 353;
const word Yposition = 390;

const word LEDmatrixIN[lednum][stages] = {
{0,882,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,1067,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,1202,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,1952,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,928,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,384,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,783,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3397,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,384,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1282,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2019,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2104,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2974,0},
};

const word LEDmatrixOUT[lednum][stages] = {
{0,882,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,1067,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,1202,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,1952,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,928,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,384,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,783,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3397,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,384,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1282,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2019,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2104,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2974,0},
};

// durations matrix. 1 = 10ms; elements of the array are referenced to as Dur in the comments, though never explicitly declared as such in the code.
// NEVER PUT THE DURATIONS TO 0 or NEGATIVE NUMBERS!
const word timing[stages] = {
600,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300
};
//600,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300,300
//200,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100
//this is a collection of vectors that switch
int pled;
char sCmd;


// Blanking and Triggering
const  int   pBlank = 49; //replace with bytes
const int Trig = 13; // replace with bytes
// set up the pulse to trigger counter
volatile int pulseToTrig = 0; //as byte only takes values up to 255. If the desired period between triggers is longer than 10ms * 255, replace datatype with 'int'
int Trigger = 600; // number of pulses received before trigger. 1 = 1k0ms. hence 100 = 1s. put -1 to get triggers only on loop start.
volatile word pulseToDur = 0; // set up counter for the number of pulses before stage changes to the next stage. Discretization same as [timings] and [Trigger].
volatile word stage = 0; //variable for cycling throigh stages, supports up to 65535 stages


//HERE HANDLE MIRROR POSITIONS
byte x = 180; // reading from the joystick
byte y = 180;
int jX = A0; //joystick pin
int jY = A1;
int mX = 2; //command to motor pin
int mY = 12;

//HERE HANDLE joystick buttons: these control the spot sizes
int INup = 4; //inner spot upregulated
int INdown = 3;
int OUTup = 6;
int OUTdown = 5; //outer spot downregulated

//HERE HANDLE SERVOS
//int thresX = 189; // reading at resting joystick
//int thresY = 192;
//Servo servoX; //declare servos
//Servo servoY;
int absmin = 200; // absolute (physically cannot go further) minimum seconds (portions of degrees) command to the servo
int absmax = 500;
int stepdist = 1; //distance in microsecs
int stepdur = 3; // duration in millisecs
//these are mins and maxes (servo positions) that one can update
volatile int xmin = 310;
volatile int xmax = 400;
volatile int ymin = 390;
volatile int ymax = 430;
int valX = 384; // absmin + (xmax-xmin)/2; //starting positions for the servos
int valY = 420; //absmin + (ymax-ymin)/2;
//these two values are joyick modifiers to the preprogrammed position
volatile word modX = 0;
volatile word modY = 0;

// The motors take positions 22 and 23 on the led driver.
//row 0 is the y position
//row 1 is the x-y position (due to curvature)
const word motormatrix[2][stages] = {
{405,405,405,405,405,405,405,405,405,405,405,405,405,405,405,405,405,405,},
{355,355,355,355,355,355,355,355,355,355,355,355,355,355,355,355,355,355,},
};


//here we deal with saving arbitrary mins and maxes
int saverXmin = 23; //these pins are just constantly HIGH
int saverXmax = 27;
int saveDXmin = 25; // this thingie lights up when button is pressed and the number saved
int saveDXmax = 29;

int saverYmin = 31; //these pins are just constantly HIGH
int saverYmax = 35;
int saveDYmin = 33; // this thingie lights up when button is pressed and the number saved
int saveDYmax = 37;

///NOW HANDLE LENS VARIABLES
//row0 is for the shorter time period
//row1 is for the longer time period
const word Lensmatrix[2][stages] = {
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

const word sizeIn = 1; //1: largest, 4094: smallest
const word sizeOut = 1;

//HERE HANDLE lens current variables
int lenspin = DAC0;
int INval = 1;
int OUTval = 4094;
int valINup = 0;
int valINdown = 0;
int valOUTup = 0;
int valOUTdown = 0;
int lensstep = 10; //0-4095 steps of lens voltage increase
//here we try to equilibrate brightness of the outer and inner annuli by adjusting the duty cycle of the ETL (proportion of time spent with f corrsponding to OUT and IN)
int Dmax = 20; //maximal Diameter of the size in mm
int Dmin = 1; //minimal -//-
float Dout = (Dmax - Dmin) * OUTval / 4094 + Dmin;
float Din = (Dmax - Dmin) * INval / 4094 + Dmin;
float ratioINout = sq(Din / 2) / sq(Dout / 2);
int MicrosIN = 2500;//10000*ratioINout;//smth is wrong with this calculation

// HERE COME FUNCTIONS
///Functions to save XY mins and maxes (position)
void savingXmin() {
  xmin = valX;
  Serial.println(xmin);
}
void savingXmax() {
  xmax = valX;
  Serial.println(xmax);
}
void savingYmin() {
  ymin = valY;
  Serial.println(ymin);
}
void savingYmax() {
  ymax = valY;
  Serial.println(ymax);
}
////HERE BE FUNCTIONS for LED updates
//set all to PWM
void setAllTo(int pwm) {
  for (int i = 0; i < lednum; i++) {
    tlc.setPWM(i, pwm);
  }
  tlc.write();
}

//cycles through all LEDs in the vector corrsponding to the current stage: refreshes the vector that is then written to LEDs with tlc.write
void updateVectorIN() {
  for (int i = 0; i < lednum; i++) {
    tlc.setPWM(i, LEDmatrixIN[i][stage]);
  }
}
void updateVectorOUT() {
  for (int i = 0; i < lednum; i++) {
    tlc.setPWM(i, LEDmatrixOUT[i][stage]);
  }
}
void updateVectorMOTOR() {
    
   pwm.setPWM(0, 0,Yposition+modY);
   pwm.setPWM(1, 0,Xposition+modX);
   //Serial.print(motormatrix[0][stage]);

  }


void Step() {
  //read fresh vector given stage
  if (stage == stages && nloop <= nloops) {
    nloop += 1;  //there will be an extra 10ms in the beginning of a loop + trigger might come in out of sync
    stage = 1;
    pulseToTrig = 0;
    pulseToDur = 0;
  }
  
  updateVectorIN();
  //write to LEDs
  tlc.write();
  analogWrite(lenspin, Lensmatrix[0][stage]*sizeIn);
  //  ///test
  //  digitalWrite(51,LOW);
  //  digitalWrite(53, HIGH);
  //  //endtest

  delayMicroseconds(MicrosIN);
  updateVectorOUT();
  tlc.write();
  analogWrite(lenspin, Lensmatrix[1][stage]*sizeOut);

  //  Serial.println(x);
  //  Serial.println(valX);
  /////JOYSTICK CONTROLS///

  x = analogRead(jX);
  y = analogRead(jY);
  
  if (x > 230)
  {
    modX += stepdist;
    if (motormatrix[1][stage]+modX > xmax) {
      modX = xmax-motormatrix[1][stage];
    };
  }
  else if (x < 25)
  {
    modX -= stepdist;
    if (motormatrix[1][stage]+modX < xmin) {
      modX = xmin-motormatrix[1][stage];
    };
  };
  //Serial.print(x);  
  /////YYYYYYYYYYYYYYYYYYYYYY/////
  if (y > 220)
  {
    modY += stepdist;
    if (motormatrix[0][stage]+modY > ymax) {
      modY = ymax-motormatrix[0][stage];
    };
  }

  else if (y < 30)
  {
    modY -= stepdist;
    if (motormatrix[0][stage]+valY < ymin) {
      modY = ymin-motormatrix[0][stage];
    };
  };

  //Serial.println(x);
  //Serial.println(y);
  //pwm.setPWM(0, 0, motormatrix[0][stage] + modX);
  //pwm.setPWM(1, 0, motormatrix[1][stage] + modY);
  //tlc.write();
  //  //test
  //  digitalWrite(53,LOW);
  //  digitalWrite(51, HIGH);
  //  //endtest

  //here we RESET the pulseToDur counter once it reaches Dur; and advance stage +1
  if (stage == 0 && pulseToDur == timing[stage]) {
    pulseToTrig = 0; //add trigger at beginning of stage 1
  }
  if (nloop == nloops && pulseToDur == timing[stage] && stage == stages - 1) {
    setAllTo(0);  //stage starts at 0 but stages is a count hence -1
    detachInterrupt(digitalPinToInterrupt(pBlank));
  }
  if (pulseToDur == timing[stage]) {
    pulseToDur = 0;
    stage += 1;
  }


  if (pulseToTrig == Trigger) {
    pulseToTrig = 0;
  }
  // Trigger signal follows when pulseToTrig is reset to 0.
  if (pulseToTrig == 1) {
    digitalWrite(Trig, HIGH);
  }
  if (pulseToTrig == 2) {
    digitalWrite(Trig, LOW); // corresponds to 1*10ms
  }

  //as says - counts the incoming pulse
  pulseToDur += 1;
  pulseToTrig += 1;
};

void setup() {
    //motor stuff:
    pwm.begin();
    pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
    //yield();

   pwm.setPWM(0, 0, motormatrix[0][0]);
   pwm.setPWM(1, 0, motormatrix[1][0]);
    
    //LED & lens stuff:
    setAllTo(0);
    analogWriteResolution(12); // make DAC0 and DAC1 high-resolution - give true analog not PWM, 05-2.7 V (0-4095)
    pinMode(lenspin, OUTPUT);
    analogWrite(lenspin, 1);
    analogReadResolution(8); //set reading from joystick resolution (0-255)

    Serial.begin(9600);
    pinMode(jX, INPUT); // set joystick pot pins to input
    pinMode(jY, INPUT);
    pinMode(saverXmin, OUTPUT); // set the "saver" pins to constant high - circuit breakers in the pathway to saveD pins
    pinMode(saverXmax, OUTPUT);

    pinMode(saveDXmin, INPUT); // get digital input on saveD pins. Later we will register those
    pinMode(saveDXmax, INPUT);

    pinMode(INup, INPUT);
    pinMode(INdown, INPUT);
    pinMode(OUTup, INPUT);
    pinMode(OUTdown, INPUT);

    //  //TESTTEST
    //  pinMode(51, OUTPUT);
    //  pinMode(53, OUTPUT);

    //attach savings
    //attachInterrupt(digitalPinToInterrupt(saveDXmin), savingXmin, FALLING); // when the button is pressed, current value of the mirror position
    //attachInterrupt(digitalPinToInterrupt(saveDXmax), savingXmax, FALLING); // in X is saved as min and max, respectively
    //attachInterrupt(digitalPinToInterrupt(saveDYmin), savingYmin, RISING); // when the button is pressed, current value of the mirror position
    //attachInterrupt(digitalPinToInterrupt(saveDYmax), savingYmax, RISING); // in Y is saved as min and max, respectively

    ///LED STUFF
    pinMode(pBlank, INPUT);
    digitalWrite(pBlank, HIGH);
    pinMode(Trig, OUTPUT);
    digitalWrite(Trig, LOW);
    tlc.begin();
    ///Prevent LEDs from lighting up initially
    if (oe >= 0) {
      pinMode(oe, OUTPUT);
      digitalWrite(oe, LOW);
    }


    Serial.println("************************************************************************************");
    Serial.println("* LED Synth                                                                        *");
    Serial.println("*                                                                                  *");
    Serial.println("* 1-9, q-y for individual LEDs *");
    Serial.println("* a for matrix *");
    Serial.println("* l for all LEDs on *");
    Serial.println("* s for all LEDs off.                                 *");
    Serial.println("* p for program.                                         *");
    Serial.println("************************************************************************************\n");

  };

void loop () {
  updateVectorMOTOR();
  //
  //  valINup = digitalRead(INup);
  //  valINdown = digitalRead(INdown);
  //  valOUTup = digitalRead(OUTup);
  //  valOUTdown = digitalRead(OUTdown);
  //
  //  if (valINup == 1){
  //    INval += lensstep;
  //    if (INval > 4094) {INval = 4094;};
  //};
  //  if (valINdown == 1){
  //    INval -= lensstep;
  //    if (INval < 1) {INval = 1;};
  //};
  //  if (valOUTup == 1){
  //    OUTval += lensstep;
  //    if (OUTval > 4094) {OUTval = 4094;};
  //};
  //  if (valOUTdown == 1){
  //    OUTval -= lensstep;
  //    if (OUTval < 1) {OUTval = 1;};
  //};
  //
  //

  //delay(stepdur);

  if (Serial.available()) {
    sCmd = Serial.read();

    // Execute command
    //
    switch (sCmd) {
      case 's':
        // All LEDs off
        //
        detachInterrupt(digitalPinToInterrupt(pBlank));
        Serial.println("-> all LEDs off");
        analogWrite(lenspin, 1);
        setAllTo(0);
        tlc.write();
        break;

      case 'l':
        // All LEDs on, min ETL current
        //
        detachInterrupt(digitalPinToInterrupt(pBlank));
        Serial.println("-> all LEDs on");
        analogWrite(lenspin, 1);
        setAllTo(4094);
        tlc.write();
        break;

      case 'k':
        // All LEDs on, max ETL current
        //
        detachInterrupt(digitalPinToInterrupt(pBlank));
        Serial.println("-> all LEDs on");
        analogWrite(lenspin, 4094);
        setAllTo(4094);
        tlc.write();
        break;

      case 'g':
        // maxETL current
        //
        detachInterrupt(digitalPinToInterrupt(pBlank));
        Serial.println("-> maximal ETL current");
        analogWrite(lenspin, 4094);
        break;

      case 'h':
        // minETL current
        //
        detachInterrupt(digitalPinToInterrupt(pBlank));
        Serial.println("-> minimal ETL current");
        analogWrite(lenspin, 1);
        break;

      case '1':
        // red LED to maximum
        //
        detachInterrupt(digitalPinToInterrupt(pBlank));
        pled = maxled;
        Serial.print("-> 1st LED to ");
        Serial.println(pled, DEC);
        tlc.setPWM(0, pled);
        tlc.write();
        break;

      case '2':
        // red LED to maximum
        //
        detachInterrupt(digitalPinToInterrupt(pBlank));
        pled = maxled;
        Serial.print("-> 2nd LED to ");
        Serial.println(pled, DEC);
        tlc.setPWM(1, pled);
        tlc.write();
        break;

      case '3':
        // red LED to maximum
        //
        detachInterrupt(digitalPinToInterrupt(pBlank));
        pled = maxled;
        Serial.print("-> 3rd LED to ");
        Serial.println(pled, DEC);
        tlc.setPWM(2, pled);
        tlc.write();
        break;

      case '4':
        // red LED to maximum
        //
        detachInterrupt(digitalPinToInterrupt(pBlank));
        pled = maxled;
        Serial.print("-> 4th LED to ");
        Serial.println(pled, DEC);
        tlc.setPWM(3, pled);
        tlc.write();
        break;

      case '5':
        // red LED to maximum
        //
        detachInterrupt(digitalPinToInterrupt(pBlank));
        pled = maxled;
        Serial.print("-> 5th LED to ");
        Serial.println(pled, DEC);
        tlc.setPWM(4, pled);
        tlc.write();
        break;

      case '6':
        // red LED to maximum
        //
        detachInterrupt(digitalPinToInterrupt(pBlank));
        pled = maxled;
        Serial.print("-> 6th LED to ");
        Serial.println(pled, DEC);
        tlc.setPWM(5, pled);
        tlc.write();
        break;

      case '7':
        // red LED to maximum
        //
        detachInterrupt(digitalPinToInterrupt(pBlank));
        pled = maxled;
        Serial.print("-> 7th LED to ");
        Serial.println(pled, DEC);
        tlc.setPWM(6, pled);
        tlc.write();
        break;

      case '8':
        // red LED to maximum
        //
        detachInterrupt(digitalPinToInterrupt(pBlank));
        pled = maxled;
        Serial.print("-> 8th LED to ");
        Serial.println(pled, DEC);
        tlc.setPWM(7, pled);
        tlc.write();
        break;

      case '9':
        // red LED to maximum
        //
        detachInterrupt(digitalPinToInterrupt(pBlank));
        pled = maxled;
        Serial.print("-> 9th LED to ");
        Serial.println(pled, DEC);
        tlc.setPWM(8, pled);
        tlc.write();
        break;

      case '0':
        // red LED to maximum
        //
        detachInterrupt(digitalPinToInterrupt(pBlank));
        pled = maxled;
        Serial.print("-> 10th LED to ");
        Serial.println(pled, DEC);
        tlc.setPWM(9, pled);
        tlc.write();
        break;

      case 'q':
        // red LED to maximum
        //
        detachInterrupt(digitalPinToInterrupt(pBlank));
        pled = maxled;
        Serial.print("-> 11th LED to ");
        Serial.println(pled, DEC);
        tlc.setPWM(10, pled);
        tlc.write();
        break;

      case 'w':
        // red LED to maximum
        //
        detachInterrupt(digitalPinToInterrupt(pBlank));
        pled = maxled;
        Serial.print("-> 12th LED to ");
        Serial.println(pled, DEC);
        tlc.setPWM(11, pled);
        tlc.write();
        break;

      case 'e':
        // red LED to maximum
        //
        detachInterrupt(digitalPinToInterrupt(pBlank));
        pled = maxled;
        Serial.print("-> 13th LED to ");
        Serial.println(pled, DEC);
        tlc.setPWM(12, pled);
        tlc.write();
        break;

      case 'r':
        // red LED to maximum
        //
        detachInterrupt(digitalPinToInterrupt(pBlank));
        pled = maxled;
        Serial.print("-> 14th LED to ");
        Serial.println(pled, DEC);
        tlc.setPWM(13, pled);
        tlc.write();
        break;

      case 't':
        // red LED to maximum
        //
        detachInterrupt(digitalPinToInterrupt(pBlank));
        pled = maxled;
        Serial.print("-> 15th LED to ");
        Serial.println(pled, DEC);
        tlc.setPWM(14, pled);
        tlc.write();
        break;

      case 'y':
        // red LED to maximum
        //
        detachInterrupt(digitalPinToInterrupt(pBlank));
        pled = maxled;
        Serial.print("-> 16th LED to ");
        Serial.println(pled, DEC);
        tlc.setPWM(15, pled);
        tlc.write();
        break;

      case 'u':
        // red LED to maximum
        //
        detachInterrupt(digitalPinToInterrupt(pBlank));
        pled = maxled;
        Serial.print("-> 17th LED to ");
        Serial.println(pled, DEC);
        tlc.setPWM(16, pled);
        tlc.write();
        break;

      case 'i':
        // red LED to maximum
        //
        detachInterrupt(digitalPinToInterrupt(pBlank));
        pled = maxled;
        Serial.print("-> 18th LED to ");
        Serial.println(pled, DEC);
        tlc.setPWM(17, pled);
        tlc.write();
        break;

      case 'a':
        //play LED matrix
        Serial.print("-> playing array ");
        //Interruptions are set up here
        //interrupt 1: counts pulses, switches between vectors in the LED matrix
        stage = 0;
        pulseToTrig = 0;
        pulseToDur = 0;
        nloop = 0;
        attachInterrupt(digitalPinToInterrupt(pBlank), Step, RISING);
    };
  };

};
