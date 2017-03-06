/*
************************************************************************
 ECE 362 - Mini-Project C Source File - Fall 2016
***********************************************************************
	 	   			 		  			 		  		
 Team ID: < 4 >

 Project Name: < Game Of Chairs>

Team Members:

   - Team/Doc Leader:   <Tejada, Daniel>      Signature: Tejada, Daniel
   
   - Software Leader:   <Vasquez, Leonel>     Signature: Vasquez, Leonel

   - Interface Leader:  <Leiva, Rodolfo>      Signature: Leiva, Rodolfo

   - Peripheral Leader: <Davila, Alexander>   Signature: Davila, Alexander


 Academic Honesty Statement:  In signing above, we hereby certify that we 
 are the individuals who created this HC(S)12 source file and that we have
 not copied the work of any other student (past or present) while completing 
 it. We understand that if we fail to honor this agreement, we will receive 
 a grade of ZERO and be subject to possible disciplinary action.

***********************************************************************

 The objective of this Mini-Project is to create a digital multiplayer 
 version of the game "Musical Chairs".  


***********************************************************************

 List of project-specific success criteria (functionality that will be
 demonstrated):

 1. Successfully navigate through the outer circle of LEDs by using pushbuttons
	without having two players occupying the same slot.  

 2.	Successfully recognize when a player has aligned his LED to one winner spots
 	when the timer runs out.

 3.	Display the respective messages as well as the timer in the LCD display in order 
 	to provide a more user friendly environment using SPI.

 4. Successfully use the PWM module in order to make the lights of the LEDs in the inner circle 
	blink at the beginning and end of the game.

 5.	Being able to adjust the velocity of the automatic movement produced to the
	Outer circle of LED’s with a potentiometer using ATD.

 6. Succesfuly use the TIE module to create real time interrupts and respective flags.

***********************************************************************

  Date code started: < 11/18/16 >

  Update history (add an entry every time a significant change is made):
  
  Date: < 11/17/16 >  Name: < Davila, Alexander >   Update: <  Tested cascoding two GALs as one bigger shift register >

  Date: < 11/18/16 >  Name: < Tejada,Daniel     >   Update: <  Created game timer, pushbuttons interface>

  Date: < 11/18/16 >  Name: < Vasquez, Leonel   >   Update: <  Created functions for displays, push buttons, worked on array initialization and updated main>
  
  Date: < 11/18/16 >  Name: < Vasquez, Leonel   >   Update: <  Worked on array initialization and updated main>

  Date: < 11/18/16 >  Name: < Davila, Alexander >   Update: <  Created functions to output the leds and movement of players >
  
  Date: < 11/18/16 >  Name: < Rodolfo Leiva >       Update: <  Started PCB schematic on Eagle >
  
  Date: < 11/27/16 >  Name: < Tejada, Daniel >      Update: <  Modify pushbuttons according to hardware >
  
  Date: < 12/02/16 >  Name: < Davila, Alexander >   Update: <  Implemented chairs and corrected bugs >
  
  Date: < 12/02/16 >  Name: < Davila, Alexander >   Update: <  Corrected bugs in the program and added winning sequence and losing states >
  
  Date: < 12/02/16 >  Name: < Vasquez, Leonel   >   Update: <  Implemented chairs and corrected bugs >

 
***********************************************************************
*/

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>

/* All functions after main should be initialized here */
char inchar(void);
void outchar(char x);
void shiftout(char); 	//LCD Drivers
void lcdwait(void);
void send_byte(char);
void send_i(char);
void chgline(char);
void print_c(char);
void pmsglcd(char[]);
void fireCircle(void);
void askPlayers(void);
void dispWelcome(void);
void dispBeg(void);
void timer3sec(void);
void tDisplay(void);
void dispHonor(void);
void clockKeep(void);
void checkNeighborRight(char);
void checkNeighborLeft(char);
void georgeMartin(void);
void sendArr(void);
void setShspeed(void);
void movePlayer1(void);
void movePlayer2(void);
void movePlayer3(void);
void whiteWalker(void);
void newKing(void);
void zeroLeds(void);
void dispGreeting(void);
void blinkChairs(void);
void allMenMustDie(void);

/* Variable declarations */
//Enable variables
int  runstp      = 0;   // run/stop flag
int	 resetpb     = 1;	// resets game
int	 startpb	 = 0;	// starts game
char numPlay[]   = {'2','3'}; //char representation of players
int  toggPlay    = 0;
int  onesecstart = 0; 	//Flag to see when one second pass for display
int  onesecstcnt = 0; 	//Counter for start display
char  numplayers; 	//number of players
int	 initflg     = 1; 	//Flag for the initial light sequence 
 
//Start Pushbutton and reset variables
int prevpstrt = 0;
int pevprst = 0;


//Player 1 variables
//Controller
int prevpl1r   = 0; 	//Previous state of player 1 right pb
int prevpl1l   = 0; 	//Previous state of player 1 left pb
int enablepl1  = 1; 	// Choose if player one will play the game
int pl1leftmov = 0; 	//Flag to check if player 1 wants to move left
int pl1rghtmov = 0; 	//Flag to check if player 1 wants to move right
//Position 
int p1p        = 23; 	//Position of playe 1 (blue) in master array (subject to change)


