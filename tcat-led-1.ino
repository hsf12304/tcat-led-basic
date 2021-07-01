//串口
#define BLINKER_PRINT Serial
//Blinker 接入方式：WiFi
#define BLINKER_WIFI
//天猫精灵接入的设备：灯
#define BLINKER_ALIGENIE_LIGHT
#include <Blinker.h>
//WS2812驱动
#include <Adafruit_NeoPixel.h>
//RGB颜色值
uint8_t colorR,colorG,colorB;
//亮度值：亮度初始值为50
uint8_t colorW=50;
//灯的模式
String wsMode;
//灯电源状态
bool wsState = false;


//备注： auth -> Blinker设备的secret Key
//      ssid ->wifi账号（注意大小写）
//      pswd ->wifi密码
char auth[] = "#######";
char ssid[] = "#######";
char pswd[] = "#######";


//定义按钮为 GPIO0
#define BUTTON_PIN_0   0
#define BUTTON_PIN_1   1
#define BUTTON_PIN_2   2
//消除抖动使用的的变量以及模式
boolean oldState = HIGH;
boolean oldState2 = HIGH;
boolean oldState3 = HIGH;
int     mode     = 0;    // Currently-active animation mode, 0-9

//定义ws2812使用的IO口为13（D7）
#define PIXEL_PIN    2  // Digital IO pin connected to the NeoPixels.
//定义ws2812灯的个数为24
#define PIXEL_COUNT 60  // Number of NeoPixels
// Declare our NeoPixel strip object:
/*
参数1：ws2812使用的灯的个数
参数2：ws2812驱动的引脚
参数3：驱动的模式和频率
*/

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  //串口初始化
  Serial.begin(115200);
  Serial.println("串口初始化...success");
  strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  Serial.println("设置亮度...start");
  //设置亮度
  strip.setBrightness(255 * colorW /100); // Set BRIGHTNESS to about 1/5 (max = 255)
  Serial.println("亮灯...start");
  strip.show();  // Initialize all pixels to 'off'
  
  //调试有关
  #if defined(BLINKER_PRINT)
      BLINKER_DEBUG.stream(BLINKER_PRINT);
  #endif
  //初始化按钮为输入上拉
  pinMode(BUTTON_PIN_0, INPUT_PULLUP);
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);

  
  //Blinker配网
  Blinker.begin(auth, ssid, pswd);
  Blinker.attachData(dataRead);
  /*注册天猫精灵有关的回调函数*/
  //状态查询
  BlinkerAliGenie.attachQuery(aligenieQuery);
  //亮度设置
  BlinkerAliGenie.attachBrightness(aligenieBright);
  //取消模式
  BlinkerAliGenie.attachCancelMode(aligeniecMode);
  //设置模式
  BlinkerAliGenie.attachMode(aligenieMode);
  //颜色控制
  BlinkerAliGenie.attachColor(aligenieColor);
  //电源状态
  BlinkerAliGenie.attachPowerState(aligeniePowerState);

}
void loop()
{
    Blinker.run();
    boolean newState = digitalRead(BUTTON_PIN_0);
    boolean newState2 = digitalRead(BUTTON_PIN_1);
Serial.print(".");
  // Check if state changed from high to low (button press).
  if((newState == LOW) && (oldState == HIGH)) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newState = digitalRead(BUTTON_PIN_0);
    if(newState == LOW) {      // Yes, still low
      //灯处于打开状态
      wsState = true;
      if(++mode > 8) mode = 0; // Advance to next mode, wrap around after #8
      switch(mode) {  
        // Start the new animation...
         case 0:
        //设置为黑色，灭灯
        colorR = 0; colorG = 0; colorB = 0;
        colorWipe(strip.Color(colorR,colorG,colorB), 50);
        colorW = 0;
        strip.setBrightness(255 * colorW /100); // Set BRIGHTNESS to about 1/5 (max = 255)
        strip.show();
          break;
        case 1:
        //设置为白色，亮度50%
        colorR = 255; colorG = 255; colorB = 255;
        colorWipe(strip.Color(colorR,colorG,colorB), 50);
        colorW = 50;
        strip.setBrightness(255 * colorW /100); // Set BRIGHTNESS to about 1/5 (max = 255)
        strip.show();
          break;
        case 2:
        //设置为黄色，亮度50%
        colorR = 255; colorG = 255; colorB = 0;
        colorWipe(strip.Color(colorR,colorG,colorB), 50);
        colorW = 50;
        strip.setBrightness(255 * colorW /100); // Set BRIGHTNESS to about 1/5 (max = 255)
        strip.show();
          break;
        case 3:
          //设置为蓝色，亮度50%
        colorR = 0; colorG = 0; colorB = 255;
        colorWipe(strip.Color(colorR,colorG,colorB), 50);
        colorW = 50;
        strip.setBrightness(255 * colorW /100); // Set BRIGHTNESS to about 1/5 (max = 255)
        strip.show();
          break;
        case 4:
        //设置为绿色，亮度50%
        colorR = 0; colorG = 255; colorB = 0;
        colorWipe(strip.Color(colorR,colorG,colorB), 50);
        colorW = 50;
        strip.setBrightness(255 * colorW /100); // Set BRIGHTNESS to about 1/5 (max = 255)
        strip.show();
          break;
        case 5:
          //设置为红色，亮度50%
        colorR = 255; colorG = 0; colorB = 0;
        colorWipe(strip.Color(colorR,colorG,colorB), 50);
        colorW = 50;
        strip.setBrightness(255 * colorW /100); // Set BRIGHTNESS to about 1/5 (max = 255)
        strip.show();
          break;
        case 6:
        //设置为青色，亮度50%
        colorR = 0; colorG = 255; colorB = 255;
        colorWipe(strip.Color(colorR,colorG,colorB), 50);
        colorW = 50;
        strip.setBrightness(255 * colorW /100); // Set BRIGHTNESS to about 1/5 (max = 255)
        strip.show();
          break;
        case 7:
           //设置为紫色，亮度50%
        colorR = 128; colorG = 0; colorB = 128;
        colorWipe(strip.Color(colorR,colorG,colorB), 50);
        colorW = 50;
        strip.setBrightness(255 * colorW /100); // Set BRIGHTNESS to about 1/5 (max = 255)
        strip.show();
          break;
        case 8:
          //设置为橙色，亮度50%
        colorR = 255; colorG = 165; colorB = 0;
        colorWipe(strip.Color(colorR,colorG,colorB), 50);
        colorW = 50;
        strip.setBrightness(255 * colorW /100); // Set BRIGHTNESS to about 1/5 (max = 255)
        strip.show();
          break;   
         default :
          break;
      }
    }
  }
  //亮度减小  --
   if((newState2 == LOW) && (oldState2 == HIGH)) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newState2 = digitalRead(BUTTON_PIN_1);
    if(newState2 == LOW) {      // Yes, still low
      //设置亮度
        if(colorW >10){
          colorW -=10;
          strip.setBrightness(255 * colorW /100); // Set BRIGHTNESS to about 1/5 (max = 255)
          strip.show();
        }
    }
    
   }
   //亮度增加  +
