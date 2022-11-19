#include <Keypad_I2C.h>
#include <Keypad.h>
#include <Wire.h>

#define I2CADDR 0x21  // กำหนด Address ของ I2C

const byte ROWS = 4;  // กำหนดจำนวนของ Rows
const byte COLS = 4;  // กำหนดจำนวนของ Columns

// กำหนด Key ที่ใช้งาน (4x4)
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

// กำหนด Pin ที่ใช้งาน (4x4)
byte rowPins[ROWS] = { 0, 1, 2, 3 };  // เชื่อมต่อกับ Pin แถวของปุ่มกด
byte colPins[COLS] = { 4, 5, 6, 7 };  // เชื่อมต่อกับ Pin คอลัมน์ของปุ่มกด

// makeKeymap(keys) : กำหนด Keymap
// rowPins : กำหนด Pin แถวของปุ่มกด
// colPins : กำหนด Pin คอลัมน์ของปุ่มกด
// ROWS : กำหนดจำนวนของ Rows
// COLS : กำหนดจำนวนของ Columns
// I2CADDR : กำหนด Address ขอ i2C
// PCF8574 : กำหนดเบอร์ IC
Keypad_I2C keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR, PCF8574);

// led
int led = 10;

// buzzer
int buzzer = 11;

// Black Line Follower
const int ir_l = 8;  //left Sensor
const int ir_r = 9;  //Right sensor

// motor one
//Right motor
const int enA = 2;  // PWM2
const int MotorAip1 = 3;
const int MotorAip2 = 4;

// motor two
//Left motor
const int MotorBip1 = 5;
const int MotorBip2 = 6;
const int enB = 7;  // PWM1

// motor speed
int SPEED = 255;

// counter when crossroad
int counter = 0;

// check status
bool c_status = false;

// // set millis
unsigned long startMillis;
unsigned long currentMillis;
unsigned long period;

void setup() {

  // เรียกการเชื่อมต่อ Wire
  Wire.begin();
  // เรียกกาเชื่อมต่อ
  keypad.begin(makeKeymap(keys));

  // check
  Serial.begin(9600);

  // led
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  pinMode(buzzer, OUTPUT);

  // IR sensor
  pinMode(ir_l, INPUT);
  pinMode(ir_r, INPUT);

  // left motor
  pinMode(MotorBip1, OUTPUT);
  pinMode(MotorBip2, OUTPUT);

  // right motor
  pinMode(MotorAip1, OUTPUT);
  pinMode(MotorAip2, OUTPUT);

  // speed control
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  // startMillis = millis();
}
bool folline_l = true;
bool folline_r = true;
bool wasTurnleft = false;
bool wasTurnRight = false;