//Player 2 variables
//Controller
int prevpl2r   = 0; 	//Previous state of player 2 right pb
int prevpl2l   = 0; 	//Previous state of player 2 left pb
int enablepl2  = 1; 	// Choose if player two will play the game
int pl2leftmov = 0; 	//Flag to check if player 2 wants to move left
int pl2rghtmov = 0; 	//Flag to check if player 2 wants to move right
//Position 
int p2p        = 13; 	//Position of playe 2 (green) in master array (subject to change)



//Player 3 variables
//Controller
int prevpl3r   = 0; 	//Previous state of player 3 right pb
int prevpl3l   = 0; 	//Previous state of player 3 left pb
int enablepl3  = 1; 	// Choose if player three will play the game
int pl3leftmov = 0; 	//Flag to check if player 3 wants to move left
int pl3rghtmov = 0; 	//Flag to check if player 3 wants to move right
int p3p        = 3;		//Position of playe 3 (red) in master array (subject to change)



//Player 4 variables
//Controller
int prevpl4r   = 0; 	//Previous state of player 4 right pb
int prevpl4l   = 0; 	//Previous state of player 4 left pb
int enablepl4  = 1; 	// Choose if player four will play the game
int pl4leftmov = 0; 	//Flag to check if player 4 wants to move left
int pl4rghtmov = 0; 	//Flag to check if player 4 wants to move right
//int p4p        = 3; 	//Position of playe 4 in master array (subject to change)

  
//Timer peripheral Variables
int shiftallcnt =  0; 	//Counter to keep track of time
int shiftallflg =  0; 	// Flag to move all positions
int speedtime = 2000;	//variable to set how fast positions will shift. This will be controlled by the potentiometer

//Timer clock Variables
int miliseconds =  0;   //Holds miliseconds for timer
int seconds 	= 30;   //Holds seconds for timer
int onemsec		=  0; 	//Flag when one milisecond have passed
int onesec 		=  0; 	//Flag when one second have passed
int mseccnt 	=  0; 	//Counter to keep track of milisecond
int seccnt 		=  0; 	//Counter to keep track of milisecon
int timesup 	=  0; 	//Flag to check if time is up
int seccout; 			//Variable to check seconds are 0
char clockdp[] 	= "Timer: 00.0s";
int halfsecrst = 0; 	//Flag for reset display
int halfsecnt  = 0; 	//Counter for reset display
int hurryflg = 0;       //Flag for hurry display
int hurrycnt = 0;       //Count for hurry display
int hurryhld = 0;

//ATD variables
int atdcnt=0; 			//Count 0.1s to sample channel 7
int atdflg = 0; 		//Flag to start atd

int chair1 = 3;
int chair2 = 12;
int chair3 = 21;

int chairct = 0;		//counter to turn off chairs
int twoflagplayers = 0; //dummy variable to indicate 2 players
int checkplayers = 0;	//keeps track of current players
int clearfl = 0;		//indicates if the display is clear


/* Special ASCII characters */
#define CR 0x0D		// ASCII return?
#define LF 0x0A		// ASCII new line?

/* LCD COMMUNICATION BIT MASKS (note - different than previous labs) */
#define RS 0x10		// RS pin mask (PTT[4])
#define RW 0x20		// R/W pin mask (PTT[5])
#define LCDCLK 0x40	// LCD EN/CLK pin mask (PTT[6])

/* LCD INSTRUCTION CHARACTERS */
#define LCDON 0x0C	    // LCD initialization command
#define LCDCLR 0x01	    // LCD clear display command
#define TWOLINE 0x38	// LCD 2-line enable command
#define CURMOV 0xFE	    // LCD cursor move instruction
#define LINE1 = 0x80	// LCD line 1 cursor position
#define LINE2 = 0xC0	// LCD line 2 cursor position

//Array of players
int masterArr[30] = {0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0}; 	//[B|G|R] with 30 spaces to have 3 ghost leds
int length		  = 27;															//length of master array

/*	 	   		
***********************************************************************
 Initializations
***********************************************************************
*/

