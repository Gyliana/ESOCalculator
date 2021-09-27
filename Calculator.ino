// includes
#include <Arduino.h>
#include <Display.h>

// constants
const int ButtonPin = 9;
const int PotPin = A0;
const int RedLed = 4;
const int GreenLed = 5;
const int YellowLed = 7;

// variables
int LastButtonState = HIGH;
int state = 0;
float ValueOne;
float ValueTwo;
int Op;

void setup() {
  pinMode(PotPin, INPUT);
  pinMode(ButtonPin, INPUT_PULLUP);
  pinMode(RedLed, OUTPUT);
  pinMode(GreenLed, OUTPUT);
  pinMode(YellowLed, OUTPUT);
}

/// <summary>
/// In the loop there are states which swap using the button click.
/// state 1 is for selecting number one;
/// state 2 is for selecting a operator;
/// state 3 is for selecting number two;
/// state 4 is for showing the answer.
/// </summary>
void loop() {
  //change the state on a button click
  ChangeState();

  if (state == 0) {
    Display.show("----");
    delay(3000);
    state++;
  }

  if (state == 1) {
    int percent = map(analogRead(PotPin), 0, 1023, -20, 20);
    ValueOne = percent;
    Display.show(percent);
  } else if (state == 2) {
    int operatorStance = map(analogRead(PotPin), 0, 1023, 1, 4);
    Op = operatorStance;
    ChangeCalculationMode(operatorStance);
  } else if (state == 3) {
    int percent = map(analogRead(PotPin), 0, 1023, -20, 20);
    ValueTwo = percent;
    Display.show(percent);
  } else if (state == 4) {
    if (Calculate() == 1000) {
      Display.show("Err");
      digitalWrite(RedLed, HIGH);
    } else {
      if (NumberAllowed(Calculate())) {
        Display.show(Calculate());
        digitalWrite(GreenLed, HIGH);
      }
      else {
        Display.show(" -- ");
        digitalWrite(YellowLed, HIGH);
      }
    }
  }
}

static void ChangeState() {
  int buttonState = digitalRead(ButtonPin);
  if (buttonState != LastButtonState) {
    LastButtonState = buttonState;
    if (buttonState == LOW) {
      delay(50);
      ClearApplication();
      state++;
      if (state > 4)state = 1;
    }
  }
}

static void ChangeCalculationMode(int operatorStance) {
  switch (operatorStance) {
    case 1: Display.show("A"); break;
    case 2: Display.show("s"); break;
    case 3: Display.show("t"); break;
    case 4: Display.show("d"); break;
    default: break;
  }
}

static float Calculate() {
  switch (Op) {
    case 1: return ValueOne + ValueTwo;
    case 2: return ValueOne - ValueTwo;
    case 3: return ValueOne * ValueTwo;
    case 4:
      if (!(ValueTwo == 0)) return ValueOne / ValueTwo;
      else return 1000;
  }
}

static bool NumberAllowed(float i) {
  if (i == 0) return true;  
  else if (i > 999.9 || i < -99.9) return false;
  else if (i >= 100 || i <= 10) {
    int j = i * 10;
    if (j = i * 10) return true;
    return false;
  }
  else if (i >= 10 || i <= 0) {
    int j = i * 100;
    if (j = i * 100) return true;
    return false;
  }
  else if (i >= 0) {
    int j = i * 1000;
    if (j = i * 1000) return true;
    return false;
  }
  else {
    return true;
  }
}

static void ClearApplication() {
  Display.clear();
  digitalWrite(RedLed, LOW);
  digitalWrite(GreenLed, LOW);
  digitalWrite(YellowLed, LOW);
}
