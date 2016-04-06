//------- TFT DISPLAY --------------------------
// This sketch has been Refurbished by BUHOSOFT
// IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.
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
#define GRAY    0x7BEF
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
float alfa;
String copy_wday;
String copy_day;
String copy_sat;
boolean valid_sync = false;
boolean valid_signal = false;
uint16_t text_color; // day / night color
uint16_t copy_text_color;

//GPS Position
int lat ;
int lon;
int minute_lat;
int minute_lon;
int day_of_year;

//Sunrise
#include <sundata.h>
int sunrise_hour = 0;
int sunrise_minute = 0;
int sundown_hour = 0;
int sundown_minute = 0;
int daylightsavingtime = 1; // add hour 1=winter  2=sommer
int copy_sun_point_xpos;
int copy_sun_point_ypos;
boolean daylight;
float el_deg;
float az_rad;

//Moonphase
const float moon_phase = 29.530589; //moon returns every 29,5 days
//Moon Phase Calender 10 years
const int moon_calender[12][2] = {
  {2016, 24}, // first full moon, day of year
  {2017, 12},
  {2018, 2},
  {2019, 21},
  {2020, 10},
  {2021, 28},
  {2022, 18},
  {2023, 7},
  {2024, 25},
  {2025, 13},
  {2026, 3},
  {2027, 22},
};
byte const moon_x_pos = 20;//24
byte const moon_y_pos = 114;
byte const moon_radius = 15;
int copy_moon_point_xpos;
int copy_moon_point_ypos;
//------------------------------------
const String sw_version = "V1.7-Beta";
//const String chip = "Chip:";
//const String edges = "Set Display Edges:";
//const String load_setup = "Load Setup OK";
//const String wait_gps = "Waiting for GPS )))";
const String sun_info_1 = "Sunrise: ";
const String sun_info_2 = "Sunset: ";
const String sync_info = "sync";

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
  //ScreenText(WHITE, x_edge_left, 40 , chip + String(identifier, HEX));
  //Serial.println(chip + text);
  int  width = tft.width(), height = tft.height();
  //ScreenText(WHITE, x_edge_left, 70 , (String(width) + "px" + String(height) + "px")); //240x320
  //Serial.println(String(width) + "x" + String(height) + "px");
  //ScreenText(WHITE, x_edge_left, 100 , (edges));
  //Serial.println(edges);
  x_edge_right = width - 1; //set display edges
  y_edge_down =  height - 1; //set display edges
  //ScreenText(WHITE, x_edge_left, 130 , ("X:" + String(x_edge_left) + "-" + String(x_edge_right) + " Y:" + String(y_edge_up) + "-" + String(y_edge_down)));
  //Serial.println("X:" + String(x_edge_left) + "-" + String(x_edge_right) + " Y:" + String(y_edge_up) + "-" + String(y_edge_down));
  //delay(3000);
  //FillScreen(BLACK);
  //ScreenText(WHITE, x_edge_left, 10 , load_setup);
  //Serial.println(load_setup);
  //ScreenText(WHITE, x_edge_left, 40 , wait_gps);
  //Serial.println(wait_gps);
  delay(3000);
  FillScreen(BLACK);

#if scale_hour_Points//every 30 degrees
  for (int i = 0; i <= 11; i++) {
    alfa = 30 * i * (pi / 180);
    point_xpos = (cos(alfa) * clock_radius) + clock_xoffset;
    point_ypos = (sin(alfa) * clock_radius) + clock_yoffset;
    SetFilledCircle(CYAN, point_xpos, point_ypos, 2);
  }
#endif

#if scale_min_Points//every 6 degrees
  for (int i = 0; i <= 59; i++) {
    alfa = 6 * i * (pi / 180);
    point_xpos = (cos(alfa) * clock_radius) + clock_xoffset;
    point_ypos = (sin(alfa) * clock_radius) + clock_yoffset;
    SetPoint(CYAN, point_xpos, point_ypos);
  }
#endif

  //config gps modul or use u-center
  //Serial.write("");//set baudrate
  //Serial.write("");//save settings
}