void  initializations(void) {

/* Set the PLL speed (bus clock = 24 MHz) */
  CLKSEL = CLKSEL & 0x80;	//; disengage PLL from system
  PLLCTL = PLLCTL | 0x40;	//; turn on PLL
  SYNR   = 0x02;            	//; set PLL multiplier
  REFDV  = 0;              	//; set PLL divider
  while (!(CRGFLG & 0x08)){  }
  CLKSEL = CLKSEL | 0x80; 	//; engage PLL

/* Disable watchdog timer (COPCTL register) */
  COPCTL = 0x40;		 	//COP off; RTI and COP stopped in BDM-mode

/* Initialize asynchronous serial port (SCI) for 9600 baud, interrupts off initially */
  SCIBDH =  0x00; 			//set baud rate to 9600
  SCIBDL =  0x9C; 			//24,000,000 / 16 / 156 = 9600 (approx)  
  SCICR1 =  0x00; 			//$9C = 156
  SCICR2 =  0x0C; 			//initialize SCI for program-driven operation
  DDRB   =  0x10; 			//set PB4 for output mode
  PORTB  =  0x10; 			//assert DTR pin on COM port

	
//Initialize Digital I/O pins
  DDRAD      = 0;			// Set the direction register for ATD as inputs
  ATDDIEN    = 0xFE;		//Sets AN1 through AN7 to digital inputs
  DDRM_DDRM0 = 0;			//Sets PM0 and PM1 as inputs
  DDRM_DDRM1 = 0;
  DDRT		 = 0x7F;		//Set Pins 0..6 to outputs Pin 7 will be used as digital inputs
  
/* Initialize peripherals */
	
//Initialize SPI for a baud rate of 6Mbps, MSB first
  SPIBR	 = 0x10;
  SPICR1 = 0x50;
  SPICR2 = 0;

//Initialize Timer channel 7 for periodic interrupts every 1.0 ms 
  TSCR1  = 0x80;			//Turn on timer channel 7 and set fast flag clear mode
  TIOS 	 =  0x80;			// Enable channel 7 as output capture mode
  TSCR2  = 0x0C;			//Set clock scalar to 16 to get 1.5MHz clock and reset TCNT when OC7 occurs
  TC7    = 1500;			// Sets Timer Output compare for 10ms interrupts
	
//Analog to Digital Initialization
  ATDCTL2	 = 0x80;		// Turn on Analog to digital converter
  ATDCTL3 	 = 0x08;		// Set conversion lenght equal to 1 Non FIFO mode
  ATDCTL4 	 = 0x85;		// Set shutter speed for ATD
  //ATDCTL5    = 0x00; // Choose channel 0 of AD as ATD

//PWM for Chairs Initializaion
  PWME = 0x1C; // Turn on Ch2..Ch4 of PWM
  PWMPOL = 0x1C; //Set polarity of channels	active high
  MODRR = 0x1C; //Choose PT2..PT4 as outputs of PWM;
  PWMPER2 = 0xFF; //Set periods of channels to 255
  PWMPER3 = 0xFF;
  PWMPER4 = 0xFF;
  PWMDTY2 = 0xFF; //Set Duty cycle of chairs to 100%
  PWMDTY3 = 0xFF; //Adjust duty cycle to 0 to turn off chairs or 0x80 to make the chairs blink
  PWMDTY4 = 0xFF;
  PWMPRCLK = 0x77; //Divide clock by 128
  PWMCLK = 0x1C; // Select Scalar clock for PWM
  PWMSCLA = 0x00; 
  PWMSCLB = 0x00;
  PWMCTL = 0x00;	
  
  	
  	
	
	
/* Initialize interrupts */

// Initialize RTI Interrupts for an 2.048ms to sample controllers from players
  CRGINT = 0x80;
  RTICTL = 0x41;
	
//Initialize LCD with SPI
  PTT_PTT1 	 = 1; 			//LCD clock high
  send_i(LCDON);
  send_i(TWOLINE);	
  send_i(LCDCLR);
  lcdwait();
	      
}

	 		  			 		  		
/*	 		  			 		  		
***********************************************************************
Main
***********************************************************************
*/
void main(void) {
  	DisableInterrupts
	initializations(); 		  			 		  		
	EnableInterrupts;

 for(;;) {

/* < start of your main loop > */ 
	 if(twoflagplayers == 1){
		enablepl3 = 0;
		masterArr[p3p] = 0;
	 }
	 
	 numplayers = '0';
	 
	 if (enablepl1 == 1){
		 masterArr[p1p] =1;
		 numplayers += 1;
	 }
	 
	 if (enablepl2 == 1){
		 masterArr[p2p] =1;		 		 		 
 		 numplayers += 1;
	 }
	 
	 if (enablepl3 == 1){
		 masterArr[p3p] =1;
		 numplayers += 1;
	 }
	 
	 if(shiftallflg == 1){
		shiftallflg = 0;
		pl1rghtmov = 1;
		pl2rghtmov = 1;
		pl3rghtmov = 1;
	
		if (enablepl1 == 1){
			movePlayer1();
		}
		if (enablepl2 == 1){
			movePlayer2();
		}
		if (enablepl3 == 1){
			movePlayer3();
		}
		 
		pl1rghtmov =0;
		pl2rghtmov =0;
		pl3rghtmov =0;		
	 }
 
	if (timesup == 1){
		chairct += 1;
		enablepl1 =0;
		enablepl2 =0;
		enablepl3 =0;
		runstp = 0;
		TIE = 0x00;
		whiteWalker();                
		miliseconds = 0;    //Holds miliseconds for timer
		seconds 	= 30;   //Holds seconds for timer
		TIE = 0x80;
		runstp  = 1;
		if (numplayers > 49){
			dispBeg();
			TIE = 0X80;
		}
	}
	 
	if(numplayers == '1'){
		runstp = 0;
		PWMDTY2 = 0x80;
		PWMDTY3 = 0x80;
		PWMDTY4 = 0x80;
		newKing();
	}
	  
	if(numplayers == '0'){
		if (clearfl == 0){
			clearfl =1;
			send_i(LCDCLR);			
		}	
		chgline(0x80);
		pmsglcd("No true heir!");
		chgline(0xC0);
		pmsglcd("Shame! Shame! ");
		runstp = 0;
	} 
	 
	if(runstp == 1){
		sendArr();
		//Check one msec passed to display timer
		if(onemsec == 1){
			onemsec = 0;
			clockKeep();
			tDisplay();
			dispHonor();
			}
		
		//Do ATD to set speed of shifting all
		if(atdflg == 1){
			atdflg=0;
			setShspeed();
		}
		if (enablepl1 == 1){
			movePlayer1();
		}		
		if (enablepl2 == 1){
			movePlayer2();
		}
		if (enablepl3 == 1){
			movePlayer3();
		}
	}
	
	//if the  resetpb flag is set, the game will be restarted to
	//initial state
	if(resetpb == 1){
		initializations();
		/*
		masterArr[p1p]=0;
		masterArr[p2p]=0;
		masterArr[p3p]=0;
		clearfl = 0;
		p1p =23;
		p2p =13;
		p3p =3;
		enablepl1 = 1;
		enablepl2 = 1;
		enablepl3 = 1;
		chairct = 0;
		checkplayers = 0;
		*/
		clearfl = 0;
		checkplayers = 0;
		chairct = 0;
		send_i(LCDCLR);
		dispGreeting();
		resetpb 	= 0;
		runstp  	= 0;
		seconds 	= 30;
		miliseconds = 0;
		TIE = 0x80;
	
		while(startpb == 0){
			PWMDTY3 = 0x80;
			PWMDTY4 = 0x80;
			PWMDTY2 = 0x80;
			fireCircle(); 
		}
		PWMDTY2 = 0XFF;
		PWMDTY3 = 0XFF;
		PWMDTY4 = 0XFF;
		send_i(LCDCLR); 
		
		if (numplayers == '2'){
			enablepl1 =1;
			enablepl2 =1;
		}
		
		if (numplayers == '3'){
			enablepl1 =1;
			enablepl2 =1;
			enablepl3 =1;
		}

		if (enablepl1 == 1){
			masterArr[p1p] = 1;
		}
		
		if (enablepl2 == 1){
			masterArr[p2p] = 1;
		}
		
		if (enablepl3 == 1){
			masterArr[p3p] = 1;
		}
	 }
	 //If the startpb flag is set, the initializing timer will be displayed
	 if(startpb == 1){
		startpb = 0;
		runstp	= 1;
		zeroLeds();
		PTT_PTT5 =0;
		dispWelcome();
		TIE = 0x80;
		send_i(LCDCLR); 
		if(resetpb  ==1){
			resetpb =0;
		} 
	 }
	 
	 if (enablepl1 == 0){
		 masterArr[p1p] = 0;
	 }
	 
	 if (enablepl2 == 0){
		 masterArr[p2p] = 0;//							¯\_(ツ)_/¯
	 }
	 
	 if (enablepl3 == 0){
		 masterArr[p3p] = 0;
	 }
		 
 } /* loop forever */
   
}   /* do not leave main */


