/* 
 *  V0.1 12.3.2017 
 */


 //-------------------- Keypad -----------------------------------------------------
 
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};
int check = 1; //Variable um Fkt1 zu deaktivieren, 0 = aus

char keyword_in[4];
char keyword_set[] = {'2','3','5','6'};

int zaehler = 0;

byte rowPins[ROWS] = {8, 7, 6, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
byte ledPin = 13; 

boolean blink = false;
boolean ledPin_state;

#include <LiquidCrystal.h>

LiquidCrystal lcd(14, 13, 12, 11, 10, 9); // Creates lcd object



//-------------------- Stangenspiel -----------------------------------------------------

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

int a = 1;  // Variable, die sich erhöht, wenn Spieler eine richctige Kombination im Spiel richtig hat

 //-------------------- Keypad -----------------------------------------------------

void setup(){
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);              // Sets the digital pin as output.
    digitalWrite(ledPin, HIGH);           // Turn the LED on.
    ledPin_state = digitalRead(ledPin);   // Store initial LED state. HIGH when LED is on.
    keypad.addEventListener(keypadEvent); // Add an event listener for this keypad
    lcd.begin(16, 2);                     // Activates 16 x 2 lcd
    LCDwelcomeScreen(); 

//-------------------------- Stangenspiel---------------------------------------------

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

//-------------------------------------------------------------------------------------------
    
}

