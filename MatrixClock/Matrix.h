#include <Microduino_Matrix.h>
uint8_t Addr[MatrixPix_X][MatrixPix_Y] = {
  { 64, 63, 62, 61}      //点阵1x4排列
};
Matrix display = Matrix(Addr, TYPE_S2); //TYPE_COLOR or TYPE_S2

#define DATE_UPDATA 3
#define TIME_UPDATA 12

unsigned long timer[4] = {0, 0, 0, 0};

boolean changesta = false;

unsigned long msgtimer;

int32_t z = display.getWidth() * 8;

void matrixTime() { //点阵显示内容
  if (millis() < timer[0]) timer[0] = millis();
  if (millis() - timer[0] > 800) { //1 second
    getRtc();
    //display.clearDisplay();
    //display.setFontMode(MODE_H);
    if (changesta) {//显示时间
      display.setCursor(3, 1);   //x, y
      display.print(dateTime.hour / 10);
      display.setCursor(9, 1);
      display.print(dateTime.hour % 10);

      display.setCursor(15, 1);
      display.print(dateTime.second % 2 ? " " : ":");
      display.setCursor(16, 1);
      display.print(dateTime.second % 2 ? " " : ":");

      display.setCursor(19, 1);   //x, y
      display.print(dateTime.minute / 10);
      display.setCursor(25, 1);
      display.print(dateTime.minute % 10);
    }
    else {//显示日期
      display.setCursor(3, 1);   //x, y
      // display.print(year());
      if (dateTime.month < 10)
        display.print('0');
      display.print(dateTime.month);

      display.drawLine(14, 4, 17, 4);

      display.setCursor(18, 1);
      if (dateTime.day < 10)
        display.print('0');
      display.print(dateTime.day);
    }
    timer[0] = millis();
  }

  if (millis() < timer[2]) timer[2] = millis();
  if (millis() - timer[2] > ( changesta ? TIME_UPDATA * 1000 : DATE_UPDATA * 1000)) { //12 or 3 second
    changesta = !changesta;
    for (int x = 0; x < display.getWidth() * 8 * 2; x++) {
      for (int y = 0; y < display.getHeight() * 8; y++) {
        randomSeed(analogRead(A0));
        if (x < display.getWidth() * 8)
          display.setLedColor(x, y, random(0, 255), random(0, 255), random(0, 255));   //x, y, r, g, b
        else
          display.setLedColor(x - display.getWidth() * 8, y, 0, 0, 0); //x, y, r, g, b
      }
      delay(2);
    }
    display.clearDisplay();

    timer[2] = millis();
    timer[0] = millis() - 1000;
  }

  if (millis() < timer[1]) timer[1] = millis();
  if (millis() - timer[1] > 3600 * 1000) { //3600 second
    getRtc();
    timer[1] = millis();
  }
}

void msg_matrix(String _myMsg, uint8_t wait)
{
  int16_t msgl;
  msgl = _myMsg.length();
  if (millis() - msgtimer > wait)
  {
    z--;
    if (z < -(msgl * 6) - display.getWidth() * 8)
    {
      z = display.getWidth() * 8;
      display.clearDisplay();
    }
    display.setCursor(z, 0);   //x, y
    display.print(_myMsg);
    msgtimer = millis();
  }
}

void ledmatrix(int8_t x, int8_t y, int8_t z)
{
  if (z)
    display.setLed(x, y, true);
  else
    display.setLed(x, y, false);
}
