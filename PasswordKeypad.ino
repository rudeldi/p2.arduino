/*
    V0.1    12.3.2017 Keypad
    V0.2    1.4.2017  Stangenspiel
    V0.2.1  5.4.2017  Keypad fixed
    V0.3    19.4.2017 Games added
    V0.3.1  19.4.2017 Countdown addad
*/
#include <Keypad.h>
#include <LiquidCrystal.h>
#include "SevSeg.h"

#define _myArray_cnt 8
      //--------------------Station colourCards------------------------------
#define S0 27
#define S1 28
#define S2 29
#define S3 30
#define sensorOut A2
#define rLED 31
#define gLED 32
#define triggerLED 33
#define progressLED1 34
#define progressLED2 35
#define progressLED3 36
      //--------------------Station colourCards------------------------------
      //--------------------Rätselspiel-------------------------------
#define quizButton1 37
#define quizButton2 38
#define quizButton3 39
#define quizButton4 40
#define quizLED1 41
#define quizLED2 42
#define quizLED3 43
#define quizLED4 44
      //--------------------Rätselspiel-------------------------------

unsigned int check_1 = 0; //Variable um Keypad zu aktivieren, 0 = aus
unsigned int check_2 = 0; //Variable um Stangenspiel zu aktivieren, 0 = aus
unsigned int check_3 = 1; //Variable um Station 3 zu aktivieren, 0 = aus
unsigned int check_4 = 0; //Variable um Station 4 zu aktivieren, 0 = aus
unsigned int check_5 = 0; //Variable um die Bombe zu öffnen, 0 = zu
unsigned int countdown = 0;

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
int buttonStateRead = 1;
int buttonStateStart = 0;

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
  
  //--------------------Station colourCards------------------------------
unsigned int red = 0;      // rgb values stored here
unsigned int green = 0;
unsigned int blue = 0;
int colourCardCounter = 0;  // sequencing
bool colourCardRemoved = true; // flag needed to exit for-loop after correct card is inserted
  //--------------------------------------------------------------

  //--------------------Rätselspiel-------------------------------
int quizCounter = 0;
int quizSequence1[4] = {1, 0, 1, 1}; // define ABCD answers, B
int quizSequence2[4] = {1, 1, 0, 1}; // C
int quizSequence3[4] = {0, 1, 1, 1}; // A
int quizSequence4[4] = {1, 1, 1, 0}; // D
bool quizButtonWait = true;
int buttonRead1 = 0;
int buttonRead2 = 0;
int buttonRead3 = 0;
int buttonRead4 = 0;
  //--------------------Rätselspiel-------------------------------

  //--------------------Countdown-----------------------------

SevSeg sevseg;
byte numDigits = 4;
byte digitPins[] = {45, 46, 47, 48};
byte segmentPins[] = {49, 50, 51, 52, 53, 54, 57, 58};
unsigned long timer = millis();
int deciSeconds = 0;
unsigned long totalTime = 300000;
int showTime = 3000;
      
   //--------------------Countdown-----------------------------
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

  pinMode(Startbutton, INPUT_PULLUP);

  //--------------------------------------------------------------

  //--------------------Station colourCards------------------------------
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(rLED, OUTPUT);
  pinMode(gLED, OUTPUT);
  pinMode(triggerLED, OUTPUT);
  // Setting frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  // Setting starting values for LEDs
  digitalWrite(rLED, HIGH);
  digitalWrite(gLED, LOW);
  digitalWrite(triggerLED, HIGH);
  //--------------------------------------------------------------

  //--------------------Rätselspiel-------------------------------
  pinMode(quizButton1, INPUT_PULLUP);
  pinMode(quizButton2, INPUT_PULLUP);
  pinMode(quizButton3, INPUT_PULLUP);
  pinMode(quizButton4, INPUT_PULLUP);
  pinMode(quizLED1, OUTPUT);
  pinMode(quizLED2, OUTPUT);
  pinMode(quizLED3, OUTPUT);
  pinMode(quizLED4, OUTPUT);
  //--------------------Rätselspiel-------------------------------

  //--------------------Countdown---------------------------------

  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins);
  sevseg.setBrightness(10);
  
  //--------------------Countdown---------------------------------

  Serial.println("Setup completed...");
}

