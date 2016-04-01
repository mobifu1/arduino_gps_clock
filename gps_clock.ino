//------- TFT DISPLAY --------------------------
//this code is to much for the uno board
// This sketch has been Refurbished by BUHOSOFT
// IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.
//#define DEBUG
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
//#include <TouchScreen.h>
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
#define GRAY    0x7BEF
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;
//Touch
//#define YP A3 // must be an analog pin, use "An" notation!
//#define XM A2 // must be an analog pin, use "An" notation!
//#define YM 9 // can be a digital pin
//#define XP 8 // can be a digital pin
////
//#define TS_MINX 960
//#define TS_MINY 150
//#define TS_MAXX 150
//#define TS_MAXY 940
// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
//TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
//#define MINPRESSURE 10
//#define MAXPRESSURE 1000
//--------------------------------------------
//------- DATE-TIME --------------------------
//--------------------------------------------
#include <Time.h>
#include <Timezone.h>
TimeChangeRule CEST = {"", Last, Sun, Mar, 2, 120};
TimeChangeRule CET = {"", Last, Sun, Oct, 3, 60};
Timezone CE(CEST, CET);
TimeChangeRule *tcr;
//char *Tag[7] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Sonnabend"};
char *Tag[7] = {"SONNTAG", "MONTAG", "DIENSTAG", "MITTWOCH", "DONNERSTAG", "FREITAG", "SONNABEND"};
//char *Tag[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
String Line = "";    // a string to hold incoming data

//Display Edges calculate
int x_edge_left = 0;
int x_edge_right;
int y_edge_up = 0;
int y_edge_down;

//Pin
#define LED A5// Pin of LED

//Clock-Grafik
#define scale_min_Points 1  // 1 = active  / every 6° a point
#define scale_hour_Points 1  // 1 = active  / every 30° a point
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
String copy_day;
String copy_sat;
boolean valid_sync = false;
boolean valid_signal = false;
uint16_t text_color; // day / night color
uint16_t copy_text_color;

//Sunrise
#include <sundata.h>
byte sunrise_hour = 0;
byte sunrise_minute = 0;
byte copy_sunrise_minute;
byte sundown_hour = 0;
byte sundown_minute = 0;
byte copy_sundown_minute;
byte daylightsavingtime = 1; // add hour 1=winter  2=sommer
//byte sun_elevation;//elevation of sun

////Moonphase
//const float moon_phase = 29.530589; //moon returns every 29,5 days
////Moon Phase Calender 10 years
//const int moon_calender[12][2] = {
//  {2016, 24}, // first full moon, day of year
//  {2017, 12},
//  {2018, 2},
//  {2019, 21},
//  {2020, 10},
//  {2021, 28},
//  {2022, 18},
//  {2023, 7},
//  {2024, 25},
//  {2025, 13},
//  {2026, 3},
//  {2027, 22},
//};
const String sw_version = "V1.2-Beta";
const String chip = "Chip:";
const String edges = "Set Display Edges:";
const String load_setup = "Load Setup OK";
const String wait_gps = "Waiting for GPS )))";
const String sun_info_1 = "Sunrise: ";
const String sun_info_2 = "Sunset: ";
const String sync_info = "sync";

