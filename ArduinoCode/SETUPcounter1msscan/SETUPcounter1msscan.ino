const byte outpin = 9;
const byte interpin = 2;
volatile byte todue = LOW;
volatile int pulsenum = 1;
volatile int duration = 500; // 1s

void setup() {
  // put your setup code here, to run once:
  pinMode(outpin, OUTPUT);
  pinMode(interpin, INPUT);
  //Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(interpin), countsignal, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
}
void countsignal(){
  //1 pulse = 2 ms
  
  if (pulsenum == 1){todue = LOW; digitalWrite(outpin, LOW);}
  if (pulsenum == 10){todue = HIGH; digitalWrite(outpin, HIGH); pulsenum=0;}
  pulsenum = pulsenum+1;
}
