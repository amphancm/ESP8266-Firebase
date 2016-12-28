
#define DHTPIN 10               // what pin we're connected to

//#define DHTTYPE DHT11         // DHT 11 
#define DHTTYPE DHT22           // DHT 22  (AM2302)
//#define DHTTYPE DHT21         // DHT 21  (AM2301)

DHT dht(DHTPIN, DHTTYPE,15);

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
//static const unsigned char PROGMEM logo16_glcd_bmp[] =
static const unsigned char logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int ledState = LOW;  
int showState = LOW; 
int lp = 0;
int lp_time = 30;            // 2000 mS x lp_time = Sent data time
int imode = 0;
int buttonState = 0;         // variable for reading the pushbutton status

boolean cloop = true;

unsigned long previousMillis = 0;
const long interval = 2000;


// WIFI
#define WIFI_SSID        "YOUR SSID"  
#define WIFI_PASSPHARSE  "YOUR PASSWORD"


// Config Firebase
#define FIREBASE_HOST   "YOUR FIREBASE HOST"
#define FIREBASE_AUTH   "YOUR AUTH KEY"





