#include <Joystick.h>

// Keyboard Matrix Tutorial Example
// baldengineer.com
// CC BY-SA 4.0

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD, 64, 0, 
        false,false,false,false,false,false,false,false,false,false,false);
 
byte rows[] = {3,4,5,6,7,8,9};
const int rowCount = sizeof(rows);

byte cols[] = {2,10,16,14,15,18,19,20,21};
const int colCount = sizeof(cols);

//byte autoSwitchoff[] = {7,8,16,17,25,26,34,35,43,44,2,3,4,11,12,13,20,21,22,29,30,31,38,39,40,47,48,49};
//byte autoSwitchoff[] = {7,8,16,17,25,26,34,35,43,44};
byte autoSwitchoff[] = {};
const int autoSwitchCount = sizeof(autoSwitchoff);

byte keys[rowCount][colCount];

void setup() {
  Serial.begin(115200);
  Joystick.begin();

  for(int x=0; x<rowCount; x++) {
    pinMode(rows[x], INPUT);
  }

  for (int x=0; x<colCount; x++) {
    pinMode(cols[x], INPUT_PULLUP);
  }
    
}

void readMatrix() {
  // iterate the columns
  for (byte colIndex=0; colIndex < colCount; colIndex++) {
    // col: set to output to low
    byte curCol = cols[colIndex];
    pinMode(curCol, OUTPUT);
    digitalWrite(curCol, LOW);

    // row: interate through the rows
    for (byte rowIndex=0; rowIndex < rowCount; rowIndex++) {
      byte buttonId = rowIndex*9+colIndex;
      byte rowCol = rows[rowIndex];
      pinMode(rowCol, INPUT_PULLUP);
      byte currentValue = !digitalRead(rowCol);
      if(keys[rowIndex][colIndex] != currentValue){
        keys[rowIndex][colIndex] = currentValue;
        Joystick.setButton(buttonId, currentValue);
        Serial.print(buttonId);Serial.print(": ");Serial.println(currentValue);    
        autoSwitchOff(buttonId, currentValue);
      }      
      pinMode(rowCol, INPUT);
    }
    // disable the column
    pinMode(curCol, INPUT);
  }
}

void autoSwitchOff(byte buttonId, byte currentValue){
  if(currentValue == 1){
    for(int autoSwitchIdx=0; autoSwitchIdx < autoSwitchCount; autoSwitchIdx++){
      if(autoSwitchoff[autoSwitchIdx] == (buttonId + 1)){
        delay(50);
        Joystick.setButton(buttonId, 0);
      }
    }
  }   
}

void loop() {
  readMatrix();
  delay(50);
}
