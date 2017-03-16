/* 
 *  V0.1 12.3.2017 
 */
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

void setup(){
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);              // Sets the digital pin as output.
    digitalWrite(ledPin, HIGH);           // Turn the LED on.
    ledPin_state = digitalRead(ledPin);   // Store initial LED state. HIGH when LED is on.
    keypad.addEventListener(keypadEvent); // Add an event listener for this keypad
    lcd.begin(16, 2);                     // Activates 16 x 2 lcd
}

void loop(){
    if (check){
      char key = keypad.getKey();
      LCDwelcomeScreen();
         
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
    lcd.setCursor(0, 1);
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
