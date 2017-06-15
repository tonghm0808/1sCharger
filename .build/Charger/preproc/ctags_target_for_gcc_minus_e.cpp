# 1 "c:\\Users\\T\\Desktop\\Charger\\Charger.ino"
# 1 "c:\\Users\\T\\Desktop\\Charger\\Charger.ino"
# 2 "c:\\Users\\T\\Desktop\\Charger\\Charger.ino" 2
# 21 "c:\\Users\\T\\Desktop\\Charger\\Charger.ino"
uint16_t value = 0;
uint16_t voltage = 0;
uint16_t temp = 0;
uint16_t v1 = 0;
uint16_t v2 = 0;
uint16_t v3 = 0;
uint16_t i = 0;
bool stat = 0;

unsigned char table[10][8] = {
    {0, 0, 0, 0, 0, 0, 1, 1}, // 0
    {1, 0, 0, 1, 1, 1, 1, 1}, // 1
    {0, 0, 1, 0, 0, 1, 0, 1}, // 2
    {0, 0, 0, 0, 1, 1, 0, 1}, // 3
    {1, 0, 0, 1, 1, 0, 0, 1}, // 4
    {0, 1, 0, 0, 1, 0, 0, 1}, // 5
    {0, 1, 0, 0, 0, 0, 0, 1}, // 6
    {0, 0, 0, 1, 1, 1, 1, 1}, // 7
    {0, 0, 0, 0, 0, 0, 0, 1}, // 8
    {0, 0, 0, 0, 1, 0, 0, 1}, // 1
};

void Display(unsigned char com, unsigned char num) {
  digitalWrite(3 /* PD3*/, 0x1); // 消除余晖
  digitalWrite(4 /* PD4*/, 0x1);
  digitalWrite(5 /* PD5*/, 0x1);
  digitalWrite(6 /* PD6*/, 0x1);
  digitalWrite(7 /* PD7*/, 0x1);
  digitalWrite(8 /* PB0*/, 0x1);
  digitalWrite(9 /* PB1*/, 0x1);
  digitalWrite(10 /* PB2*/, 0x1);

  switch (com) //选通位选
  {
    case 1:
      digitalWrite(11 /* PB3*/, 0x1); //选择位1
      digitalWrite(12 /* PB4*/, 0x0);
      digitalWrite(13 /* PB5*/, 0x0);
      break;
    case 2:
      digitalWrite(11 /* PB3*/, 0x0);
      digitalWrite(12 /* PB4*/, 0x1); //选择位2
      digitalWrite(10 /* PB2*/, 0x0);
      digitalWrite(13 /* PB5*/, 0x0);
      break;
    case 3:
      digitalWrite(11 /* PB3*/, 0x0);
      digitalWrite(12 /* PB4*/, 0x0);
      digitalWrite(13 /* PB5*/, 0x1); //选择位3
      break;
    default:
      break;
  }

  digitalWrite(3 /* PD3*/, table[num][0]); // a查询码值表
  digitalWrite(4 /* PD4*/, table[num][1]);
  digitalWrite(5 /* PD5*/, table[num][2]);
  digitalWrite(6 /* PD6*/, table[num][3]);
  digitalWrite(7 /* PD7*/, table[num][4]);
  digitalWrite(8 /* PB0*/, table[num][5]);
  digitalWrite(9 /* PB1*/, table[num][6]);
}

void setup() {
  pinMode(3 /* PD3*/, 0x1); //设置为输出引脚
  pinMode(4 /* PD4*/, 0x1);
  pinMode(5 /* PD5*/, 0x1);
  pinMode(6 /* PD6*/, 0x1);
  pinMode(7 /* PD7*/, 0x1);
  pinMode(8 /* PB0*/, 0x1);
  pinMode(9 /* PB1*/, 0x1);
  pinMode(10 /* PB2*/, 0x1);

  pinMode(11 /* PB3*/, 0x1);
  pinMode(12 /* PB4*/, 0x1);
  pinMode(13 /* PB5*/, 0x1);

  pinMode(0 /* PD0*/, 0x0);
  pinMode(2 /* PD2*/, 0x0);
  pinMode(1 /* PD1*/, 0x1);

  attachInterrupt(((2 /* PD2*/) == 2 ? 0 : ((2 /* PD2*/) == 3 ? 1 : -1)), power, 1);
  wdt_enable(7);
}

void loop() {
  i++;
  value = 0;
  value = analogRead(0 /* ADC0*/);
  voltage += map(value, 0, 1023, 0, 330);
  if (i >= 36) {
    temp = (voltage / 3) + 20;
    v1 = temp / 1000;
    v2 = (temp % 1000) / 100;
    v3 = (temp % 100) / 10;

    if (digitalRead(0 /* PD0*/) == 0 && temp <= 1120) {
      stat = 0;
    } else if (digitalRead(0 /* PD0*/) == 1 && temp <= 1500) {
      stat = 0;
    } else {
    }
    digitalWrite(1 /* PD1*/, stat);
    i = 0;
    voltage = 0;
    temp = 0;
    __asm__ __volatile__ ("wdr");
  }
  Display(1, v1);
  Display(2, v2);
  Display(3, v3);
}

void power() {
  if (digitalRead(2 /* PD2*/) == 0x1) {
    stat = !stat;
  }
}
