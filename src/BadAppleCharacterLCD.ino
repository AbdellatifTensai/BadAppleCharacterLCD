#define serialPin 2
#define latchPin 3
#define clckPin 4
#define enablePin 5
#define readWritePin 6
#define registerSelectPin 7

/*<10001>1<01010>2<00100>3<01010>4<10001>5<00000>6<11111>7<00000>8
  <00100>9<01010>10<10101>11<01010>12<00100>13<01010>14<10101>15<10101>16
  <00001>17<00010>18<00010>19<10100>20<10100>21<01000>22<10110>23<00000>24
  <10001>25<01010>26<01010>27<01010>28<01010>29<10001>30<10001>31<01110>32
  <00100>33<00100>34<00100>35<01010>36<10001>37<10001>38<10001>39<01110>40
  <10101>41<11011>42<10001>43<10101>44<11011>45<10101>46<10001>47<01110>48
  <00001>49<00010>50<00010>51<00001>52<00010>53<00010>54<00100>55<00000>56
  <10000>57<01000>58<01000>59<10000>60<01000>61<01000>62<00100>63<00000>64*/

char receivedByte[8];
byte received;
int loc;
unsigned long currentMillis;
unsigned long startMillis;

void setup() {
  Serial.begin(115200);
  pinMode(serialPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clckPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(readWritePin, OUTPUT);
  pinMode(registerSelectPin, OUTPUT);
//    while(1){writeData(0xFF);}
  displayMsg();
  delay(3000);
  setUp();
  startMillis = millis();
  Serial.println(F("write something"));
}

void loop() {
  if (getData()) makeChar(received);
}
bool getData() {
  static bool incomingData = false;
  static bool incomingLoc = false;
  int r = Serial.read();
  r != -1? r = r:false;
  switch (r) {
    case '<': incomingData = true; received = 0; break;
    case '>': incomingData = false; incomingLoc = true; break;
    case '0':
      if (incomingData) received <<= 1;
      break;
    case '1':
      if (incomingData) {
        received <<= 1;
        received |= 1;
      }
      break;
    case -1: return false; break;
    default:  break;
  }

  if (!incomingData && incomingLoc) {
    loc = Serial.parseInt();
    incomingLoc = false;
    return true;
  }
  return false;
}

void writeInst(byte value) {
  currentMillis = millis();
  digitalWrite(readWritePin, LOW);
  digitalWrite(registerSelectPin, LOW);
  digitalWrite(latchPin, LOW);
  shiftOut(serialPin, clckPin, MSBFIRST, value);
  digitalWrite(latchPin, HIGH);
  if(currentMillis - startMillis >= 1) digitalWrite(enablePin, HIGH);
  digitalWrite(enablePin, LOW);
}

void writeData(byte value) {
  currentMillis = millis();
  digitalWrite(readWritePin, LOW);
  digitalWrite(registerSelectPin, HIGH);
  digitalWrite(latchPin, LOW);
  shiftOut(serialPin, clckPin, MSBFIRST, value );
  digitalWrite(latchPin, HIGH);
  if(currentMillis - startMillis >= 1) digitalWrite(enablePin, HIGH);
  digitalWrite(enablePin, LOW);
}
void writeInstSlow(byte value){
  digitalWrite(readWritePin, LOW);
  digitalWrite(registerSelectPin, LOW);
  digitalWrite(latchPin, LOW);
  shiftOut(serialPin, clckPin, MSBFIRST, value);
  digitalWrite(latchPin, HIGH);
  delay(100);
  digitalWrite(enablePin, HIGH);
  digitalWrite(enablePin, LOW);
}
void writeDataSlow(byte value){
  digitalWrite(readWritePin, LOW);
  digitalWrite(registerSelectPin, HIGH);
  digitalWrite(latchPin, LOW);
  shiftOut(serialPin, clckPin, MSBFIRST, value);
  digitalWrite(latchPin, HIGH);
  delay(100);
  digitalWrite(enablePin, HIGH);
  digitalWrite(enablePin, LOW);
}
void makeChar(byte pattern) {
//  Serial.println(F("")); Serial.print(F("Location: ")); Serial.println(loc);
  writeInst(0x40 + loc);
  writeData(pattern);
}
void displayMsg(){
  writeInstSlow(0x01);
  writeInstSlow(0x38);
  writeInstSlow(0x0C);
  
  writeInstSlow(0x01);
  writeInstSlow(0x80);
  writeDataSlow(0x42);
  writeDataSlow(0x61);
  writeDataSlow(0x64);
  writeInstSlow(0xC0);
  writeDataSlow(0x41);
  writeDataSlow(0x70);
  writeDataSlow(0x70);
  writeDataSlow(0x6C);
  writeDataSlow(0x65);
  writeDataSlow(0x21);
  writeDataSlow(0x21);
}
void setUp() {
  writeInstSlow(0x01);
  writeInstSlow(0x38);
  writeInstSlow(0x0C);
 
  writeInstSlow(0x86);
  writeDataSlow(0x00);

  writeInstSlow(0x87);
  writeDataSlow(0x02);

  writeInstSlow(0x88);
  writeDataSlow(0x04);

  writeInstSlow(0x89);
  writeDataSlow(0x06);

  writeInstSlow(0xC6);
  writeDataSlow(0x01);

  writeInstSlow(0xC7);
  writeDataSlow(0x03);

  writeInstSlow(0xC8);
  writeDataSlow(0x05);

  writeInstSlow(0xC9);
  writeDataSlow(0x07);

}