void loop() {

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
      daylight = true;
    }
    else {
      text_color = BLUE;//night color
      digitalWrite(LED, HIGH);
      daylight = false;
    }
    if (copy_text_color != text_color) {
      valid_sync = false;//refresh color sunrise text
      copy_text_color = text_color;
    }

    ScreenText(text_color, x_edge_left + 10, 10 , wday);
    ScreenText(text_color, x_edge_left + 10, 40 , date);
    //ScreenText(text_color, x_edge_left + 10, 120 , tim);

    //Sekundenzeiger
    alfa = (270 + (30 * hour() + (0.5 * minute()))) * pi / 180;
    hour_arrow_xpos = (cos(alfa) * clock_radius * 0.7) + clock_xoffset;
    hour_arrow_ypos = (sin(alfa) * clock_radius * 0.7) + clock_yoffset;

    alfa = (270 + (6 * minute())) * pi / 180;
    min_arrow_xpos = (cos(alfa) * clock_radius * 0.9) + clock_xoffset;
    min_arrow_ypos = (sin(alfa) * clock_radius * 0.9) + clock_yoffset;

    alfa = (270 + (6 * second())) * pi / 180;
    sec_arrow_xpos = (cos(alfa) * clock_radius * 0.9) + clock_xoffset;
    sec_arrow_ypos = (sin(alfa) * clock_radius * 0.9) + clock_yoffset;

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

    if ((minute() == 59) && (second() == 59)) {
      valid_sync = false;
    }

    if ((second() == 31) || (second() == 1)) {
      if (valid_signal = true) {
        if ((lat > 0) && (lon > 0) && (lat < 90) && (lon < 180)) {
          sunrise (lat, minute_lat, lon, minute_lon, daylightsavingtime);//Hamburg 53,5° 10,0°
          moon(hour(), minute());
        }
      }
    }

    os = second();
  }//second
}//loop

