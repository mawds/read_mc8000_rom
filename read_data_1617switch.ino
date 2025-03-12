//Pin connected to ST_CP of 74HC595
int latchPin = 2;  //8;
////Pin connected to DS of 74HC595
int dataPin = 3;  //11;
//Pin connected to SH_CP of 74HC595
int clockPin = 4;  //12;

int CS = A0;
int OE = A1;
int WE = A2;
int A16 = A3;
int A17 = A4;


void setup() {

  //set pins to output because they are addressed in the main loop

  pinMode(latchPin, OUTPUT);

  pinMode(clockPin, OUTPUT);

  pinMode(dataPin, OUTPUT);
  pinMode(CS, OUTPUT);
  pinMode(OE, OUTPUT);
  pinMode(WE, OUTPUT);

  // Manual switching of top 2 bits to avoid another shift register
  pinMode(A16, OUTPUT);
  pinMode(A17, OUTPUT);

  digitalWrite(A16, LOW);
  digitalWrite(A17, LOW);

  digitalWrite(CS, LOW);
  digitalWrite(OE, LOW);
  digitalWrite(WE, LOW);


  // Aready set as input by default

  // pinMode(5, INPUT);
  // pinMode(6, INPUT);
  // pinMode(7, INPUT);
  // pinMode(8, INPUT);
  // pinMode(9, INPUT);
  // pinMode(10, INPUT);
  // pinMode(11, INPUT);
  // pinMode(12, INPUT);

  Serial.begin(115200);
}

void setAddress(long d) {
  // Set the address to read/write from
  //ground latchPin and hold low for as long as you are transmitting

  digitalWrite(latchPin, LOW);

  shiftOut(dataPin, clockPin, LSBFIRST, lowByte(d));
  shiftOut(dataPin, clockPin, LSBFIRST, highByte(d));

  //return the latch pin high to signal chip that it

  //no longer needs to listen for information

  digitalWrite(latchPin, HIGH);

  // Set upper two bits manually
  int line16 = bitRead(d,16);
  if(line16 == 1){
    digitalWrite(A16, HIGH);
   // Serial.println("A16 high");
  } else {
    digitalWrite(A16, LOW);
  }

  
  int line17 = bitRead(d,17);
  if(line17 == 1){
    digitalWrite(A17, HIGH);
  //  Serial.println("A17 high");
  } else {
    digitalWrite(A17, LOW);
  }
  
//   digitalWrite(A16, bitRead(d, 16));
//   digitalWrite(A17, bitRead(d, 17));
}


byte flash_data_get() {
  /* get data from data bus */
  /* Taken from https://github.com/warber0x/FMPUNO */

  byte data = 0;
  boolean state = LOW;
  boolean state2 = LOW;

  for (int i = 5, j = 0; i < 13; i++, j++) {
    state = digitalRead(i);
    state2 = digitalRead(i);
    if (state == state2)
      if (state == HIGH)
        bitWrite(data, j, HIGH);
  }
  return data;
}

void read_memory() {
  //count up routine
  byte d;
  //Serial.print("In main loop");
 

  digitalWrite(CS, LOW);
  
  for (long y = 0; y < 0x20000; y++) {  //65536
    setAddress(y);


    digitalWrite(CS, HIGH);
    digitalWrite(OE, HIGH);

    delayMicroseconds(25);
    //delay(1);
    d = flash_data_get();
    //Serial.print(j);
    // Serial.print(": ");
    Serial.print(y, HEX);
    Serial.print(" ");
    Serial.println(d, HEX);

    digitalWrite(OE, LOW);
    digitalWrite(CS, LOW);
    delayMicroseconds(25);
    //delay(1);
  }

}

char cmd;

void loop() {


  while (!Serial.available())
    ;
  cmd = Serial.read();

  switch (cmd) {
    case 'R':
      read_memory();
      Serial.flush();
      exit(0);
      break;
  }
}