//boolean show_data = false;//for touch
//#########################################################################
//#########################################################################
void setup() {

  Line.reserve(100);
  Serial.begin(9600);//Baudrate for GPS reciever
  //use serial monitor in Arduino IDE > Data via USB-Cable.
  //Serial.println();

  pinMode(LED, OUTPUT);
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
  ScreenText(WHITE, x_edge_left, 10 , (sw_version));
  //Serial.println(sw_version);
  ScreenText(WHITE, x_edge_left, 40 , chip + String(identifier, HEX));
  //Serial.println(chip + text);
  int  width = tft.width(), height = tft.height();
  ScreenText(WHITE, x_edge_left, 70 , (String(width) + "x" + String(height) + "px")); //240x320
  //Serial.println(String(width) + "x" + String(height) + "px");
  ScreenText(WHITE, x_edge_left, 100 , (edges));
  //Serial.println(edges);
  x_edge_right = width - 1; //set display edges
  y_edge_down =  height - 1; //set display edges
  ScreenText(WHITE, x_edge_left, 130 , ("X:" + String(x_edge_left) + "-" + String(x_edge_right) + " Y:" + String(y_edge_up) + "-" + String(y_edge_down)));
  //Serial.println("X:" + String(x_edge_left) + "-" + String(x_edge_right) + " Y:" + String(y_edge_up) + "-" + String(y_edge_down));
  delay(3000);
  FillScreen(BLACK);
  ScreenText(WHITE, x_edge_left, 10 , load_setup);
  //Serial.println(load_setup);
  ScreenText(WHITE, x_edge_left, 40 , wait_gps);
  //Serial.println(wait_gps);
  delay(3000);
  FillScreen(BLACK);

#if scale_hour_Points//every 30 degrees
  for (int i = 0; i <= 11; i++) {
    clock_point_angle_rad = 30 * i * (pi / 180);
    point_xpos = (cos(clock_point_angle_rad) * clock_radius) + clock_xoffset;
    point_ypos = (sin(clock_point_angle_rad) * clock_radius) + clock_yoffset;
    SetFilledCircle(CYAN, point_xpos, point_ypos, 2);
  }
#endif

#if scale_min_Points//every 6 degrees
  for (int i = 0; i <= 59; i++) {
    clock_point_angle_rad = 6 * i * (pi / 180);
    point_xpos = (cos(clock_point_angle_rad) * clock_radius) + clock_xoffset;
    point_ypos = (sin(clock_point_angle_rad) * clock_radius) + clock_yoffset;
    SetPoint(CYAN, point_xpos, point_ypos);
  }
#endif

  //config gps modul or use u-center
  //Serial.write("");s//et baudrate
  //Serial.write("");//save settings
}

