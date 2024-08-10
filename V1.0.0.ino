/*
For arduino uno/nano board

*IMPORTANT NOTICE:
For some of the ST7735 green tab screen, you have to change the library file for proper color showing
See details on: github.com/adafruit/Adafruit-ST7735-Library/issues/154
-------------------------------------------------------------------------
Seems like the RGB coding that this screen expects is ST77XX_MADCTL_RGB instead of ST7735_MADCTL_BGR
Initializing it with BLACKTAB works for color but gets bottom and right edges wrong.
Initializing it with GREENTAB works for edges but not for colors

Seems like people are referring to these screens as YELLOWTAB ... anyway these ones seem to be a little bit annoying to use.

In order to solve out the problem I had to modify the library file "Adafruit_ST7735.cpp" and change these lines :

...
switch (rotation) {
case 0:
if ((tabcolor == INITR_BLACKTAB) || (tabcolor == INITR_MINI160x80)) {
madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST77XX_MADCTL_RGB;
} else {
// madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST7735_MADCTL_BGR;
madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST77XX_MADCTL_RGB;
}
...
case 1:
if ((tabcolor == INITR_BLACKTAB) || (tabcolor == INITR_MINI160x80)) {
madctl = ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
} else {
// madctl = ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST7735_MADCTL_BGR;
madctl = ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
}
...
case 2:
if ((tabcolor == INITR_BLACKTAB) || (tabcolor == INITR_MINI160x80)) {
madctl = ST77XX_MADCTL_RGB;
} else {
// madctl = ST7735_MADCTL_BGR;
madctl = ST77XX_MADCTL_RGB;
}
...
case 3:
if ((tabcolor == INITR_BLACKTAB) || (tabcolor == INITR_MINI160x80)) {
madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
} else {
// madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST7735_MADCTL_BGR;
madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
...
-------------------------------------------------------------------------

Linking:
TFT_CS    10
TFT_RST   9
TFT_DC    8
MOSI      11
SCLK      13
Servo     6
RockerX   A0
RockerY   A1
RockerSw  A2

abbr.
num: number of pictures
int: interval time
dur: total duraion

*/

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
#include <Servo.h>

//For ST7735
#define TFT_CS        10
#define TFT_RST        9 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
Servo servo;

int num = 0;

enum MODE {
  AUTO_NUM, AUTO_INT, AUTO_DUR
};

struct {
  int numOfPic = 10;
  long totalDuration = 20;
  int intervalTime = 2;
  MODE mode = AUTO_DUR;
} scanTask;

struct {
  int x;
  int y;
  bool sw;
  bool isAny;
} rocker;

void setup(void) {
  Serial.begin(9600);

  pinMode(A2, INPUT_PULLUP);

  tft.initR(INITR_GREENTAB);      // Init ST7735S chip, green tab, for fixing color problem, remember to change the ST7735_BGR to ST77xx_RGB in st7735.cpp(weird issue)
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  Serial.println(F("ST7735 is initialized"));
  
  tft.setCursor(10,50);
  tft.setTextColor(ST77XX_BLUE,ST77XX_BLACK);
  tft.setTextSize(2);
  tft.print("Hello world!");
  // delay(2000);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(3);

  servo.attach(6);
  servo.write(40);
}

void loop() {
  homepage();
  delay(1000);
}

