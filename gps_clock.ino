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

//Clock-Grafik
int clock_radius = 90;
int clock_xoffset = 120;
int clock_yoffset = 200;
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

float pi = 3.14159265;
float sec_alfa;
float min_alfa;
float hour_alfa;
String copy_wday;
String copy_sat;
boolean valid_sync = false;
uint16_t text_color;

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
  ScreenText(WHITE, 0, 10 , "Chip:" + text);
  int  width = tft.width(), height = tft.height();
  ScreenText(WHITE, 0, 40 , (String(width) + "x" + String(height) + "px")); //240x320
  delay(3000);
  FillScreen(BLACK);
  ScreenText(WHITE, 0, 10 , "Waiting for GPS");
  delay(3000);
  FillScreen(BLACK);
  SetCircle(GREEN, clock_xoffset, clock_yoffset, clock_radius); // set clock
  SetPoint(GREEN, clock_xoffset, clock_yoffset);// set clock middle
  Line.reserve(100);
  Serial.begin(9600);
}

void loop()
{ static int os = -1;

  if (!(os % 3))if (getline("$GPGGA"))GGA();
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
      SetFilledRect(BLACK , 0, 0, 149, 100);
    }
    if ((hour() > 6) && (hour() < 22)) {
      text_color = WHITE;//day color
    }
    else {
      text_color = BLUE;//night clolor
    }
    ScreenText(text_color, 0, 10 , wday);
    ScreenText(text_color, 0, 40 , date);
    //ScreenText(text_color, 0, 70 , tim);
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

void GGA()//FIX SAT ect.
{ //Serial.print (getparam(7)); Serial.println(" Sat");

  if (getparam(7) != copy_sat) {
    SetFilledRect(BLACK , 150, 0, 239, 39);
  }
  ScreenText(text_color, 150, 10 , (getparam(7)) + " Sat");
  copy_sat = getparam(7);
}//GPGGA

void RMC()//TIME DATE
{ setTime(getparam(1).substring(0, 0 + 2).toInt(),
          getparam(1).substring(2, 2 + 2).toInt(),
          getparam(1).substring(4, 4 + 2).toInt(),
          getparam(9).substring(0, 0 + 2).toInt(),
          getparam(9).substring(2, 2 + 2).toInt(),
          getparam(9).substring(4, 4 + 2).toInt());
  time_t cet = CE.toLocal(now(), &tcr);
  setTime(cet);
  //Serial.println("sync");
  if (valid_sync == false) {
    SetFilledRect(BLACK , 150, 40, 239, 69); //clear sync on display
  }
  ScreenText(text_color, 150, 40 , "Sync");
  valid_sync = true;
}//GPRMC

void SerialClear()
{ while (Serial.available())Serial.read();
}

//----------------------------------------------
//--------------GPS-ROUTINEN--------------------
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

unsigned long SetRect(uint16_t color , int xr1pos, int yr1pos, int xr2pos, int yr2pos) {
  unsigned long start, t;
  tft.drawRect(xr1pos, yr1pos, xr2pos, yr2pos, color);
  return micros() - start;
}

unsigned long SetFilledRect(uint16_t color , int xr1pos, int yr1pos, int xr2pos, int yr2pos) {
  unsigned long start, t;
  tft.fillRect(xr1pos, yr1pos, xr2pos, yr2pos, color);
  return micros() - start;
}

unsigned long SetCircle(uint16_t color , int xcpos, int ycpos, int radius) {

  unsigned long start, t;
  tft.drawCircle(xcpos, ycpos, radius, color);
  return micros() - start;
}
