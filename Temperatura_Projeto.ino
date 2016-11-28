#include <LiquidCrystal.h>

int lm35 = A0;
float temp;
int bright = 6;
int fonte = 7;

String inputString = "";
boolean stringComplete = false;

int peltier =  9;

float readings = 100;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(lm35, INPUT);
  pinMode(bright, OUTPUT);
  analogWrite(bright, 50);
  pinMode(fonte, OUTPUT);
  pinMode(peltier, OUTPUT);
}

void loop() {
  temp = 0;
  if (stringComplete) {
    if(inputString.equals("RT\n")){
        lcd.clear();
        for(int i=0; i<readings; i++){
          temp+=(float(analogRead(lm35))*5/(1023))/0.01;
        }
        lcd.print("Temp: ");
        lcd.print((temp/readings));
        Serial.println((temp/readings)); 
    }else if(inputString.startsWith("OUTPUT: ")){
      inputString.replace("OUTPUT: ", "");
      int output = inputString.toInt();
      analogWrite(peltier, output);
      if(output <= 0){
        digitalWrite(fonte, LOW);
      }else{
        digitalWrite(fonte, HIGH);
      }
    }else if(inputString.equals("SHUTDOWN\n")){
      lcd.clear();
      digitalWrite(fonte, LOW);
    }
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