void loop() {

  //  //touch screen
  //  TSPoint p = ts.getPoint();
  //  //if sharing pins, you'll need to fix the directions of the touchscreen pins
  //  //pinMode(XP, OUTPUT);
  //  pinMode(XM, OUTPUT);
  //  pinMode(YP, OUTPUT);
  //  //pinMode(YM, OUTPUT);
  //  // we have some minimum pressure we consider 'valid'
  //  // pressure of 0 means no pressing!
  //  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
  //    //    Serial.print("X = "); Serial.print(p.x);
  //    //    Serial.print("\tY = "); Serial.print(p.y);
  //    //    Serial.print("\tPressure = "); Serial.println(p.z);
  //    // scale from 0->1023 to tft.width
  //    p.x =  (map(p.x, TS_MINX, TS_MAXX, tft.width(), 0));
  //    p.y =  (map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));
  //    //    Serial.print("("); Serial.print(p.x);
  //    //    Serial.print(", "); Serial.print(p.y);
  //    if (p.x > 200 && p.x < 240 && p.y > 0 && p.y < 40) {
  //      delay(300);
  //      if (show_data == false) {
  //        show_data = true;//show GPS-position in raw format
  //      }
  //      else {
  //        show_data = false;
  //        SetFilledCircle(BLACK , clock_xoffset, clock_yoffset, (clock_radius * 0.9)); //clear GPS-Position
  //      }
  //    }
  //  }

  static int os = -1;

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

    if (copy_wday != Tag[weekday() - 1] || copy_day != date) {
      SetFilledRect(BLACK , x_edge_left, y_edge_up, 149, 69);
      copy_wday = Tag[weekday() - 1];
      copy_day = date;
    }
    //Day and Night color
    int minutes_of_day = ((hour() * 60) + minute());
    int minutes_of_sunrise = ((sunrise_hour * 60) + sunrise_minute);
    int minutes_of_sundown = ((sundown_hour * 60) + sundown_minute);

    if ((minutes_of_day >= minutes_of_sunrise)  && (minutes_of_day <= minutes_of_sundown)) {
      text_color = WHITE;//day color
      digitalWrite(LED, LOW);
    }
    else {
      text_color = BLUE;//night color
      digitalWrite(LED, HIGH);
    }
    if (copy_text_color != text_color) {
      valid_sync = false;//refresh color sunrise text
      copy_text_color = text_color;
    }

    ScreenText(text_color, x_edge_left + 10, 10 , wday);
    ScreenText(text_color, x_edge_left + 10, 40 , date);
    //ScreenText(text_color, x_edge_left + 10, 120 , tim);

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

    SetLines(CYAN, clock_xoffset, clock_yoffset, sec_arrow_xpos, sec_arrow_ypos );
    copy_sec_arrow_xpos = sec_arrow_xpos;
    copy_sec_arrow_ypos = sec_arrow_ypos;

    //if ((minute() == 59) && (second() == 59)) {
    if (second() == 59) {
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

  if (CE.locIsDST(cet)) { //ask for DST=Daylight saving time
    daylightsavingtime = 2; //true = 2 hour
  }
  else {
    daylightsavingtime = 1;//false = 1 hour
  }

  //  if (show_data == true) {//show GPS-Position in the middle of clock
  //    SetFilledCircle(BLACK , clock_xoffset, clock_yoffset, (clock_radius * 0.9));
  //    ScreenText(text_color, x_edge_left + 50, 170 , "N" + getparam(3));
  //    ScreenText(text_color, x_edge_left + 50, 200 , "E" + getparam(5));
  //    ScreenText(text_color, x_edge_left + 70, 230 , String(sun_elevation) + "' Elev");//Declination Sun, Range:-23,5°-0°-23,5°
  //  }

  if (getparam(2) == "A") { //valid GPS-signal  A/V
    SetRect(text_color , 150, 15, 10, 10);//satellite grafic
    SetRect(text_color , 160, 15, 10, 10);
    SetLines(text_color, 170, 20, 176, 20);
    SetCircle(text_color , 180, 20, 4);
    SetLines(text_color, 184, 20, 190, 20);
    SetRect(text_color , 190, 15, 10, 10);
    SetRect(text_color , 200, 15, 10, 10);
    valid_signal = true;
  }
  else {
    SetFilledRect(BLACK , 150, y_edge_up, 89, 40);//clear sat grafic
    valid_signal = false;
  }

  if (valid_sync == false) {
    SetFilledRect(BLACK , 150, 40, 89, 29); //clear sync on display
    if (valid_signal = true) {
      int lat = getparam(3).substring(0, 2).toInt();
      int lon = getparam(5).substring(0, 3).toInt();
      int decimal_lat = getparam(3).substring(2, 4).toInt();//decimal place
      int decimal_lon = getparam(5).substring(3, 5).toInt();//decimal place
      //int day_of_year = int(((month() - 1) * 30.4) + day());
      if ((lat > 0) && (lon > 0) && (lat < 90) && (lon < 180)) {
        //sunrise (day_of_year, lat, decimal_lat, lon, decimal_lon); //Hamburg 53,5° 10,0°
        sunrise (lat, decimal_lat, lon, decimal_lon, daylightsavingtime);
        //moon(day_of_year);
        ScreenText(text_color, 150, 40 , sync_info);
        valid_sync = true;
        //Serial.println(sync_info);
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
void sunrise( float latitude , float decimal_latitude, float longitude , float decimal_longitude, byte daylightsavingtime) {

  latitude = latitude + (decimal_latitude / 60);
  longitude = longitude + (decimal_longitude / 60);

  //sundata test=sundata(35.5,25.2,2);                       //creat test object with latitude and longtitude declared in degrees and time difference from Greenwhich
  sundata sun = sundata(latitude, longitude, daylightsavingtime);
  //test.time(2013, 4,1 , 8, 30,0);                          //insert year, month, day, hour, minutes and seconds
  sun.time( year(), month(), day(), hour(), minute(), second());
  sun.calculations();                                     //update calculations for last inserted time

  //float el_rad=sun.elevation_rad();                        //store sun's elevation in rads
  float el_deg = sun.elevation_deg();                      //store sun's elevation in degrees
  Serial.println(String(el_deg) + "Elevation");

  float az_rad = sun.azimuth_rad();                        //store sun's azimuth in rads
  float az_deg = sun.azimuth_deg();                        //store sun's azimuth in degrees
  Serial.println(String(az_deg) + "Azimuth");

  float sunrise = sun.sunrise_time();                      //store sunrise time in decimal form
  Serial.println(String(sunrise) + "Sunrise");

  float sunset = sun.sunset_time();                        //store sunset time in decimal form
  Serial.println(String(sunset) + "Sunset");

  //sun position on the clock scale
  point_xpos = (cos(az_rad) * ((clock_radius / 2) + (el_deg * 0.5))) + clock_xoffset;//0.5 = Gain Factor
  point_ypos = (sin(az_rad) * ((clock_radius / 2) + (el_deg * 0.5))) + clock_yoffset;

  SetFilledCircle(BLACK , clock_xoffset, clock_yoffset, (clock_radius * 0.9)); //clear sun position on tft
  SetCircle(GRAY, point_xpos, point_ypos, clock_radius / 2); //horizantal line
  SetFilledCircle(YELLOW, point_xpos, point_ypos, 2);// Day color
  //SetFilledCircle(GRAY, point_xpos, point_ypos, 2);//Night color

  SetFilledRect(BLACK , x_edge_left, 70, x_edge_right, 29); //clear sunrise value on display
  SetFilledCircle(YELLOW , 220, 80, 6);
  SetLines(YELLOW , 210, 80, 230 , 80);
  SetFilledRect(BLACK , 210, 81, 230, 20);
  ScreenText(text_color, x_edge_left + 10, 70 , sun_info_1 + String(sunrise));

  SetFilledRect(BLACK , x_edge_left, 300, x_edge_right, 19); //clear sunset value on display
  SetFilledCircle(ORANGE , 220, 305, 6);
  SetLines(ORANGE , 210, 305, 230 , 305);
  SetFilledRect(BLACK , 210, 290, 230, 15);
  ScreenText(text_color, x_edge_left + 10, 305 , sun_info_2 + String(sunset));
}
//----------------------------------------------
//--------------Calculation Moon-Phases---------
//----------------------------------------------
//void moon(int day_of_year) {

//  //Serial.println(String(day_of_year) + " = day_of_year");
//  for (int i = 0; i < 12 ; i++) {
//    if (year() == moon_calender[i][0]) {
//      int int_moon_phase = round(moon_phase);
//      int days_to_next_full_moon;
//      int diff;
//
//      if (day_of_year >= moon_calender[i][1]) {
//        diff = int_moon_phase + (day_of_year - moon_calender[i][1]);//72-24= 48
//        days_to_next_full_moon = int_moon_phase - (diff % int_moon_phase); // % = Modulo Operation
//      }
//      else {
//        days_to_next_full_moon = (moon_calender[i][1] - day_of_year);
//      }
//      //Serial.println(String(days_to_next_full_moon) + " = days_to_next_full_moon");
//      SetFilledRect(BLACK , x_edge_left, 100, 29, 29); // clear moon icon
//      if ((days_to_next_full_moon >= 14) && (days_to_next_full_moon <= 16)) {// new moon
//        SetCircle(GRAY , x_edge_left + 17, 107, 7);
//      }
//      if ((days_to_next_full_moon > 1) && (days_to_next_full_moon < 14)) {//day 2-13 = 1. half moon +
//        SetFilledCircle(WHITE , x_edge_left + 17, 107, 6);//11-23
//        SetFilledRect(BLACK , x_edge_left, 100, (days_to_next_full_moon + 10), 20); //12-23 /19
//        SetCircle(GRAY , x_edge_left + 17, 107, 7);
//      }
//      if ((days_to_next_full_moon == 29) || (days_to_next_full_moon == 30) || (days_to_next_full_moon == 0) || (days_to_next_full_moon == 1)) {
//        SetFilledCircle(WHITE , x_edge_left + 17, 107, 6); //full moon
//        SetCircle(GRAY , x_edge_left + 17, 107, 7);
//      }
//      if ((days_to_next_full_moon > 16) && (days_to_next_full_moon < 29)) {//day 17-28 = 2. half moon -
//        SetFilledCircle(WHITE , x_edge_left + 17, 107, 6);//11-23
//        SetFilledRect(BLACK , x_edge_left + (days_to_next_full_moon - 5 ) , 100, 13, 20); //12-23
//        SetCircle(GRAY , x_edge_left + 17, 107, 7);
//      }
//    }
//  }
//}
