#include <MD_AD9833.h>

const long BAUDRATE  = 115200;  // Baud rate of UART in bps

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
int counter = 0;
const int SG_fsyncPin = 4;
const int SG_CLK = 3;
const int SG_DATA = 2;

const int SG_fsyncPin2 = 10;
const int SG_CLK2 = 9;
const int SG_DATA2 = 8;

MD_AD9833 ch1 = MD_AD9833(SG_DATA, SG_CLK, SG_fsyncPin);
MD_AD9833 ch2 = MD_AD9833(SG_DATA2, SG_CLK2, SG_fsyncPin2);

const int CH_SEL_BTN = 11;
const int WF_SEL_BTN = A0;
const int freq_sel = A1;
const int phase_sel = A2;
const int mux_ch1 = A3;
const int mux_ch2 = A4;

int F_ch1 = 1000;
int F_ch2 = 1000;

int P_ch1 = 0;
int P_ch2 = 0;

int freq_or_phase = true;

int wf_ch1 = 0;
int wf_ch2 = 0;

const int freq_add = 13;
const int freq_dec = 12; 

const int led_ch1 = 5;
const int led_ch2 = 6;
int ch_sel = 0;



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
  pinMode(freq_sel, INPUT);
  pinMode(phase_sel, INPUT);
  pinMode(mux_ch1, OUTPUT);
  pinMode(mux_ch2, OUTPUT);
  digitalWrite(led_ch1,HIGH);
  digitalWrite(led_ch2,LOW);
  ch1.begin();
  ch2.begin();
  ch1.setFrequency(ch1.CHAN_0, 1000);
  ch2.setFrequency(ch2.CHAN_0, 1000); 
  //result = ch1.setPhase(ch1.CHAN_0, 300);
  ch2.setMode(ch2.MODE_SQUARE1);
  ch1.setMode(ch1.MODE_SINE);
  digitalWrite(mux_ch1,HIGH);
  digitalWrite(mux_ch2,LOW);
}