void homepage() {
  int cursor = 0;//0-number of pictures  1-total duration  2-interval time 3-mode 4-start
  tft.fillScreen(ST77XX_BLACK);
  refreshHomepageScreen(cursor, scanTask.mode);

  while(1) {
    getRocker();
    if(rocker.isAny) {
      if(rocker.sw) {
        switch(cursor) {
          case 0:
            setNum();
            break;
          case 1:
            setInt();
            break;
          case 2:
            setDur();
            break;
          case 3:
            setMode();
            break;
          case 4:
            startTask();
            servo.write(40);//Servo back to default position
            break;
        }
        scanTask.totalDuration = (long)scanTask.intervalTime * scanTask.numOfPic;
        tft.fillScreen(ST77XX_BLACK);
      }
      if(rocker.x) {
        cursor += rocker.x;
        if(cursor > 4) {
          cursor = 0;
        }
        if(cursor < 0) {
          cursor = 4;
        }
        if(cursor == scanTask.mode) {
          cursor += rocker.x;
        }
      }
      refreshHomepageScreen(cursor, scanTask.mode);
      while(rocker.isAny) {
        getRocker();
      }
    }
  }
  
}

void refreshHomepageScreen(int cursor, MODE mode) {
  uint8_t disable = mode;
  uint8_t w = 75;
  uint8_t h = 50;
  uint8_t r = 7;
  uint16_t color_selected = tft.color565(120, 120, 255);
  uint16_t color_unselected = tft.color565(220, 220, 255);
  uint16_t color_disable = tft.color565(110, 110, 110);

  //basic rectangle
  tft.fillRoundRect(0, 0, w, h, r, disable == 0 ? color_disable : (cursor == 0 ? color_selected : color_unselected));
  tft.fillRoundRect(85, 0, w, h, r, disable == 1 ? color_disable : (cursor == 1 ? color_selected : color_unselected));
  tft.fillRoundRect(0, 54, w, h, r, disable == 2 ? color_disable : (cursor == 2 ? color_selected : color_unselected));
  tft.fillRoundRect(85, 54, w, h, r, cursor == 3 ? color_selected : color_unselected);
  tft.fillRoundRect(0, 108, 160, 20, r, cursor == 4 ? tft.color565(80, 255, 80) : tft.color565(140, 230, 140));
  
  tft.setTextColor(tft.color565(60, 60, 80));

  //basic text
  tft.setTextSize(1);
  tft.setCursor(28, 3);
  tft.print("Num");
  tft.setCursor(115, 3);
  tft.print("Int");
  tft.setCursor(28, 57);
  tft.print("Dur");
  tft.setCursor(110, 57);
  tft.print("Mode");

  tft.setTextSize(2);
  tft.setCursor(45, 110);
  tft.print("Start");

  //data text
  //number of pictures
  if(scanTask.numOfPic > 999) {
    tft.setTextSize(2);
    tft.setCursor(13, 20);
    tft.print(scanTask.numOfPic);
  } else {
    tft.setTextSize(3);
    tft.setCursor(10, 17);
    tft.print(scanTask.numOfPic);
  }

  //interval time
  if(scanTask.intervalTime / 60 >= 1) {
    tft.setCursor(88, 21);
    tft.setTextSize(2);
    tft.print((int)floor(scanTask.intervalTime / 60) % 60);
    tft.print("m");
    tft.print(scanTask.intervalTime % 60);
    tft.print("s");
  } else {
    tft.setCursor(95, 17);
    tft.setTextSize(3);
    tft.print(scanTask.intervalTime);
    tft.print("s");
  }
  
  //total duration
  if(scanTask.totalDuration >= 360000) {
    tft.setCursor(3, 75);
    tft.setTextSize(2);
    tft.print(int(scanTask.totalDuration / 3600));
    tft.print("h");
  } else if(scanTask.totalDuration >= 3600) {
    tft.setCursor(3, 75);
    tft.setTextSize(2);
    tft.print(int(scanTask.totalDuration / 3600));
    tft.print("h");
    tft.print(int(scanTask.totalDuration / 60) % 60);
    tft.print("m");
    // tft.print(scanTask.totalDuration % 60);
    // tft.print("s");
  } else if(scanTask.totalDuration >= 60) {
    tft.setCursor(3, 75);
    tft.setTextSize(2);
    tft.print(int(scanTask.totalDuration / 60) % 60);
    tft.print("m");
    tft.print(scanTask.totalDuration % 60);
    tft.print("s");
  } else {
    tft.setCursor(10, 71);
    tft.setTextSize(3);
    tft.print(scanTask.totalDuration);
    tft.print("s");
  }

  tft.setTextSize(2);
  tft.setCursor(105, 75);
  switch(mode) {
    case AUTO_NUM:
      tft.print("Num");
      break;
    case AUTO_INT:
      tft.print("Int");
      break;
    case AUTO_DUR:
      tft.print("Dur");
      break;
  }


}