void loop() {

  // สร้างตัวแปรชื่อ key ชนิด char เพื่อเก็บตัวอักขระที่กด
  char key = keypad.getKey();

  switch (key) {  //  และเลือก Relay ที่ต้องการควบคุม
    case 'A':

      Serial.println("Destination : A");

      while (c_status == false) {

        // while detect black line
        // Serial.println("Starting Driving");
        DriveMotor();

        if (counter == 1 || counter == 5) {

          // check
          Serial.println("Counter = ");
          Serial.println(counter);

          // Cross the line
          Serial.println("Crossing the line");
          Moveforward();
          delay(700);
          Serial.println("Delay 2 sec");

          // Starting moving again
          // Serial.println("Turn on IR");
          // DriveMotor();
          counter = counter + 1;
          // exit(0);
        }
        if(counter == 2 || 4 || 6){
          // Serial.println("Even counter");
          DriveMotor();
        }

        if (counter == 3) {

          // check
          Serial.println("Counter = ");
          Serial.println(counter);

          Serial.println("Backward");
          Movebackward();
          delay(1000);

          Serial.println("Turn left 3 sec");
          Turnleft();
          delay(3000);

          Serial.println("stop");
          // Stopmoving();
          Stopmoving_without_c();
          // c_status = true;

          // //// servo -------------------
          Serial.println("forward");
          Moveforward();
          delay(500);
          // //// servo -------------------


          Serial.println("Turn left 3 sec");
          Turnleft();
          delay(3000);
          // folline_l = false;

          Serial.println("forward");
          Moveforward();
          delay(600);

          // Serial.println("stop");
          // Stopmoving_without_c();
          // c_status = true;

          /**
          ** TEST
          */
          // ! use for stop testing
          // Serial.println("stop");
          // Stopmoving();
          // c_status = true;
          // ! use for stop testing

          /**
          ** error
          */
          // // Starting moving again
          // Serial.println("Turn on IR");
          // DriveMotor();

          counter = counter + 1;
          // exit(0);
        }

        // if (counter == 5) {
        //   // check
        //   Serial.println("Counter = ");
        //   Serial.println(counter);

        //   // Cross the line
        //   // Serial.println("Crossing the line");
        //   Moveforward();
        //   delay(600);

        //   // Starting moving again
        //   // Serial.println("Turn on IR");
        //   // DriveMotor();
        //   counter = counter + 1;
        //   exit(0);
        //   }

        if (counter == 7) {
          // check
          Serial.println("Counter = ");
          Serial.println(counter);

          // 360 turn for set to start
          Turnleft();
          Serial.println("Turn 360");
          delay(6000);

          //  stop moving
          Serial.println("stop");
          Stopmoving();

          // counter = counter + 1;
          Serial.print("counter = ");
          Serial.println(counter);
        }
      }

      Serial.println("Break");
      break;

    // ======================================================================================================
    case 'B':

      Serial.println("Destination : B");

      while (c_status == false) {

        // while detect black line
        // Serial.println("Starting Driving");
        DriveMotor();

        if (counter == 1) {


          // check
          Serial.println("Counter = ");
          Serial.println(counter);

          Serial.println("forward");
          Moveforward();
          delay(1000);

          Serial.println("Turn right 3 sec");
          Turnright();
          delay(3000);

          folline_r = false;

          // Starting moving again
          Serial.println("Turn on IR");
          DriveMotor();

          // delay(5000);

          // TEST
          // Serial.println("stop");
          // Stopmoving_without_c();
          // c_status = true;
        }

        if (counter == 2) {

          // check
          Serial.println("Counter = ");
          Serial.println(counter);

          Serial.println("Backward");
          Movebackward();
          delay(1000);

          Serial.println("Turn left 3 sec");
          Turnleft();
          delay(3000);

          // Serial.println("stop");
          // Stopmoving();
          Stopmoving_without_c();
          // c_status = true;

          // //// servo -------------------
          Serial.println("forward");
          Moveforward();
          delay(1000);
          // //// servo -------------------

          Serial.println("Turn left 3 sec");
          Turnleft();
          delay(3000);
          folline_l = false;

          // Starting moving again
          Serial.println("Turn on IR");
          DriveMotor();
        }

        if (counter == 3) {
          // check
          Serial.println("Counter = ");
          Serial.println(counter);

          Serial.println("Turn left 3 sec");
          Turnleft();
          delay(3000);

          folline_l = false;

          // Starting moving again
          Serial.println("Turn on IR");
          DriveMotor();
        }

        if (counter == 4) {

          // 360 turn for set to start
          Turnleft();
          Serial.println("Turn 360");
          delay(6000);

          //  start moving again
          Serial.println("stop");
          Stopmoving();

          // counter = counter + 1;
          Serial.print("counter = ");
          Serial.println(counter);
        }
      }


      Serial.println("Break");
      break;

    case 'C':
      Serial.println("Destination : C");

      while (c_status == false) {

        // while detect black line
        // Serial.println("Starting Driving");
        DriveMotor();
      }
      break;
  }
}