//   if((newState3 == LOW) && (oldState3 == HIGH)) {
//      // Short delay to debounce button.
//      delay(20);
//      // Check if button is still low after debounce.
//      if(newState3 == LOW) {      // Yes, still low
//      //设置亮度
//        if(colorW <100){
//          colorW +=10;
//          strip.setBrightness(255 * colorW /100); // Set BRIGHTNESS to about 1/5 (max = 255)
//          strip.show();
//        }
//      }
//   }

  // Set the last-read button state to the old state.
  oldState = newState;
  oldState2 = newState2;
//  oldState3 = newState3;
}




//天猫精灵状态查询
void aligenieQuery(int32_t queryCode)
{
    BLINKER_LOG("AliGenie Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("AliGenie Query All");
            BlinkerAliGenie.powerState(wsState ? "on" : "off");
            BlinkerAliGenie.brightness(colorW);
             BlinkerAliGenie.color(getColor());
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("AliGenie Query Power State");
            BlinkerAliGenie.powerState(wsState ? "on" : "off");
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_COLOR_NUMBER :
            BLINKER_LOG("AliGenie Query Color");
            BlinkerAliGenie.color(getColor());
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_MODE_NUMBER :
            BLINKER_LOG("AliGenie Query Mode");
            BlinkerAliGenie.mode(wsMode);
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_BRIGHTNESS_NUMBER :
            BLINKER_LOG("AliGenie Query Brightness");
            BlinkerAliGenie.brightness(colorW);
            BlinkerAliGenie.print();
            break;
        default :
            break;
    }
}
void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);

    Blinker.vibrate();

    uint32_t BlinkerTime = millis();
    Blinker.print(BlinkerTime);
    Blinker.print("millis", BlinkerTime);
}
//电源状态
void aligeniePowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
        //设置为白色，亮度50%
        colorR = 255; colorG = 255; colorB = 255;
        colorWipe(strip.Color(colorR,colorG,colorB), 50);
        colorW = 50;
        strip.setBrightness(255 * colorW /100); // Set BRIGHTNESS to about 1/5 (max = 255)
        strip.show();
        BlinkerAliGenie.powerState("on");
        BlinkerAliGenie.print();
        wsState=true;
    }
    else if (state == BLINKER_CMD_OFF) {
     
        colorR = 0; colorG = 0; colorB = 0;
        colorWipe(strip.Color(colorR,colorG,colorB), 50);
        BlinkerAliGenie.powerState("off");
        BlinkerAliGenie.print();
        wsState=false;
    }
}
//设置颜色
void aligenieColor(const String & color)
{
    BLINKER_LOG("need set color: ", color);

    //如果是设置颜色,则灯的状态应该是打开的，故同步状态
    wsState = true;
    if (color == "Red") {
        colorR = 255; colorG = 0; colorB = 0;
        //colorWipe(strip.Color( 255,0,0), 50);    // Black/off
    }
    else if (color == "Yellow") {
        colorR = 255; colorG = 255; colorB = 0;
        //colorWipe(strip.Color(220,20,60), 50);
    }
    else if (color == "Blue") {
        colorR = 0; colorG = 0; colorB = 255;
        //colorWipe(strip.Color(0,0,255), 50);
    }
    else if (color == "Green") {
        colorR = 0; colorG = 255; colorB = 0;
        
    }
    else if (color == "White") {
        colorR = 255; colorG = 255; colorB = 255;
    }
    else if (color == "Black") {
        colorR = 0; colorG = 0; colorB = 0;
    }
    else if (color == "Cyan") {
        colorR = 0; colorG = 255; colorB = 255;
    }
    else if (color == "Purple") {
        colorR = 128; colorG = 0; colorB = 128;
    }
    else if (color == "Orange") {
        colorR = 255; colorG = 165; colorB = 0;
    }

    colorWipe(strip.Color(colorR,colorG,colorB), 50);
    //亮度默认为50
    //设置亮度
    colorW = 50;
    strip.setBrightness(255 * colorW /100); // Set BRIGHTNESS to about 1/5 (max = 255)
    strip.show();
    
    BlinkerAliGenie.color(color);
    BlinkerAliGenie.print();
}
//设置灯的模式
void aligenieMode(const String & mode)
{
    BLINKER_LOG("need set mode: ", mode);
    //设置灯的模时，灯应该为打开状态
    wsState = true;
    if (mode == BLINKER_CMD_READING) {
        // Your mode function
        //阅读模式：灯的颜色设置为橙色，亮度为80%
        colorR = 255; colorG = 165; colorB = 0;
        colorWipe(strip.Color(colorR,colorG,colorB), 50);
        //设置亮度
        colorW = 80;
        strip.setBrightness(255 * colorW /100); // Set BRIGHTNESS to about 1/5 (max = 255)
        strip.show();
        
    }
    else if (mode == BLINKER_CMD_MOVIE) {
        // Your mode function
        //影院模式：灯的颜色设置为黄色，亮度为50%
        colorR = 255; colorG = 0; colorB = 0;
        colorWipe(strip.Color(colorR,colorG,colorB), 50);
        //设置亮度
        colorW = 50;
        strip.setBrightness(255 * colorW /100); // Set BRIGHTNESS to about 1/5 (max = 255)
        strip.show();
    }
    else if (mode == BLINKER_CMD_SLEEP) {
        // Your mode function
        //睡眠模式：灯的颜色设置为黄色，亮度为10%
        colorR = 255; colorG = 255; colorB = 0;
        colorWipe(strip.Color(colorR,colorG,colorB), 50);
        //设置亮度
        colorW = 10;
        strip.setBrightness(255 * colorW /100); // Set BRIGHTNESS to about 1/5 (max = 255)
        strip.show();
    }
    else if (mode == BLINKER_CMD_HOLIDAY) {
        // Your mode function
        theaterChase(strip.Color(  0,   0, 127), 50); // Blue
        //设置亮度:100%
        colorW = 100;
        strip.setBrightness(255 * colorW /100); // Set BRIGHTNESS to about 1/5 (max = 255)
        strip.show();
    }
    else if (mode == BLINKER_CMD_MUSIC) {
        // Your mode function
        //音乐模式：则为彩虹灯
        rainbow(10);
        //设置亮度:100%
        colorW = 100;
        strip.setBrightness(255 * colorW /100); // Set BRIGHTNESS to about 1/5 (max = 255)
        strip.show();
        
    }
    else if (mode == BLINKER_CMD_COMMON) {
        // Your mode function
        //普通模式：灯的颜色设置为白色，亮度为50%
        colorR = 255; colorG = 255; colorB = 255;
        colorWipe(strip.Color(colorR,colorG,colorB), 50);
        //设置亮度
        colorW = 50;
        strip.setBrightness(255 * colorW /100); // Set BRIGHTNESS to about 1/5 (max = 255)
        strip.show();
    }

    wsMode = mode;
    BlinkerAliGenie.mode(mode);
    BlinkerAliGenie.print();
}
//取消模式
void aligeniecMode(const String & cmode)
{
    BLINKER_LOG("need cancel mode: ", cmode);

    if (cmode == BLINKER_CMD_READING) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_MOVIE) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_SLEEP) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_HOLIDAY) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_MUSIC) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_COMMON) {
        // Your mode function
    }

    wsMode = BLINKER_CMD_COMMON; // new mode

    BlinkerAliGenie.mode(wsMode); // must response
    BlinkerAliGenie.print();
}
//天猫精灵亮度控制
void aligenieBright(const String & bright)
{
    int set_brightness;
    BLINKER_LOG("need set brightness: ", bright);

    if (bright == BLINKER_CMD_MAX) {
        colorW = 100;
    }
    else if (bright == BLINKER_CMD_MIN) {
        colorW = 0;
    }
    else {
        colorW = bright.toInt();
    }
    //进行亮度值转换
    set_brightness = 255 * colorW /100;

    //设置亮度
    strip.setBrightness(set_brightness); // Set BRIGHTNESS to about 1/5 (max = 255)
    strip.show();
    BLINKER_LOG("now set brightness: ", colorW);
    BlinkerAliGenie.brightness(colorW);
    BlinkerAliGenie.print();
}
String getColor(){
  uint32_t color = colorR << 16 | colorG << 8 | colorB;
 
    switch (color)
    {
        case 0xFF0000 :
            return "Red";
            break;
        case 0xFFFF00 :
            return "Yellow";
            break;
        case 0x0000FF :
            return "Blue";
            break;
        case 0x00FF00 :
            return "Green";
            break;
        case 0xFFFFFF :
            return "White";
            break;
        case 0x000000 :
            return "Black";
            break;
        case 0x00FFFF :
            return "Cyan";
            break;
        case 0x800080 :
            return "Purple";
            break;
        case 0xFFA500 :
            return "Orange";
            break;
        default :
            return "White";
            break;
    }
  }



void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 3 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 3*65536. Adding 256 to firstPixelHue each time
  // means we'll make 3*65536/256 = 768 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}
// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}
