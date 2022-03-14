// Arduino Tic-Tac-Toe Project
// Student: Lazea Dragos-Bogdan
// Group: 30233
// 2021-2022

int player = 1; // the current playe
int selectedPosition = 0; // current selected position
String inString = ""; // for reading the posisions
int color1 = 1; // 1-Red, 2-Green
int color2 = 2; // 1-Red, 2-Green
int redPins[] = {0, 22, 24, 26, 28, 30, 32, 34, 36, 38}; // red pins on the board
int greenPins[] = {0, 23, 25, 27, 29, 31, 33, 35, 37, 39}; // green pins on the board
int bluePins[] = {0, 40, 41, 42, 43, 44, 45, 46, 47, 48}; // blue pins on the board 

// the grid
int grid[3][3] = {
  {0,0,0},
  {0,0,0},
  {0,0,0},
};

/*
Grid looks like:
1 2 3
4 5 6
7 8 9
*/

// returns the value at position pos
int getGridValue (int pos) {
  return grid[(pos - 1) / 3][(pos - 1) % 3];
}

// returns the value (index) of the led
int getLedValue (int pos) {
  if (pos == 1 || pos == 5 || pos == 9)
    return pos;
  else if (pos == 3 || pos == 7)
    return (10 - pos);
  else if (pos == 2)
    return 4;
  else if (pos == 4)
    return 2;
  else if (pos == 6)
    return 8;
  else
    return 6;
}

// sets the grid value at position pos
void setGridValue (int pos) {
  grid[(pos - 1) / 3][(pos - 1) % 3] = player;
}

// checks if a position is valid (free)
boolean validPosition (int pos) {
  if (pos >= 1 && pos <=9 && getGridValue(pos) == 0) {
    return true;
  }
  return false;
}

// displays the grid in Serial Monitor
void displayGrid () {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      Serial.print(grid[i][j], DEC);
      Serial.print(" ");
    }
    Serial.println();
  }
}

// checks if the grid is full
boolean noFreePosLeft () {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (grid[i][j] == 0) {
        return false;
      }
    }
  }
  return true;
}

// switch players during the game
void switchPlayers () {
  if (player == 1)
    player = 2;
  else
    player = 1;
}

// vertical win
boolean verticalWin () {
  for (int pos = 1; pos <= 3; pos++) {
    if (getGridValue(pos) == getGridValue(pos + 3) && getGridValue(pos) == getGridValue(pos + 6) && getGridValue(pos) != 0) {
      return true;
    }
  }
  return false;
}

// horizontal win
boolean horizontalWin () {
  for (int pos = 1; pos <= 7; pos = pos + 3) {
    if (getGridValue(pos) == getGridValue(pos + 1) && getGridValue(pos) == getGridValue(pos + 2) && getGridValue(pos) != 0) {
      return true;
    }
  }
  return false;
}

// first diagonal win
boolean firstDiagonalWin () {
  if (getGridValue(1) == getGridValue(5) && getGridValue(1) == getGridValue(9) && getGridValue(1) != 0)
    return true;
  else
    return false;
}

// second diagonal win
boolean secondDiagonalWin () {
  if (getGridValue(3) == getGridValue(5) && getGridValue(3) == getGridValue(7) && getGridValue(3) != 0)
    return true;
  else
    return false;
}

// win function
boolean win () {
  return verticalWin() || horizontalWin() || firstDiagonalWin() || secondDiagonalWin();
}

// colors a led red
void RGB_color_red(int red_light_pin, int green_light_pin, int blue_light_pin) {
  digitalWrite(red_light_pin, LOW);
  digitalWrite(green_light_pin, HIGH);
  digitalWrite(blue_light_pin, HIGH);
}

// colors a led greed
void RGB_color_green(int red_light_pin, int green_light_pin, int blue_light_pin) {
  digitalWrite(red_light_pin, HIGH);
  digitalWrite(green_light_pin, LOW);
  digitalWrite(blue_light_pin, HIGH);
}

// turn a led off
void RGB_off(int red_light_pin, int green_light_pin, int blue_light_pin) {
  digitalWrite(red_light_pin, HIGH);
  digitalWrite(green_light_pin, HIGH);
  digitalWrite(blue_light_pin, HIGH);
}

// turns all the leds off
void turnLedsOff(){
  for (int i = 1; i <= 9; i++) {
    RGB_off(redPins[i], greenPins[i], bluePins[i]);
  }
}

// color the led depending on player
void colorLed(int pos) {
  if (getGridValue(pos) == 0) {
    RGB_off(redPins[getLedValue(pos)], greenPins[getLedValue(pos)], bluePins[getLedValue(pos)]);
  }
  else if (getGridValue(pos) == 1) {
    RGB_color_red(redPins[getLedValue(pos)], greenPins[getLedValue(pos)], bluePins[getLedValue(pos)]);
  }
  else {
    RGB_color_green(redPins[getLedValue(pos)], greenPins[getLedValue(pos)], bluePins[getLedValue(pos)]);
  }
}

// reset the game (called after the game has ended)
void resetGame () {
  switchPlayers();
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      grid[i][j] = 0;
    }
  }
  //chooseColor();
  Serial.println("Game started!");
  displayGrid();
  turnLedsOff();
}

// setup
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // set all leds as outputs
  for (int i = 1; i <= 9; i++) {
    pinMode(redPins[i], OUTPUT);
    pinMode(greenPins[i], OUTPUT);
    pinMode(bluePins[i], OUTPUT);
  }
  // displays the grid
  displayGrid();
  // turn all the leds off
  turnLedsOff();
}

// loop
void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      inString += (char)inChar;
    }
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n') {
      Serial.print("Player: ");
      Serial.println(player);
      Serial.print("Selected position: ");
      Serial.println(inString.toInt());
      selectedPosition = inString.toInt();
      if (validPosition(selectedPosition)) {
        setGridValue(selectedPosition);
        colorLed(selectedPosition);
        if (win()) {
          displayGrid();
          Serial.print("Finished! Winner is: Player ");
          Serial.print(player);
          Serial.println(". Congrats! :)");
          delay(10000);
          resetGame();
        }
        else {
          if (noFreePosLeft()) {
            Serial.println("No space left on the board. Game over!");
            delay(10000);
            resetGame();
          }
          else {
            switchPlayers();
            displayGrid();
          }
        }
        
      }
      else {
        Serial.println("Invalid position. Try again!");
      }
      inString = "";
    }
  }
}