/*
***********************************************************************   ????  ? ??????   ?? 
 RTI interrupt service routine: RTI_ISR
************************************************************************
*/

interrupt 7 void RTI_ISR(void)
{
  	// clear RTI interrupt flagt 
  	CRGFLG = CRGFLG | 0x80;
   //Player 1 subroutine controller
	if(PORTAD0_PTAD1 == 0){
	   if(PORTAD0_PTAD1 != prevpl1l && enablepl1 == 1){  //Set flag only when pl1 enabled 
	   pl1leftmov = 1;
	   }	
	}	
	prevpl1l = PORTAD0_PTAD1;
	
	if(PORTAD0_PTAD2 == 0){
	   if(PORTAD0_PTAD2 != prevpl1r && enablepl1 == 1){  //Set flag only when pl1 enabled 
	   pl1rghtmov = 1;
	   }	
	}	
	prevpl1r = PORTAD0_PTAD2;
	
	//Player 2 Subroutine controller
	if(PORTAD0_PTAD3 == 0){
	   if(PORTAD0_PTAD3 != prevpl2l && enablepl2 == 1){  //Set flag only when pl1 enabled 
	   pl2leftmov = 1;
	   }	
	}	
	prevpl2l = PORTAD0_PTAD3;
	
	if(PORTAD0_PTAD4 == 0){
	   if(PORTAD0_PTAD4 != prevpl2r && enablepl2 == 1){  //Set flag only when pl1 enabled 
	   pl2rghtmov = 1;
	   }	
	}	
	prevpl2r = PORTAD0_PTAD4;
	
	//Player 3 Subroutine controller
	if(PORTAD0_PTAD5 == 0){
	   if(PORTAD0_PTAD5 != prevpl3l && enablepl3 == 1){  //Set flag only when pl1 enabled 
	   pl3leftmov = 1;
	   }	
	}	
	prevpl3l = PORTAD0_PTAD5;
	
	if(PORTAD0_PTAD6 == 0){
	   if(PORTAD0_PTAD6 != prevpl3r && enablepl3 == 1){  //Set flag only when pl1 enabled 
	   pl3rghtmov = 1;
	   }	
	}	
	prevpl3r = PORTAD0_PTAD6;
	
	//Player 4 Subroutine Controller
	if(PTM_PTM0 == 0){
	   if(PTM_PTM0 != prevpl4l && enablepl4 == 1){ 		 //Set flag only when pl1 enabled 
	   pl4leftmov = 1;
	   }	
	}	
	prevpl4l = PTM_PTM0;
	
	if(PTM_PTM1 == 0){
	   if(PTM_PTM1 != prevpl4r && enablepl4 == 1){  	 //Set flag only when pl1 enabled 
	   pl4rghtmov = 1;
	   }	
	}	
	prevpl4r = PTM_PTM1;
	
	//Start Pushbutton 
	 
	if(PTT_PTT7 == 0){
	   if(PTT_PTT7 != prevpstrt){  						 //Set flag only when pl1 enabled 
	   startpb = 1;
	   }	
	}	
	prevpstrt = PTT_PTT7;
	
	//Reset Pushbutton
	
	if(PORTAD0_PTAD7 == 0){
	   if(PORTAD0_PTAD7 !=  pevprst ){  //Set flag only when pl1 enabled 
	   resetpb = 1;
	   }	
	}	
	pevprst = PORTAD0_PTAD7;
}

