const  int   pBlank          = 52;
int pulsenum = 0;
const int led = 13;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pBlank, INPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led,LOW); 
//  digitalWrite(pBlank, HIGH);

}

void loop() {
  if (digitalRead (pBlank) == HIGH) {digitalWrite(led, HIGH);}
  if (digitalRead (pBlank) == LOW) {digitalWrite(led, LOW);}
//  Serial.println(pulsenum);
}
