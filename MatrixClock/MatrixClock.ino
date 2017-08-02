#include "rtc.h"
#include "Matrix.h"
#include "BLEJson.h"

void setup() {
  BLE_Serial.begin(57600);

  Wire.begin();
  getRtc();
  //delay(5000);
  display.setBrightness(255);
  display.clearDisplay();//清理点阵显示
}

void loop() {
  readserail();

  if (modeC != mode)
  {
    display.clearDisplay();
    modeC = mode;
    z = display.getWidth() * 8;
  }

  switch (mode)
  {
    case MODETIME:
      matrixTime();
      break;
    case MODEMATRIX:
      if (changeM)
        ledmatrix(8 * num + mymatrix[0], mymatrix[1], mymatrix[2]);
      break;
    case MODEMSG:
      msg_matrix(myMsg, 100);
      break;
    default:
      break;
  }
}