/*
***********************************************************************   ????  ? ??????   ?? 
  TIM interrupt service routine	  		
***********************************************************************
*/

interrupt 15 void TIM_ISR(void)
{
  	// clear TIM CH 7 interrupt flag 
  	TFLG1 = TFLG1 | 0x80; 
    
	//Set the shift all flag when the right amount of time has passed
	shiftallcnt++;
	if(shiftallcnt >= speedtime){
	shiftallcnt = 0;
	shiftallflg = 1;
	}
		
	//Wait for start display
	onesecstcnt++;
	if(onesecstcnt >= 1000){
		onesecstcnt = 0;
		onesecstart = 1;
	}
	
	//Count 1 second to start ATD 
	atdcnt++;
	if(atdcnt >= 100){
		atdcnt =0;
		atdflg = 1;
	}
		
	//Timer
	mseccnt++;
	if(mseccnt >= 10){
		mseccnt = 0;
		onemsec = 1;
	}
	
	//reset disp
	halfsecnt++;
	if(halfsecnt >= 175){
		halfsecnt = 0;
		halfsecrst = 1;
	}
}

/*
***********************************************************************  
  SCI interrupt service routine		 		  		
***********************************************************************
*/

interrupt 20 void SCI_ISR(void){}

/*
***********************************************************************
 Name:         fireCircle 
 Description:  turns on LEDs of the outer circle in a specefic sequence
***********************************************************************
*/
void fireCircle(void){	
	int l;
	zeroLeds();
	PTT_PTT6 = 0;
	PTT_PTT5 = 1;
	PTT_PTT6 = 1;	
	PTT_PTT5 = 0;
	
	for (l=0;l<29;l++){
		if (startpb == 1){
			break;
		}
		while(halfsecrst == 0){};
		halfsecrst =0;
		PTT_PTT6 = 0;
		PTT_PTT6 = 1;
	}
}

/*
***********************************************************************
 Name:         dispWelcome 
 Description:  displays Welcome message
***********************************************************************
*/
void dispWelcome(void){
	enablepl1 = 1;
	enablepl2 = 1;
	enablepl3 = 1;
	send_i(LCDCLR);
	chgline(0x80);
	pmsglcd("Insert number of");
	chgline(0xC0);
	pmsglcd("Lords: ");
	askPlayers();
}

/*
***********************************************************************
 Name:         askPlayers 
 Description:  inputs number of players
***********************************************************************
*/
void askPlayers(void){
	do{
		if(pl1rghtmov == 1){
		  	pl1rghtmov = 0;
			toggPlay = (toggPlay + 1) % 2;
			print_c(numPlay[toggPlay]);
			send_i(CURMOV);
			send_i(0xC7);
		}
	}while(pl1leftmov != 1);
	numplayers =  numPlay[toggPlay];

	if (numplayers == '2'){
		twoflagplayers = 1;
		PWMDTY2 = 0x00;
		chairct =1;
		checkplayers +=1;
	}
	else{
		twoflagplayers = 0;
	}
	dispBeg();
	pl1leftmov = 0;
}

/*
***********************************************************************
 Name:         dispBeg 
 Description:  Displays beggining of game message and counter:
***********************************************************************
*/
void dispBeg(void){
	send_i(LCDCLR);
	chgline(0x80);
	pmsglcd("Brace yourselves");
	chgline(0xC0);
	pmsglcd("War in: ");
	//pmsglcd("Winter is ");
	//pmsglcd("comming in:  ");
	TIE = 0x80;
	timer3sec();	
}