void loop(){

//---------------------------------------- Stangenspiel ---------------------------------------

//Zielabfolge: 3 - 1 -2 - 4 - 2


buttonStateStart = analogRead(Startbutton);

Serial.println ("Startbutton Status");
Serial.println (buttonStateStart);

if (buttonStateStart ==1018) //Startbutton einbauen
{

digitalWrite (red1,LOW);
digitalWrite (green1,LOW);
digitalWrite (red2,LOW);
digitalWrite (green2,LOW);
digitalWrite (red3,LOW);
digitalWrite (green3,LOW);
digitalWrite (red4,LOW);
digitalWrite (green4,LOW);

digitalWrite (red3,HIGH);
digitalWrite (green3,HIGH);

delay(1500);

digitalWrite (red3,LOW);
digitalWrite (green3,LOW);

digitalWrite (red1,HIGH);
digitalWrite (green1,HIGH);

delay(1500);

digitalWrite (red1,LOW);
digitalWrite (green1,LOW);

digitalWrite (red2,HIGH);
digitalWrite (green2,HIGH);

delay(1500);

digitalWrite (red2,LOW);
digitalWrite (green2,LOW);

digitalWrite (red4,HIGH);
digitalWrite (green4,HIGH);

delay(1500);

digitalWrite (red4,LOW);
digitalWrite (green4,LOW);

digitalWrite (red2,HIGH);
digitalWrite (green2,HIGH);

delay(1500);

digitalWrite (red1,LOW);
digitalWrite (green1,LOW);
digitalWrite (red2,LOW);
digitalWrite (green2,LOW);
digitalWrite (red3,LOW);
digitalWrite (green3,LOW);
digitalWrite (red4,LOW);
digitalWrite (green4,LOW);

buttonStateStart = 0;
}


else if (buttonStateStart == 0) 
{
digitalWrite (red1,LOW);
digitalWrite (green1,LOW);
digitalWrite (red2,LOW);
digitalWrite (green2,LOW);
digitalWrite (red3,LOW);
digitalWrite (green3,LOW);
digitalWrite (red4,LOW);
digitalWrite (green4,LOW);

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


 if ((buttonState3 == 0) && (a == 0))  {                            //erstes Kriterium: Button_3 drücken
    Serial.println ("Knopf 3 gedrückt - richtig!"); 
    digitalWrite (green3,HIGH);
    delay(2000);
    digitalWrite (green3,LOW);
    a = a+1;
    }
//--------------------------------------------------------------
 else if (((buttonState1 == 0) && (a == 0)) || ((buttonState2 == 0) && (a == 0)) || ((buttonState4 == 0) && (a == 0)) ) {            //erstes Kriterium Fehler: Button_1 _2 oder _4 drücken
    for(int i; i<10; i++)
    {
    Serial.println("falsch - erstes Kriterium!"); 
    digitalWrite (red1,HIGH);
    digitalWrite (red2,HIGH);
    digitalWrite (red3,HIGH);
    digitalWrite (red4,HIGH);
    delay(100);
    digitalWrite (red1,LOW);
    digitalWrite (red2,LOW);
    digitalWrite (red3,LOW);
    digitalWrite (red4,LOW);
    delay(100);
    }
    a=0;
    }
    
//--------------------------------------------------------------

    else                              //erstes Kriterium: nichts gedrückt
    {
    digitalWrite (green1,LOW);
    digitalWrite (green2,LOW);
    digitalWrite (green3,LOW);
    digitalWrite (green4,LOW);
    
    digitalWrite (red1,LOW);
    digitalWrite (red2,LOW);
    digitalWrite (red3,LOW);
    digitalWrite (red4,LOW);
    }

//--------------------------------------------------------------

    if ((buttonState1 == 0) && (a == 1)) {
    Serial.println("Knopf 1 nach 3 gedrückt - richtig!"); 

    digitalWrite (green1,HIGH);
    delay(2000);
    digitalWrite (green1,LOW);

    a=a+1; //a ist jetzt 2
    
    }

//--------------------------------------------------------------
   
    else if  (((buttonState2 == 0) && (a == 1))|| ((buttonState4 == 0) && (a == 1)) )  {
    for(int i; i<10; i++)
    {
    Serial.println("falsch - zweites Kriterium!"); 
    
    digitalWrite (red1,HIGH);
    digitalWrite (red2,HIGH);
    digitalWrite (red3,HIGH);
    digitalWrite (red4,HIGH);
    delay(100);
    digitalWrite (red1,LOW);
    digitalWrite (red2,LOW);
    digitalWrite (red3,LOW);
    digitalWrite (red4,LOW);
    delay(100);
    }
    a = 0;
    }
    
    else                              //zweites Kriterium: nichts gedrückt
    {
    digitalWrite (green1,LOW);
    digitalWrite (green2,LOW);
    digitalWrite (green3,LOW);
    digitalWrite (green4,LOW);
    
    digitalWrite (red1,LOW);
    digitalWrite (red2,LOW);
    digitalWrite (red3,LOW);
    digitalWrite (red4,LOW);
    }
 
 
//--------------------------------------------------------------

    if ((buttonState2 == 0) && (a == 2)) {
    Serial.println("Knopf 2 nach 1 gedrückt - richtig!"); 

    digitalWrite (green2,HIGH);
    delay(2000);
    digitalWrite (green2,LOW);

    a=a+1; //a ist jetzt 3
    
    }

//--------------------------------------------------------------
   
    else if  ( ((buttonState3 == 0) && (a == 2)) || ((buttonState4 == 0) && (a == 2)) )  {
    for(int i; i<10; i++)
    {
    Serial.println("falsch - drittes Kriterium!"); 
    
    digitalWrite (red1,HIGH);
    digitalWrite (red2,HIGH);
    digitalWrite (red3,HIGH);
    digitalWrite (red4,HIGH);
    delay(100);
    digitalWrite (red1,LOW);
    digitalWrite (red2,LOW);
    digitalWrite (red3,LOW);
    digitalWrite (red4,LOW);
    delay(100);
    }
    a = 0;
    }
    
    else                              //drittes Kriterium: nichts gedrückt
    {
    digitalWrite (green1,LOW);
    digitalWrite (green2,LOW);
    digitalWrite (green3,LOW);
    digitalWrite (green4,LOW);
    
    digitalWrite (red1,LOW);
    digitalWrite (red2,LOW);
    digitalWrite (red3,LOW);
    digitalWrite (red4,LOW);
    }

 //--------------------------------------------------------------

    if ((buttonState4 == 0) && (a == 3)) {
    Serial.println("Knopf 4 nach 1 gedrückt - richtig!"); 

    digitalWrite (green4,HIGH);
    delay(2000);
    digitalWrite (green4,LOW);

    a=a+1; //a ist jetzt 4
    
    }

//--------------------------------------------------------------
   
    else if  (((buttonState1 == 0) && (a == 3)) || ((buttonState3 == 0) && (a == 3)) )  {
    for(int i; i<10; i++)
    {
    Serial.println("falsch - viertes Kriterium!"); 
    
    digitalWrite (red1,HIGH);
    digitalWrite (red2,HIGH);
    digitalWrite (red3,HIGH);
    digitalWrite (red4,HIGH);
    delay(100);
    digitalWrite (red1,LOW);
    digitalWrite (red2,LOW);
    digitalWrite (red3,LOW);
    digitalWrite (red4,LOW);
    delay(100);
    }
    a = 0;
    }
    
    else                              //viertes Kriterium: nichts gedrückt
    {
    digitalWrite (green1,LOW);
    digitalWrite (green2,LOW);
    digitalWrite (green3,LOW);
    digitalWrite (green4,LOW);
    
    digitalWrite (red1,LOW);
    digitalWrite (red2,LOW);
    digitalWrite (red3,LOW);
    digitalWrite (red4,LOW);
    }
 
//--------------------------------------------------------------

     if ((buttonState2 == 0) && (a == 4)) {
    Serial.println("Knopf 2 nach 4 gedrückt - richtig!"); 

    digitalWrite (green2,HIGH);
    delay(2000);
    digitalWrite (green2,LOW);

    a=a+1; //a ist jetzt 5
    
    }

//--------------------------------------------------------------
   
    else if  (((buttonState1 == 0) && (a == 4)) || ((buttonState3 == 0) && (a == 4))  )  {
    for(int i; i<10; i++)
    {
    Serial.println("falsch - fünftes Kriterium!"); 
    
    digitalWrite (red1,HIGH);
    digitalWrite (red2,HIGH);
    digitalWrite (red3,HIGH);
    digitalWrite (red4,HIGH);
    delay(100);
    digitalWrite (red1,LOW);
    digitalWrite (red2,LOW);
    digitalWrite (red3,LOW);
    digitalWrite (red4,LOW);
    delay(100);
    }
    a = 0;
    }
    
    else                              //fünftes Kriterium: nichts gedrückt
    {
    digitalWrite (green1,LOW);
    digitalWrite (green2,LOW);
    digitalWrite (green3,LOW);
    digitalWrite (green4,LOW);
    
    digitalWrite (red1,LOW);
    digitalWrite (red2,LOW);
    digitalWrite (red3,LOW);
    digitalWrite (red4,LOW);
    }

//--------------------------------------------------------------
    if (a== 5)
    {

      for(int r = 0; r<10; r++) {
        
        digitalWrite (green1,HIGH);
        digitalWrite (green2,HIGH);
        digitalWrite (green3,HIGH);
        digitalWrite (green4,HIGH);
        delay(100);
        digitalWrite (green1,LOW);
        digitalWrite (green2,LOW);
        digitalWrite (green3,LOW);
        digitalWrite (green4,LOW);
        delay(100);
        }
        
      Serial.println("Spiel gewonnen - CODE lautet ABC123!"); 
      delay(10000);
      a=0;
      
      }
      
//-------------------------- Keypad --------------------------------- 

    if (check){
      char key = keypad.getKey();
         
      if (key) {
          
          if (zaehler <= 3){
            keyword_in[zaehler] = key;
            LCDpassword();   
            zaehler += 1;
          }
          
          Serial.println(key);
          Serial.println(keyword_set);
          Serial.println(keyword_in);
          
          if (zaehler == 4){
            Serial.println("Press * for check password");
            Serial.println("Press # for delete last number");
            lcd.setCursor(0, 1);
            lcd.print("Press * Confirm");            // Visual cue
          }
      }
    }
    if (blink){
        digitalWrite(ledPin,!digitalRead(ledPin));    // Change the ledPin from Hi2Lo or Lo2Hi.
        delay(100);
    }
}

// Taking care of some special events.
void keypadEvent(KeypadEvent key){
    switch (keypad.getState()){
    case PRESSED:
        if (key == '#') {
            digitalWrite(ledPin,!digitalRead(ledPin));
            ledPin_state = digitalRead(ledPin);        // Remember LED state, lit or unlit.
            if (zaehler >=1){
              keyword_in[zaehler] = 'X';
              zaehler -= 1;
            }
        }
        if (key == '*') {
          if (zaehler == 4){
            if (keyword_in[0] == keyword_set[0] and keyword_in[1] == keyword_set[1] and
                keyword_in[2] == keyword_set[2] and keyword_in[3] == keyword_set[3]){
              Serial.println("You got the right Code");
              LCDcorrect();
              zaehler = 0;
              keyword_in[4];
              break;
            }
            if (keyword_in != keyword_set){
              Serial.println("Your Code is WRONG");
              Serial.println("Your timeout is 300 seconds");
              LCDwrong();                      
              break;
            }
          }
          else {
            Serial.println("You have to press more numbers");
          }
        }
        break;

    case RELEASED:
        if (key == '*') {
            digitalWrite(ledPin,ledPin_state);    // Restore LED state from before it started blinking.
            blink = false;
        }
        break;

    case HOLD:
        if (key == '*') {
            blink = true;    // Blink the LED when holding the * key.
        }
        break;
    }
}

//===LCD functions by James Low
void LCDwelcomeScreen(){
    lcd.clear();
    lcd.setCursor(0, 0);          // (pos, row) starting with 0
    lcd.print("Enter Serial Nr:"); 
    lcd.setCursor(2, 1);
    lcd.print("===MCTUBS==="); 
}

void LCDpassword(){
    lcd.clear();                    
    for(int i=0; i<=zaehler; i++){  // for loop required for #Delete f(x)
        lcd.setCursor(zaehler, 0);  
        lcd.print(keyword_in[zaehler]);              
        lcd.setCursor(0, 1); 
        lcd.print("Press # Delete");            
    }  
}

void LCDdotting(int pos, int row){
   for(int i=pos; i<=16-pos; i++){
      lcd.setCursor(i, row);
      lcd.print(".");
      delay(100);
   } 
}

void LCDcorrect(){
    for(int i=3; i>=1; i--){
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
    for(int cursorPos = 0; cursorPos < 100; cursorPos++){
        lcd.scrollDisplayLeft();
        delay(300);
    }  
}

void LCDwrong(){}

//===LCD functions by James Low
