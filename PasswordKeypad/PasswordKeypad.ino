/*
    V0.1    12.03.2017 Keypad
    V0.2    01.04.2017 Stangenspiel
    V0.2.1  05.04.2017 Keypad fixed
    V0.3    19.04.2017 Games added
    V0.3.1  19.04.2017 Countdown added
    V0.4    16.06.2017 Checkpoints in Functions added
    V0.5    11.08.2017 SoftwareVersion als Stamp hinzugefügt, Zeitlayout angepasst
                       correctSequencing Funktion hinzugefügt
    V0.5.1  13.08.2017 Überarbeitung Stangenspiel, Änderung der Buttoninputs mit internem Pullup
    V0.5.2  20.10.1017 Änderung des define sensorOut von A2 in Arduino Pin 56/ Verifikation ausstehend!
    V0.6    21.10.2017 Neuprogrammierung Stangenspiel, Umbenennung Keypad in Keypad1
    V0.7    06.11.1017 Änderung des Stangenspiels in Matrix, Hinzufügen der Kontrollstrukturen, Setzen des Checkpoints ausstehend!! Z475
    V0.7.1  11.11.2017 Änderung der Deklaration der LEDs des Stangenspiels, LED Feedback hinzugefügt
    V0.8    09.02.2018 Änderung des Rätselspiels in Matrix
*/
#include <Keypad.h>
#include <LiquidCrystal.h>
#include "SevSeg.h"


#define SoftwareVersion "201802092007"  // IMMER AKTUALISIEREN YYYYMMDDHHMM

#define _myArray_cnt 8 //Array Groesse Keypad
#define _myArray_cnt2 5 //Array Groesse Stangenspiel
#define _myArray_cnt_raetsel 4 //Array Groesse Rätselspiel

//--------------------Station colourCards------------------------------
#define S0 27
#define S1 28
#define S2 29
#define S3 30
#define sensorOut 56
#define rLED 34
#define gLED 35

#define progressLED1 31
#define progressLED2 32
#define progressLED3 33
//--------------------Station colourCards------------------------------

unsigned int check_1 = 1; //Variable um Keypad zu aktivieren, 0 = aus
unsigned int check_2 = 0; //Variable um Colourcards zu aktivieren, 0 = aus
unsigned int check_3 = 0; //Variable um Raetselspiel zu aktivieren, 0 = aus
unsigned int check_4 = 0; //Variable um Stangenspiel zu aktivieren, 0 = aus
unsigned int check_5 = 0; //Variable um die Bombe zu öffnen, 0 = zu
unsigned int countdown = 0; //Vriable um den Countdown zu starten


//---------------------Keypad-----------------------------------

LiquidCrystal lcd(14, 13, 12, 11, 10, 9); // Creates lcd object

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

char keyword_in[_myArray_cnt] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
char keyword_set[_myArray_cnt] = {'0', '6', '0', '4', '2', '0', '1', '7'};

int zaehler = 0;