/*
***********************************************************************
 Name:         timer3sec 
 Description:  Sets counter from 3 to 0 seconds
***********************************************************************
*/
void timer3sec(void){
	//wait for 3 seconds
	//onesecstart
	int i  = 0;
	char dummy; 
	for (i =0;i<4;i++){
		dummy = '4' - i;
		print_c(dummy);
		send_i(CURMOV);
		send_i(0xC8);
		while(onesecstart == 0){}
		onesecstart = 0;
	}
	shiftallcnt = 0;
	shiftallflg = 0;
	onesecstart = 0;
	mseccnt 	= 0;
	onemsec 	= 0;
	atdcnt 		= 0;
	TIE  		= 0x00;
}

/*
***********************************************************************
 Name:         checkNeighborRight 
 Description:  checks if the adjacent spot to the right is occupied
***********************************************************************
*/
void checkNeighborRight(char color){
	if(color == 'B'){
		if(p1p >= length - 1 ){
			if(masterArr[0] == 1 || masterArr[1] == 1){
				p1p = p1p - 3;
			}
		}
		else{
			if(masterArr[p1p + 1] == 1 || masterArr[p1p + 2] == 1 ){
				p1p = p1p - 3;
			}
		}
	}
	
	else if(color == 'G'){
		if(p2p >= length - 2 ){
			if(masterArr[0] == 1 || masterArr[2] == 1){
				p2p = p2p - 3;
			}
		}
		else{
			if(masterArr[p2p + 2] == 1 || masterArr[p2p + 4] == 1 ){
				p2p = p2p - 3;
			}
		}
	}

	else if(color == 'R'){
		if(p3p >= length - 3 ){
			if(masterArr[1] == 1 || masterArr[2] == 1){
				p3p = p3p - 3;
			}
		}
		else{
			if(masterArr[p3p + 4] == 1 || masterArr[p3p + 5] == 1 ){
				p3p = p3p - 3;
			}
		}
	}
}

/*
***********************************************************************
 Name:         checkNeighborLeft 
 Description:  checks if the adjacent spot to the left is occupied
***********************************************************************
*/
void checkNeighborLeft(char color){
	if(color == 'B'){
		if(p1p == 2){
			if(masterArr[length-2] == 1 || masterArr[length -3] == 1){
				p1p = p1p +3;
			}
		}
		else{
			if(masterArr[p1p - 4] == 1 || masterArr[p1p - 5] == 1 ){
				p1p = p1p + 3;
			}
		}
	}
	
	else if(color == 'G'){
		if(p2p == 1){
			if(masterArr[length-1] == 1 || masterArr[length -3] == 1){
				p2p = p2p +3;
			}
		}
		else{
			if(masterArr[p2p - 4] == 1 || masterArr[p2p - 2] == 1 ){
				p2p = p2p + 3;
			}
		}
	}

	else if(color == 'R'){
		if(p3p == 0){
			if(masterArr[length-1] == 1 || masterArr[length -2] == 1){
				p3p = p3p +3;
			}
		}
		else{
			if(masterArr[p3p - 1] == 1 || masterArr[p3p - 2] == 1 ){
				p3p = p3p + 3;
			}
		}
	}
}

/*
***********************************************************************
 Name:         sendarr
 Description:  Sends the masterArr as a string of bits to the first GAL
***********************************************************************
*/
void sendArr(void){
	int k;
	if(enablepl1 || enablepl2 || enablepl3){
		for (k=0;k<27;k++){
			PTT_PTT6 =0;
			PTT_PTT5 = masterArr[k];
			PTT_PTT6 =1;
		}
	}
}

/*
***********************************************************************
 Name:         inchar
 Description:  inputs ASCII character from SCI serial port and returns it
***********************************************************************
*/
char inchar(void) {
    while (!(SCISR1 & 0x20)); /* wait for input */
    return SCIDRL;
}

/*
***********************************************************************
 Name:         outchar    (use only for DEBUGGING purposes)
 Description:  outputs ASCII character x to SCI serial port
***********************************************************************
*/
void outchar(char x) {
    while (!(SCISR1 & 0x80));  /* wait for output buffer empty */
    SCIDRL = x;
}

/*
***********************************************************************
 Name:         outchar    (use only for DEBUGGING purposes)
 Description:  outputs ASCII character x to SCI serial port
***********************************************************************
*/
void shiftout(char x)
{
  int j=0;
  while(SPISR_SPTEF == 0){}
  SPIDR = x;
  for(j=0; j<10; j++){}	
}

/*
***********************************************************************
 Name:         lcdwait 
 Description:  LCD wait for a reasonable omunt of time
***********************************************************************
*/
void lcdwait()
{
  int i=0;
  for(i=0; i<16000; i++){}
}

/*
***********************************************************************
 Name:         send_byte 
 Description:  send character x to LCD
***********************************************************************
*/	
void send_byte(char x)
{
  shiftout(x);
  PTT_PTT1 = 0;
  PTT_PTT1 = 1;
  PTT_PTT1 = 0;
  lcdwait();
}

	/*
***********************************************************************
 Name:         send_i 
 Description:  send instruction Byte x to LCD
***********************************************************************
*/	
void send_i(char x)
{
   PTT_PTT0 = 0;
   send_byte(x);
	
}

