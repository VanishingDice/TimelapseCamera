Introduction video on [Bilibili](https://www.bilibili.com/video/BV1rCYEewEnP/)<br>
Hardware opensource on [OSHWHub](https://oshwhub.com/vanishingdice/long-time-camera)<br>

For arduino uno/nano board<br>

*IMPORTANT NOTICE:
==================
For some of the ST7735 green tab screen, you have to change the library file for proper color showing<br>
See details on [Here](https://github.com/adafruit/Adafruit-ST7735-Library/issues/154)<br>
<br>
>Seems like the RGB coding that this screen expects is ST77XX_MADCTL_RGB instead of ST7735_MADCTL_BGR
>Initializing it with BLACKTAB works for color but gets bottom and right edges wrong.
>Initializing it with GREENTAB works for edges but not for colors
>
>Seems like people are referring to these screens as YELLOWTAB ... anyway these ones seem to be a little bit annoying to use.
>
>In order to solve out the problem I had to modify the library file "Adafruit_ST7735.cpp" and change these lines :
```
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
```

Linking:
--------
TFT_CS&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;10<br>
TFT_RST&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;9<br>
TFT_DC&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;8<br>
MOSI&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;11<br>
SCLK&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;13<br>
Servo&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;6<br>
RockerX&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;A0<br>
RockerY&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;A1<br>
RockerSw&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;A2<br>

Abbr.
-----
num: number of pictures<br>
int: interval time<br>
dur: total duraion<br>
