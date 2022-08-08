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
const byte SWITCH_COUNT = 3;
const byte SWITCH_SIZE = 6;

byte switches[SWITCH_COUNT][SWITCH_SIZE] = {{1,10,19,28,37,46},{2,11,20,29,38,47},{3,12,21,30,39,48}};
byte switchesNewValue[SWITCH_COUNT];
byte switchesPrevValue[SWITCH_COUNT];
unsigned long switchesDebouncing[SWITCH_COUNT];

const int DEBOUNCING_BUTTON_TIME = 50;
const int DEBOUNCING_SWITCH_TIME = 100;


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
      int switchIdx = getSwitchIdx(buttonId);
      
      pinMode(rowCol, INPUT_PULLUP);
      byte currentValue = !digitalRead(rowCol);
      
      if(switchIdx >= 0){
        if(currentValue > 0){
          switchesNewValue[switchIdx] = buttonId;          
        }
      } else {
        if(keys[rowIndex][colIndex] != currentValue){
          if(debouncing[rowIndex][colIndex] == 0){
            debouncing[rowIndex][colIndex] = millis() + DEBOUNCING_BUTTON_TIME;
            Serial.print("Debouncing button: ");Serial.println(buttonId);
          } else if(debouncing[rowIndex][colIndex] < millis()){
            debouncing[rowIndex][colIndex] = 0;
            keys[rowIndex][colIndex] = currentValue;
            Joystick.setButton(buttonId, currentValue);
            Serial.print(buttonId);Serial.print(": ");Serial.println(currentValue);    
          }
        } else if(debouncing[rowIndex][colIndex] != 0 && debouncing[rowIndex][colIndex] < millis()){
          Serial.print("Reset debouncing button: ");Serial.println(buttonId);
        }
      }     
      pinMode(rowCol, INPUT);
    }
    // disable the column
    pinMode(curCol, INPUT);
  }
  updateSwitches();
}

void updateSwitches(){ //update switch in next way: release all buttons first and when press selected
  for(byte i = 0; i < SWITCH_COUNT; i++){ //for each switch
    if(switchesPrevValue[i] != switchesNewValue[i]){
       if(switchesDebouncing[i] == 0){
          switchesDebouncing[i] = millis() + DEBOUNCING_SWITCH_TIME;
          Serial.print("Debouncing switch: ");Serial.println(i);
       } else if(switchesDebouncing[i] < millis()){
          switchesDebouncing[i] = 0;
          for(byte j = 0; j < SWITCH_SIZE; j++){ //for each button related to switch
            if(switches[i][j] != switchesNewValue[i]){ //if button is not selected
              Joystick.setButton(switches[i][j], 0); //release button
              //Serial.print(switches[i][j]);Serial.print(": 0; ");
            }
          }
          switchesPrevValue[i] = switchesNewValue[i];
          Joystick.setButton(switchesNewValue[i], 1); //press selected button
          Serial.print("Updating switch ");Serial.print(i);Serial.print(" to ");Serial.println(switchesNewValue[i]);
       }
    } else if(switchesDebouncing[i] != 0 && switchesDebouncing[i] < millis()){//reset debouncing
      Serial.print("Reset debouncing switch: ");Serial.println(i);
      switchesDebouncing[i] = 0;
    }
  }
}

int getSwitchIdx(byte buttonId){
  for(byte i = 0; i < SWITCH_COUNT; i++){
    for(byte j = 0; j < SWITCH_SIZE; j++){
      if(switches[i][j] == buttonId){
        return i;
      }
    }
  }
  return -1;
}

void loop() {
  readMatrix();
}
