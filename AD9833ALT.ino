

#include <math.h>
#include <MD_AD9833.h>


const long BAUDRATE  = 115200;  // Baud rate of UART in bps

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

const int SG_fsyncPin = 4;
const int SG_CLK = 3;
const int SG_DATA = 2;

const int SG_fsyncPin2 = 10;
const int SG_CLK2 = 9;
const int SG_DATA2 = 8;

String waveType = "SINE";
const String waveBuffer1[3] = {"SINE", "TG", "SQ"};

MD_AD9833 ch1 = MD_AD9833(SG_DATA, SG_CLK, SG_fsyncPin);
MD_AD9833 ch2 = MD_AD9833(SG_DATA2, SG_CLK2, SG_fsyncPin2);

const int CH_SEL_BTN = 11;
const int WF_SEL_BTN = A0;

int F_ch1 = 1000;
int F_ch2 = 1000;

int P_ch1 = 0;
int P_ch2 = 0;

int wf_ch1 = 0;
int wf_ch2 = 0;

const int freq_add = 13;
const int freq_dec = 12; 

const int led_ch1 = 5;
const int led_ch2 = 6;
bool ch_sel = true;

const int SG_POWER = 32;
int SG_iSweep,SG_nSweep;



//-----------------------------------------------------------------------------
// Main routines
// The setup function
//-----------------------------------------------------------------------------
void setup (void) {
  // Open serial port with a baud rate of BAUDRATE b/s
  Serial.begin(BAUDRATE);

  Serial.println("SigGen " __DATE__); // compilation date
  Serial.println("OK");

  pinMode(CH_SEL_BTN, INPUT);
  pinMode(led_ch1,OUTPUT);
  pinMode(led_ch2,OUTPUT);
  pinMode(freq_add, INPUT);
  pinMode(freq_dec, INPUT);
  pinMode(WF_SEL_BTN, INPUT);
  ch1.begin();
  ch2.begin();
  boolean result = ch1.setFrequency(ch1.CHAN_0, (int)(1000*0.976));
  result = ch2.setFrequency(ch2.CHAN_0, 1000); 
  result = ch1.setPhase(ch1.CHAN_0, 300);
  
}

//-----------------------------------------------------------------------------
// Main routines
// loop
//-----------------------------------------------------------------------------
void loop (void) {
  //SerialCommand();
      int sel_state = digitalRead(CH_SEL_BTN);
      int add_state = digitalRead(freq_add);
      int dec_state = digitalRead(freq_dec);
      int wf_state = digitalRead(WF_SEL_BTN);
      if(sel_state == HIGH){
        if(ch_sel) {
          ch_sel = false;
          digitalWrite(led_ch1,HIGH);
          digitalWrite(led_ch2,LOW);
        }
        else {
          ch_sel = true;
          digitalWrite(led_ch1,LOW);
          digitalWrite(led_ch2,HIGH);
         }
        do{sel_state = digitalRead(CH_SEL_BTN);}while(sel_state == HIGH);  
      }
      if(wf_state == HIGH){
        if(ch_sel) {
           wf_ch1++;
           if(wf_ch1==4) wf_ch1=0;
           if(wf_ch1==0) ch1.setMode(ch1.MODE_SINE);
           if(wf_ch1==1) ch1.setMode(ch1.MODE_TRIANGLE);
           if(wf_ch1==2) ch1.setMode(ch1.MODE_SQUARE1); 
           if(wf_ch1==3) ch1.setMode(ch1.MODE_OFF); 
        }
        else {
          wf_ch2++;
          if(wf_ch2==4) wf_ch2=0;
          if(wf_ch1==0) ch2.setMode(ch2.MODE_SINE);
          if(wf_ch1==1) ch2.setMode(ch2.MODE_TRIANGLE);
          if(wf_ch1==2) ch2.setMode(ch2.MODE_SQUARE1); 
          if(wf_ch1==3) ch2.setMode(ch2.MODE_OFF);  
         }
        
        do{wf_state = digitalRead(WF_SEL_BTN);}while(wf_state == HIGH);  
      }
      
      if (add_state == HIGH) {
      Serial.println("holaa");
      if(ch_sel){
        if(F_ch1 <=15000){
         F_ch1 += 100;
         Serial.println(F_ch1);
         //boolean result = ch1.setFrequency(ch1.CHAN_0, (int)(0.976*F_ch1));
         P_ch1 += 300;
         boolean result = ch1.setPhase(ch1.CHAN_0, P_ch1);
         result = ch1.setActivePhase(ch1.CHAN_0);
        }
      }else {
        if(F_ch2 <=15000){
          F_ch2 += 100;
          Serial.println(F_ch2);
          //boolean result = ch2.setFrequency(ch2.CHAN_0, F_ch2); 
          P_ch2 += 300;
          boolean result = ch2.setPhase(ch2.CHAN_0, P_ch2);
          result = ch2.setActivePhase(ch2.CHAN_0);
        }
      }
      delay(200);
      add_state = digitalRead(freq_add);
    }

    if (dec_state == HIGH) {
      if(ch_sel){
        if(F_ch1 >= 40){
         F_ch1 -= 100;
         Serial.println(F_ch1);
         boolean result = ch1.setFrequency(ch1.CHAN_0, (int)(0.976*F_ch1));  
        }
      }else {
        if(F_ch2 >= 40){
          F_ch2 -= 100;
          Serial.println(F_ch2);
          boolean result = ch2.setFrequency(ch2.CHAN_0, F_ch2); 
        }
      }
      delay(160);
      dec_state = digitalRead(freq_dec);
    }
}