void setNum() {
  int num = scanTask.numOfPic;
  int cursor = 3;
  tft.fillScreen(ST77XX_BLACK);
  refreshSetNumScreen(cursor, num);

  while(rocker.isAny) {
    getRocker();
  }
  while(1) {
    getRocker();
    if(rocker.isAny) {
      if(rocker.sw) {
        scanTask.numOfPic = num;
        break;
      }
      if(rocker.x) {
        cursor += rocker.x;
        if(cursor > 3) {
          cursor = 0;
        }
        if(cursor < 0) {
          cursor = 3;
        }
      }
      if(rocker.y > 0) {
        num += pow(10, 3-cursor);
        if(num > 9999) {
          num -= 10000;
        }
      }
      if(rocker.y < 0) {
        num -= pow(10, 3-cursor);
        if(num < 0) {
          num += 10000;
        }
      }
      refreshSetNumScreen(cursor, num);
      while(rocker.isAny) {
        getRocker();
      }
    }
  }
}

void refreshSetNumScreen(int cursor, int num) {
  uint8_t w = 30;
  uint8_t h = 45;
  uint8_t r = 7;
  uint16_t color_selected = tft.color565(120, 120, 255);
  uint16_t color_unselected = tft.color565(220, 220, 255);

  //basic rectangle
  tft.fillScreen(tft.color565(150, 180, 200));
  tft.fillRoundRect(5, 58, w, h, r, cursor == 0 ? color_selected : color_unselected);
  tft.fillRoundRect(45, 58, w, h, r, cursor == 1 ? color_selected : color_unselected);
  tft.fillRoundRect(85, 58, w, h, r, cursor == 2 ? color_selected : color_unselected);
  tft.fillRoundRect(125, 58, w, h, r, cursor == 3 ? color_selected : color_unselected);

  tft.setTextColor(tft.color565(60, 60, 80));

  //basic text
  tft.setTextSize(2);
  tft.setCursor(25, 10);
  tft.print("Number Of");
  tft.setCursor(30, 32);
  tft.print("Pictures");

  //data text
  tft.setTextColor(tft.color565(60, 60, 80));
  tft.setTextSize(3);
  tft.setCursor(13, 70);
  tft.print(int(num / 1000));
  tft.setCursor(53, 70);
  tft.print(int(num / 100) % 10);
  tft.setCursor(93, 70);
  tft.print(int(num / 10) % 10);
  tft.setCursor(133, 70);
  tft.print(num % 10);
}

void setInt() {
  int num = scanTask.intervalTime;
  int cursor = 3;
  tft.fillScreen(ST77XX_BLACK);
  refreshSetIntScreen(cursor, num);

  while(rocker.isAny) {
    getRocker();
  }
  while(1) {
    getRocker();
    if(rocker.isAny) {
      if(rocker.sw) {
        scanTask.intervalTime = num;
        break;
      }
      if(rocker.x) {
        cursor += rocker.x;
        if(cursor > 3) {
          cursor = 0;
        }
        if(cursor < 0) {
          cursor = 3;
        }
      }
      if(rocker.y > 0) {
        num += pow(10, 3-cursor);
        if(num > 9999) {
          num -= 10000;
        }
      }
      if(rocker.y < 0) {
        num -= pow(10, 3-cursor);
        if(num < 0) {
          num += 10000;
        }
      }
      refreshSetIntScreen(cursor, num);
      while(rocker.isAny) {
        getRocker();
      }
    }
  }
}