byte rowPins[ROWS] = {8, 7, 6, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad1 = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//--------------------Stangenspiel------------------------------

const byte ROWS_s = 1;
const byte COLS_s = 5;

char stang_keys[ROWS_s][COLS_s] = {
  {'X', '1', '2', '3', '4'}
};

byte row_sPins[ROWS_s] = {36};
byte col_sPins[COLS_s] = {53, 23, 24, 25, 26};

char folge_in[_myArray_cnt2] = {' ', ' ', ' ', ' ', ' '};
char folge_set[_myArray_cnt2] = {'3', '1', '2', '4', '2'};

Keypad keypad2 = Keypad( makeKeymap(stang_keys), row_sPins, col_sPins, ROWS_s, COLS_s );

int zaehler2 = 0;

//int startButton = 53;
int startbutton_pressed = 0;

int redleds[4] = {15, 17, 19, 21};
int greenleds[4] = {16, 18, 20, 22};
int intfolge_set[5] = {3, 1, 2, 4, 2};

//--------------------------------------------------------------

//--------------------Rätselspiel-------------------------------

const byte ROWS_r = 1;
const byte COLS_r = 4;

char raetsel_keys[ROWS_r][COLS_r] = {
  {'1', '2', '3', '4'}
};

char quizButton[_myArray_cnt_raetsel] = {'2', '3', '1', '4'};

byte row_rPins[ROWS_r] = {45};
byte col_rPins[COLS_r] = {37, 38, 39, 40};

Keypad keypad3 = Keypad( makeKeymap(raetsel_keys), row_rPins, col_rPins, ROWS_r, COLS_r);

int quizFragen = 0;

int quizleds[4] = {41, 42, 43, 44};
int intanwser_set[4] = {2, 3, 1, 4};

//--------------------Rätselspiel-------------------------------


//--------------------Station colourCards------------------------------
unsigned int red = 0;      // rgb values stored here
unsigned int green = 0;
unsigned int blue = 0;
int colourCardCounter = 0;  // sequencing
bool colourCardRemoved = true; // flag needed to exit for-loop after correct card is inserted
//--------------------------------------------------------------


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
  Serial.println(SoftwareVersion);

  lcd.begin(16, 2);                     // Activates 16 x 2 lcd
  LCDwelcomeScreen();
  if (check_1 == 0) {
    LCDsolved();
  }

  //---------------------Keypad-----------------------------------

  keypad1.addEventListener(keypadEvent); // Add an event listener for this keypad
  keypad2.addEventListener(stangenspielEvent); //Add Event Listener fuer Stangenspiel
  keypad3.addEventListener(raetselspielEvent); //Add Event Listener fuer Raetselspiel

  //--------------------Stangenspiel------------------------------
  for(int i = 0; i <= 3; i++){
    pinMode(redleds[i], OUTPUT);
    pinMode(greenleds[i], OUTPUT);
  }

  //--------------------------------------------------------------

  //--------------------Station colourCards------------------------------
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(progressLED1, OUTPUT);
  pinMode(progressLED2, OUTPUT);
  pinMode(progressLED3, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(rLED, OUTPUT);
  pinMode(gLED, OUTPUT);

  // Setting frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  // Setting starting values for LEDs
  digitalWrite(rLED, HIGH);
  digitalWrite(gLED, LOW);

  //--------------------------------------------------------------

  //--------------------Rätselspiel-------------------------------

  for(int i = 0; i <= 3; i++){
    pinMode(quizleds[i], OUTPUT);
  }

  //--------------------Rätselspiel-------------------------------

  //--------------------Countdown---------------------------------

  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins);
  sevseg.setBrightness(10);

  //--------------------Countdown---------------------------------

  Serial.println("Setup completed...");
}

void loop() {

  if (countdown) {
    sevseg.setNumber(countDown(), 1);
    sevseg.refreshDisplay();
  }

  if (check_1) {
    keyPadcode();
  }

  if (check_2) {
    colourCards();
  }

  if (check_3) {
    raetselSpiel();
  }

  if (check_4) {
    stangenSpiel();
  }

  if (check_5) {
    ende();
  }
}

