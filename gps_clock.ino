//------- TFT DISPLAY --------------------------
// This sketch has been Refurbished by BUHOSOFT
// IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.
//#define DEBUG
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).
// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define ORANGE  0xFBE0
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;
//--------------------------------------------
//------- DATE-TIME --------------------------
//--------------------------------------------
#include <Time.h>
#include <Timezone.h>
TimeChangeRule CEST = {"", Last, Sun, Mar, 2, 120};
TimeChangeRule CET = {"", Last, Sun, Oct, 3, 60};
Timezone CE(CEST, CET);
TimeChangeRule *tcr;
char *Tag[7] = {"SONNTAG", "MONTAG", "DIENSTAG", "MITTWOCH", "DONNERSTAG", "FREITAG", "SONNABEND"};
String Line = "";    // a string to hold incoming data
String text = "";

//Display Edges calculate
int x_edge_left = 0;
int x_edge_right;
int y_edge_up = 0;
int y_edge_down;

//Clock-Grafik
#define scale_Points 1  // 1 = active  / every 30° a point
#define scale_Circle 0  // 0 = inactive / full circle
const int clock_radius = 90;//global adjustment for the clock, default 90
const int clock_xoffset = 120;//global adjustment for the clock, default 120
const int clock_yoffset = 195;//global adjustment for the clock, default 195
float clock_point_angle_rad = 0;
int point_xpos = 0;
int point_ypos = 0;
int sec_arrow_xpos = clock_xoffset;
int sec_arrow_ypos = clock_yoffset - clock_radius;
int min_arrow_xpos = clock_xoffset;
int min_arrow_ypos = clock_yoffset - clock_radius;
int hour_arrow_xpos = clock_xoffset;
int hour_arrow_ypos = clock_yoffset - clock_radius;

int copy_sec_arrow_xpos;
int copy_sec_arrow_ypos;
int copy_min_arrow_xpos;
int copy_min_arrow_ypos;
int copy_hour_arrow_xpos;
int copy_hour_arrow_ypos;

const float pi = 3.14159265;
float sec_alfa;
float min_alfa;
float hour_alfa;
String copy_wday;
String copy_sat;
boolean valid_sync = false;
boolean valid_signal = false;
uint16_t text_color; // day / night color
uint16_t copy_text_color;

//Sunrise
int int_sunrise_hour = 0;
int int_sunrise_minute = 0;
int copy_int_sunrise_minute;
int int_sundown_hour = 0;
int int_sundown_minute = 0;
int copy_int_sundown_minute;

//Sommer Winter Time switch Calender 12 years
const int calender[12][7] = {
  {2016, 3, 27, 2, 10, 30, 3}, // 27.03.2016 > 02:00 Uhr
  {2017, 3, 26, 2, 10, 29, 3},
  {2018, 3, 25, 2, 10, 28, 3},
  {2019, 3, 31, 2, 10, 27, 3},
  {2020, 3, 29, 2, 10, 25, 3},
  {2021, 3, 28, 2, 10, 31, 3},
  {2022, 3, 27, 2, 10, 30, 3},
  {2023, 3, 26, 2, 10, 29, 3},
  {2024, 3, 31, 2, 10, 27, 3},
  {2025, 3, 30, 2, 10, 26, 3},
  {2026, 3, 29, 2, 10, 25, 3},
  {2027, 3, 28, 2, 10, 31, 3},
};
int sommer_winter_time = 1; // add hour 1=winter  2=sommer
//#########################################################################
//#########################################################################
void setup() {

  tft.reset();
  uint16_t identifier = tft.readID();
  if (identifier == 0x9325) {
  } else if (identifier == 0x9328) {
  } else if (identifier == 0x7575) {
  } else if (identifier == 0x9341) {
  } else if (identifier == 0x8357) {
  } else {
    identifier = 0x9341;
  }
  tft.begin(identifier);
  FillScreen(BLACK);
  text = String(identifier, HEX);
  ScreenText(WHITE, x_edge_left, 10 , "Chip:" + text);
  int  width = tft.width(), height = tft.height();
  ScreenText(WHITE, x_edge_left, 40 , (String(width) + "x" + String(height) + "px")); //240x320
  ScreenText(WHITE, x_edge_left, 70 , ("Set Display Edges:"));
  x_edge_right = width - 1; //set display edges
  y_edge_down =  height - 1; //set display edges
  ScreenText(WHITE, x_edge_left, 100 , ("X:" + String(x_edge_left) + "-" + String(x_edge_right) + " Y:" + String(y_edge_up) + "-" + String(y_edge_down)));
  delay(3000);
  FillScreen(BLACK);
  ScreenText(WHITE, x_edge_left, 10 , "Waiting for GPS )))");
  delay(3000);
  FillScreen(BLACK);

#if scale_Circle
  SetCircle(GREEN, clock_xoffset, clock_yoffset, clock_radius); // set clock
#endif

#if scale_Points
  for (int i = 0; i <= 11; i++) {
    clock_point_angle_rad = 30 * i * (pi / 180);
    point_xpos = (cos(clock_point_angle_rad) * clock_radius) + clock_xoffset;
    point_ypos = (sin(clock_point_angle_rad) * clock_radius) + clock_yoffset;
    SetFilledCircle(GREEN, point_xpos, point_ypos, 2);
  }
#endif

  Line.reserve(100);
  Serial.begin(9600);
}

