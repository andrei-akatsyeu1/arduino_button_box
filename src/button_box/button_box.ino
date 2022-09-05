#include <Joystick.h>

// Keyboard Matrix Tutorial Example
// baldengineer.com
// CC BY-SA 4.0

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD, 64, 0, 
        false,false,false,false,false,false,false,false,false,false,false);
 
byte rows[] = {3,4,5,6,7,8,9};
const int ROW_COUNT = sizeof(rows);

byte cols[] = {2,10,16,14,15,18,19,20,21};
const int COL_COUNT = sizeof(cols);

const int DEBOUNCING_BUTTON_TIME = 50;

byte keys[ROW_COUNT][COL_COUNT];
unsigned long debouncing[ROW_COUNT][COL_COUNT];

void setup() {
  Serial.begin(115200);
  Joystick.begin();

  for(int x=0; x < ROW_COUNT; x++) {
    pinMode(rows[x], INPUT);
  }

  for (int x=0; x < COL_COUNT; x++) {
    pinMode(cols[x], INPUT_PULLUP);
  }  
}

void readMatrix() {
  // iterate the columns
  for (byte colIndex=0; colIndex < COL_COUNT; colIndex++) {
    // col: set to output to low
    byte curCol = cols[colIndex];
    pinMode(curCol, OUTPUT);
    digitalWrite(curCol, LOW);

    // row: interate through the rows
    for (byte rowIndex=0; rowIndex < ROW_COUNT; rowIndex++) {
      byte buttonId = rowIndex*9+colIndex;      
      byte rowCol = rows[rowIndex];
      
      pinMode(rowCol, INPUT_PULLUP);
      byte currentValue = !digitalRead(rowCol);
      
      if(keys[rowIndex][colIndex] != currentValue){
        if(debouncing[rowIndex][colIndex] == 0){
          debouncing[rowIndex][colIndex] = millis() + DEBOUNCING_BUTTON_TIME;
          //Serial.print("Debouncing button: ");Serial.println(buttonId);
        } else if(debouncing[rowIndex][colIndex] < millis()){
          debouncing[rowIndex][colIndex] = 0;
          keys[rowIndex][colIndex] = currentValue;
          Joystick.setButton(buttonId, currentValue);
          //Serial.print(buttonId);Serial.print(": ");Serial.println(currentValue);    
        }
      } else if(debouncing[rowIndex][colIndex] != 0 && debouncing[rowIndex][colIndex] < millis()){
        //Serial.print("Reset debouncing button: ");Serial.println(buttonId);
      }     
      pinMode(rowCol, INPUT);
    }
    // disable the column
    pinMode(curCol, INPUT);
  }
}

void loop() {
  readMatrix();
}