void DriveMotor() {

  // Moveforward
  // if ir are light
  if (digitalRead(ir_l) == HIGH && digitalRead(ir_r) == HIGH) {

    // follow line left = false
    // Turn left
    // if (folline_l == false) {
    //   Turnleft();
    //   wasTurnleft = true;
    // }

    // follow line right = false
    // Turn right
    // else if (folline_r == false) {
    //   Turnright();
    //   wasTurnRight = true;
    // }

    // if not move forward
    // else {
    // Serial.println("Moveforward");
    Moveforward();
    // }
  }

  // Turn left
  // ir left is off
  if (digitalRead(ir_l) == LOW && digitalRead(ir_r) == HIGH) {
    Serial.println("Turnleft");
    Turnleft();

    // reset follow right line
    // if (wasTurnRight == true) {
    //   wasTurnRight = false;
    //   folline_r = true;
    // }
  }

  //  Turn right
  // ir right is off
  if (digitalRead(ir_l) == HIGH && digitalRead(ir_r) == LOW) {
    Serial.println("Turnright");
    Turnright();

    // reset follow right line
    // if (wasTurnleft == true) {
    //   wasTurnleft = false;
    //   folline_l = true;
    // }
  }

  // ir light are off
  // stop moving
  if (digitalRead(ir_l) == LOW && digitalRead(ir_r) == LOW) {
    Serial.println("Stop");
    Stopmoving();
  }

  // check msg when finish
  if (c_status == true) {
    Serial.print("Finish status = ");
    Serial.println(c_status);
  }
}

void Moveforward() {

  // led off
  digitalWrite(led, LOW);

  //Move both the Motors
  digitalWrite(MotorAip1, HIGH);
  digitalWrite(MotorAip2, LOW);
  digitalWrite(MotorBip1, HIGH);
  digitalWrite(MotorBip2, LOW);
  analogWrite(enA, SPEED);
  analogWrite(enB, SPEED);
}

void Movebackward() {
  //Move both the Motors
  digitalWrite(MotorAip1, LOW);
  digitalWrite(MotorAip2, HIGH);
  digitalWrite(MotorBip1, LOW);
  digitalWrite(MotorBip2, HIGH);
  analogWrite(enA, SPEED);
  analogWrite(enB, SPEED);
}

void Turnright() {
  // Tilt robot towards right by stopping the right wheel and moving the left one
  // If I want to turn right then the speed of the right wheel should be less than that of the left wheel, here, let a be the right wheel
  digitalWrite(MotorAip1, HIGH);
  digitalWrite(MotorAip2, LOW);
  digitalWrite(MotorBip1, LOW);
  digitalWrite(MotorBip2, HIGH);
  analogWrite(enA, SPEED);
  analogWrite(enB, SPEED);
  delay(100);
}

void Turnleft() {
  // Tilt robot towards left by stopping the left wheel and moving the right one
  digitalWrite(MotorAip1, LOW);
  digitalWrite(MotorAip2, HIGH);
  digitalWrite(MotorBip1, HIGH);
  digitalWrite(MotorBip2, LOW);
  analogWrite(enA, SPEED);
  analogWrite(enB, SPEED);
  delay(100);
}

// ! TEST FUNCTION
void Turnleft3s() {
  // Tilt robot towards left by stopping the left wheel and moving the right one
  digitalWrite(MotorAip1, LOW);
  digitalWrite(MotorAip2, HIGH);
  digitalWrite(MotorBip1, HIGH);
  digitalWrite(MotorBip2, LOW);
  analogWrite(enA, SPEED);
  analogWrite(enB, SPEED);
  delay(3000);
  exit(0);
}

void Stopmoving_without_c() {
  //Stop both Motors
  digitalWrite(MotorAip1, LOW);
  digitalWrite(MotorAip2, LOW);
  digitalWrite(MotorBip1, LOW);
  digitalWrite(MotorBip2, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  digitalWrite(led, HIGH);
}
void Stopmoving() {

  Serial.print("STOP FUNC ------");
  Serial.println("delay 1 sec");
  delay(1000);

  //Stop both Motors
  digitalWrite(MotorAip1, LOW);
  digitalWrite(MotorAip2, LOW);
  digitalWrite(MotorBip1, LOW);
  digitalWrite(MotorBip2, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);

  // increse counter
  counter = counter + 1;

  // turn on led
  digitalWrite(led, HIGH);

  // // turn on buzzer
  // tone(buzzer, 2000,1000);
  // Serial.println("BUZZERRRRRRRRRRR");
  // delay(500);
  // noTone(buzzer);

  Serial.print("Counter = ");
  Serial.println(counter);

  // when counter = 5
  // led off
  // finish status == true turn off switch case
  if (counter == 8) {
    c_status = true;
    digitalWrite(led, LOW);
  }
  

  return counter;
}