/*
***********************************************************************
 Name:         chgline 
 Description:  Move LCD Cursor to position x
***********************************************************************
*/
void chgline(char x)
{
	send_i(CURMOV);
	send_i(x);
}	

/*
***********************************************************************
 Name:         print_c 
 Description:  Print single character x on LCD
***********************************************************************
*/
void print_c(char x)
{
	PTT_PTT0 = 1;
	send_byte(x);
}

/*
***********************************************************************
 Name:         pmsglcd 
 Description:  Print character string str[] on LCD
***********************************************************************
*/
void pmsglcd(char str[]){
	int z=0;
	while(str[z] != '\0'){
		print_c(str[z]);
		z++;
	}
}

/*
***********************************************************************
 Name:         clockKeep 
 Description:  Adjust memory for timer
***********************************************************************
*/
void clockKeep(void){
	miliseconds--;
	if(miliseconds <= 0){
		miliseconds = 9;
		seconds--;
		if(seconds == 10){
			blinkChairs();
		}
		if(seconds <= 0 && miliseconds == 9){
		timesup = 1;
		miliseconds = 0;	
		TIE = 0x00;
		}
	}	
}

/*
***********************************************************************
 Name:         tDisplay 
 Description:  Display time on LCD
***********************************************************************
*/
void tDisplay(void){
	clockdp[7] = '0' + ((seconds/10)%10);
	clockdp[8] = '0' + (seconds%10);
	clockdp[10] = '0' + (miliseconds%10);
	
	chgline(0x80);
	pmsglcd(clockdp);
	if(seconds <= 0 && miliseconds == 9){
		  seconds = 30; //Reset timer memory
          miliseconds = 0;
		}		
}

/*
***********************************************************************
 Name:         dispHonor 
 Description:  Display predefined message in LCD
***********************************************************************
*/
void dispHonor(void){
	chgline(0xC0);
	pmsglcd("Valar Morghulis!");	
}

/*
***********************************************************************
 Name:         setShspeed 
 Description:  Sample AN7 to change shift speed
***********************************************************************
*/
void setShspeed(void){
	 ATDCTL5 = 0x00;
     while(ATDSTAT0_SCF == 0){}
	 speedtime = (ATDDR0H*8)+100;
}

/*
***********************************************************************
 Name:         movePlayer1 "BLUE"
 Description:  Allows player1 to move respecting conditions
***********************************************************************
*/
void movePlayer1(void){
	if(pl1leftmov == 1){
		pl1leftmov = 0;
		checkNeighborLeft('B');
		masterArr[p1p] = 0;
		p1p = p1p - 3;
		if (p1p < 0){
			p1p = length-1;		
		}
		masterArr[p1p] = 1;		
	}
			
	if(pl1rghtmov == 1){
		pl1rghtmov = 0;
		checkNeighborRight('B');
		masterArr[p1p] =0;
		p1p = p1p + 3;
		if (p1p >= length ){
			p1p = 2;
		}
		masterArr[p1p] = 1;		
	}				
}

/*
***********************************************************************
 Name:         movePlayer2 "GREEN"
 Description:  Allows player2 to move respecting conditions
***********************************************************************
*/
void movePlayer2(void){
	if(pl2leftmov == 1){
		pl2leftmov = 0;
		checkNeighborLeft('G');
		masterArr[p2p] = 0;
		p2p = p2p - 3;
		if (p2p < 0){
			p2p = length-2;		
		}
		masterArr[p2p] = 1;		
	}
			
	if(pl2rghtmov == 1){
		pl2rghtmov = 0;
		checkNeighborRight('G');
		masterArr[p2p] =0;
		p2p = p2p + 3;
		if (p2p >= length ){
			p2p = 1;
		}
		masterArr[p2p] = 1;		
	}
}

/*
***********************************************************************
 Name:         movePlayer3 "RED"
 Description:  Allows player3 to move respecting conditions
***********************************************************************
*/
void movePlayer3(void){
	if(pl3leftmov == 1){
		pl3leftmov = 0;
		checkNeighborLeft('R');
		masterArr[p3p] = 0;
		p3p = p3p - 3;
		if (p3p < 0){
			p3p = length-3;		
		}
		masterArr[p3p] = 1;		
	}
			
	if(pl3rghtmov == 1){
		pl3rghtmov = 0;
		checkNeighborRight('R');
		masterArr[p3p] = 0;
		p3p = p3p + 3;
		if (p3p >= length ){
			p3p = 0;
		}
		masterArr[p3p] = 1;		
	}					
}