// Taking care of some special events.
void keypadEvent(KeypadEvent key) {
  switch (keypad1.getState()) {
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
            LCDsolved();
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
  lcd.setCursor(3, 1);
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

  check_1 = 0;
  check_2 = 1;

  lcd.setCursor(3, 0);
  lcd.print("Proceed to");
  lcd.setCursor(3, 1);
  lcd.print("Station 2!");
  delay(5000);

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

void LCDsolved() {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Station");
  lcd.setCursor(4, 1);
  lcd.print("Solved!");
}

//--------------------Keypad LCD--------------------------------

void keyPadcode() {
  char key = keypad1.getKey();

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
//--------------------Keypad LCD--------------------------------

void stangenspielEvent(KeypadEvent folge) {
  switch (keypad2.getState()) {
    case PRESSED:
      if (folge == 'X' and zaehler2 == 0) { // == 'X' Spiel intitialisieren
        zeigeFolge();
        //startbutton_pressed = 1;
        if (zaehler2 == 0){
          zaehler2 +=1;
        }
        break;
      }

      if (folge == folge_set[(zaehler2)-1] and zaehler2 >= 1){ //Gezogene Stange ist korrekt
        folge_in[(zaehler2)-1] = folge;
        Serial.println("Correct");
        Serial.println(folge_in[(zaehler2)-1]);
        correctSequencing((int(folge)-49));
        zaehler2 += 1;
      }
      else {//(folge != folge_set[(zaehler2)-1] and zaehler2 >= 1){ //zurücksetzen bei Fehler
        falseSequencing(int(folge)-49);
        resetStangenspiel();
      }

      if (zaehler == 6){
        //correctSequencing(int(folge));
        //counterXYXYXYXYXYXYXYXYXY
      }
      
    case RELEASED:
      if (folge == ' ') {
      }
      break;

    case HOLD:
      if (folge == ' ') {
      }
      break;
  }
}

void stangenSpiel() {
  //Zielabfolge: 3 - 1 -2 - 4 - 2
  Serial.println("stangenSpiel() aktiviert");
  char folge = keypad2.getKey();

  if (folge) {
    Serial.println(folge);
    Serial.println(folge_set);
    Serial.println(folge_in);
    Serial.println(zaehler2);
  }
}

void falseSequencing(int A) {

  for (int i = 0; i <= 4; i++){
    digitalWrite(redleds[A], HIGH);
    delay(300);
    digitalWrite(redleds[A], LOW);
    delay(100);
  }

}

void correctSequencing(int A) {
  
  for (int i = 0; i <= 4; i++){
    digitalWrite(greenleds[A], HIGH);
    delay(300);
    digitalWrite(greenleds[A], LOW);
    delay(100);
  }
}

void zeigeFolge() {
  for (int i = 0; i <= 4; i++){
    digitalWrite(greenleds[intfolge_set[i]-1], HIGH);
    delay(300);
    digitalWrite(greenleds[intfolge_set[i]-1], LOW);
    delay(100);
  }
}
void resetStangenspiel() {
  for (unsigned int i = 0; i < _myArray_cnt2; i++) {
    folge_in[i] = ' ';
  }
  zaehler2 = 0;
}

//--------------------------------------------------------------

//--------------------Rätselspiel-------------------------------

void raetselspielEvent(KeypadEvent antwort){
  switch (keypad3.getState()){
    case PRESSED:
      if (antwort == quizButton[quizFragen]){
        Serial.println("Correct answer");
        digitalWrite(quizleds[intanwser_set[quizFragen]], HIGH);
        delay(1000);
        digitalWrite(quizleds[intanwser_set[quizFragen]], LOW);
        quizFragen += 1;
        }
        
        else {
          Serial.println("FALSE"); //Plus Zeitstrafe
          for (int a = 0; a <= 3; a++){
            for (int i = 0; i <= 3; i++){
              digitalWrite(quizleds[i], HIGH);
              delay(300);
              digitalWrite(quizleds[i], LOW);
              delay(300);
            }
          }
        
      }

    case RELEASED:
      if (antwort = ' ') {
        break;
      }

    case HOLD:
      if (antwort = ' ') {
       break;
      }
  }
}
void raetselSpiel() {
  Serial.println("raetselSpiel() aktiviert");

  char antwort = keypad3.getKey();

  if (antwort) {
    Serial.println(antwort);
    Serial.println(quizFragen);
    Serial.println(quizButton); //Antworten
  }
}

//--------------------ColourCards-------------------------------
bool colourCardRed() {
  int r = 980;
  int g = 1290;
  int b = 1000;
  int tol = 200;
  if ((red >= (r - tol)) && (red <= (r + tol)) && (green >= (g - tol))
      && (green <= (g + tol)) && (blue >= (b - tol)) && (blue <= (b + tol))) {
    Serial.println("Red!");
    return true;
  }
  else {
    return false;
  }
}

bool colourCardYellow() {
  int r = 780;
  int g = 570;
  int b = 955;
  int tol = 200;
  if ((red >= (r - tol)) && (red <= (r + tol)) && (green >= (g - tol))
      && (green <= (g + tol)) && (blue >= (b - tol)) && (blue <= (b + tol))) {
    Serial.println("Yellow!");
    return true;
  }
  else {
    return false;
  }
}

bool colourCardBlue() {
  int r = 1330;
  int g = 1060;
  int b = 995;
  int tol = 200;
  if ((red >= (r - tol)) && (red <= (r + tol)) && (green >= (g - tol))
      && (green <= (g + tol)) && (blue >= (b - tol)) && (blue <= (b + tol))) {
    Serial.println("Blue!");
    return true;
  }
  else {
    return false;
  }
}

bool colourCardIn() { // testing to see if card is in the slot with green triggerLED
  int r = 63;
  int g = 50;
  int b = 50;
  int tol = 50;
  if ((red >= (r - tol)) && (red <= (r + tol)) && (green >= (g - tol))
      && (green <= (g + tol)) && (blue >= (b - tol)) && (blue <= (b + tol))) {
    colourCardRemoved = true; // when card is removed, the loop is ready for testing again
    digitalWrite(rLED, HIGH);
    digitalWrite(gLED, LOW);
    Serial.println("Card slot empty!");
    return false;
  }
  else {
    return true;
  }
}

void colourSense() {
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

void colourCards() {
  switch (colourCardCounter) {
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
  if (colourCardIn() && colourCardRemoved) {
    for (int i = 0; i < 3; i++) { // takes the third value
      colourSense();
    }
    if (colourCardCounter == 0 && colourCardRed() && !colourCardYellow() && !colourCardBlue()) {
      colourCardCounter = 1;
      digitalWrite(rLED, LOW);
      digitalWrite(gLED, HIGH);
      colourCardRemoved = false; // this prevents the loop from comparing the
      //current red card in the slot with colourCardCounter = 1;
      // = true will trigger when card is removed in colourCardIn();
      Serial.println("Card is Red!");
    }
    else if (colourCardCounter == 1 && colourCardYellow() && !colourCardRed() && !colourCardBlue()) {
      colourCardCounter = 2;
      digitalWrite(rLED, LOW);
      digitalWrite(gLED, HIGH);
      colourCardRemoved = false;
      Serial.println("Card is Yellow!");
    }
    else if (colourCardCounter == 2 && colourCardBlue() && !colourCardYellow() && !colourCardRed()) {
      // COLOURUNLOCKED!
      colourCardCounter = 3;
      digitalWrite(rLED, LOW);
      digitalWrite(gLED, HIGH);
      colourCardRemoved = false;
      Serial.println("Card is Blue!");
      // Shut down colour sensor
      digitalWrite(S0, LOW);
      digitalWrite(S1, LOW);
      check_2 = 0;
      check_3 = 1;
      // visual and/or audio cue
      check_4 = 0;
      check_5 = 1;
      // lock this station and unlock next station/prize

    }
    else { // What happens when the card is inserted in the wrong sequence
      colourCardCounter = 0; // here we start over
      // visual and/or audio cue
      for (int i = 0; i < 3; i++) { // blinks five times
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

  if (millis() >= timer) {
    timer += 100;
    Serial.println(millis());
    Serial.println(timer);
    deciSeconds = 100;
    totalTime -= deciSeconds;
    showTime = (totalTime / 100);
    Serial.println(showTime);
    return showTime;
  }
}
//--------------------------------------------------------------

//--------------------Ende-Reset--------------------------------

void ende() {

}

//--------------------------------------------------------------

