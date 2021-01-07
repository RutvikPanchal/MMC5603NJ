//  S -> SET
//  R -> RESET
//  1 - 8 -> select I2C Bus
//  D -> Debug output
//  J -> JSON Output

#include <Wire.h>

char input;
char Serialflag = 'J';
int businput = 2;
byte I2C_ACK;
byte I2C_Data[9];
long I2C_ValueTemp;
long I2C_ValueX, I2C_ValueY, I2C_ValueZ;
float I2C_ValuefX, I2C_ValuefY, I2C_ValuefZ;
float I2C_ValuefTemp;

String Data;

void TCA9584(int bus){
  Wire.beginTransmission(0x70);
  Wire.write(1 << bus);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  TCA9584(businput);
  
  input = Serial.read();
  switch(input){
    case 'S':
      Wire.beginTransmission(0x30);
        Wire.write(0x1B);
        Wire.write(0x08); //SET
      I2C_ACK = Wire.endTransmission();
      delay(500);
      break;
    case 'R':
      Wire.beginTransmission(0x30);
        Wire.write(0x1B);
        Wire.write(0x10); //RESET
      I2C_ACK = Wire.endTransmission();
      delay(500);
      break;
    case 'D':
      Serialflag = 'D';
      break;
    case 'J':
      Serialflag = 'J';
      break;
    case '0':
      businput = 0;
      break;
    case '1':
      businput = 1;
      break;
    case '2':
      businput = 2;
      break;
    case '3':
      businput = 3;
      break;
    case '4':
      businput = 4;
      break;
    case '5':
      businput = 5;
      break;
    case '6':
      businput = 6;
      break;
    case '7':
      businput = 7;
      break;
  }
  
  Wire.beginTransmission(0x30);
    Wire.write(0x1B);
    Wire.write(0x01);
  I2C_ACK = Wire.endTransmission();
  Wire.beginTransmission(0x30);
    Wire.write(byte(0x00));
  I2C_ACK = Wire.endTransmission();
    Wire.requestFrom(0x30, 9);
      for(int i = 0; i < 9; i++){
        I2C_Data[i] = Wire.read();
      }
      I2C_ValueX = 0;
      I2C_ValueY = 0;
      I2C_ValueZ = 0;
      for(int i = 7; i >= 0; i--){
        I2C_ValueX += pow(2, 12 + i) * bitRead(I2C_Data[0], i);
        I2C_ValueY += pow(2, 12 + i) * bitRead(I2C_Data[2], i);
        I2C_ValueZ += pow(2, 12 + i) * bitRead(I2C_Data[4], i);
      }
      for(int i = 7; i >= 0; i--){
        I2C_ValueX += pow(2, 4 + i) * bitRead(I2C_Data[1], i);
        I2C_ValueY += pow(2, 4 + i) * bitRead(I2C_Data[3], i);
        I2C_ValueZ += pow(2, 4 + i) * bitRead(I2C_Data[5], i);
      }
      for(int i = 7; i >= 4; i--){
        I2C_ValueX += pow(2, i) * bitRead(I2C_Data[6], i);
        I2C_ValueY += pow(2, i) * bitRead(I2C_Data[7], i);
        I2C_ValueZ += pow(2, i) * bitRead(I2C_Data[8], i);
      }
      I2C_ValuefX = (I2C_ValueX - 524288)*0.00625;
      I2C_ValuefY = (I2C_ValueY - 524288)*0.00625;
      I2C_ValuefZ = (I2C_ValueZ - 524288)*0.00625;
      switch(Serialflag){
        case 'D':
          Serial.print("X:");
          Serial.print(I2C_ValuefX);
          Serial.print(",");
          Serial.print("Y:");
          Serial.print(I2C_ValuefY);
          Serial.print(",");
          Serial.print("Z:");
          Serial.print(I2C_ValuefZ);
          Serial.print(",");
          Serial.print("X+Y+Z:");
          Serial.println(I2C_ValuefX + I2C_ValuefY + I2C_ValuefZ);
          break;
        case 'J':
          Data = "{\"adc0\": " + String(I2C_ValuefX) + ", \"adc1\": " + String(I2C_ValuefY) + ", \"adc2\": " + String(I2C_ValuefZ) + ", \"adc3\": " + String(I2C_ValuefX + I2C_ValuefY + I2C_ValuefZ) + "}";
          Serial.println(Data);
          break;
      }
  I2C_ACK = Wire.endTransmission();
  delay(5);
}