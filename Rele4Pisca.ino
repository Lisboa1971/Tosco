int rele1 = 12;
int rele2 = 13;
int rele3 = 25;
int rele4 = 26;
 
void setup() {
 pinMode(rele1, OUTPUT);
 pinMode(rele2, OUTPUT);
 pinMode(rele3, OUTPUT);
 pinMode(rele4, OUTPUT);
}
 
void loop() {
 digitalWrite(rele1, HIGH);
 delay(100);
 digitalWrite(rele1, LOW);
 delay(100);
 digitalWrite(rele2, HIGH);
 delay(100);
 digitalWrite(rele2, LOW);
 delay(100);
  digitalWrite(rele3, HIGH);
 delay(100);
 digitalWrite(rele3, LOW);
 delay(100);
 digitalWrite(rele4, HIGH);
 delay(100);
 digitalWrite(rele4, LOW);
 delay(100);
}
