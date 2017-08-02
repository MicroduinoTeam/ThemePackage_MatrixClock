/*
  {"ID":"project"}
  {"Time":"18:30:30"}
  {"Date":"2017-3-31"}
  {"number":"1"}
  {"point":"111"}
  {"Mode":"0"}
  {"msg":"mCookie Microduino mPuzzle mPie"}
*/
#define BLE_Serial Serial

#define MODETIME   0
#define MODEMATRIX 1
#define MODEMSG    2

String inputCMD = "";
String inputValue = "";
String outputValue = "";
char inByte;

int readValue = 0, readdata = 0;
boolean jsonready, jsonname;
unsigned long times;

#define timeout 300

char mId[10];

String mname[7] = {"ID", "Time", "Date", "number" , "point", "Mode", "msg"};

String ID = "{\"ID\":\"594744f110f74e000164bbdf\"}";

String myMsg = "Hello mCookie!";
String myMsgC = "";

int8_t mytime[3] = {0, 0, 0};
int mydate[3] = {0, 0, 0};
int8_t mymatrix[3] = {0, 0, 0};
int8_t mode, modeC;
int16_t num, numC;
int16_t point;

boolean changeM;

void IDupload()
{
  BLE_Serial.print(ID.substring(0, 17));
  delay(100);
  BLE_Serial.println(ID.substring(17, ID.length()));
  delay(100);
}

void uploaddata(String _st, int _data)
{
  String send_data;
  int number;
  send_data = "{\"";
  send_data += _st;
  send_data += "\":\"";
  send_data += _data;
  send_data += "\"\}";
  number = send_data.length() / 17;

  if (number == 0)
  {
    BLE_Serial.println(send_data);
    delay(30);
  }
  else
  {
    while (number >= 0)
    {
      BLE_Serial.print(send_data.substring(0, 17));
      send_data = send_data.substring(17, send_data.length());
      delay(30);
      number--;
    }
    BLE_Serial.print("\n");
  }
}

void solutiondata(String _data)
{
  if (_data == mname[0])
  {
    sscanf(inputCMD.c_str(), "ID:%s", &mId);
    if (String(mId) == "project")
    {
      IDupload();
      uploaddata("number", num);
    }
  }
  else if (_data == mname[1])
  {
    sscanf(inputCMD.c_str(), "Time:%d:%d:%d", &mytime[0], &mytime[1], &mytime[2]);
    SetTime(mytime[0], mytime[1] , mytime[2]);
    display.clearDisplay();
    delay(100);
    mode = MODETIME;
  }
  else if (_data == mname[2])
  {
    sscanf(inputCMD.c_str(), "Date:%d-%d-%d", &mydate[0], &mydate[1], &mydate[2]);
    SetDate(mydate[0], mydate[1] , mydate[2]);
    display.clearDisplay();
    delay(100);
    mode = MODETIME;
  }
  else if (_data == mname[3])
  {
    sscanf(inputCMD.c_str(), "number:%d", &num);
    if (numC != num)
    {
      changeM = false;
      numC = num;
    }
  }
  else if (_data == mname[4])
  {
    if (inputCMD != "point:-1")
      sscanf(inputCMD.c_str(), "point:%d", &point);
    mymatrix[0] = point / 100;
    mymatrix[1] = point / 10 % 10;
    mymatrix[2] = point % 10;
    mode = MODEMATRIX;
    changeM = true;
  }
  else if (_data == mname[5])
  {
    if (inputCMD != "Mode:-1")
      sscanf(inputCMD.c_str(), "Mode:%d", &mode);
  }
  else if (_data == mname[6])
  {
    mode = MODEMSG;
    myMsg = inputCMD.substring(4, inputCMD.length());
  }
}

void readserail()
{
  if (BLE_Serial.available() > 0) {
    inByte = BLE_Serial.read();
    if (inByte == '{')
      readValue++;
    else if (inByte == '}')
      readValue--;
    else if (readValue > 0 && inByte != '"')
    {
      jsonready = true;
      inputCMD += inByte;
      if (inByte == ':')
        jsonname = true;
      if (!jsonname)
        inputValue += inByte;
    }
    if (jsonready && readValue == 0)
    {
      //Serial.println("--------Json ok---------");
      //Serial.println(inputCMD);
      //Serial.println("------------------------");
      solutiondata(inputValue);
      //Serial.println("------------------------");
      inputCMD = "";
      inputValue = "";
      jsonready = false;
      jsonname = false;
    }
    times = millis();
  }
  if (millis() - times > timeout && readValue > 0)
  {
    readValue = 0;
    inputCMD = "";
    inputValue = "";
    jsonready = false;
    jsonname = false;
    //Serial.println("--------Json fail---------");
  }
}