void loop()
{ static int os = -1;

  if (!(os % 4))if (getline("$GPRMC"))RMC();
  SerialClear();

  if (second() != os) //every second
  { char wday[10];
    char date[20];
    //char tim[20];
    //sprintf(s, "%s : %02d.%02d.%04d um %02d:%02d:%02d", Tag[weekday() - 1], day(), month(), year(), hour(), minute(), second());
    sprintf(wday, "%s", Tag[weekday() - 1]);
    sprintf(date, "%02d.%02d.%04d", day(), month(), year());
    //sprintf(tim, "%02d:%02d:%02d", hour(), minute(), second());

    if (copy_wday != Tag[weekday() - 1]) {
      SetFilledRect(BLACK , x_edge_left, y_edge_up, 149, 70);
    }
    //Day and Night color
    int minutes_of_day = ((hour() * 60) + minute());
    int minutes_of_sunrise = ((int_sunrise_hour * 60) + int_sunrise_minute);
    int minutes_of_sundown = ((int_sundown_hour * 60) + int_sundown_minute);

    if ((minutes_of_day >= minutes_of_sunrise)  && (minutes_of_day <= minutes_of_sundown)) {
      text_color = WHITE;//day color
    }
    else {
      text_color = BLUE;//night clolor
    }
    if (copy_text_color != text_color) {
      valid_sync = false;//refreshh color sunrise text
      copy_text_color = text_color;
    }

    ScreenText(text_color, x_edge_left, 10 , wday);
    ScreenText(text_color, x_edge_left, 40 , date);
    //ScreenText(text_color, x_edge_left, 120 , tim);
    copy_wday = Tag[weekday() - 1];

    //Sekundenzeiger
    hour_alfa = (270 + (30 * hour() + (0.5 * minute()))) * pi / 180;
    hour_arrow_xpos = (cos(hour_alfa) * clock_radius * 0.7) + clock_xoffset;
    hour_arrow_ypos = (sin(hour_alfa) * clock_radius * 0.7) + clock_yoffset;

    min_alfa = (270 + (6 * minute())) * pi / 180;
    min_arrow_xpos = (cos(min_alfa) * clock_radius * 0.9) + clock_xoffset;
    min_arrow_ypos = (sin(min_alfa) * clock_radius * 0.9) + clock_yoffset;

    sec_alfa = (270 + (6 * second())) * pi / 180;
    sec_arrow_xpos = (cos(sec_alfa) * clock_radius * 0.9) + clock_xoffset;
    sec_arrow_ypos = (sin(sec_alfa) * clock_radius * 0.9) + clock_yoffset;

    if ((copy_hour_arrow_xpos != hour_arrow_xpos) || (copy_hour_arrow_ypos != hour_arrow_ypos)) {
      SetLines(BLACK, clock_xoffset, clock_yoffset, copy_hour_arrow_xpos, copy_hour_arrow_ypos );
    }

    if ((copy_min_arrow_xpos != min_arrow_xpos) || (copy_min_arrow_ypos != min_arrow_ypos)) {
      SetLines(BLACK, clock_xoffset, clock_yoffset, copy_min_arrow_xpos, copy_min_arrow_ypos );
    }

    SetLines(BLACK, clock_xoffset, clock_yoffset, copy_sec_arrow_xpos, copy_sec_arrow_ypos );
    SetLines(RED, clock_xoffset, clock_yoffset, hour_arrow_xpos, hour_arrow_ypos );
    copy_hour_arrow_xpos = hour_arrow_xpos;
    copy_hour_arrow_ypos = hour_arrow_ypos;

    SetLines(RED, clock_xoffset, clock_yoffset, min_arrow_xpos, min_arrow_ypos );
    copy_min_arrow_xpos = min_arrow_xpos;
    copy_min_arrow_ypos = min_arrow_ypos;

    SetLines(GREEN, clock_xoffset, clock_yoffset, sec_arrow_xpos, sec_arrow_ypos );
    copy_sec_arrow_xpos = sec_arrow_xpos;
    copy_sec_arrow_ypos = sec_arrow_ypos;

    if ((minute() == 59) && (second() == 59)) {
      valid_sync = false;
    }

    os = second();
  }//second
}//loop

