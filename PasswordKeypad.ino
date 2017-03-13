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

char keyword_in[4];
char keyword_set[] = {'2','3','5','6'};

int zaehler = 0;
int abc = 1;

int test = 0; 


byte rowPins[ROWS] = {8, 7, 6, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
byte ledPin = 13; 

boolean blink = false;
boolean ledPin_state;

void setup(){
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);              // Sets the digital pin as output.
    digitalWrite(ledPin, HIGH);           // Turn the LED on.
    ledPin_state = digitalRead(ledPin);   // Store initial LED state. HIGH when LED is on.
    keypad.addEventListener(keypadEvent); // Add an event listener for this keypad
}

void loop(){
    char key = keypad.getKey();

    if (key) {
        
        if (zaehler <= 3){
          keyword_in[zaehler] = key;
          zaehler += 1;
        }
        
        Serial.println(key);
        Serial.println(keyword_set);
        Serial.println(keyword_in);
        
        if (zaehler == 4){
          Serial.println("Press * for check password");
          Serial.println("Press # for delete last number");
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
            if (keyword_in == keyword_set){
              Serial.println("You got the right Code");
              zaehler = 0;
              keyword_in[4];
              break;
            }
            if (keyword_in != keyword_set){
              Serial.println("Your Code is WRONG");
              Serial.println("Your timeout is 300 seconds");
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
