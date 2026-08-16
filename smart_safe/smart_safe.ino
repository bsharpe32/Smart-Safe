#include <LiquidCrystal.h>
#include <Servo.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int button1 = 6;
const int button2 = 7;
const int button3 = 8;
const int enterButton = 9;

const int buzzer = 10;

const int greenLED = 13;
const int redLED = A0;

Servo lockServo;
const int servoPin = A1;

String inputCode = "";
String displayCode ="";

int failedAttempts = 0;
bool lockedOut = false;

void setup() {
  lcd.begin(16, 2);

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(enterButton, INPUT_PULLUP);

  pinMode (buzzer, OUTPUT);

  lockServo.attach(servoPin);
  lockServo.write(0); 

  lcd.print("SMART SAFE");
  lcd.setCursor(0,1);
  lcd.print("Code: ");

  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, LOW);
}

void loop() {
  if (lockedOut) {  // begins lockout sequence
    lcd.clear();
    lcd.print("LOCKED OUT");

    for (int i = 30; i > 0;i--) {
      lcd.setCursor(0, 1);
      lcd.print("Wait ");
      lcd.print(i);
      lcd.print(" sec ");

      digitalWrite(redLED, !digitalRead(redLED));
      tone(buzzer, 400, 100);
      delay(1000);
    }

    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);

    failedAttempts = 0;
    lockedOut = false;

    lcd.clear();
    lcd.print("SMART SAFE");
    lcd.setCursor(0, 1);
    lcd.print("Code: ");

    return;
    }

  if (digitalRead(button1) == LOW) {
    inputCode += "1";
    displayCode += "*";
    tone(buzzer, 1000, 40);

    lcd.setCursor(6,1);
    lcd.print(displayCode);
    delay(250);
  }

  if (digitalRead(button2) == LOW) {
    inputCode += "2";
    displayCode += "*";
    tone(buzzer, 1000, 40);

    lcd.setCursor(6,1);
    lcd.print(displayCode);
    delay(250);
  }

  if (digitalRead(button3) == LOW) {
    inputCode += "3";
    displayCode += "*";
    tone(buzzer, 1000, 40);

    lcd.setCursor(6,1);
    lcd.print(displayCode);
    delay(250);
  }

  if (digitalRead(enterButton) == LOW) {

    lcd.clear();

    if (inputCode == "2312") {
      failedAttempts = 0;

      lcd.print("Access Granted");

      digitalWrite(redLED, LOW);
      digitalWrite(greenLED, HIGH);

      tone(buzzer, 1200, 100);
      delay(120);
      tone(buzzer, 1600, 150);

      lockServo.write(90);
      delay(7000);

      lockServo.write(0);

      digitalWrite(greenLED, LOW);
      digitalWrite(redLED, HIGH);
    }
    else {
      failedAttempts ++;

      lcd.clear();
      lcd.print("Access Denied");

      lcd.setCursor(0, 1);
      lcd.print("Attempts ");
      lcd.print(failedAttempts);
      lcd.print("/3");

      for (int i = 0; i < 3; i++) {
        digitalWrite(redLED, LOW);
        delay(150);
        digitalWrite(redLED, HIGH);
        delay(150);
      }
      tone(buzzer, 300, 500);

      delay(2000);

      if (failedAttempts >= 3) {
        lockedOut = true;
      }
    }

    delay(1000);

    inputCode = "";
    displayCode = "";

    lcd.clear();
    lcd.print("SMART SAFE");
    lcd.setCursor(0,1);
    lcd.print("Code: ");

    delay(300);
  }
}