void RMC() { //TIME DATE

  setTime(getparam(1).substring(0, 0 + 2).toInt(),
          getparam(1).substring(2, 2 + 2).toInt(),
          getparam(1).substring(4, 4 + 2).toInt(),
          getparam(9).substring(0, 0 + 2).toInt(),
          getparam(9).substring(2, 2 + 2).toInt(),
          getparam(9).substring(4, 4 + 2).toInt());
  time_t cet = CE.toLocal(now(), &tcr);
  setTime(cet);

  if (getparam(2) == "A") { //valid GPS-signal  A/V
    SetRect(text_color , 150, 15, 10, 10);
    SetRect(text_color , 160, 15, 10, 10);
    SetLines(text_color, 170, 20, 176, 20);
    SetCircle(text_color , 180, 20, 4);
    SetLines(text_color, 184, 20, 190, 20);
    SetRect(text_color , 190, 15, 10, 10);
    SetRect(text_color , 200, 15, 10, 10);
    valid_signal = true;
  }
  else {
    SetFilledRect(BLACK , 150, y_edge_up, 89, 40);
    valid_signal = false;
  }

  int lat = getparam(3).substring(0, 2).toInt();
  int lon = getparam(5).substring(0, 3).toInt();

  if (valid_sync == false) {
    SetFilledRect(BLACK , 150, 40, 89, 29); //clear sync on display
    //sunrise (30, 52.5, 13.5);// start sunrise calculation > result: 07:52 Uhr & 16:47 Uhr
    int day_of_year = int(((month() - 1) * 30.4) + day());
    if ((lat > 0) && (lon > 0) && (lat < 90) && (lon < 180)) {
      if (valid_signal = true) {
        sunrise (day_of_year, lat, lon);//Hamburg 53,0° 10,0°
        ScreenText(text_color, 150, 40 , "Sync");
        valid_sync = true;
      }
    }
  }
}//GPRMC

void SerialClear() {
  while (Serial.available())Serial.read();
}
//----------------------------------------------
//--------------RS232-ROUTINEN------------------
//----------------------------------------------
//String Line="";

boolean getline(char *phrase) //HARD POLLING
{ char s[100]; byte b, n; unsigned long t = millis();
  for (int i = 0; i < sizeof(s); i++)s[i] = 0;
  Line = "";
  do
  { b = Serial.read();
    if (millis() > (t + 100))return false;
  } while (b != '$');
  s[0] = b;
  n = Serial.readBytesUntil('\n', &s[1], 90);
  s[n] = 0;
  if (strstr(s, phrase) == s)
  { for (int i = 0; i < n; i++)Line += s[i];
    return true;
  }
  return false;
}

#define hex(i)  ((i<=9) ? ('0'+i): ('A'- 10 + i))

boolean checksum()
{ byte b = 0; int e;
  e = Line.indexOf('*');
  if (e > 10)
  { for (int i = 1; i < e; i++)b ^= Line[i];
    if ((hex((b & 15)) == Line[e + 2]) &&
        (hex(b / 16) == Line[e + 1]))return true;
  }
  return false;
}

String getparam(int ix)
{ int c, cc = 0;
  if (checksum())
  { do
    { c = Line.indexOf(',', cc);
      if (c >= 0)cc = c + 1; else break;
    } while (--ix);
    return (Line.substring(c + 1, Line.indexOf(',', c + 1)));
  }
  return "xx"; //debug
}
//----------------------------------------------
//--------------GRAFIK-ROUTINEN-----------------
//----------------------------------------------
unsigned long FillScreen(uint16_t color) {
  tft.fillScreen(color);
}

