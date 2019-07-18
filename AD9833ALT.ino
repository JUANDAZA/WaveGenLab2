#include <MD_AD9833.h>

const long BAUDRATE = 115200; // Baud rate of UART in bps

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
int counter = 0;
const int SG_fsyncPin = 4; //Pin sync of AD9833, stablish a communication with  arduino
const int SG_CLK = 3;	  //FlipFlop of the AD9833, init registers.
const int SG_DATA = 2;	 //Insert data bytes into AD9833,

const int SG_fsyncPin2 = 10;
const int SG_CLK2 = 9;
const int SG_DATA2 = 8;

MD_AD9833 ch1 = MD_AD9833(SG_DATA, SG_CLK, SG_fsyncPin); //Instantiates channel 1, creates a connection between MD_AD9833 library and the real AD9833 module
MD_AD9833 ch2 = MD_AD9833(SG_DATA2, SG_CLK2, SG_fsyncPin2);

const int CH_SEL_BTN = 11; // Channel selector button
const int WF_SEL_BTN = A0; // Waveform selector button
const int freq_sel = A1;   //  if it is pressed, the increase and decrease buttons changes the frequency
const int phase_sel = A2;  // if it is pressed, the increase and decrease buttons changes the phase
const int mux_ch1 = A3;	//[channel1] 1: the multiplexer output is the 4th wave (truncated sawtooth), 0: multiplexer output is any wave of (square, sin, triangle)
const int mux_ch2 = A4;	//[channel2] 1: the multiplexer output is the 4th wave (truncated sawtooth), 0: multiplexer output is any wave of (square, sin, triangle)

int F_ch1 = 1000; //Channel1 frecuency
int F_ch2 = 1000; //Channel2 frecuency

int P_ch1 = 0; // Channel1 phase
int P_ch2 = 0; // Channel2 phase

int freq_or_phase = true; //Save the state of the frequency or phase, true: the last button was frequency, false: the last button was phase.

int wf_ch1 = 0; //[CHANNEL1] waveform selector variable 0: sin, 1: triangle, 2: square, 3: sawtooth
int wf_ch2 = 0; //[CHANNEL2] waveform selector variable 0: sin, 1: triangle, 2: square, 3: sawtooth

const int freq_add = 13; //increase frecuency/phase depending on the selected mode.
const int freq_dec = 12; //decrease frecuency/phase depending on the selected mode.

const int led_ch1 = 5; // led channel indicator
const int led_ch2 = 6; // led channel indicator
int ch_sel = 0;		   //Channel selector variable

//-----------------------------------------------------------------------------
// Main routines
// The setup function
//-----------------------------------------------------------------------------
void setup(void)
{
	// Open serial port with a baud rate of BAUDRATE b/s
	Serial.begin(BAUDRATE);

	Serial.println("SigGen " __DATE__); // compilation date
	Serial.println("OK");
	// Pin initialization

	pinMode(CH_SEL_BTN, INPUT);
	pinMode(led_ch1, OUTPUT);
	pinMode(led_ch2, OUTPUT);
	pinMode(freq_add, INPUT);
	pinMode(freq_dec, INPUT); //
	pinMode(WF_SEL_BTN, INPUT);
	pinMode(freq_sel, INPUT);
	pinMode(phase_sel, INPUT);
	pinMode(mux_ch1, OUTPUT);
	pinMode(mux_ch2, OUTPUT);
	digitalWrite(led_ch1, HIGH);
	digitalWrite(led_ch2, LOW);
	ch1.begin(); //Initialise the object data. This needs to be called during setup() to initialise new data for the class that cannot be done during the object creation.
	ch2.begin();
	ch1.setFrequency(ch1.CHAN_0, 1000); //Set the specified AD9833 channel output frequency.

	ch2.setFrequency(ch2.CHAN_0, 1000);
	//result = ch1.setPhase(ch1.CHAN_0, 300);
	ch2.setMode(ch2.MODE_SQUARE1);
	ch1.setMode(ch1.MODE_SINE); //Set the specified AD9833 channel output waveform to one of the mode_t types.
	digitalWrite(mux_ch1, HIGH);
	digitalWrite(mux_ch2, LOW);
}