void loop() {

  if (deciSeconds == 0){
    Serial.println("Loop is running...");
    Serial.print("Actual millis is... ");
    Serial.print(timer);
  }

  if(countdown){
    sevseg.setNumber(countDown(), 1);
    sevseg.refreshDisplay();
  }

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


  buttonStateRead = digitalRead(Startbutton);

  Serial.println ("Startbutton Status");
  Serial.println (buttonStateStart);

  if (buttonStateRead == 0) //Startbutton einbauen
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

    buttonStateStart = 1;
  }


  else if (buttonStateRead == 1)
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

  if(buttonStateStart == 1){
    if ((buttonState3 == 0) && (buttonState1 == 1) && (buttonState2 == 1) && (buttonState4 == 1) && (a == 0))  {                            //erstes Kriterium: Button_3 drÃ¼cken
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
  
    if ((buttonState3 == 1) && (buttonState1 == 0) && (buttonState2 == 1) && (buttonState4 == 1) && (a == 1)) {
  
      Serial.println("Knopf 1 nach 3 gedrÃ¼ckt - richtig!");
  
      digitalWrite (green1, HIGH);
      delay(2000);
      digitalWrite (green1, LOW);
  
      a = a + 1; //a ist jetzt 2
  
    }
  
    //--------------------------------------------------------------
  
    else if  (((buttonState2 == 0) && (a == 1)) || ((buttonState4 == 0) && (a == 1)) || ((buttonState3 == 0) && (a == 1)) )  {
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
  
    if ((buttonState3 == 1) && (buttonState1 == 1) && (buttonState2 == 0) && (buttonState4 == 1) && (a == 2)) {
      Serial.println("Knopf 2 nach 1 gedrÃ¼ckt - richtig!");
  
      digitalWrite (green2, HIGH);
      delay(2000);
      digitalWrite (green2, LOW);
  
      a = a + 1; //a ist jetzt 3
  
    }
  
    //--------------------------------------------------------------
  
    else if  ( ((buttonState3 == 0) && (a == 2)) || ((buttonState4 == 0) && (a == 2)) || ((buttonState1 == 0) && (a == 2)))  {
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
  
    if ((buttonState3 == 1) && (buttonState1 == 1) && (buttonState2 == 1) && (buttonState4 == 0) && (a == 3)) {
      Serial.println("Knopf 4 nach 1 gedrÃ¼ckt - richtig!");
  
      digitalWrite (green4, HIGH);
      delay(2000);
      digitalWrite (green4, LOW);
  
      a = a + 1; //a ist jetzt 4
  
    }
  
    //--------------------------------------------------------------
  
    else if  (((buttonState1 == 0) && (a == 3)) || ((buttonState3 == 0) && (a == 3)) || ((buttonState2 == 0) && (a == 3)) )  {
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
  
    if ((buttonState3 == 1) && (buttonState1 == 1) && (buttonState2 == 0) && (buttonState4 == 1) && (a == 4)) {
      Serial.println("Knopf 2 nach 4 gedrÃ¼ckt - richtig!");
  
      digitalWrite (green2, HIGH);
      delay(2000);
      digitalWrite (green2, LOW);
  
      a = a + 1; //a ist jetzt 5
  
    }
  
    //--------------------------------------------------------------
  
    else if  (((buttonState1 == 0) && (a == 4)) || ((buttonState3 == 0) && (a == 4)) || ((buttonState4) && (a == 4)) )  {
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
      buttonStateStart = 0;
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
  buttonRead1 = digitalRead(quizButton1);
  buttonRead2 = digitalRead(quizButton2);
  buttonRead3 = digitalRead(quizButton3);
  buttonRead4 = digitalRead(quizButton4);
  delay(100);
  
  switch (quizCounter){ // progress bar
    case 0:
      digitalWrite(quizLED1, LOW);
      digitalWrite(quizLED2, LOW);
      digitalWrite(quizLED3, LOW);
      digitalWrite(quizLED4, LOW);
      break;
    case 1:
      digitalWrite(quizLED1, HIGH);
      break;
    case 2:
      digitalWrite(quizLED1, HIGH);
      digitalWrite(quizLED2, HIGH);
      break;
    case 3:
      digitalWrite(quizLED1, HIGH);
      digitalWrite(quizLED2, HIGH);
      digitalWrite(quizLED3, HIGH);
      break;
    case 4:
      digitalWrite(quizLED1, HIGH);
      digitalWrite(quizLED2, HIGH);
      digitalWrite(quizLED3, HIGH);
      digitalWrite(quizLED4, HIGH);
      break;
  }
  
    if(buttonRead1 == 1 && buttonRead2 == 1 && buttonRead3 == 1 && buttonRead4 == 1){
    quizButtonWait = true; // Once all buttons are released, it is ready to enter the loop again.
    }
    
  if(quizCounter != 4){  
    if(quizButtonWait && (buttonRead1 == 0 || buttonRead2 == 0 || buttonRead3 == 0 || buttonRead4 == 0)){ // Ready to enter loop as long as no buttons are pressed
      if(quizCounter == 0 && buttonRead1 == quizSequence1[0] && buttonRead2 == quizSequence1[1] && buttonRead3 == quizSequence1[2] && buttonRead4 == quizSequence1[3]){
        quizCounter = 1;
        quizButtonWait = false;
      }
      else if(quizCounter == 1 && buttonRead1 == quizSequence2[0] && buttonRead2 == quizSequence2[1] && buttonRead3 == quizSequence2[2] && buttonRead4 == quizSequence2[3]){
        quizCounter = 2;
        quizButtonWait = false; 
      }
      else if(quizCounter == 2 && buttonRead1 == quizSequence3[0] && buttonRead2 == quizSequence3[1] && buttonRead3 == quizSequence3[2] && buttonRead4 == quizSequence3[3]){
        quizCounter = 3;  
        quizButtonWait = false;
      }
      else if(quizCounter == 3 && buttonRead1 == quizSequence4[0] && buttonRead2 == quizSequence4[1] && buttonRead3 == quizSequence4[2] && buttonRead4 == quizSequence4[3]){
        quizCounter = 4;
        quizButtonWait = false;
      }
      else{
        //quizCounter = 0; // Here we start over
        for(int i=0; i<3; i++){
          digitalWrite(quizLED1, HIGH);
          digitalWrite(quizLED2, HIGH);
          digitalWrite(quizLED3, HIGH);
          digitalWrite(quizLED4, HIGH); 
          delay(300); 
          digitalWrite(quizLED1, LOW);
          digitalWrite(quizLED2, LOW);
          digitalWrite(quizLED3, LOW);
          digitalWrite(quizLED4, LOW);
          delay(300);
          quizButtonWait = false;    
        }
      }
    }
  }
}

  //--------------------ColourCards-------------------------------
bool colourCardRed(){
  // (255, 0, 0)
  if(red < green && red < blue && green > blue && red < 20){
    return true;
  }
  else{
    return false;
  }
}

bool colourCardYellow(){
  // (255, 255, 0)
  if(abs(blue-green) < 30 && red < blue && green < blue && green < 50){
    return true;
  }
  else{
    return false;
  }
}

bool colourCardBlue(){
  //(0, 0, 255)
  if(red + green + blue > 60 && blue < red && red > green){
    return true;
  }
  else{
    return false;
  } 
}

bool colourCardIn(){ // testing to see if card is in the slot with green triggerLED
  //(0, 255, 0)
  if(red + green + blue < 40 && red > green && red > blue){
    colourCardRemoved = true; // when card is removed, the loop is ready for testing again
    digitalWrite(rLED, HIGH);
    digitalWrite(gLED, LOW);
    return false;
  }
  else{
    return true;
  } 
}

void colourSense(){
  // Setting RED photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  // Reading and saving
  red = pulseIn(sensorOut, LOW);
  Serial.print("R = ");
  Serial.print(red);
  Serial.print("  ");
  delay(100);

  // Setting GREEN photodiodes to be read
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  // Reading and saving
  green = pulseIn(sensorOut, LOW);
  Serial.print("G = ");
  Serial.print(green);
  Serial.print("  ");
  delay(100);

  // Setting BLUE photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  // Reading and saving
  blue = pulseIn(sensorOut, LOW);
  Serial.print("B = ");
  Serial.print(blue);
  Serial.println("  ");
  delay(100);  
}
  //--------------------ColourCards-------------------------------

void colourCards(){
  switch(colourCardCounter){
    case 0:
      digitalWrite(progressLED1, LOW);  
      digitalWrite(progressLED2, LOW); 
      digitalWrite(progressLED3, LOW); 
      break;
    case 1:
      digitalWrite(progressLED1, HIGH);
      break;
    case 2:
      digitalWrite(progressLED2, HIGH);
      break;
    case 3:
      digitalWrite(progressLED3, HIGH);
      break;
  }
    
  colourSense();  
  // Reading cards, comparing rgb values
  if(colourCardIn() && colourCardRemoved){
    for(int i=0; i<3; i++){ // takes the third value
      colourSense();
    }
    if(colourCardCounter == 0 && colourCardRed() && !colourCardYellow() && !colourCardBlue()){
      colourCardCounter = 1;  
      digitalWrite(rLED, LOW);
      digitalWrite(gLED, HIGH);
      colourCardRemoved = false; // this prevents the loop from comparing the 
                              //current red card in the slot with colourCardCounter = 1;
                              // = true will trigger when card is removed in colourCardIn();
      Serial.println("Card is Red!");
    }
    else if(colourCardCounter == 1 && colourCardYellow() && !colourCardRed() && !colourCardBlue()){
      colourCardCounter = 2;
      digitalWrite(rLED, LOW);
      digitalWrite(gLED, HIGH);
      colourCardRemoved = false;
      Serial.println("Card is Yellow!");
    }
    else if(colourCardCounter == 2 && colourCardBlue() && !colourCardYellow() && !colourCardRed()){
      // UNLOCKED!
      colourCardCounter = 3;
      digitalWrite(rLED, LOW);
      digitalWrite(gLED, HIGH);
      colourCardRemoved = false;
      Serial.println("Card is Blue!");
      // Shut down colour sensor
      digitalWrite(S0, LOW);
      digitalWrite(S1, LOW);
      // visual and/or audio cue
      // lock this station and unlock next station/prize
    }
    else{ // What happens when the card is inserted in the wrong sequence
      colourCardCounter = 0; // here we start over
      // visual and/or audio cue
      for(int i=0; i<3; i++){ // blinks five times
        digitalWrite(rLED, LOW);
        digitalWrite(gLED, LOW);
        delay(300);
        digitalWrite(rLED, HIGH);
        delay(300);
      }
      colourCardRemoved = false;
      digitalWrite(rLED, HIGH);
      digitalWrite(gLED, LOW);
    }    
  }
}

  //--------------------------------------------------------------

  //--------------------Countdown---------------------------------

int countDown() {

    if (millis() >= timer){
    timer += 100;
    Serial.println(millis());
    Serial.println(timer);
    deciSeconds = 100;
    totalTime -= deciSeconds;
    showTime = (totalTime/100);
    Serial.println(showTime);
    return showTime;
    }
}
  //--------------------------------------------------------------

  //--------------------ÖffneBombe--------------------------------

void openBomb() {
  
}

  //--------------------------------------------------------------

