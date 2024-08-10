Introduction video on: www.bilibili.com/video/BV1rCYEewEnP

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

'''
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
'''
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
