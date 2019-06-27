//Serial to 74HC595 shift registers
//D.Bailey 6/26/2019

//Test this using Serial Monitor or another terminal program

int myArray[16][2]; //first initializer = number of rows,
                    //second initializer = number of shift registers
#define latchPin 8
#define clockPin 12
#define dataPin 11
int i = 0;
int j = 0;
int data = 0;
byte clearline = B00000000; //byte used to clear shift register
bool looping = false;
String readString;

//set timing intervals here
int onTime = 75;
int offTime = 25;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  //clear shift registers
  digitalWrite(latchPin, LOW);
  for (int z = 0; z < 2; z++) { //change to reflect the number of shift registers as needed
    shiftOut(dataPin, clockPin, MSBFIRST, clearline);
  }
  digitalWrite(latchPin, HIGH);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    if (c == ',') {
      if (readString.length() > 0) {
        data = readString.toInt();  //convert readString into an integer
        myArray[i][j] = data;

  // debugging
        Serial.print("data in " );
        Serial.println(data);
        Serial.print("line #");
        Serial.println(i);
        Serial.print("element #");
        Serial.println(j);
        Serial.println("");
        Serial.println("");
        
        j++;
        if (j > 1) {
          i++; //increment to next line of data
          j = 0;
        }
        readString = ""; //clears variable for new input
      }
    }
    else if (c == 'l')looping = true; //listen for "loop" command
    else if (c == 's')looping = false; //listen for "stop" command
    else if (c == 'n') {  //clear registers for new data
      looping = false;
      digitalWrite(latchPin, LOW);
      for (int z = 0; z < 2; z++) { //change to reflect the number of shift registers as needed
        shiftOut(dataPin, clockPin, MSBFIRST, clearline);
      }
      digitalWrite(latchPin, HIGH);
    }
    else {
      readString += c; //makes the string readString
    }
  }

  //shift data into registers, then enable outputs
  if ((i == 16) || (looping)) { //make sure the value of i and y is the same as the
    //number of rows set in the array intializer
    for (int y = 0; y < 16; y++) {
      digitalWrite(latchPin, LOW);
      for (int x = 0; x < 2; x++) {
        shiftOut(dataPin, clockPin, MSBFIRST, myArray[y][x]);
      }
      digitalWrite(latchPin, HIGH);
      delay(onTime); //on time
      for (int z = 0; z < 2; z++) { //change to reflect the number of shift registers as needed
        shiftOut(dataPin, clockPin, MSBFIRST, clearline);
      }
      digitalWrite(latchPin, HIGH);
      delay (offTime); //off time
    }
    i = 0;
    data = 0;
  }

}

