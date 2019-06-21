#include <Wire.h>
#include "SSD1306.h" 
 
SSD1306  display(0x3c, 21, 22);
int displayBuffer[128];
int samplingBuffer[2048];
int pos = 2048;

void fillSB(){
 for (int i=0; i<2048; i++){
     samplingBuffer[i]=analogRead(13);
 }
 return; 
}

void setup() {
 
  display.init();
  display.flipScreenVertically();
  display.display();
}


 
void loop() {
   //displayBuffer[pos] = analogRead(13);
   //samplingBuffer[pos]=analogRead(13);
   //delayMicroseconds(10);
   //pos++;
   fillSB();
   if(pos == 2048){
    int max_signal=0;
    int min_signal=4096;
    for(int i=0; i < pos; i++){
        if(samplingBuffer[i]>max_signal){
            max_signal=samplingBuffer[i];
        }
        if(samplingBuffer[i]<min_signal){
            min_signal=samplingBuffer[i];  
        }        
    }
    int flag=0;
    int t_min;
    int t_mid;
    int t_max;
    for(int i=0; i < pos; i++){
        if(samplingBuffer[i]>(min_signal + 0.05*max_signal) && flag==0){
           flag=1; 
        }
        if(samplingBuffer[i]<=(min_signal) && flag==1){
            t_min=i;
            flag=2;
        }
        if(samplingBuffer[i]>=(0.90*max_signal) && flag==2){
            t_mid=i;
            flag=3;
        }
        if(samplingBuffer[i]<=(min_signal) && flag==3){
            t_max=i;
            flag=4; 
        }
    }
    int init_display = (int) (t_min + t_max)/2;
    int end_display = (int) init_display + 3*(t_max-t_min);
    float scale = (end_display-init_display)/128;
    for(int i=0; i<128; i++){
        
        displayBuffer[i]=samplingBuffer[i+t_min];
    }
    display.resetDisplay();
    int prev_pixel;
    for(int i=0; i<128;i++){
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
    int vppInt = (int)((max_signal-min_signal)*330/4096);
    float vpp = vppInt/100; 
    display.drawString(0,0,"F="+String(88062/(t_max-t_min))+"Hz Vpp="+(float(vppInt)/100)+"V");
    display.display();
    delay(1000);
    pos = 2048;

   }
}