//-----------------------------------------------------------------------------
// Main routines
// loop
//-----------------------------------------------------------------------------
void loop(void)
{
	if (counter == 155)
	{
		counter = 0;
		P_ch2 -= 1;
		ch2.setPhase(ch2.CHAN_0, P_ch2);
		if (P_ch2 == 0)
			P_ch2 = 3600;
	}
	counter++;
	int sel_state = digitalRead(CH_SEL_BTN); //Channel number, switch between channel1 and channel2
	int add_state = digitalRead(freq_add); //Increase magnitude frequency/phase    
	int dec_state = digitalRead(freq_dec); //decrease magnitude frequency/phase 
	int wf_state = digitalRead(WF_SEL_BTN); //reads the state of the waveform button to iterates and select over the waveforms 
	int freq_state = digitalRead(freq_sel); //reads the state of the frequency button if it is the last pressed, the user can increase/decrease the frecuency
	int phase_state = digitalRead(phase_sel); //reads the state of the frequency button if it is the last pressed, the user can increase/decrease the frecuency
	if (sel_state == HIGH)
	{
		if (ch_sel == 0)
		{
			ch_sel = 1;
			digitalWrite(led_ch1, LOW);
			digitalWrite(led_ch2, HIGH);
		}
		else if (ch_sel == 1)
		{
			ch_sel = 2;
			digitalWrite(led_ch1, HIGH);
			digitalWrite(led_ch2, HIGH);
		}
		else
		{
			ch_sel = 0;
			digitalWrite(led_ch1, HIGH);
			digitalWrite(led_ch2, LOW);
		}
		do
		{
			sel_state = digitalRead(CH_SEL_BTN);
		} while (sel_state == HIGH);
	}
	if (wf_state == HIGH)
	{
		if (ch_sel == 0)
		{
			wf_ch1++;
			if (wf_ch1 == 4)
				wf_ch1 = 0;
			if (wf_ch1 == 0)
				ch1.setMode(ch1.MODE_SINE);
			if (wf_ch1 == 1)
				ch1.setMode(ch1.MODE_TRIANGLE);
			if (wf_ch1 == 2)
				ch1.setMode(ch1.MODE_SQUARE1);
			if (wf_ch1 == 3)
			{
				ch1.setMode(ch1.MODE_SQUARE1);
				digitalWrite(mux_ch1, HIGH);
			}
			else
				digitalWrite(mux_ch1, LOW);
		}
		else if (ch_sel == 1)
		{
			wf_ch2++;
			if (wf_ch2 == 4)
				wf_ch2 = 0;
			if (wf_ch2 == 0)
				ch2.setMode(ch2.MODE_SINE);
			if (wf_ch2 == 1)
				ch2.setMode(ch2.MODE_TRIANGLE);
			if (wf_ch2 == 2)
				ch2.setMode(ch2.MODE_SQUARE1);
			if (wf_ch1 == 3)
			{
				ch2.setMode(ch2.MODE_SQUARE1);
				digitalWrite(mux_ch2, HIGH);
			}
			else
				digitalWrite(mux_ch2, LOW);
		}

		do
		{
			wf_state = digitalRead(WF_SEL_BTN);
		} while (wf_state == HIGH);
	}

	if (add_state == HIGH)
	{
		if (ch_sel == 0)
		{
			if (freq_or_phase)
			{
				if (F_ch1 < 100)
					F_ch1 += 10;
				else if (F_ch1 > 100)
					F_ch1 += 100;
				else if (F_ch1 > 100 && F_ch1 < 4000)
					F_ch1 += 100;
				else if (F_ch1 >= 4000 && F_ch1 < 15000)
					F_ch1 += 500;
				else if (F_ch1 >= 20000 && F_ch1 < 100000)
					F_ch1 += 10000;
				else if (F_ch1 > 100000 && F_ch1 <= 1000000)
					F_ch1 += 100000;
				else if (F_ch1 > 1000000 && F_ch1 < 5000000)
					F_ch1 += 1000000;

				Serial.println(F_ch1);
				ch1.setFrequency(ch1.CHAN_0, F_ch1);
			}
			else
			{
				P_ch1 += 100;
				if (P_ch1 >= 3600)
					P_ch1 -= 3600;
				ch1.setPhase(ch1.CHAN_0, P_ch1);
				ch1.setActivePhase(ch1.CHAN_0);
			}
		}
		else if (ch_sel == 1)
		{
			if (freq_or_phase)
			{
				if (F_ch1 < 100)
					F_ch2 += 10;
				else if (F_ch2 >= 100 && F_ch2 < 4000)
					F_ch2 += 100;
				else if (F_ch2 >= 4000 && F_ch2 < 15000)
					F_ch2 += 500;
				else if (F_ch2 >= 20000 && F_ch2 < 100000)
					F_ch2 += 10000;
				else if (F_ch2 >= 100000 && F_ch2 < 1000000)
					F_ch2 += 100000;
				else if (F_ch2 >= 1000000 && F_ch2 < 5000000)
					F_ch2 += 1000000;
				Serial.println(F_ch2);
				ch2.setFrequency(ch2.CHAN_0, F_ch2);
			}
			else
			{
				if (P_ch2 >= 3600)
					P_ch2 -= 3600;
				P_ch2 += 100;
				ch2.setPhase(ch2.CHAN_0, P_ch2);
				ch2.setActivePhase(ch2.CHAN_0);
			}
		}
		delay(200);
		add_state = digitalRead(freq_add);
	} //

	if (dec_state == HIGH)
	{
		if (ch_sel == 0)
		{
			if (freq_or_phase)
			{
				if (F_ch1 > 1000000 && F_ch2 <= 5000000)
					F_ch1 -= 1000000;
				else if (F_ch1 > 100000 && F_ch1 <= 1000000)
					F_ch1 -= 100000;
				else if (F_ch1 > 20000 && F_ch1 <= 100000)
					F_ch1 -= 10000;
				else if (F_ch1 > 4000 && F_ch1 <= 15000)
					F_ch1 -= 500;
				else if (F_ch1 > 100 && F_ch1 <= 4000)
					F_ch1 -= 100;
				else if (F_ch1 > 40 && F_ch1 <= 100)
					F_ch1 -= 10;
				Serial.println(F_ch1);
				ch1.setFrequency(ch1.CHAN_0, F_ch1);
			}
			else
			{
				P_ch1 -= 100;
				if (P_ch1 < 0)
					P_ch1 += 3600;
				ch1.setPhase(ch1.CHAN_0, P_ch1);
				ch1.setActivePhase(ch1.CHAN_0);
			}
		}
		else if (ch_sel == 1)
		{
			if (freq_or_phase)
			{
				if (F_ch2 > 1000000 && F_ch2 <= 5000000)
					F_ch2 -= 1000000;
				else if (F_ch2 > 100000 && F_ch2 <= 1000000)
					F_ch2 -= 100000;
				else if (F_ch2 > 20000 && F_ch2 <= 100000)
					F_ch2 -= 10000;
				else if (F_ch2 > 4000 && F_ch2 <= 15000)
					F_ch2 -= 500;
				else if (F_ch2 > 100 && F_ch2 <= 4000)
					F_ch2 -= 100;
				else if (F_ch2 > 40 && F_ch2 <= 100)
					F_ch2 -= 10;
				Serial.println(F_ch2);
				ch2.setFrequency(ch2.CHAN_0, F_ch2);
			}
			else
			{
				P_ch2 += 100;
				if (P_ch2 >= 3600)
					P_ch2 -= 3600;
				ch2.setPhase(ch2.CHAN_0, P_ch2);
				ch2.setActivePhase(ch2.CHAN_0);
			}
		}
		delay(160);
		dec_state = digitalRead(freq_dec);
	}
	if (freq_state == HIGH)
	{
		freq_or_phase = true;
		do
		{
			freq_state = digitalRead(freq_sel);
		} while (freq_state == HIGH);
	}
	if (phase_state == HIGH)
	{
		freq_or_phase = false;
		do
		{
			phase_state = digitalRead(phase_sel);
		} while (phase_state == HIGH);
	}
}
