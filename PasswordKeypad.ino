/*
    V0.1    12.3.2017 Keypad
    V0.2    1.4.2017  Stangenspiel
    V0.2.1  5.4.2017  Keypad fixed
*/
#include <Keypad.h>
#include <LiquidCrystal.h>

#define _myArray_cnt 8

unsigned int check_1 = 1; //Variable um Keypad zu aktivieren, 0 = aus
unsigned int check_2 = 0; //Variable um Stangenspiel zu aktivieren, 0 = aus
unsigned int check_3 = 0; //Variable um Station 3 zu aktivieren, 0 = aus
unsigned int check_4 = 0; //Variable um Station 4 zu aktivieren, 0 = aus
unsigned int check_5 = 0; //Variable um die Bombe zu öffnen, 0 = zu

LiquidCrystal lcd(14, 13, 12, 11, 10, 9); // Creates lcd object

  //---------------------Keypad-----------------------------------

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

char keyword_in[_myArray_cnt] = {' ', ' ', ' ', ' ',' ', ' ', ' ', ' '};
char keyword_set[_myArray_cnt] = {'0', '6', '0', '4', '2', '0', '1', '7'};

int zaehler = 0;

byte rowPins[ROWS] = {8, 7, 6, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

  //--------------------Stangenspiel------------------------------

int Startbutton = 0;
int buttonStateStart;

int buttonState1;  // Stangenschalter ist auf 1 also eingeschaltet
int buttonState2;
int buttonState3;
int buttonState4;

int red1 =  15;
int red2 =  16;
int red3 =  17;
int red4 =  18;

int green1 =  19;
int green2 =  20;
int green3 =  21;
int green4 =  22;

int buttonPin1 = 23;
int buttonPin2 = 24;
int buttonPin3 = 25;
int buttonPin4 = 26;

int a = 1;  // Variable, die sich erhöht, wenn Spieler eine Kombination im Spiel richtig hat
  //--------------------------------------------------------------


void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);                     // Activates 16 x 2 lcd
  LCDwelcomeScreen();

  //---------------------Keypad-----------------------------------

  keypad.addEventListener(keypadEvent); // Add an event listener for this keypad


  //--------------------Stangenspiel------------------------------

  pinMode(red1, OUTPUT);
  pinMode(red2, OUTPUT);
  pinMode(red3, OUTPUT);
  pinMode(red4, OUTPUT);

  pinMode(green1, OUTPUT);
  pinMode(green2, OUTPUT);
  pinMode(green3, OUTPUT);
  pinMode(green4, OUTPUT);

  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);

  pinMode(Startbutton, INPUT);

  //--------------------------------------------------------------
}

void loop() {

  if (check_1) {
    keyPadcode();
  }

  if (check_2) {
    stangenSpiel();
  }

  if (check_3) {
    raetselSpiel();
  }

  if (check_4) {
    colourCards();
  }

  if (check_5) {
    openBomb();
  }
}

// Taking care of some special events.
void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()) {
    case PRESSED:
      if (key == '#') {
        if (zaehler > 0) {
          zaehler -= 1;
          keyword_in[zaehler] = ' ';
          LCDpassword();
        }
      }
      if (key == '*') {
        if (zaehler == _myArray_cnt) {
          if (keyword_in[0] == keyword_set[0] and keyword_in[1] == keyword_set[1] and
              keyword_in[2] == keyword_set[2] and keyword_in[3] == keyword_set[3] and
              keyword_in[4] == keyword_set[4] and keyword_in[5] == keyword_set[5] and
              keyword_in[6] == keyword_set[6] and keyword_in[7] == keyword_set[7]) {

            Serial.println("You got the right Code");
            LCDcorrect();
            resetKeypad();
            check_1 = 0;
            check_2 = 1;
            break;
          }
          if (keyword_in != keyword_set) {
            Serial.println("Your Code is WRONG");
            Serial.println("Your timeout is 300 seconds");
            LCDwrong();
            delay(3000);
            lcd.clear();
            resetKeypad();
            LCDwelcomeScreen();

            break; // need to reset zaehler to 0
          }
        }
        else {
          Serial.println("You have to press more numbers");
          LCDmoreNumbers();
          LCDpassword();
          break;

        }
      }
      break;

    case RELEASED:
      if (key == '*') {
      }
      break;

    case HOLD:
      if (key == '*') {
      }
      break;
  }
}

  //--------------------Keypad LCD--------------------------------

void LCDwelcomeScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);          // (pos, row) starting with 0
  lcd.print("Enter Serial Nr:");
  lcd.setCursor(2, 1);
  lcd.print("===MCBS===");
}

