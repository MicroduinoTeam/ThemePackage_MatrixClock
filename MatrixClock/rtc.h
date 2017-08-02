#include <Microduino_RTC.h>

RTC rtc;//初始化实时时钟
DateTime dateTime;

void getRtc() {
  rtc.getDateTime(&dateTime);
}


void SetDate(uint16_t _year, uint8_t _month, uint8_t _day)
{
  rtc.clearAll();  //set a time to start with.
  rtc.setDate( _year - 2000, _month, 0,_day); // century(1=1900, 0=2000), year(0-99), month, weekday,day
}

void SetTime(uint8_t _hour, uint8_t _minute, uint8_t _second)
{
  rtc.clearAll();  //set a time to start with.
  rtc.setTime(_hour, _minute , _second); //hr, min, sec
}
