#include<Wire.h>

const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

char output_char = '0';

const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output

#define USE_MPU6050
#define USE_JOYSTICK

char last_cmd = '0';

int buzzer = 12;

int incomingByte = 0;

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);

  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);

}

void loop() {
  // read from the Serial port:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    Serial.println(incomingByte);

    if (incomingByte == 'E') {
      // flip buzzer
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      delay(1);
    }
  }

  //Serial.print("Switch:  ");
  //Serial.print(digitalRead(SW_pin));
  //Serial.print("\n");
  //Serial.print("X-axis: ");
  //Serial.print(analogRead(X_pin));
  //Serial.print("\n");
  //Serial.print("Y-axis: ");
  //Serial.println(analogRead(Y_pin));
  //Serial.print("\n\n");
  //delay(500);

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  //Serial.print(" | AcX = "); Serial.println(AcX);
  //Serial.print(" | AcY = "); Serial.println(AcY);
  //Serial.print(" | AcZ = "); Serial.println(AcZ);
  //Serial.print(" | Tmp = "); Serial.println(Tmp / 340.00 + 36.53); //equation for temperature in degrees C from datasheet
  //Serial.print(" | GyX = "); Serial.println(GyX);
  //Serial.print(" | GyY = "); Serial.println(GyY);
  //Serial.print(" | GyZ = "); Serial.println(GyZ);
  //Serial.println();
  delay(333);

  if (analogRead(X_pin) < 100 || AcX > 5000) {
    Serial.print("a");
    //Serial.print("\n");
  }
  if (analogRead(X_pin) > 1000 || AcX < -5000) {
    Serial.print("d");
    //Serial.print("\n");
  }
  if (analogRead(Y_pin) < 100 || AcY < -4000) {
    Serial.print("w");
    //Serial.print("\n");
  }
  if (analogRead(Y_pin) > 1000 || AcY > 4000) {
    Serial.print("s");
    //Serial.print("\n");
  }
}
