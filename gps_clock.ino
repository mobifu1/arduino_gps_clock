//------- TFT DISPLAY --------------------------
// This sketch has been Refurbished by BUHOSOFT
// IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.
//#include <Adafruit_GFX.h>    // Core graphics library
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

String Tag[7] = {"SONNTAG", "MONTAG", "DIENSTAG", "MITTWOCH", "DONNERSTAG", "FREITAG", "SONNABEND"};
//String Tag[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
String Line = "";    // a string to hold incoming data

//Display Edges calculate
const int x_edge_left = 0;
int x_edge_right;
const int y_edge_up = 0;
int y_edge_down;

//Pin
#define LED A5// Pin of LED

//Clock-Grafik
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

//const float pi = 3.14159265;
//pi/180=0.017453293
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
int now_day_length = 0; //minutes
int last_day_length = 0; //minutes
int last_day = 0;//day number
boolean daylight;
float sun_az_rad;

//Moon Phase Calender 10 years
const int moon_calender[10][17] = {
  {2016, 555, 1267, 1981, 2695, 3407, 4117, 4825, 5531, 6237, 6942, 7647, 8353, 9060, 0, 9, 0}, // year,all full moons in hour of year,
  {2017, 276, 985, 1696, 2408, 3119, 3831, 4542, 5252, 5961, 6669, 7374, 8081, 8787, 0, 2, 0},  //value=((day of full moon -1) *24) + hour
  {2018, 27, 734, 1442, 2150, 2859, 3568, 4279, 4990, 5702, 6413, 7123, 7831, 8539, 9246, 2, 8},//value of pos: 15&16 = number of moon eclipse in year
  {2019, 486, 1193, 1899, 2605, 3311, 4018, 4727, 5438, 6150, 6863, 7574, 8286, 8996, 0, 1, 7},
  {2020, 236, 944, 1651, 2356, 3061, 3765, 4471, 5178, 5887, 6599, 7312, 8026, 8740, 9452, 1, 6},
  {2021, 668, 1377, 2085, 2789, 3493, 4197, 4900, 5606, 6314, 7025, 7738, 8453, 9169, 0, 11, 0},
  {2022, 409, 1122, 1832, 2541, 3246, 3950, 4653, 5356, 6060, 6767, 7476, 8189, 8904, 0, 5, 0},
  {2023, 144, 859, 1574, 2286, 2996, 3702, 4406, 5109, 5812, 6516, 7222, 7930, 8642, 9355, 5, 11},
  {2024,  595, 1310, 2024, 2738, 3448, 4155, 4860, 5564, 6268, 6973, 7678, 8386, 9095, 0, 3, 9},
  {2025,  311, 1023, 1736, 2450, 3163, 3874, 4583, 5290, 5996, 6702, 7406, 8112, 8819, 0, 3, 9},
};
const int moon_culmination[10][15] = {
  {0, 15, 10, 0, -10, -15, -20, -18, -14, -8, 4, 10, 17, 18, 0},//2016 ,max.moon elevation is more or less than 37°  37=90°-latitude
  {17, 18, 12, 6, -5, -11, -19, -21, -19, -11, -4, 8, 17, 19, 0},//2017 jede culmination hat einen vorgänger
  {17, 19, 17, 9,  1, -11, -17, -22, -22, -16, -5, 2, 14, 20, 20},//2018 {dez.vorjahr, jan,feb,-------dez,jan folgejahr}
  {20, 20, 16, 3, -5, -13, -22 , -24, -20, -10, -2, 14, 20, 22, 0},//2019
  {20, 22, 18, 12, -3, -12, -20, -25, -24, -15, -7, 11, 18, 24, 23 }, //2020
  {24, 23, 13, 5, -11, 20, -26, -26, -20, -6, 3, 16, 24, 24, 0}, //2021
  {24, 24, 19, 6, -4, -20 , -26, -28, -22, -13, -3, 18, 24, 26, 0}, //2022
  {24, 26, 22, 11, -5, -14, -27, -29, -27, -14, -4, 7, 22, 27, 25}, //2023
  {27, 25, 14, -1, -16, -24, -30, -28, -22, -5, 6, 22, 27, 27, 0},//2024
  {27, 27, 18, 3, -12, -21, -29, -29, -22, -13, 5, 20, 26, 28, 0}, //2025
};
const byte moon_x_pos = 20;//moon icon big
const byte moon_y_pos = 125;
const byte moon_radius = 15;
int copy_moon_point_xpos;// moon small icon
int copy_moon_point_ypos;
float moon_az_rad;
int tide_last = 0;
//const float zero_offset = 0;
//change location:
//const float cux_offset = -0.800;//default 0.349rad / 1hour = 0.261rad
const float ham_offset = -2.200;//rad
//------------------------------------
//const String chip = "Chip:";
//const String edges = "Set Display Edges:";
//const String load_setup = "Load Setup OK";
//const String wait_gps = "Waiting for GPS )))";
//#########################################################################
//#########################################################################
void setup() {

  Line.reserve(100);
  Serial.begin(9600);//Baudrate for GPS reciever
  //use serial monitor in Arduino IDE > Data via USB-Cable.
  //Serial.println();

  pinMode(LED, OUTPUT);
  tft.reset();
  //uint16_t identifier = tft.readID();
  //  if (identifier == 0x9325) {
  //  } else if (identifier == 0x9328) {
  //  } else if (identifier == 0x7575) {
  //  } else if (identifier == 0x9341) {
  //  } else if (identifier == 0x8357) {
  //  } else {
  //uint16_t identifier = 0x9341;
  //  }
  //tft.begin(identifier);
  tft.begin(0x9341);
  //tft.fillScreen(BLACK);
  ScreenText(WHITE, x_edge_left, 10 , 2, F("V3.3-RC1"));// Arduino IDE 1.6.11
  //Serial.println(sw_version);
  //ScreenText(WHITE, x_edge_left, 40 , chip + String(identifier, HEX));
  //Serial.println(chip + text);
  //int  width = tft.width(), height = tft.height();
  //ScreenText(WHITE, x_edge_left, 70 , (String(width) + "px" + String(height) + "px")); //240x320
  //Serial.println(String(width) + "x" + String(height) + "px");
  //ScreenText(WHITE, x_edge_left, 100 , (edges));
  //Serial.println(edges);
  //x_edge_right = width - 1; //set display edges
  x_edge_right = 239; //set display edges
  //y_edge_down =  height - 1; //set display edges
  y_edge_down =  319; //set display edges
  //ScreenText(WHITE, x_edge_left, 130 , ("X:" + String(x_edge_left) + "-" + String(x_edge_right) + " Y:" + String(y_edge_up) + "-" + String(y_edge_down)));
  //Serial.println("X:" + String(x_edge_left) + "-" + String(x_edge_right) + " Y:" + String(y_edge_up) + "-" + String(y_edge_down));
  //delay(3000);
  //FillScreen(BLACK);
  //ScreenText(WHITE, x_edge_left, 10 , load_setup);
  //Serial.println(load_setup);
  //ScreenText(WHITE, x_edge_left, 40 , wait_gps);
  //Serial.println(wait_gps);
  delay(3000);
  tft.fillScreen(BLACK);
  face();
  //tide();

  //config gps modul or use u-center
  //Serial.write("");//set baudrate
  //Serial.write("");//save settings
}