void refreshSetIntScreen(int cursor, int num) {
  uint8_t w = 30;
  uint8_t h = 45;
  uint8_t r = 7;
  uint16_t color_selected = tft.color565(120, 120, 255);
  uint16_t color_unselected = tft.color565(220, 220, 255);

  //basic rectangle
  tft.fillScreen(tft.color565(150, 180, 200));
  tft.fillRoundRect(5, 58, w, h, r, cursor == 0 ? color_selected : color_unselected);
  tft.fillRoundRect(45, 58, w, h, r, cursor == 1 ? color_selected : color_unselected);
  tft.fillRoundRect(85, 58, w, h, r, cursor == 2 ? color_selected : color_unselected);
  tft.fillRoundRect(125, 58, w, h, r, cursor == 3 ? color_selected : color_unselected);

  tft.setTextColor(tft.color565(60, 60, 80));

  //basic text
  tft.setTextSize(2);
  tft.setCursor(25, 10);
  tft.print("Interval");
  tft.setCursor(30, 32);
  tft.print("Time (s)");

  //data text
  tft.setTextColor(tft.color565(60, 60, 80));
  tft.setTextSize(3);
  tft.setCursor(13, 70);
  tft.print(int(num / 1000));
  tft.setCursor(53, 70);
  tft.print(int(num / 100) % 10);
  tft.setCursor(93, 70);
  tft.print(int(num / 10) % 10);
  tft.setCursor(133, 70);
  tft.print(num % 10);
}

void setDur() {
  int num = scanTask.totalDuration;
  int cursor = 2;
  tft.fillScreen(ST77XX_BLACK);
  refreshSetDurScreen(cursor, num);

  while(rocker.isAny) {
    getRocker();
  }
  while(1) {
    getRocker();
    if(rocker.isAny) {
      if(rocker.sw) {
        break;
      }
      if(rocker.x) {
        cursor += rocker.x;
        if(cursor > 2) {
          cursor = 0;
        }
        if(cursor < 0) {
          cursor = 2;
        }
      }
      if(rocker.y > 0) {
        if(scanTask.mode == AUTO_NUM) {
          scanTask.numOfPic += ceil((float)pow(60, 2-cursor) / scanTask.intervalTime);
          num = (long)scanTask.numOfPic * scanTask.intervalTime;
        } else if(scanTask.mode == AUTO_INT) {
          scanTask.intervalTime += ceil((float)pow(60, 2-cursor) / scanTask.numOfPic);
          num = (long)scanTask.numOfPic * scanTask.intervalTime;
        }
      }
      if(rocker.y < 0) {
        if(scanTask.mode == AUTO_NUM) {
          scanTask.numOfPic -= ceil((float)pow(60, 2-cursor) / scanTask.intervalTime);
          num = (long)scanTask.numOfPic * scanTask.intervalTime;
        } else if(scanTask.mode == AUTO_INT) {
          scanTask.intervalTime -= ceil((float)pow(60, 2-cursor) / scanTask.numOfPic);
          num = (long)scanTask.numOfPic * scanTask.intervalTime;
        }
      }
      refreshSetDurScreen(cursor, num);
      while(rocker.isAny) {
        getRocker();
      }
    }
  }
}