/*
***********************************************************************
 Name:         whiteWalker 
 Description:  checks when a player has lost
***********************************************************************
*/
void whiteWalker(void){
	enablepl1 = 0;
	enablepl2 = 0;
	enablepl3 = 0;

	if (PWMDTY2 == 0X80){
		if (p1p == chair1 + 2){
			enablepl1 = 1;
			PWMDTY2 = 0XFF;
		}
		if (p2p == chair1 + 1){
			enablepl2 = 1;
			PWMDTY2 = 0XFF;
		}
		if (p3p == chair1){
			enablepl3 = 1;	
			PWMDTY2 = 0XFF;
		}
	}
	
	if (PWMDTY3 == 0X80){
		if (p1p == chair2 + 2){
			enablepl1 = 1;
			PWMDTY3 = 0XFF;
		}
		if (p2p == chair2 + 1){
			enablepl2 = 1;
			PWMDTY3 = 0XFF;
		}
		if (p3p == chair2){
			enablepl3 = 1;
			PWMDTY3 = 0XFF;
		}
	}

	if (PWMDTY4 == 0X80){
		if (p1p == chair3 + 2){
			enablepl1 = 1;
			PWMDTY4 = 0XFF;
		}
		if (p2p == chair3 + 1){
			enablepl2 = 1;
			PWMDTY4 = 0XFF;
		}
		if (p3p == chair3){
			enablepl3 = 1;
			PWMDTY4 = 0XFF;
		}
	}
	timesup = 0;
	allMenMustDie();
}


/*
***********************************************************************
 Name:  	allmenmustdie
 Descripcion: Turns off the LED for players who lose
***********************************************************************
*/

void allMenMustDie(void){
	if (chairct == 1){	
		PWMDTY2 = 0x00;	
		PWMDTY3 = 0xFF;
		PWMDTY4 = 0xFF;
	}
	
	if (chairct == 2){
		PWMDTY2 = 0x00;
		PWMDTY3 = 0x00;
		PWMDTY4 = 0xFF;
	}
	
	if (chairct == 3){
		PWMDTY2 = 0x00;
		PWMDTY3 = 0x00;
		PWMDTY4 = 0x00;
	}
	
	numplayers += checkplayers;
	if(enablepl1 == 0){	
		masterArr[p1p] = 0;
		chgline(0x80);
		pmsglcd("Player 1");
		chgline(0xC0);
		pmsglcd("is no true King")
		numplayers = numplayers-1;
		checkplayers += 1;
	}	
	
	if(enablepl2 == 0){
		masterArr[p2p] = 0;
		chgline(0x80);
		pmsglcd("Player 2");
		chgline(0xC0);
		pmsglcd("is no true King")
		numplayers = numplayers-1;
		checkplayers += 1;
	}	
	
	if(enablepl3 == 0){
		masterArr[p3p] = 0;
		chgline(0x80);
		pmsglcd("Player 3");
		chgline(0xC0);
		pmsglcd("is no true King")
		numplayers = numplayers-1;
		checkplayers += 1;
	}		
}

/*
***********************************************************************
 Name:         newKing 
 Description:  displays winner
***********************************************************************
*/
void newKing(void){
	if (clearfl == 0){
			clearfl =1;
			send_i(LCDCLR);

	chgline(0x80);
	
	if(enablepl1 == 1){
		pmsglcd("Player 1 is the king");
		chgline(0xC0);	
		pmsglcd("Long live the king");
		masterArr[p2p] = 0;
		masterArr[p3p] = 0;
		enablepl2 = 0;
		enablepl3 = 0;
	}
	else if(enablepl2 == 1){
		pmsglcd("Player 2 is the king");
		chgline(0xC0);	
		pmsglcd("Long live the king");
		masterArr[p3p] = 0;
		masterArr[p1p] = 0;
		enablepl1 = 0;
		enablepl3 = 0;
	}
	else if(enablepl3 == 1){
		pmsglcd("Player 3 is the king");
		chgline(0xC0);	
		pmsglcd("Long live the king");
		masterArr[p1p] = 0;
		masterArr[p2p] = 0;
		enablepl1 = 0;
		enablepl2 = 0;
	}
}

/*
***********************************************************************
 Name:         zeroLeds
 Description:  sets all the LEDs to zero
***********************************************************************
*/
void zeroLeds(void){
	int m;
	PTT_PTT5 =0;
	for (m=0;m<29;m++){
		PTT_PTT6 = 0;
		PTT_PTT6 = 1;
	}
}

/*
***********************************************************************
 Name:         dispGreeting
 Description:  display welcome message for initial state
***********************************************************************
*/
void dispGreeting(void){
	chgline(0x80);
	pmsglcd("Welcome to  ");
	chgline(0xC0);
	pmsglcd("   Game of Chairs   ");
}

/*
***********************************************************************
 Name:         blinkChairs
 Description:  make chair LEDs blink
***********************************************************************
*/
void blinkChairs(void){
	if (chairct == 0){
		PWMDTY2 = 0x80;
		PWMDTY3 = 0x80;
		PWMDTY4 = 0x80;
	}
	if (chairct == 1){
		PWMDTY2 = 0x00;
		PWMDTY3 = 0x80;
		PWMDTY4 = 0x80;
	}
	if (chairct == 2){
		PWMDTY2 = 0x00;
		PWMDTY3 = 0x00;
		PWMDTY4 = 0x80;
	}	
}

/*
***********************************************************************
 Name:         turoffChairs
 Description:  turn off chair LEDs 
***********************************************************************
*/
void turnoffChairs(void){
  PWMDTY2 = 0x00; 
  PWMDTY3 = 0x00; 
  PWMDTY4 = 0x00;
}