void loop() {

  static int os = -1;

  if (!(os % 4))if (getline(F("$GPRMC")))RMC();
  SerialClear();

  if (second() != os) { //every second

    char date[20];
    sprintf(date, "%02d.%02d.%04d", day(), month(), year());

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

    ScreenText(text_color, x_edge_left + 10, 10 , 2, copy_wday);
    ScreenText(text_color, x_edge_left + 10, 40 , 2, copy_day);
    //ScreenText(text_color, x_edge_left + 10, 120 , tim);

    //Sekundenzeiger
    alfa = (270 + (30 * hour() + (0.5 * minute()))) * 0.017453293;// pi/180=0.017453293
    hour_arrow_xpos = (cos(alfa) * clock_radius * 0.7) + clock_xoffset;
    hour_arrow_ypos = (sin(alfa) * clock_radius * 0.7) + clock_yoffset;

    alfa = (270 + (6 * minute())) * 0.017453293;// pi/180=0.017453293
    min_arrow_xpos = (cos(alfa) * clock_radius * 0.9) + clock_xoffset;
    min_arrow_ypos = (sin(alfa) * clock_radius * 0.9) + clock_yoffset;

    alfa = (270 + (6 * second())) * 0.017453293;// pi/180=0.017453293
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

    if ((second() == 1) || (second() == 31)) {
      if (valid_signal == true) {
        if ((lat > 0) && (lon > 0) && (lat < 90) && (lon < 180)) {
          sunrise (lat, minute_lat, lon, minute_lon, daylightsavingtime);//Hamburg 53,5° 10,0°
          moon(hour());
          tide();
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
    if (valid_signal == true) {
      lat = getparam(3).substring(0, 2).toInt();
      lon = getparam(5).substring(0, 3).toInt();
      minute_lat = getparam(3).substring(2, 4).toInt();//minute value
      minute_lon = getparam(5).substring(3, 5).toInt();//minute value
      if ((lat > 0) && (lon > 0) && (lat < 90) && (lon < 180)) {
        sunrise (lat, minute_lat, lon, minute_lon, daylightsavingtime);//Hamburg 53,5° 10,0°
        moon(hour());
        tide();
        face();
        ScreenText(text_color, 150, 40 , 2, F("sync"));
        valid_sync = true;
        //Serial.println(sync_info);
      }
    }
  }
}
//----------------------------------------------
//--------------RS232-ROUTINEN------------------
//----------------------------------------------
void SerialClear() {
  while (Serial.available())Serial.read();
}
//----------------------------------------------
boolean getline(String phrase) { //HARD POLLING

  char s[100];
  byte b, n;
  unsigned long t = millis();

  for (unsigned int i = 0; i < sizeof(s); i++) {
    s[i] = 0;
  }
  Line = "";
  do {
    b = Serial.read();
    if (millis() > (t + 100))return false;
  }
  while (b != '$');

  s[0] = b;
  n = Serial.readBytesUntil('\n', &s[1], 90);
  s[n] = 0;

  for (int i = 0; i < n; i++) {
    Line += s[i];
  }
  //Serial.println(Line);
  int index = Line.indexOf(phrase);
  if (index > -1) {
    //Serial.println("found:" + phrase);
    //Serial.println(Line);
    return true;
  }
  return false;
}

#define hex(i)  ((i<=9) ? ('0'+i): ('A'- 10 + i))
//----------------------------------------------
boolean checksum() {
  byte b = 0; int e;
  e = Line.indexOf('*');
  if (e > 10)
  { for (int i = 1; i < e; i++)b ^= Line[i];
    if ((hex((b & 15)) == Line[e + 2]) &&
        (hex(b / 16) == Line[e + 1]))return true;
  }
  return false;
}
//----------------------------------------------
String getparam(int ix) {
  int c, cc = 0;
  if (checksum())
  { do
    { c = Line.indexOf(',', cc);
      if (c >= 0)cc = c + 1; else break;
    } while (--ix);
    return (Line.substring(c + 1, Line.indexOf(',', c + 1)));
  }
  return F("xx"); //debug
}
//----------------------------------------------
//--------------GRAFIK-ROUTINEN-----------------
//----------------------------------------------
void ScreenText(uint16_t color, int xtpos, int ytpos, int text_size , String text) {
  tft.setCursor(xtpos, ytpos);
  tft.setTextColor(color);
  tft.setTextSize(text_size);
  tft.println(text);
}

void SetLines(uint16_t color , int xl1pos, int yl1pos, int xl2pos, int yl2pos) {
  tft.drawLine(xl1pos, yl1pos, xl2pos, yl2pos, color);
}

void SetPoint(uint16_t color, int xppos, int yppos) {
  tft.drawPixel(xppos, yppos, color);
}

void SetRect(uint16_t color , int xr1pos, int yr1pos, int xr2width, int yr2hight) {
  tft.drawRect(xr1pos, yr1pos, xr2width, yr2hight, color);
}

void SetFilledRect(uint16_t color , int xr1pos, int yr1pos, int xr2width, int yr2hight) {
  tft.fillRect(xr1pos, yr1pos, xr2width, yr2hight, color);
}

void SetCircle(uint16_t color , int xcpos, int ycpos, int radius) {
  tft.drawCircle(xcpos, ycpos, radius, color);
}
void SetFilledCircle(uint16_t color , int xcpos, int ycpos, int radius) {
  tft.fillCircle(xcpos, ycpos, radius, color);
}
//----------------------------------------------
//--------------Clock-Face----------------------
//----------------------------------------------
void face() {

  for (int i = 0; i <= 59; i++) {
    alfa = 6 * i * (0.017453293);// pi/180=0.017453293
    point_xpos = (cos(alfa) * clock_radius) + clock_xoffset;
    point_ypos = (sin(alfa) * clock_radius) + clock_yoffset;
    SetPoint(CYAN, point_xpos, point_ypos);
    if ((i % 5) == 0) {
      SetFilledCircle(CYAN, point_xpos, point_ypos, 2);
    }
  }
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

  float sun_el_rad = sun.elevation_rad();                    //store sun's elevation in rads
  //float sun_el_deg = sun.elevation_deg();                  //store sun's elevation in degrees
  //Serial.println(String(el_deg) + "Elevation");

  sun_az_rad = sun.azimuth_rad();                            //store sun's azimuth in rads
  //float az_deg = sun.azimuth_deg();                        //store sun's azimuth in degrees
  //Serial.println(String(az_deg) + "Azimuth");

  float sunrise = sun.sunrise_time();                        //store sunrise time in decimal form
  //Serial.println(String(sunrise) + "Sunrise");
  //sunrise = (sunrise - 0.141666667); //correction factor -8,5 min > Sundata.h calculates unexact
  sunrise_hour = int(sunrise);
  sunrise_minute = int((sunrise - sunrise_hour) * 60);

  float sunset = sun.sunset_time();                          //store sunset time in decimal form
  //Serial.println(String(sunset) + "Sunset");
  //sunset = (sunset + 0.1625); //correction factor +9,75 min > Sundata.h calculates unexact
  sundown_hour = int(sunset);
  sundown_minute = int((sunset - sundown_hour) * 60);

  //sun position on the clock scale
  int sun_point_xpos = int(cos(sun_az_rad + 4.712388) * ((clock_radius / 2) + (sun_el_rad * 28.65))) + clock_xoffset; //0.5 = Gain Factor
  int sun_point_ypos = int(sin(sun_az_rad + 4.712388) * ((clock_radius / 2) + (sun_el_rad * 28.65))) + clock_yoffset;

  SetFilledCircle(BLACK, copy_sun_point_xpos, copy_sun_point_ypos, 2);//clear sun icon
  SetCircle(GRAY, clock_xoffset, clock_yoffset, clock_radius / 2); //horizon
  SetPoint(CYAN, clock_xoffset, clock_yoffset + (clock_radius / 2) + ((90 - latitude + 23.45) * 0.5)); //Scale highest sommer elevation, 90 - lat +23.45
  SetPoint(CYAN, clock_xoffset, clock_yoffset + (clock_radius / 2) + ((90 - latitude) * 0.5));         //Scale Spring Point elevation, 90 - lat
  SetPoint(CYAN, clock_xoffset, clock_yoffset + (clock_radius / 2) + ((90 - latitude - 23.45) * 0.5)); //Scale highest Winter elevation, 90 - lat -23.45

  if (daylight == true) {
    if ((sun_el_rad * 57.3) < 4) {
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

    String lead_zero = ("");
    if (sunrise_minute < 10) {
      lead_zero = (F("0"));
    }
    String text = F("Sunrise: ");
    text += String(sunrise_hour) + ":" + lead_zero + String(sunrise_minute);
    ScreenText(text_color, x_edge_left + 10, 70 , 2, text );

    SetFilledRect(BLACK , x_edge_left, 300, x_edge_right, 19); //clear sunset value on display
    SetFilledCircle(ORANGE , 220, 305, 6);
    SetLines(ORANGE , 210, 305, 230 , 305);
    SetFilledRect(BLACK , 210, 290, 230, 15);

    lead_zero = ("");
    if (sundown_minute < 10) {
      lead_zero = (F("0"));
    }
    text = "";
    text = F("Sunset: ");
    text += String(sundown_hour) + ":" + lead_zero + String(sundown_minute);
    ScreenText(text_color, x_edge_left + 10, 305 , 2, text );

    //calculate day_Length and the delta:
    if (day() != last_day) {
      last_day = day();
      last_day_length = now_day_length;
    }
    else {
      now_day_length = ((sundown_hour * 60) + sundown_minute) - ((sunrise_hour * 60) + sunrise_minute);
    }
    if (last_day_length > 0) {
      SetFilledRect(BLACK , 200, 108, 39, 12);
      ScreenText(text_color, 202, 110 , 1, String(last_day_length - now_day_length) + "min");
    }
  }
}
//----------------------------------------------
//--------------Calculation Moon-Phases---------
//----------------------------------------------
void moon(int now_hour) {

  boolean mooneclipse = false;
  int next_culmination = 0;
  int past_culmination = 0;
  int delta_culmination = 0;

  for (int i = 0; i < 10 ; i++) {
    if (year() == moon_calender[i][0]) {
      int hour_of_year = (((day_of_year - 1) * 24) + (now_hour)); //7.4.2016 11:00 > 2363
      int hour_to_next_full_moon = 0;
      int now_culmination;

      for (int x = 1; x < 15; x++) {
        if  (hour_of_year <= moon_calender[i][x]) {
          hour_to_next_full_moon = moon_calender[i][x] - hour_of_year ;
          next_culmination = moon_culmination[i][x];//load the predefined culmination from table
          past_culmination = moon_culmination[i][x - 1]; //load the predefined culmination from table

          if (x == moon_calender[i][15] || x == moon_calender[i][16]) {//max. 2 eclipses per year
            mooneclipse = true;//next fullmoon is a moon eclipse
          }
          break;
        }
      }

      //Test for moon position on face:
      //der azimuth der sonne ist bekannt, somit kann über next_full_moon der azimuth für den mond abgeleitet werden.
      //der mond erreicht seinen höhepunkt immer im süden.
      //maximum moon_el_deg change in the year, values can between:18°-54° > factor can between -18.3 and +18.3 ; default middle value = 37° ; www.timeanddate.de
      //Die Kulminationshöhe unseres Mondes ergibt sich nach der Formel 90° minus geografische Breite des Beobachtungsstandortes plus Deklination des Mondes.
      //Die höchsten und tiefsten Kulminationshöhen des Mondes sind von Jahr zu Jahr verschieden.
      //Als Ursache gelten die wechselnden Extremweiten der Monddeklination,
      //die im Jahr, Werte sowohl zwischen +18,3° und -18,3° als auch solche von +28,6° und -28,6° im Jahr annehmen kann.
      alfa = ((hour_to_next_full_moon / 112.840) + 3.141);// 2pi=6.2832   pi=3.141
      moon_az_rad = (sun_az_rad  + alfa);
      //(cos(x) * -1 * 37) * factor;  default factor = 0  >  http://www.walterzorn.de/grapher/grapher.htm
      delta_culmination = next_culmination - (past_culmination);
      now_culmination = int(past_culmination + ((float(delta_culmination) / 715) * (715 - hour_to_next_full_moon)));//-14
      int moon_el_deg = (cos(sun_az_rad + alfa)) * -(90 - lat);
      //now_culmination: Berechnung der Abweichung vom Mittelwert 37°+-18°
      int moon_point_xpos = int(cos(sun_az_rad - 1.5707 + alfa) * (1 + (now_culmination * 0.0056)) * ((clock_radius * 0.5) + (moon_el_deg * 0.5))) + clock_xoffset; //0.5 = Gain Factor
      int moon_point_ypos = int(sin(sun_az_rad - 1.5707 + alfa) * (1 + (now_culmination * 0.0056)) * ((clock_radius * 0.5) + (moon_el_deg * 0.5))) + clock_yoffset;

      SetFilledCircle(BLACK, copy_moon_point_xpos, copy_moon_point_ypos, 2);//clear moon icon
      if (moon_el_deg < 1) {
        SetFilledCircle(GRAY, moon_point_xpos, moon_point_ypos, 2);
      }
      else {
        SetFilledCircle(WHITE, moon_point_xpos, moon_point_ypos, 2);
      }
      copy_moon_point_xpos = moon_point_xpos;
      copy_moon_point_ypos = moon_point_ypos;
      //Test end

      if (valid_sync == false) {

        SetFilledCircle(WHITE , moon_x_pos, moon_y_pos, (moon_radius - 1));//Set moon

        if ((mooneclipse == true) && (hour_to_next_full_moon < 10)) { // indication 10 hours before
          SetFilledCircle(RED , moon_x_pos, moon_y_pos, (moon_radius - 1));//Set moon
        }

        //2*(cos(pi*x*0.033)) >  http://www.walterzorn.de/grapher/grapher.htm
        const float shadow = (-20 * (cos(3.14 * 0.033 * (hour_to_next_full_moon / 24))));
        SetFilledCircle(BLACK , (moon_x_pos + (round(hour_to_next_full_moon / 12)) - 29 + shadow), moon_y_pos , ((moon_radius - 1) + abs(shadow))); // set silluette
        //SetFilledCircle(RED , (moon_x_pos + (round(hour_to_next_full_moon / 12)) - 29 + shadow), moon_y_pos , ((moon_radius - 1) + abs(shadow))); // set silluette for test
        SetCircle(GRAY , moon_x_pos, moon_y_pos, moon_radius);

        if (hour_to_next_full_moon < 13) {
          //SetFilledCircle(BLACK , moon_x_pos, moon_y_pos, 2);//full moon indicator
          ScreenText(BLACK, moon_x_pos - 7, moon_y_pos - 3 , 1, "-" + String(hour_to_next_full_moon)); //full moon indicator
        }
        //day to full moon:
        //day 29,5 = full moon    > 709
        //day 14,9-29,4 = 2. half moon 356-708
        //day 14,8 = new moon     > 355
        //day 0-14,7 = 1. half moon + 0-354
      }
    }
    break;
  }
}
//-----------------------------------------------
//-------------Calculation-Tide------------------
//-----------------------------------------------
void tide() {

  uint16_t tide_color = BLUE;

  // small calculation for springtide:
  int tide_strength = 10 * cos((sun_az_rad - moon_az_rad) * 2);
  if (tide_strength > 7) {
    tide_color = RED;
  }
  //2pi = 12h 25min
  //30min = 0.2512;
  //Cux = -0.349 rad
  //HH  = -1.228 rad
  //cos((x*2)+Cux_offset);

  //change location:
  //int tide_hight = round(12 * (cos((moon_az_rad * 2) + cux_offset)));
  int tide_hight = round(12 * (cos((moon_az_rad * 2) + ham_offset)));

  //Test Tide in Flusslandschaft
  //cos(2*(x+(-0.4*(abs(sin(x)))))); //langsamer Ablauf, schneller Auflauf ;verzögerung des Niedrigwasser um 1 Stunde
  //int tide_hight = round(12 * (cos((moon_az_rad * 2) + ham_offset + (-0.4 * (abs(sin(moon_az_rad)))))));

  //SetFilledRect(BLACK , x_edge_left, 210, 30, 50);
  //int moon_az_deg = moon_az_rad * 57.306;
  //moon_az_deg = moon_az_deg % 360;
  //ScreenText(text_color, 0, 230 , 1, String(moon_az_deg));
  //ScreenText(text_color, 0, 230 , 1, String(tide_hight));
  //ScreenText(text_color, 0, 210 , 1, String(tide_strength));
  //ScreenText(text_color, 0, 230 , 1, String(tide_steigung));

  //change location:
  //ScreenText(text_color, 12, 240 , 1, F("Cux"));
  ScreenText(text_color, 12, 240 , 1, F("Ham"));

  SetFilledRect(BLACK , 5, 252, 30, 36);
  SetRect(GRAY , 5, 255, 30, 30);
  SetLines(GRAY, 5, 270, 33, 270 );
  SetLines(tide_color, 7 , 270 - tide_hight, 32, 270 - tide_hight );

  //-sin(2*x) = 1.Ableitung von cos(2*x)

  //change location:
  //int tide_steigung = round(12 * (-sin((moon_az_rad * 2) + cux_offset))); //Anstieg der Tide > Steigung der cos-funktion , Cux
  int tide_steigung = round(12 * (-sin((moon_az_rad * 2) + ham_offset))); //Anstieg der Tide > Steigung der cos-funktion , HH
  SetFilledRect(RED , 6, 269 - tide_steigung, 3, 3);
  //SetFilledCircle(RED , 5 , 270 - tide_steigung , 1);
  //-----------------------------------------------
}
