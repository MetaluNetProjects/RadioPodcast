/*********************************************************************
 *               analog example for Versa2.0
 *	Analog capture on connectors K1, K2, K3 and K5. 
 *********************************************************************/

#define BOARD Versa2

#include <fruit.h>
#include <analog.h>
#include <switch.h>

t_delay mainDelay;

void setup(void) {	
//----------- Setup ----------------
	fruitInit();
			
	pinModeDigitalOut(LED); 	// set the LED pin mode to digital out
	digitalClear(LED);		// clear the LED
	delayStart(mainDelay, 5000); 	// init the mainDelay to 5 ms

//----------- Analog setup ----------------
	analogInit();		// init analog module
	analogInitTouch();	// enable capacitive touch (for K5)
	
	analogSelectTouch(0, TUNING);
	analogSelect(1, VOLUME);
	
//----------- Switch setup ----------------
	switchInit();		// init switch module
	switchSelect(0, POWERSW);
	switchSelect(1, STAT_PU);
	switchSelect(2, STAT_GO);
	switchSelect(3, STAT_PO);
	switchSelect(4, STAT_BE);
	switchSelect(5, STAT_OC);
	
	pinModeAnalogOut(LAMP);
}

void loop() {
// ---------- Main loop ------------
	fraiseService();	// listen to Fraise events
	analogService();	// analog management routine
	switchService();

	if(delayFinished(mainDelay)) // when mainDelay triggers :
	{
		delayStart(mainDelay, 5000); 	// re-init mainDelay
		if(!switchSend()) analogSend();		// send analog channels that changed
	}
}

// Receiving

void fraiseReceiveChar() // receive text
{
	unsigned char c;
	
	c=fraiseGetChar();
	if(c=='L'){		//switch LED on/off 
		c=fraiseGetChar();
		digitalWrite(LED, c!='0');		
	}
	else if(c=='E') { 	// echo text (send it back to host)
		printf("C");
		c = fraiseGetLen(); 			// get length of current packet
		while(c--) printf("%c",fraiseGetChar());// send each received byte
		putchar('\n');				// end of line
	}	
}

void fraiseReceive()
{
	unsigned char c = fraiseGetChar();
	unsigned int i;
	
	if(c == 1) {
		i = fraiseGetInt();
		analogWrite(LAMP, i);
	}
}
