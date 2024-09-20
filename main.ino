// C++ code
//servo motor library
#include <Servo.h>
Servo myservo;

//LCD screen library
#include <LiquidCrystal.h>

const int trigPin = 7;
const int echoPin = 6;

//Stepper motor speed (10RPM)
int motorSpeed = 10;

//initialize duration and distance for the ping sensor
float duration, distance;

//Stepper Motor library
#include <Stepper.h>

Stepper myStepper(2048, 8, 10, 9, 11);

//pin setup
const int button1 = 13;
const int button2 = 12;
const int button3 = 2;

const int pingPin = 6;

const int LEDpins[] = {48, 46, 44};

//game parameters
int passcode[3];
int codeEntry[3];

const int rs = 40, en = 42, d4 = A15, d5 = 52, d6 = 53, d7 = 51;
LiquidCrystal lcd_1(rs, en, d4, d5, d6, d7);

//win
//call this function when the code is correct
void win()
{  
  //turn off LEDS
  digitalWrite(LEDpins[0], LOW);
  digitalWrite(LEDpins[1], LOW);
  digitalWrite(LEDpins[2], LOW);
  
  //display congrats on LCD monitor
  lcd_1.clear();
  lcd_1.print("Mr.Bear is free!");
  
  lcd_1.setCursor(7, 1);
  lcd_1.print(":D");

  //open the door lock
  myservo.write(180);
  delay(1000);
  exit(0); //end the program
}

//times up
//call this function when the code isn't 
//guessed and the zookeeper gets to the cage
void times_up()
{
  //display on LCD monitor that animal is still caged
  lcd_1.clear();
  lcd_1.print("Mr.Bear couldn't");
  
  lcd_1.setCursor(0, 1);
  lcd_1.print("escape in time");
  
  //turn all LEDs off
  digitalWrite(LEDpins[0], LOW);
  digitalWrite(LEDpins[1], LOW);
  digitalWrite(LEDpins[2], LOW);
  
  exit(0); //end the program
}

//incorrect
//call this function when the code is incorrect 
//but the zookeeper ISN'T at the cage
void incorrect()
{
  lcd_1.clear();
  lcd_1.print("Incorrect Code!");
  
  lcd_1.setCursor(0, 1);
  lcd_1.print("Try Again!");
  //blink lights
  for (int i = 0; i < 3; i++)
  {
    
    digitalWrite(LEDpins[0], HIGH);
    digitalWrite(LEDpins[1], HIGH);
    digitalWrite(LEDpins[2], HIGH);
    delay(250);
    
    digitalWrite(LEDpins[0], LOW);
    digitalWrite(LEDpins[1], LOW);
    digitalWrite(LEDpins[2], LOW);
    delay(250);
  }
}

void setup()
{
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);

  myservo.attach(4);
  myservo.write(0);

  for (int i = 0; i < 3; i++) 
  {
    pinMode(LEDpins[i], OUTPUT);
  }
  
  lcd_1.begin(16, 2); // Set up the number of columns and rows on the LCD.
  
  // Print the welcome message
  lcd_1.print("Guess the code");
  
  lcd_1.setCursor(0, 1);
  lcd_1.print("& free Mr.Bear!");
  delay(2000);
  lcd_1.clear();
  
  lcd_1.setCursor(5,0);
  lcd_1.print("Ready");
  delay(1000);
  lcd_1.clear();
  
  lcd_1.setCursor(6,0);
  lcd_1.print("Set");
  delay(1000);
  lcd_1.clear();
  
  lcd_1.setCursor(6,0);
  lcd_1.print("Go");
  
  pinMode(trigPin, OUTPUT);  
  pinMode(echoPin, INPUT);
  
  Serial.begin(9600);
  
  //choose random passcode
  randomSeed(analogRead(0)); //Seed the random number generator
  for (int i = 0; i < 3; i++) 
  {
    passcode[i] = random(1, 4); // Generate random number between 1 and 3
  }
  
  Serial.println(passcode[0]);
  Serial.println(passcode[1]);
  Serial.println(passcode[2]);
  
}

void loop()
{
  //initialize each button read
  int code1 = digitalRead(button1);
  int code2 = digitalRead(button2);
  int code3 = digitalRead(button3);

  long duration;
  int zookeeperdistance;
  
  static int passcodeIndex = 0;
  myStepper.setSpeed(motorSpeed);
  myStepper.step(90);


  digitalWrite(trigPin, LOW);
  //delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  //delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  zookeeperdistance = (duration*.0343)/2;
  //sense the buttons, put the corresponding code into the array, and light the corresponding LEDs
  if (code1 == HIGH)
  {
    digitalWrite(LEDpins[0], HIGH);
    delay(250);
    digitalWrite(LEDpins[0], LOW);
    codeEntry[passcodeIndex] = 1;
    passcodeIndex++;
  }
  else if (code2 == HIGH)
  {
    digitalWrite(LEDpins[1], HIGH);
    delay(250);
    digitalWrite(LEDpins[1], LOW);
    codeEntry[passcodeIndex] = 2;
    passcodeIndex++;
  }
  else if (code3 == HIGH)
  {
    digitalWrite(LEDpins[2], HIGH);
    delay(250);
    digitalWrite(LEDpins[2], LOW);
    codeEntry[passcodeIndex] = 3;
    passcodeIndex++;
  }
  
  //when the array is filled (3 values), check if the 
  //code matches the correct code
  bool arraysEqual = true;
  
  if (passcodeIndex == 3)
  {
    for (int i=0;i<3;i++)
    {
      if (passcode[i] != codeEntry[i])
      {
        arraysEqual = false;
      }
    }
  
    //depending on if the entered code is correct, 
    //call the corresponding function
    if (arraysEqual)
    {
      win();
    }
    else if (!arraysEqual)
    {
      incorrect();
    }
    passcodeIndex = 0;
  }
  
  //check if the zookeeper has reached the cage
  if (zookeeperdistance < 3)
  {
    times_up();
  }
}