void LCDpassword() {
  lcd.clear();

  for ( unsigned i = 0; i < _myArray_cnt; i++) {
    lcd.setCursor(i, 0);
    lcd.print(keyword_in[i]);
    Serial.print("LCDPW on pos ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(keyword_in[i]);
  }
  Serial.print("LCDPW ");
  Serial.println(keyword_in);
  lcd.setCursor(0, 1);
  lcd.print("#Delete *Confirm");
}

void LCDdotting(int pos, int row) {
  for (int i = pos; i <= 16 - pos; i++) {
    lcd.setCursor(i, row);
    lcd.print(".");
    delay(100);
  }
}

void LCDcorrect() {
  for (int i = 3; i >= 1; i--) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Self-destruct in..");
    lcd.setCursor(0, 1);
    lcd.print(i);
    LCDdotting(1, 1);
  }
  delay(1500);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Station 2");
  lcd.setCursor(3, 1);
  lcd.print("Unlocked!");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Proceed to Station 2");
  lcd.setCursor(6, 1);
  lcd.print("on your right!");
  delay(2000);
  for (int cursorPos = 0; cursorPos < 100; cursorPos++) {
    lcd.scrollDisplayLeft();
    delay(300);
  }
}

void LCDwrong() {
  for (int i = 3; i >= 1; i--) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Self-destruct in..");
    lcd.setCursor(0, 1);
    lcd.print(i);
    LCDdotting(1, 1);
  }
  //delay(1500);
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("ERROR");
  lcd.setCursor(2, 1);
  lcd.print("PIN Invalid!");
  delay(3000);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Penalty");
  lcd.setCursor(2, 1);
  lcd.print("-30 Seconds");
  delay(3000);
}

void LCDmoreNumbers() {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Requires");
  lcd.setCursor(2, 1);
  lcd.print("More Digits!");
  delay(2000);
}

  //--------------------Keypad LCD--------------------------------

void keyPadcode() {
  char key = keypad.getKey();

  if (key) {

    if (zaehler <= (_myArray_cnt - 1) and key != '*' and key != '#') {
      keyword_in[zaehler] = key;
      LCDpassword();
      zaehler += 1;
    }

    Serial.println(key);
    Serial.println(keyword_set);
    Serial.println(keyword_in);
    Serial.println(zaehler);

    if (zaehler == _myArray_cnt) {
      LCDpassword();
      Serial.println("Press * for check password");
      Serial.println("Press # for delete last number");
      lcd.setCursor(0, 1);
      lcd.print("Press * Confirm");            // Visual cue
    }
  }
}

void resetKeypad() {
  for (unsigned int i = 0; i < _myArray_cnt; i++) {
    keyword_in[i] = ' ';
  }
  zaehler = 0;
  Serial.println(keyword_in);
}

