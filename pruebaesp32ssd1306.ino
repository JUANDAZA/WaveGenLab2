#include <Wire.h>
#include "SSD1306.h" 
 
SSD1306  display(0x3c, 21, 22);
int displayBuffer[128];
int samplingBuffer[1024];
int pos = 0;
void setup() {
 
  display.init();
  display.flipScreenVertically();
  display.display();
}
 
void loop() {
   displayBuffer[pos] = analogRead(13);
   delayMicroseconds(900);
   pos++;
   if(pos == 128){
    int max_signal=0;
    for(int i=0; i < pos; i++){
        if(displayBuffer[i]>max_signal){
            max_signal=displayBuffer[i];
        }
    }
    int prev_pixel;
    for(int i=0; i<pos;i++){
      int pixel = (int) - (displayBuffer[i] * 48 / (max_signal+1)) + 63;
      if(i==0) prev_pixel=pixel;
      display.setPixel(i,pixel);
      if(abs(pixel-prev_pixel) > 1 && abs(pixel-prev_pixel) <= 10){
        display.drawLine(i-1,prev_pixel,i,pixel);
      }
      else if(prev_pixel-pixel > 10){
        display.drawVerticalLine(i,pixel,prev_pixel-pixel);
      }
      else if(pixel-prev_pixel > 10){
        display.drawVerticalLine(i,prev_pixel,pixel-prev_pixel);
      }
      display.setPixel(i,40);
      prev_pixel=pixel;
    }
    display.display();
    delay(1000);
    pos = 0;
    display.resetDisplay();
    display.drawString(0,0,"F=900KHz 50uS/div");
   }
}