void RMC() { //TIME DATE

  //$GPRMC,121000,A,4735.5634,N,00739.3538,E,0.0,0.0,060416,0.4,E,A*19
  //setTime(12, 10, 0, 6, 4, 16);
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
      lat = getparam(3).substring(0, 2).toInt();
      lon = getparam(5).substring(0, 3).toInt();
      minute_lat = getparam(3).substring(2, 4).toInt();//minute value
      minute_lon = getparam(5).substring(3, 5).toInt();//minute value
      if ((lat > 0) && (lon > 0) && (lat < 90) && (lon < 180)) {
        sunrise (lat, minute_lat, lon, minute_lon, daylightsavingtime);//Hamburg 53,5° 10,0°
        moon(hour(), minute());
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
void sunrise( float latitude , float minute_latitude, float longitude , float minute_longitude, int daylightsavingtime) {

  latitude = latitude + (minute_latitude / 60);
  longitude = longitude + (minute_longitude / 60);

  sundata sun = sundata(latitude, longitude, daylightsavingtime);//creat object with latitude and longtitude declared in degrees and time difference from Greenwhich
  sun.time( year(), month(), day(), hour(), minute(), second());//insert year, month, day, hour, minutes and seconds
  sun.calculations();                                           //update calculations for last inserted time

  day_of_year = sun.day_of_year();

  //float el_rad=sun.elevation_rad();                        //store sun's elevation in rads
  el_deg = sun.elevation_deg();                      //store sun's elevation in degrees
  //Serial.println(String(el_deg) + "Elevation");

  az_rad = sun.azimuth_rad();                        //store sun's azimuth in rads
  //float az_deg = sun.azimuth_deg();                        //store sun's azimuth in degrees
  //Serial.println(String(az_deg) + "Azimuth");

  float sunrise = sun.sunrise_time();                      //store sunrise time in decimal form
  //Serial.println(String(sunrise) + "Sunrise");
  sunrise_hour = int(sunrise);
  sunrise_minute = int((sunrise - sunrise_hour) * 60);

  float sunset = sun.sunset_time();                        //store sunset time in decimal form
  //Serial.println(String(sunset) + "Sunset");
  sundown_hour = int(sunset);
  sundown_minute = int((sunset - sundown_hour) * 60);

  //sun position on the clock scale
  int sun_point_xpos = int(cos(az_rad + 4.712388) * ((clock_radius / 2) + (el_deg * 0.5))) + clock_xoffset; //0.5 = Gain Factor
  int sun_point_ypos = int(sin(az_rad + 4.712388) * ((clock_radius / 2) + (el_deg * 0.5))) + clock_yoffset;

  SetFilledCircle(BLACK, copy_sun_point_xpos, copy_sun_point_ypos, 2);//clear sun icon
  SetCircle(GRAY, clock_xoffset, clock_yoffset, clock_radius / 2); //horizon
  SetPoint(CYAN, clock_xoffset, clock_yoffset + (clock_radius / 2) + ((90 - latitude + 23.45) * 0.5)); //Scale highest sommer elevation, 90 - lat +23.45
  SetPoint(CYAN, clock_xoffset, clock_yoffset + (clock_radius / 2) + ((90 - latitude) * 0.5));         //Scale Spring Point elevation, 90 - lat
  SetPoint(CYAN, clock_xoffset, clock_yoffset + (clock_radius / 2) + ((90 - latitude - 23.45) * 0.5)); //Scale highest Winter elevation, 90 - lat -23.45

  if (daylight == true) {
    if (el_deg < 4) {
      SetFilledCircle(ORANGE, sun_point_xpos, sun_point_ypos, 2);// Day color 1
    }
    else {
      SetFilledCircle(YELLOW, sun_point_xpos, sun_point_ypos, 2);// Day color 2
    }
  }
  else {
    SetFilledCircle(BLUE, sun_point_xpos, sun_point_ypos, 2);//Night color
  }
  copy_sun_point_xpos = sun_point_xpos;
  copy_sun_point_ypos = sun_point_ypos;

  if (valid_sync == false) {

    SetFilledRect(BLACK , x_edge_left, 70, x_edge_right, 29); //clear sunrise value on display
    SetFilledCircle(YELLOW , 220, 80, 6);
    SetLines(YELLOW , 210, 80, 230 , 80);
    SetFilledRect(BLACK , 210, 81, 230, 20);

    if (sunrise_minute < 10) {
      ScreenText(text_color, x_edge_left + 10, 70 , sun_info_1 + String(sunrise_hour) + ":0" + String(sunrise_minute));
    }
    else {
      ScreenText(text_color, x_edge_left + 10, 70 , sun_info_1 + String(sunrise_hour) + ":" + String(sunrise_minute));
    }

    SetFilledRect(BLACK , x_edge_left, 300, x_edge_right, 19); //clear sunset value on display
    SetFilledCircle(ORANGE , 220, 305, 6);
    SetLines(ORANGE , 210, 305, 230 , 305);
    SetFilledRect(BLACK , 210, 290, 230, 15);

    if (sundown_minute < 10) {
      ScreenText(text_color, x_edge_left + 10, 305 , sun_info_2 + String(sundown_hour) + ":0" + String(sundown_minute));
    }
    else {
      ScreenText(text_color, x_edge_left + 10, 305 , sun_info_2 + String(sundown_hour) + ":" + String(sundown_minute));
    }
  }
}
//----------------------------------------------
//--------------Calculation Moon-Phases---------
//----------------------------------------------
void moon(int now_hour, int now_minute) {

  for (int i = 0; i < 12 ; i++) {
    if (year() == moon_calender[i][0]) {
      int int_moon_phase = round(moon_phase);
      int days_to_next_full_moon;
      int diff;

      if (day_of_year >= moon_calender[i][1]) {
        diff = int_moon_phase + (day_of_year - moon_calender[i][1]);//72-24= 48
        days_to_next_full_moon = int_moon_phase - (diff % int_moon_phase); // % = Modulo Operation
      }
      else {
        days_to_next_full_moon = (moon_calender[i][1] - day_of_year);
      }

      //Test for moon position on scale:  pi=3.14159265
      //(((now_hour * 60) + now_minute) / 1440);//beta: value from 0-1, is for fine calculation
      alfa = ((((days_to_next_full_moon + (((now_hour * 60) + now_minute) / 1440)) * 12) + 180) * (pi / 180));
      int moon_elev = int(el_deg); //need a solution for the moon elevation???
      int moon_point_xpos = int(cos(az_rad + 4.712388 + alfa ) * ((clock_radius / 2) + (moon_elev * 0.5))) + clock_xoffset; //0.5 = Gain Factor
      int moon_point_ypos = int(sin(az_rad + 4.712388 + alfa ) * ((clock_radius / 2) + (moon_elev * 0.5))) + clock_yoffset;
      SetFilledCircle(BLACK, copy_moon_point_xpos, copy_moon_point_ypos, 2);//clear moon icon
      SetFilledCircle(WHITE, moon_point_xpos, moon_point_ypos, 2);
      copy_moon_point_xpos = moon_point_xpos;
      copy_moon_point_ypos = moon_point_ypos;
      //Test end

      if (valid_sync == false) {

        SetFilledCircle(BLACK , moon_x_pos, moon_y_pos, moon_radius); // clear moon icon
        SetFilledCircle(WHITE , moon_x_pos, moon_y_pos, (moon_radius - 1));//Set moon
        SetFilledCircle(BLACK , (moon_x_pos + (days_to_next_full_moon * 2) - 30), moon_y_pos , moon_radius - 1);//Set Phases
        SetCircle(GRAY , moon_x_pos, moon_y_pos, moon_radius);
        //day to full moon:
        //day 30 = full moon
        //day 16-29 = 2. half moon -
        //day 15 = new moon
        //day 1-14 = 1. half moon +
      }
    }
  }
}
