#include <avr/wdt.h>

#define SEG_A 3    // PD3
#define SEG_B 4    // PD4
#define SEG_C 5    // PD5
#define SEG_D 6    // PD6
#define SEG_E 7    // PD7
#define SEG_F 8    // PB0
#define SEG_G 9    // PB1
#define SEG_DP 10  // PB2

#define COM1 11  // PB3
#define COM2 12  // PB4
#define COM3 13  // PB5

#define button 2      // PD2
#define adcPin 0      // ADC0
#define cellsPin 0    // PD0
#define controlPin 1  // PD1

uint16_t value = 0;
uint16_t voltage = 0;
uint16_t temp = 0;
uint16_t v1 = 0;
uint16_t v2 = 0;
uint16_t v3 = 0;
uint16_t i = 0;
bool stat = 0;

//数码管码表
unsigned char table[10][8] = {
    {0, 0, 0, 0, 0, 0, 1, 1},  // 0
    {1, 0, 0, 1, 1, 1, 1, 1},  // 1
    {0, 0, 1, 0, 0, 1, 0, 1},  // 2
    {0, 0, 0, 0, 1, 1, 0, 1},  // 3
    {1, 0, 0, 1, 1, 0, 0, 1},  // 4
    {0, 1, 0, 0, 1, 0, 0, 1},  // 5
    {0, 1, 0, 0, 0, 0, 0, 1},  // 6
    {0, 0, 0, 1, 1, 1, 1, 1},  // 7
    {0, 0, 0, 0, 0, 0, 0, 1},  // 8
    {0, 0, 0, 0, 1, 0, 0, 1},  // 1
};

void Display(unsigned char com, unsigned char num) {
  digitalWrite(SEG_A, HIGH);  // 消除余晖
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, HIGH);
  digitalWrite(SEG_E, HIGH);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, HIGH);
  digitalWrite(SEG_DP, HIGH);

  switch (com)  //选通位选
  {
    case 1:
      digitalWrite(COM1, HIGH);  //选择位1
      digitalWrite(COM2, LOW);
      digitalWrite(COM3, LOW);
      break;
    case 2:
      digitalWrite(COM1, LOW);
      digitalWrite(COM2, HIGH);  //选择位2
      digitalWrite(SEG_DP, LOW);
      digitalWrite(COM3, LOW);
      break;
    case 3:
      digitalWrite(COM1, LOW);
      digitalWrite(COM2, LOW);
      digitalWrite(COM3, HIGH);  //选择位3
      break;
    default:
      break;
  }

  digitalWrite(SEG_A, table[num][0]);  // a查询码值表
  digitalWrite(SEG_B, table[num][1]);
  digitalWrite(SEG_C, table[num][2]);
  digitalWrite(SEG_D, table[num][3]);
  digitalWrite(SEG_E, table[num][4]);
  digitalWrite(SEG_F, table[num][5]);
  digitalWrite(SEG_G, table[num][6]);
}

void setup() {
  pinMode(SEG_A, OUTPUT);  //设置为输出引脚
  pinMode(SEG_B, OUTPUT);
  pinMode(SEG_C, OUTPUT);
  pinMode(SEG_D, OUTPUT);
  pinMode(SEG_E, OUTPUT);
  pinMode(SEG_F, OUTPUT);
  pinMode(SEG_G, OUTPUT);
  pinMode(SEG_DP, OUTPUT);

  pinMode(COM1, OUTPUT);
  pinMode(COM2, OUTPUT);
  pinMode(COM3, OUTPUT);

  pinMode(cellsPin, INPUT);
  pinMode(button, INPUT);
  pinMode(controlPin, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(button), power, CHANGE);
  wdt_enable(WDTO_2S);
}

void loop() {
  i++;
  value = 0;
  value = analogRead(adcPin);
  voltage += map(value, 0, 1023, 0, 330);
  if (i >= 36) {
    temp = (voltage / 3) + 20;
    v1 = temp / 1000;
    v2 = (temp % 1000) / 100;
    v3 = (temp % 100) / 10;

    if (digitalRead(cellsPin) == 0 && temp <= 1120) {
      stat = 0;
    } else if (digitalRead(cellsPin) == 1 && temp <= 1500) {
      stat = 0;
    } else {
    }
    digitalWrite(controlPin, stat);
    i = 0;
    voltage = 0;
    temp = 0;
    wdt_reset();
  }
  Display(1, v1);
  Display(2, v2);
  Display(3, v3);
}

void power() {
  if (digitalRead(button) == HIGH) {
    stat = !stat;
  }
}