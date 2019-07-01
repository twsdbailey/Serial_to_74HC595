//Serial to 74HC595 shift registers
//D.Bailey 6/26/2019

//Test this using Serial Monitor or another terminal program.
//See README for more info on how to send the data.


//*********configuration*******************
int dataArray[16][2]; //first initializer = total rows to render, second initializer = total number of shift registers
int rows = 16; //should match first intitalizer in dataArray
int registers = 2; //should match 2nd intitializer in dataArray, each register has 8 outputs

//set timing intervals
int onTime = 75;  //"on" time of the outputs
int offTime = 25; //"off" time of the outputs

//define Arduino pinouts to connect to the shift registers
#define latchPin 8
#define clockPin 12
#define dataPin 11

bool debug = true; //set this true to echo input data for troubleshooting back to Serial Monitor or other
                  //terminal program


//********no changes needed below this point*******

//other variables
int i = 0;
int j = 0;
int data = 0;
bool looping = false;
bool rendered = false;
String readString;


void clearRow() { //this shifts 0s through the registers to turn all outputs off
  digitalWrite(latchPin, LOW);
  for (int z = 0; z < 2; z++) { //change to reflect the number of shift registers as needed
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
  }
  digitalWrite(latchPin, HIGH);
}


void renderRow() { //this reads the 2-dimensional array, and turns on the shift register outputs row by row
                    //number of rows set in the dataArray intializer
  for (int y = 0; y < rows; y++) { //"y" is the number of rows to render
    digitalWrite(latchPin, LOW);
    for (int x = 0; x < registers; x++) { //"x" is the number of registers, eight outputs each
      shiftOut(dataPin, clockPin, MSBFIRST, dataArray[y][x]);
    }
    digitalWrite(latchPin, HIGH);
    delay(onTime); //on time
    clearRow();
    delay (offTime); //off time
  }
  clearRow();
  delay (offTime); //off time
  i = 0;
  data = 0;
}


void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  clearRow();//clear shift registers
  Serial.begin(9600);

}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    if (c == ',') {
      if (readString.length() > 0) {
        data = readString.toInt();  //convert readString into an integer
        dataArray[i][j] = data;
        if (debug) {
          // debugging
          Serial.print("data in " );
          Serial.println(data);
          Serial.print("line #");
          Serial.println(i);
          Serial.print("element #");
          Serial.println(j);
          Serial.println("");
          Serial.println("");
        }
        j++;//increment next byte of data to send to shift registers
        if (j > 1) {
          i++; //increment to next row of data
          j = 0;
        }
        readString = ""; //clears variable for new input
      }
    }
    else if (c == 'l') {
      looping = true; //listen for "loop" command
    }
    else if (c == 's') {
      looping = false; //listen for "stop" command
      clearRow(); //clear the row
    }
    else if (c == 'n') {  //clear registers for new data
      looping = false;
      rendered = false;
      clearRow();
      j = 0; //reset dataArray byte pointer
      i = 0; //reset dataAarray row pointer
    }
    else {
      readString += c; //makes the string readString
    }
  }
  if (i == 16) { //call the renderRow function if outputs when the array is filled,
    renderRow();
    rendered = true;
  }
  if ((looping) && (rendered)) renderRow(); //looping is enabled only the rows have all been rendered once

}