void stangenSpiel() {
  //Zielabfolge: 3 - 1 -2 - 4 - 2


  buttonStateStart = analogRead(Startbutton);

  Serial.println ("Startbutton Status");
  Serial.println (buttonStateStart);

  if (buttonStateStart == 1018) //Startbutton einbauen
  {

    nothingPressed();

    digitalWrite (red3, HIGH);
    digitalWrite (green3, HIGH);

    delay(1500);

    digitalWrite (red3, LOW);
    digitalWrite (green3, LOW);

    digitalWrite (red1, HIGH);
    digitalWrite (green1, HIGH);

    delay(1500);

    digitalWrite (red1, LOW);
    digitalWrite (green1, LOW);

    digitalWrite (red2, HIGH);
    digitalWrite (green2, HIGH);

    delay(1500);

    digitalWrite (red2, LOW);
    digitalWrite (green2, LOW);

    digitalWrite (red4, HIGH);
    digitalWrite (green4, HIGH);

    delay(1500);

    digitalWrite (red4, LOW);
    digitalWrite (green4, LOW);

    digitalWrite (red2, HIGH);
    digitalWrite (green2, HIGH);

    delay(1500);

    nothingPressed();

    buttonStateStart = 0;
  }


  else if (buttonStateStart == 0)
  {
    nothingPressed();

    buttonState1 = digitalRead(buttonPin1);
    buttonState2 = digitalRead(buttonPin2);
    buttonState3 = digitalRead(buttonPin3);
    buttonState4 = digitalRead(buttonPin4);

  }

  //--------------------------------------------------------------

  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  buttonState4 = digitalRead(buttonPin4);



  Serial.println ("Button Status 1");
  Serial.println (buttonState1);
  Serial.println ("Button Status 2");
  Serial.println (buttonState2);
  Serial.println ("Button Status 3");
  Serial.println (buttonState3);
  Serial.println ("Button Status 4");
  Serial.println (buttonState4);


  if ((buttonState3 == 0) && (a == 0))  {                            //erstes Kriterium: Button_3 drÃ¼cken
    Serial.println ("Knopf 3 gedrÃ¼ckt - richtig!");
    digitalWrite (green3, HIGH);
    delay(2000);
    digitalWrite (green3, LOW);
    a = a + 1;
  }
  //--------------------------------------------------------------
  else if (((buttonState1 == 0) && (a == 0)) || ((buttonState2 == 0) && (a == 0)) || ((buttonState4 == 0) && (a == 0)) ) {            //erstes Kriterium Fehler: Button_1 _2 oder _4 drÃ¼cken
    for (int i; i < 10; i++)
    {
      Serial.println("falsch - erstes Kriterium!");

      falseSequencing();

    }
    a = 0;
  }

  //--------------------------------------------------------------

  else                              //erstes Kriterium: nichts gedrÃ¼ckt
  {
    nothingPressed();
  }

  //--------------------------------------------------------------

  if ((buttonState1 == 0) && (a == 1)) {
    Serial.println("Knopf 1 nach 3 gedrÃ¼ckt - richtig!");

    digitalWrite (green1, HIGH);
    delay(2000);
    digitalWrite (green1, LOW);

    a = a + 1; //a ist jetzt 2

  }

  //--------------------------------------------------------------

  else if  (((buttonState2 == 0) && (a == 1)) || ((buttonState4 == 0) && (a == 1)) )  {
    for (int i; i < 10; i++)
    {
      Serial.println("falsch - zweites Kriterium!");

      falseSequencing();

    }
    a = 0;
  }

  else                              //zweites Kriterium: nichts gedrÃ¼ckt
  {
    nothingPressed();
  }


  //--------------------------------------------------------------

  if ((buttonState2 == 0) && (a == 2)) {
    Serial.println("Knopf 2 nach 1 gedrÃ¼ckt - richtig!");

    digitalWrite (green2, HIGH);
    delay(2000);
    digitalWrite (green2, LOW);

    a = a + 1; //a ist jetzt 3

  }

  //--------------------------------------------------------------

  else if  ( ((buttonState3 == 0) && (a == 2)) || ((buttonState4 == 0) && (a == 2)) )  {
    for (int i; i < 10; i++)
    {
      Serial.println("falsch - drittes Kriterium!");

      falseSequencing();

    }
    a = 0;
  }

  else                              //drittes Kriterium: nichts gedrÃ¼ckt
  {
    nothingPressed();
  }

  //--------------------------------------------------------------

  if ((buttonState4 == 0) && (a == 3)) {
    Serial.println("Knopf 4 nach 1 gedrÃ¼ckt - richtig!");

    digitalWrite (green4, HIGH);
    delay(2000);
    digitalWrite (green4, LOW);

    a = a + 1; //a ist jetzt 4

  }

  //--------------------------------------------------------------

  else if  (((buttonState1 == 0) && (a == 3)) || ((buttonState3 == 0) && (a == 3)) )  {
    for (int i; i < 10; i++)
    {
      Serial.println("falsch - viertes Kriterium!");

      falseSequencing();

    }
    a = 0;
  }

  else                              //viertes Kriterium: nichts gedrÃ¼ckt
  {
    nothingPressed();
  }

  //--------------------------------------------------------------

  if ((buttonState2 == 0) && (a == 4)) {
    Serial.println("Knopf 2 nach 4 gedrÃ¼ckt - richtig!");

    digitalWrite (green2, HIGH);
    delay(2000);
    digitalWrite (green2, LOW);

    a = a + 1; //a ist jetzt 5

  }

  //--------------------------------------------------------------

  else if  (((buttonState1 == 0) && (a == 4)) || ((buttonState3 == 0) && (a == 4))  )  {
    for (int i; i < 10; i++)
    {
      Serial.println("falsch - fÃ¼nftes Kriterium!");

      falseSequencing();

    }
    a = 0;
  }

  else                              //fÃ¼nftes Kriterium: nichts gedrÃ¼ckt
  {
    nothingPressed();
  }

  if (a == 5)
  {

    for (int r = 0; r < 10; r++) {

      digitalWrite (green1, HIGH);
      digitalWrite (green2, HIGH);
      digitalWrite (green3, HIGH);
      digitalWrite (green4, HIGH);
      delay(100);
      digitalWrite (green1, LOW);
      digitalWrite (green2, LOW);
      digitalWrite (green3, LOW);
      digitalWrite (green4, LOW);
      delay(100);
    }

    Serial.println("Spiel gewonnen - CODE lautet ABC123!");
    delay(10000);
    a = 0;

  }

}

void falseSequencing() {

  digitalWrite (red1, HIGH);
  digitalWrite (red2, HIGH);
  digitalWrite (red3, HIGH);
  digitalWrite (red4, HIGH);
  delay(100);
  digitalWrite (red1, LOW);
  digitalWrite (red2, LOW);
  digitalWrite (red3, LOW);
  digitalWrite (red4, LOW);
  delay(100);

}

void nothingPressed() {

  digitalWrite (green1, LOW);
  digitalWrite (green2, LOW);
  digitalWrite (green3, LOW);
  digitalWrite (green4, LOW);

  digitalWrite (red1, LOW);
  digitalWrite (red2, LOW);
  digitalWrite (red3, LOW);
  digitalWrite (red4, LOW);
}

  //--------------------------------------------------------------

  //--------------------Rätselspiel-------------------------------

void raetselSpiel(){
  
}

  //--------------------------------------------------------------

  //--------------------ColourCards-------------------------------

void colourCards(){
  
}

  //--------------------------------------------------------------

  //--------------------Countdown---------------------------------

void countDown() {

}

  //--------------------------------------------------------------

  //--------------------ÖffneBombe--------------------------------

void openBomb() {
  
}

  //--------------------------------------------------------------

