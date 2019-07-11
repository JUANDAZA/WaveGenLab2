#include <Wire.h>
#include "SSD1306.h" 
 
SSD1306  display(0x3c, 21, 22);
int displayBuffer[128];
int ch1_samplingBuffer[8192];
int ch2_samplingBuffer[8192];
int ch3_samplingBuffer[8192];
int pos = 8192;

void fillSB(){
 for (int i=0; i< 8192; i++){
     ch1_samplingBuffer[i]=analogRead(13);
 }
 for (int i=0; i< 8192; i++){
     ch2_samplingBuffer[i]=analogRead(13);
 }
 for (int i=0; i< 8192; i++){
     ch3_samplingBuffer[i]=analogRead(13);
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
   //ch3_samplingBuffer[pos]=analogRead(13);
   //delayMicroseconds(10);
   //pos++;
   fillSB();
   if(pos == 8192){
    int max_signal=0;
    int min_signal=4096;
    for(int i=0; i < pos; i++){
        if(ch3_samplingBuffer[i]>max_signal){
            max_signal=ch3_samplingBuffer[i];
        }
        if(ch3_samplingBuffer[i]<min_signal){
            min_signal=ch3_samplingBuffer[i];  
        }        
    }
    int flag=0;
    int t_min;
    int t_mid;
    int t_max;
    for(int i=0; i < pos; i++){
        if(ch3_samplingBuffer[i]>(min_signal + 0.05*max_signal) && flag==0){
           flag=1; 
        }
        if(ch3_samplingBuffer[i]<=(min_signal) && flag==1){
            t_min=i;
            flag=2;
        }
        if(ch3_samplingBuffer[i]>=(0.90*max_signal) && flag==2){
            t_mid=i;
            flag=3;
        }
        if(ch3_samplingBuffer[i]<=(min_signal) && flag==3){
            t_max=i;
            flag=4; 
        }
    }
    
    float Freq = 88062/(t_max-t_min);
    int period;
    if(Freq < 5000) period = 3;
    else if (Freq >= 5000 && Freq < 8000) period = 6; 
    else if (Freq >= 8000 && Freq < 11000) period = 9;
    else if (Freq >= 11000 && Freq < 15000) period = 12;
    int end_display = (int) t_min + period*(t_max-t_min);
    float scale = (float)(end_display-t_min)/128;
    if (Freq <= 40) {scale=64; t_min=0;}
    for(int i=0; i<128; i++){
        if(scale>1){
            displayBuffer[i]=ch3_samplingBuffer[(int)scale*i+t_min];      
        }
        else{
            displayBuffer[i]=ch3_samplingBuffer[(int)(i*scale)+t_min];  
        }
        
    }
    display.resetDisplay();
    int prev_pixel;
    for(int i=0; i<128;i++){
      int pixel = (int) - (displayBuffer[i] * 48 / (max_signal+1)) + 63;
      if(i==0) prev_pixel=pixel;
      display.setPixel(i,pixel);
      if(abs(pixel-prev_pixel) > 1 && abs(pixel-prev_pixel) <= 25){
        display.drawLine(i-1,prev_pixel,i,pixel);
      }
      else if(prev_pixel-pixel > 25){
        display.drawVerticalLine(i,pixel,prev_pixel-pixel);
      }
      else if(pixel-prev_pixel > 25){
        display.drawVerticalLine(i,prev_pixel,pixel-prev_pixel);
      }
      display.setPixel(i,40);
      prev_pixel=pixel;
    }
    int vppInt = (int)((max_signal-min_signal)*330/4096);
    float vpp = vppInt/100; 
    //display.drawString(0,0,"F="+String(88062/(t_max-t_min))+"Hz Vpp="+String(scale));//(float(vppInt)/100)+"V");
    display.drawString(0,0,"F="+String(88062/(t_max-t_min))+"Hz Vpp="+(float(vppInt)/100)+"V");
    display.display();
    delay(1000);
    pos = 8192;

   }
}