void refreshSetDurScreen(int cursor, long num) {
  uint8_t w = 55;
  uint8_t h = 40;
  uint8_t r = 7;
  uint16_t color_selected = tft.color565(120, 120, 255);
  uint16_t color_unselected = tft.color565(220, 220, 255);

  //basic rectangle
  tft.fillScreen(tft.color565(150, 180, 200));
  tft.fillRoundRect(5, 30, w, h, r, cursor == 0 ? color_selected : color_unselected);
  tft.fillRoundRect(5, 80, w, h, r, cursor == 1 ? color_selected : color_unselected);
  tft.fillRoundRect(85, 80, w, h, r, cursor == 2 ? color_selected : color_unselected);

  tft.setTextColor(tft.color565(60, 60, 80));

  //basic text
  tft.setTextSize(2);
  tft.setCursor(33, 8);
  tft.print("Duration");
  tft.setCursor(65, 43);
  tft.print("h");
  tft.setCursor(65, 93);
  tft.print("m");
  tft.setCursor(145, 93);
  tft.print("s");

  //data text
  tft.setTextColor(tft.color565(60, 60, 80));
  tft.setTextSize(3);
  tft.setCursor(13, 40);
  tft.print(int(num / 3600));
  tft.setCursor(13, 90);
  tft.print(int(num / 60) % 60);
  tft.setCursor(93, 90);
  tft.print(num % 60);
}

void setMode() {
  MODE mode = scanTask.mode;
  tft.fillScreen(ST77XX_BLACK);
  refreshSetModeScreen(mode);

  while(rocker.isAny) {
    getRocker();
  }
  while(1) {
    getRocker();
    if(rocker.isAny) {
      if(rocker.sw) {
        scanTask.mode = mode;
        break;
      }
      if(rocker.y) {
        mode = (int)mode - rocker.y;
        if(mode > 2) {
          mode = 0;
        }
        if(mode < 0) {
          mode = 2;
        }
      }
      refreshSetModeScreen(mode);
      while(rocker.isAny) {
        getRocker();
      }
    }
  }
}

void refreshSetModeScreen(MODE mode) {
  uint8_t w = 160;
  uint8_t h = 30;
  uint8_t r = 7;
  uint16_t color_selected = tft.color565(120, 120, 255);
  uint16_t color_unselected = tft.color565(220, 220, 255);

  //basic rectangle
  tft.fillRoundRect(0, 25, w, h, r, mode == 0 ? color_selected : color_unselected);
  tft.fillRoundRect(0, 60, w, h, r, mode == 1 ? color_selected : color_unselected);
  tft.fillRoundRect(0, 95, w, h, r, mode == 2 ? color_selected : color_unselected);
  
  tft.setTextColor(tft.color565(60, 60, 80));

  //text
  tft.setTextSize(2);
  tft.setCursor(45, 0);
  tft.print("Mode");

  tft.setTextSize(2);
  tft.setCursor(3, 28);
  tft.print("Auto Num");
  tft.setCursor(3, 63);
  tft.print("Auto Int");
  tft.setCursor(3, 98);
  tft.print("Auto Dur");
}

void startTask() {
  for(int i = 0; i < scanTask.numOfPic; ++i) {
    refreshStartTaskScreen(i+1);
    servo.write(75);
    delay(200);
    servo.write(65);

    getRocker();
    if(rocker.isAny) {
      break;
    }

    delay(1000*scanTask.intervalTime - 200);

    getRocker();
    if(rocker.isAny) {
      break;
    }
  }
}

