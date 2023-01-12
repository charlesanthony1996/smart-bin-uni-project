// libraries for oled module
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// oled width and height initialized here
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

// oled connected via i2c
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);



// add the ultra sonic sensor here

#include <Stepper.h>

// for stepper motor

const int steps_per_rev = 2048;  


// ultra sonic sensor pins
const int trigPin = 5;
const int echoPin = 18;


// define sound speed
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

// variables for ultrasonic
long duration;
float distanceCm;
float distanceInch;

// making sure motor is turned off
bool motorRunning = false;




#define IN1 26
#define IN2 25
#define IN3 33
#define IN4 32

Stepper motor(steps_per_rev, IN1, IN3, IN2, IN4);

void setup() {
  motor.setSpeed(15);
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // setup for oled module
  // 0x3D for 128 x 64 screen
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 Allocation failed!"));
    for(;;);
  }

  // delay(2000);
  // display.clearDisplay();
  // display.setTextSize(1);
  // display.setTextColor(WHITE);
  // // display static text
  // // display.println("Hello world");
  // display.println("Distance: ");
  // display.println(distanceCm);
  // display.display();


}

void loop() {
  delay(100);
  display.clearDisplay();
  display.setTextSize(0.8);
  display.setTextColor(WHITE);
  display.setCursor(1, 1);
  // display static text
  // display.println("Hello world");
  display.println("Distance: ");
  display.print(distanceCm);
  display.print("\n");
  display.println("Motor: ");
  display.print(motorRunning);
  display.print("\n");
  display.println("Weight: ");
  display.display();

  

  // ultrasonic sensor part
  // clears the trig pin here
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // sets the trig pin on high state for 10 micro seconds

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);


  // reads the echopin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // calculate the distance
  distanceCm = duration * SOUND_SPEED / 2;

  // convert to inches
  distanceInch = distanceCm * CM_TO_INCH;

  // prints the distance in the serial monitor

  // both conditions have to be true for this.temp state of motor goes to true
  if (distanceCm < 50 && !motorRunning) {
    // prints the distance when this fn starts
    Serial.print("Distance (cm)");
    Serial.println(distanceCm);
    Serial.println("turning");
    motorRunning = true;
    motor.step(steps_per_rev * 3);
      
  }

  // same prin... as earlier. distance is > 50 and the motor state switches back to false
  if (distanceCm >= 50 && motorRunning) {
    Serial.print("Distance (cm)");
    Serial.println(distanceCm);
    Serial.println("closing");
    // state goes to false
    motorRunning = false;
    // 2.5 doesnt keep the string taught, so switch to 3 
    motor.step(-steps_per_rev * 3);

  }



}