unsigned long ScreenText(uint16_t color, int xtpos, int ytpos, String text) {
  unsigned long start = micros();
  tft.setCursor(xtpos, ytpos);
  tft.setTextColor(color);
  tft.setTextSize(2);
  tft.println(text);
  return micros() - start;
}

unsigned long SetLines(uint16_t color , int xl1pos, int yl1pos, int xl2pos, int yl2pos) {
  unsigned long start, t;
  tft.drawLine(xl1pos, yl1pos, xl2pos, yl2pos, color);
  return micros() - start;
}

unsigned long SetPoint(uint16_t color, int xppos, int yppos) {
  unsigned long start = micros();
  tft.drawPixel(xppos, yppos, color);
  return micros() - start;
}

unsigned long SetRect(uint16_t color , int xr1pos, int yr1pos, int xr2width, int yr2hight) {
  unsigned long start, t;
  tft.drawRect(xr1pos, yr1pos, xr2width, yr2hight, color);
  return micros() - start;
}

unsigned long SetFilledRect(uint16_t color , int xr1pos, int yr1pos, int xr2width, int yr2hight) {
  unsigned long start, t;
  tft.fillRect(xr1pos, yr1pos, xr2width, yr2hight, color);
  return micros() - start;
}

unsigned long SetCircle(uint16_t color , int xcpos, int ycpos, int radius) {
  unsigned long start, t;
  tft.drawCircle(xcpos, ycpos, radius, color);
  return micros() - start;
}
unsigned long SetFilledCircle(uint16_t color , int xcpos, int ycpos, int radius) {
  unsigned long start, t;
  tft.fillCircle(xcpos, ycpos, radius, color);
  return micros() - start;
}
//----------------------------------------------
//--------------Calculation Sun-Rise------------
//----------------------------------------------
//sunrise (30, 52.5, 13.5);
void sunrise(int day_of_year, float latitude , float longitude) {
  float location;
  float declination;
  float time_diff;
  float zeit_gleichung;
  float local_rising_time;
  float local_down_time;
  float sunrising;//
  float sunrise_hour;
  float sunrise_minute;
  float sundowning;//
  float sundown_hour;
  float sundown_minute;

  calc_sommer_time();
  //Es soll der Sonnenaufgang für Berlin am 30. Januar bestimmt werden.
  //30. Januar bedeutet T = 30    Berlin liegt auf  13.5° Ost, 52.5° Nord
  //Berlin = Pi * 52.5° / 180 = 52.5°/57.29578 = 0.9163 rad (Pi=3.14159)
  location = (pi * latitude / 180);
  //Deklination der Sonne = 0.4095*sin(0.016906*(30-80.086))  = -0.30677 rad = -17.58°
  //Sonnenaufgang h=-50 Bogenminuten = -0.0145 rad
  declination = 0.4095 * sin(0.016906 * (day_of_year - 80.086));
  //Zeitdifferenz = 12*arccos((sin(-0.0145) - sin(0.9163)*sin(-0.30677)) / (cos(0.9163)*cos(-0.30677)))/Pi = 4.479 Stunden.
  time_diff = 12 * acos((sin(-0.0145) - sin(location) * sin(declination)) / (cos(location) * cos(declination))) / pi;
  //Sonnenaufgang um 12 - 4.479 = 7.521 Uhr Wahre Ortszeit.
  //Sonnenuntergang um 12 + 4.479 = 16.479 Uhr Wahre Ortszeit.
  sunrising = 12 - time_diff;
  sundowning = 12 + time_diff;
  //Zeitgleichung = -0.171*sin(0.0337*30 + 0.465) - 0.1299*sin(0.01787*30 - 0.168)
  //= -0.217 Stunden = WOZ - MOZ
  zeit_gleichung = -0.171 * sin(0.0337 * day_of_year + 0.465) - (0.1299 * sin(0.01787 * day_of_year - 0.168));
  //MOZ = WOZ + 0.217 Stunden = 7.738
  local_rising_time = sunrising + (-1 * zeit_gleichung);// = 7.738
  local_down_time = sundowning + (-1 * zeit_gleichung);// = 16,696
  //von MOZ zu MEZ:
  //für Berlin, mit +13.5° östlicher Länge (Division durch 15 erzeugt eine Zeitdifferenz in Stunden),
  //und als Zeitzone die Mitteleuropäische Zeit MEZ mit einer Korrektur von +1 Stunde (MESZ wäre +2), also:
  sunrise_hour = (local_rising_time + (-1 * longitude / 15) + sommer_winter_time);// 7.738 + (-13.5/15) +1 = 7.838 Uhr MEZ
  sundown_hour = (local_down_time + (-1 * longitude / 15) + sommer_winter_time); // 16,696 + (-13.5/15) +1 = 17.03 Uhr MEZ
  //Die Zeit 7.838 Uhr MEZ ist in Stunden. Für die Bestimmung der Anzahl Minuten nach 7 Uhr wird der nichtganzzahlige Anteil 0.838 mit 60 multipliziert: 0.838*60=50.3 Minuten

  sunrise_minute = sunrise_hour - int(sunrise_hour);
  sunrise_minute = sunrise_minute * 60; //nur Nachkomma!!
  int_sunrise_hour = int(sunrise_hour);
  int_sunrise_minute = int(sunrise_minute);

  sundown_minute = sundown_hour - int(sundown_hour);
  sundown_minute = sundown_minute * 60; //nur Nachkomma!!
  int_sundown_hour = int(sundown_hour);
  int_sundown_minute = int(sundown_minute);

  //Schlussendlich wird der Sonnenaufgang für Berlin auf 7 Uhr 50 bestimmt!
  //Ein Vergleich mit CalSky.com ergibt 7 Uhr 52 für den Sonnenaufgang und Sonnenuntergang 16 Uhr 48.
  //Das ist OK, mit so einfachen Formeln kann man keine bessere Genauigkeit erwarten.

  if (valid_sync == false) {
    SetFilledRect(BLACK , x_edge_left, 70, x_edge_right, 29); //clear sunrise on display
    SetFilledCircle(YELLOW , 220, 80, 6);
    SetLines(YELLOW , 210, 80, 230 , 80);
    SetFilledRect(BLACK , 210, 81, 230, 20);

    if (int_sunrise_minute < 10) {
      ScreenText(text_color, x_edge_left, 70 , "Sunrise: " + String(int_sunrise_hour) + ":0" + String(int_sunrise_minute));
    }
    else {
      ScreenText(text_color, x_edge_left, 70 , "Sunrise: " + String(int_sunrise_hour) + ":" + String(int_sunrise_minute));
    }
  }
  if (valid_sync == false)  {
    SetFilledRect(BLACK , x_edge_left, 300, x_edge_right, 19); //clear sunrise on display
    SetFilledCircle(ORANGE , 220, 305, 6);
    SetLines(ORANGE , 210, 305, 230 , 305);
    SetFilledRect(BLACK , 210, 290, 230, 15);

    if (int_sunrise_minute < 10) {
      ScreenText(text_color, x_edge_left, 305 , "Sunset: " + String(int_sundown_hour) + ":0" + String(int_sundown_minute));
    }
    else {
      ScreenText(text_color, x_edge_left, 305 , "Sunset: " + String(int_sundown_hour) + ":" + String(int_sundown_minute));
    }
  }
  //ScreenText(text_color, x_edge_left, 120 , String(day_of_year) + "," + String(latitude) + "," + String(longitude));
  //ScreenText(text_color, x_edge_left, 160 , String(location) + "," + String(declination)+ "," + String(time_diff)+ "," + String(zeit_gleichung)+ "," + String(local_time)+ "," + String(sunrising)+ "," + String(sunrise_hour) + "," + String(sunrise_minute));
}
//----------------------------------------------
//--------------Calender Sommer Wintertime------
//----------------------------------------------
void calc_sommer_time() {

  unsigned long minute_of_year1;
  unsigned long minute_of_year2;
  unsigned long minute_of_year3;

  for (int i = 0; i < 12 ; i++) {
    if (year() == calender[i][0]) {
      minute_of_year1 = ((month() * 30 * 1440) + (day() * 1440) + (hour() * 60) + minute());
      minute_of_year2 = ((calender[i][1] * 30 * 1440) + (calender[i][2] * 1440) + (calender[i][3] * 60));
      minute_of_year3 = ((calender[i][4] * 30 * 1440) + (calender[i][5] * 1440) + (calender[i][6] * 60));

      if ((minute_of_year1 >= minute_of_year2) && (minute_of_year1 < minute_of_year3)) {
        sommer_winter_time = 2;
      }
      else {
        sommer_winter_time = 1;
      }
    }
  }
}