void refreshStartTaskScreen(int num) {
  uint8_t disable = scanTask.mode;
  uint8_t w = 75;
  uint8_t h = 50;
  uint8_t r = 7;
  uint16_t color_unselected = tft.color565(220, 220, 255);
  uint16_t color_disable = tft.color565(110, 110, 110);

  //basic rectangle
  tft.fillRoundRect(0, 0, w, h, r, disable == 0 ? color_disable : color_unselected);
  tft.fillRoundRect(85, 0, w, h, r, disable == 1 ? color_disable : color_unselected);
  tft.fillRoundRect(0, 54, w, h, r, disable == 2 ? color_disable : color_unselected);
  tft.fillRoundRect(85, 54, w, h, r, color_unselected);
  tft.fillRoundRect(0, 108, 160, 20, r, tft.color565(80, 255, 80));
  
  tft.setTextColor(tft.color565(60, 60, 80));

  //basic text
  tft.setTextSize(1);
  tft.setCursor(28, 3);
  tft.print("Num");
  tft.setCursor(115, 3);
  tft.print("Int");
  tft.setCursor(28, 57);
  tft.print("Dur");
  tft.setCursor(110, 57);
  tft.print("Mode");

  tft.setTextSize(2);
  tft.setCursor(60, 110);
  tft.print(num);

  //data text
  //number of pictures
  if(scanTask.numOfPic > 999) {
    tft.setTextSize(2);
    tft.setCursor(13, 20);
    tft.print(scanTask.numOfPic);
  } else {
    tft.setTextSize(3);
    tft.setCursor(10, 17);
    tft.print(scanTask.numOfPic);
  }

  //interval time
  if(scanTask.intervalTime / 60 >= 1) {
    tft.setCursor(88, 21);
    tft.setTextSize(2);
    tft.print((int)floor(scanTask.intervalTime / 60) % 60);
    tft.print("m");
    tft.print(scanTask.intervalTime % 60);
    tft.print("s");
  } else {
    tft.setCursor(95, 17);
    tft.setTextSize(3);
    tft.print(scanTask.intervalTime);
    tft.print("s");
  }
  
  //total duration
  if(scanTask.totalDuration >= 360000) {
    tft.setCursor(3, 75);
    tft.setTextSize(2);
    tft.print(int(scanTask.totalDuration / 3600));
    tft.print("h");
  } else if(scanTask.totalDuration >= 3600) {
    tft.setCursor(3, 75);
    tft.setTextSize(2);
    tft.print(int(scanTask.totalDuration / 3600));
    tft.print("h");
    tft.print(int(scanTask.totalDuration / 60) % 60);
    tft.print("m");
    // tft.print(scanTask.totalDuration % 60);
    // tft.print("s");
  } else if(scanTask.totalDuration >= 60) {
    tft.setCursor(3, 75);
    tft.setTextSize(2);
    tft.print(int(scanTask.totalDuration / 60) % 60);
    tft.print("m");
    tft.print(scanTask.totalDuration % 60);
    tft.print("s");
  } else {
    tft.setCursor(10, 71);
    tft.setTextSize(3);
    tft.print(scanTask.totalDuration);
    tft.print("s");
  }

  tft.setTextSize(2);
  tft.setCursor(105, 75);
  switch(scanTask.mode) {
    case AUTO_NUM:
      tft.print("Num");
      break;
    case AUTO_INT:
      tft.print("Int");
      break;
    case AUTO_DUR:
      tft.print("Dur");
      break;
  }


}

//test functions
void numUpDownTest() {
  refreshTestScreen();
  int tmp = 0;
  while(1) {
    tmp = getRockerY();
    if(tmp != 0 ) {
      num += tmp;
      refreshTestScreen();
      while(getRockerY() != 0) {}
    }
    if(getRockerSwitch() == true) {
      Serial.println("Exit");
      break;
    }
  }
}

void refreshTestScreen() {
  tft.setCursor(60,50);
  tft.print("     ");
  tft.setCursor(60,50);
  tft.print(num);
}

//basic rocker functions
int getRockerX() {
  int tmp = analogRead(A0);
  if(tmp > 900) {
    return 1;
  } else if(tmp < 100) {
    return -1;
  } else {
    return 0;
  }
}

int getRockerY() {
  int tmp = 1024-analogRead(A1);
  if(tmp > 900) {
    return 1;
  } else if(tmp < 100) {
    return -1;
  } else {
    return 0;
  }
}

bool getRockerSwitch() {
  return analogRead(A2)<500 ? true : false;
}

void getRocker() {
  rocker.x = getRockerX();
  rocker.y = getRockerY();
  rocker.sw = getRockerSwitch();
  rocker.isAny = (rocker.x!=0) || (rocker.y!=0) || rocker.sw;
}

//other functions
int pow(int x, int y) {
  int t = 1;
  while(y > 0) {
    t *= x;
    --y;
  }
  return t;
}