//-----------------------------------------------------------------------------
// Main routines
// loop
//-----------------------------------------------------------------------------
void loop (void) {
      if(counter == 155){
        counter = 0;
        P_ch2 -= 1;
        ch2.setPhase(ch2.CHAN_0, P_ch2);
        if(P_ch2==0) P_ch2=3600;
      }
      counter++;      
      int sel_state = digitalRead(CH_SEL_BTN);
      int add_state = digitalRead(freq_add);
      int dec_state = digitalRead(freq_dec);
      int wf_state = digitalRead(WF_SEL_BTN);
      int freq_state = digitalRead(freq_sel);
      int phase_state = digitalRead(phase_sel);
      if(sel_state == HIGH){
        if(ch_sel==0) {
          ch_sel = 1;
          digitalWrite(led_ch1,LOW);
          digitalWrite(led_ch2,HIGH);
        }
        else if(ch_sel==1){
          ch_sel = 2;
          digitalWrite(led_ch1,HIGH);
          digitalWrite(led_ch2,HIGH);
         }
         else{
          ch_sel = 0;
          digitalWrite(led_ch1,HIGH);
          digitalWrite(led_ch2,LOW);
         }
        do{sel_state = digitalRead(CH_SEL_BTN);}while(sel_state == HIGH);  
      }
      if(wf_state == HIGH){
        if(ch_sel==0) {
           wf_ch1++;
           if(wf_ch1==4) wf_ch1=0;
           if(wf_ch1==0) ch1.setMode(ch1.MODE_SINE);
           if(wf_ch1==1) ch1.setMode(ch1.MODE_TRIANGLE);
           if(wf_ch1==2) ch1.setMode(ch1.MODE_SQUARE1);
           if(wf_ch1==3) {
            ch1.setMode(ch1.MODE_SQUARE1);
            digitalWrite(mux_ch1, HIGH);
           }else digitalWrite(mux_ch1, LOW);
        }
        else if(ch_sel==1){
          wf_ch2++;
          if(wf_ch2==4) wf_ch2=0;
          if(wf_ch2==0) ch2.setMode(ch2.MODE_SINE);
          if(wf_ch2==1) ch2.setMode(ch2.MODE_TRIANGLE);
          if(wf_ch2==2) ch2.setMode(ch2.MODE_SQUARE1);
          if(wf_ch1==3) {
            ch2.setMode(ch2.MODE_SQUARE1);
            digitalWrite(mux_ch2, HIGH);
           }else digitalWrite(mux_ch2, LOW);
         }
        
        do{wf_state = digitalRead(WF_SEL_BTN);}while(wf_state == HIGH);  
      }
      
      if (add_state == HIGH) {
      Serial.println("holaa");
      if(ch_sel==0){
        if(freq_or_phase){
          if(F_ch1 < 100) F_ch1 += 10;
          else if(F_ch1 > 100) F_ch1 += 100;
          else if(F_ch1 > 100 && F_ch1 <4000) F_ch1 += 100;
          else if(F_ch1 >= 4000 && F_ch1 < 15000) F_ch1 += 500;
          else if(F_ch1 >= 20000 && F_ch1 < 100000) F_ch1 += 10000;
          else if(F_ch1 > 100000 && F_ch1 <=1000000) F_ch1 += 100000;
          else if(F_ch1 > 1000000 && F_ch1 <5000000) F_ch1 += 1000000;
            
         Serial.println(F_ch1);
         ch1.setFrequency(ch1.CHAN_0, F_ch1);
        
        }
        else{
          P_ch1 += 100;
          if(P_ch1 >= 3600)P_ch1 -= 3600;
          ch1.setPhase(ch1.CHAN_0, P_ch1);
          ch1.setActivePhase(ch1.CHAN_0);
        }
        
      }else if(ch_sel==1){
        if(freq_or_phase){
          if(F_ch1 < 100) F_ch2 += 10;
          else if(F_ch2 >= 100 && F_ch2 <4000) F_ch2 += 100;
          else if(F_ch2 >= 4000 && F_ch2 < 15000) F_ch2 += 500;
          else if(F_ch2 >= 20000 && F_ch2 < 100000) F_ch2 += 10000;
          else if(F_ch2 >= 100000 && F_ch2 < 1000000) F_ch2 += 100000;
          else if(F_ch2 >= 1000000 && F_ch2 <5000000) F_ch2 += 1000000;
          Serial.println(F_ch2);
          ch2.setFrequency(ch2.CHAN_0, F_ch2); 
        
        }
        else{
          if(P_ch2 >= 3600)P_ch2 -= 3600;
          P_ch2 += 100;
          ch2.setPhase(ch2.CHAN_0, P_ch2);
          ch2.setActivePhase(ch2.CHAN_0);
        }
      }
      delay(200);
      add_state = digitalRead(freq_add);
    }

    if (dec_state == HIGH) {
      if(ch_sel == 0){
        if(freq_or_phase){
         if(F_ch1 > 1000000 && F_ch2 <=5000000) F_ch1 -= 1000000;
         else if(F_ch1 > 100000 && F_ch1 <=1000000) F_ch1 -= 100000;
         else if(F_ch1 > 20000 && F_ch1 <= 100000) F_ch1 -= 10000;
         else if(F_ch1 > 4000 && F_ch1 <= 15000) F_ch1 -= 500;
         else if(F_ch1 > 100 && F_ch1 <= 4000) F_ch1 -= 100;
         else if(F_ch1 > 40 && F_ch1 <= 100) F_ch1 -= 10;
         Serial.println(F_ch1);
         ch1.setFrequency(ch1.CHAN_0, F_ch1);  
        
        }
        else{
          P_ch1 -= 100;
          if(P_ch1 < 0) P_ch1 += 3600;
          ch1.setPhase(ch1.CHAN_0, P_ch1);
          ch1.setActivePhase(ch1.CHAN_0);
        }
        
      }else if(ch_sel == 1){
        if(freq_or_phase){
          if(F_ch2 > 1000000 && F_ch2 <=5000000) F_ch2 -= 1000000;
          else if(F_ch2 > 100000 && F_ch2 <=1000000) F_ch2 -= 100000;
          else if(F_ch2 > 20000 && F_ch2 <= 100000) F_ch2 -= 10000;
          else if(F_ch2 > 4000 && F_ch2 <= 15000) F_ch2 -= 500;
          else if(F_ch2 > 100 && F_ch2 <= 4000) F_ch2 -= 100;
          else if(F_ch2 > 40 && F_ch2 <= 100) F_ch2 -= 10;
          Serial.println(F_ch2);
          ch2.setFrequency(ch2.CHAN_0, F_ch2); 
        }
        else{
          P_ch2 += 100;
          if(P_ch2 >= 3600)P_ch2 -= 3600;
          ch2.setPhase(ch2.CHAN_0, P_ch2);
          ch2.setActivePhase(ch2.CHAN_0);
        }
        }
      delay(160);
      dec_state = digitalRead(freq_dec);
    }
    if(freq_state == HIGH){
      freq_or_phase = true;
      do{freq_state = digitalRead(freq_sel);}while(freq_state == HIGH);  
    }
    if(phase_state == HIGH){
      freq_or_phase = false;
      do{phase_state = digitalRead(phase_sel);}while(phase_state == HIGH); 
    }
}
