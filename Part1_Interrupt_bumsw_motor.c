#include <stdint.h>
#include "msp.h"

#include "inc/Clock.h"
#include "inc/SysTick.h"
#include "inc/motor.h"


#define RED       0x01
#define GREEN     0x02
#define YELLOW    0x03
#define BLUE      0x04
#define PINK      0x05
#define SKYBLUE   0x06
#define WHITE     0x07

// bit-banded addresses, positive logic
#define SW1IN ((*((volatile uint8_t *)(0x42098004)))^1)
#define SW2IN ((*((volatile uint8_t *)(0x42098010)))^1) // input: switch SW2
#define REDLED (*((volatile uint8_t *)(0x42098040)))    // output: red LED


void BumpEdgeTrigger_Init(void){
    P4->SEL0 &= ~0xED;
    P4->SEL1 &= ~0xED;      // configure as GPIO
    P4->DIR &= ~0xED;       // make in
    P4->REN |= 0xED;        // enable pull resistors
    P4->OUT |= 0xED;        // pull-up
    P4->IES |= 0xED;        // falling edge event
    P4->IFG &= ~0xED;       // clear flag
    P4->IE |= 0xED;         // arm the interrupt
    // priority 2 on port4
    NVIC->IP[9] = (NVIC->IP[9]&0xFF00FFFF)|0x00400000;
    // enable interrupt 38 in NVIC on port4
    NVIC->ISER[1] = 0x00000040;
}


// Uses P4IV IRQ handler to solve critical section/race
void PORT4_IRQHandler(void){

    uint8_t status;
    Port2_Output(0);        // turn off the coloured LED
    EnableInterrupts();

    // Interrupt Vector of Port4
      status = P4->IV;      

    switch(status){

        case 0x02: // Bump switch 1
            if (mode_en == 2){
            
				Port2_Output(GREEN);
				Motor_BackwardSimple(500, 200);            
				Port2_Output(0);
				Motor_StopSimple(100);          
				Port2_Output(YELLOW);            
				Motor_LeftSimple(500, 100);
				Port2_Output(0);
				Motor_StopSimple(100);
            }
            else if (mode_en == 1){
                mode_en = 0;
                Motor_StopSimple(10000);
            }
            break;

        case 0x06: // Bump switch 2
            if (mode_en == 2){
				Port2_Output(GREEN);           
				Motor_BackwardSimple(500, 200);           
				Port2_Output(0);           
				Motor_StopSimple(100);           
				Port2_Output(YELLOW);
				Motor_LeftSimple(500, 200);            
				Port2_Output(0);
				Motor_StopSimple(100);
            }
            else if (mode_en == 1){
                mode_en = 0;
                Motor_StopSimple(10000);
            }
            break;

        case 0x08: // Bump switch 3
            if (mode_en == 2){
           
				Port2_Output(GREEN);            
				Motor_BackwardSimple(500, 200);          
				Port2_Output(0);          
				Motor_StopSimple(100);

				Port2_Output(YELLOW);            
				Motor_LeftSimple(500, 300);           
				Port2_Output(0);
				Motor_StopSimple(100);
            }
			
            else if (mode_en == 1){
                mode_en = 0;
                Motor_StopSimple(10000);
            }
            break;

        case 0x0C: // Bump switch 4
            if (mode_en == 2){
				Port2_Output(GREEN);
				Motor_BackwardSimple(500, 200);            
				Port2_Output(0);           
				Motor_StopSimple(100);        
				Port2_Output(BLUE);            
				Motor_RightSimple(500, 300);           
				Port2_Output(0);            
				Motor_StopSimple(100);
            }
            else if (mode_en == 1){
                mode_en = 0;
                Motor_StopSimple(10000);
            }
            break;
 
        case 0x0E: // Bump switch 5
            if (mode_en == 2){
				Port2_Output(GREEN);           
				Motor_BackwardSimple(500, 200);            
				Port2_Output(0);           
				Motor_StopSimple(100);            
				Port2_Output(BLUE);            
				Motor_RightSimple(500, 200);            
				Port2_Output(0);
				Motor_StopSimple(100);
            }
			
            else if (mode_en == 1){
                mode_en = 0;
                Motor_StopSimple(10000);
            }
            break;

        case 0x10: // Bump switch 6

            if (mode_en == 2){           
				Port2_Output(GREEN);           
				Motor_BackwardSimple(500, 200);           
				Port2_Output(0);           
				Motor_StopSimple(100);           
				Port2_Output(BLUE);            
				Motor_RightSimple(500, 100);            
				Port2_Output(0);
				Motor_StopSimple(100);
            }
            else if (mode_en == 1){
                mode_en = 0;
                Motor_StopSimple(10000);
            }
			break;

        case 0xED: // none of the switches are pressed
			break;
      }

      P4->IFG &= ~0xED; // clear flag
}


uint8_t Bump_Read_Input(void){
	return (P4->IN&0xED); // read P4.7, 4.6, 4.5, 4.3, 4.2, 4.0 inputs
}

          
void checkbumpswitch(uint8_t status){
       
    Port2_Output(0);        // turn off the coloured LED
    DisableInterrupts();

    switch(status){

      //case 0x02: // Bump switch 1 (for interrupt vector)
        case 0x6D: // Bump 1
            if (mode_en == 2){
                        
                Port2_Output(GREEN);                        
                Motor_BackwardSimple(500, 200);                        
                Port2_Output(0);                       
                Motor_StopSimple(100);                       
                Port2_Output(YELLOW);                        
                Motor_LeftSimple(500, 100);        
                Port2_Output(0);
                Motor_StopSimple(100);
                }
            else if (mode_en == 1){
                mode_en = 0;
                Motor_StopSimple(100);
                }
			break;
		
      
        case 0xAD:
            if (mode_en == 2){
                // Change the coloured LED into green (backward)
                Port2_Output(GREEN);
                Motor_BackwardSimple(500, 200);
                Port2_Output(0);
                Motor_StopSimple(100);
                Port2_Output(YELLOW);
                Motor_LeftSimple(500, 200);
                Port2_Output(0);                       
                Motor_StopSimple(100);
                    }
            else if (mode_en == 1){
                mode_en = 0;
                Motor_StopSimple(100);

                }
	
			break;
      //case 0x08: // Bump switch 3 (for interrupt vector)
        case 0xCD: // Bump 3
            if (mode_en == 2){       
                Port2_Output(GREEN);      
                Motor_BackwardSimple(500, 200);                    
                Port2_Output(0);                   
                Motor_StopSimple(100);                    
                Port2_Output(YELLOW);                    
                Motor_LeftSimple(500, 300);                   
                Port2_Output(0);                    
                Motor_StopSimple(100)
                }
            else if (mode_en == 1){
                mode_en = 0;
                Motor_StopSimple(100);

                    }

			break;
      //case 0x0C: // Bump switch 4 (for interrupt vector)
        case 0xE5: // Bump 4
            if (mode_en == 2){
                        // Change the coloured LED into green (backward)
                Port2_Output(GREEN);
                        // Move backward at 500 duty for 200ms
                Motor_BackwardSimple(500, 200);
                        // turn off the coloured LED
                Port2_Output(0);
                        // Stop for 1000ms
                Motor_StopSimple(100);
                        // Change the coloured LED into blue (turn right)
                Port2_Output(BLUE);
                        // Make a Right turn at 500 duty for 300ms
                Motor_RightSimple(500, 300);
                        // turn off the coloured LED
                Port2_Output(0);
                        // Stop for 1000ms
                Motor_StopSimple(100);
            }
            else if (mode_en == 1){
                mode_en = 0;
                Motor_StopSimple(100);

            }

			break;
      //case 0x0E: // Bump switch 5 (for interrupt vector)
        case 0xE9: // Bump 5
            if (mode_en == 2){
                
                Port2_Output(GREEN);                        
                Motor_BackwardSimple(500, 200);                        
                Port2_Output(0);                        
                Motor_StopSimple(100);                       
                Port2_Output(BLUE);                        
                Motor_RightSimple(500, 200);                        
                Port2_Output(0);                        
                Motor_StopSimple(100);
                }
            else if (mode_en == 1){
                mode_en = 0;
                Motor_StopSimple(100);
			}
			
			break;
      
        case 0xEC: // Bump 6
            if (mode_en == 2){
            
				Port2_Output(GREEN);           
				Motor_BackwardSimple(500, 200);           
				Port2_Output(0);           
				Motor_StopSimple(100);           
				Port2_Output(YELLOW);           
				Motor_LeftSimple(500, 100);
				Port2_Output(0);
				Motor_StopSimple(100);
            }
            else if (mode_en == 1){
                mode_en = 0;
                Motor_StopSimple(100);
            }

			break;
		
		case 0xED: // neither switch pressed
			break;
    }
}


void Port1_Init(void){
	P1->SEL0 &= ~0x01;
	P1->SEL1 &= ~0x01;        // configure P1.0 as GPIO
	P1->DIR |= 0x01;          //make P1.0 out, the built-in LED1
}

void Port2_Init(void){
    P2->SEL0 &= ~0xC7;
    P2->SEL1 &= ~0xC7;        // configure P2.2 P2.1 P2.0, and P2.6 P2.7 as GPIO
    P2->DIR |= 0x07;          // make P2.2-P2.0 out
    P2->DS |= 0x07;           // activate increased drive strength
    P2->OUT &= ~0x07;         // all LEDs off
    P2->DIR |= 0xC0;          // Direction of the motor
}

void Port2_Output(uint8_t data){
    
    P2->OUT = (P2->OUT&0xF8)|data;
}

void Switch_Init(void){
    // negative logic built-in Button 1 connected to P1.1
    // negative logic built-in Button 2 connected to P1.4
    P1->SEL0 &= ~0x12;
    P1->SEL1 &= ~0x12;      // configure P1.4 and P1.1 as GPIO
    P1->DIR &= ~0x12;       // make P1.4 and P1.1 in
    P1->REN |= 0x12;        // enable pull resistors on P1.4 and P1.1
    P1->OUT |= 0x12;        // P1.4 and P1.1 are pull-up
}

//Case Value
int mode_en = 0;
int interrupting = 0;
int polling = 1;

int main(void){
    uint8_t status;

	Clock_Init48MHz();        // Initialise clock with 48MHz frequency
	Switch_Init();            // Initialise switches
	SysTick_Init();           // Initialise SysTick timer
	Port1_Init();             // Initialise P1.1 and P1.4 built-in buttons
	

	if (polling == 1){
		Port2_Init();
		Port2_Output(GREEN);
  }
	else if (interrupting == 1){
		Port2_Init();
		Port2_Output(BLUE);
  }

	while(mode_en==0){            
		Motor_StopSimple(10);o
		REDLED = !REDLED;     // The red LED is blinking waiting for command


		if SW1IN{
			if (polling == 1){
			interrupting = 1;
			polling = 0;
			Port2_Output(RED);
			SysTick_Wait10ms(100);
        }
        else if (interrupting==1){
			Port2_Output(GREEN);
			polling = 1;
			interrupting = 0;
			SysTick_Wait10ms(100);
        }

        if SW1IN{
			mode_en=1;
        }
         
     }
     if SW2IN{
        if (interrupting == 1){
             Port2_Output(GREEN);
             polling = 1;
             interrupting = 0;
             SysTick_Wait10ms(100);
        }
        else if(polling == 1){
            interrupting = 1;
            polling = 0;
            Port2_Output(RED);
            SysTick_Wait10ms(100);
        }
         //Port2_Output(RED);
         //SysTick_Wait10ms(100);
        if SW2IN{
			mode_en=2;
        }
    }
  }


	REDLED = 0;               // Turn off the red LED
	BumpEdgeTrigger_Init();   // Initialise bump switches using edge interrupt

	Port2_Init();             // Initialise P2.2-P2.0 built-in LEDs
	Port2_Output(WHITE);      // White is the colour to represent moving forward
	Motor_InitSimple();       // Initialise DC Motor
	Motor_StopSimple(100);    // Stop the motor on initial state

  

	while(1){
      
          status = Bump_Read_Input();
        if (status == 0x6D || status == 0xAD || status == 0xCD || status == 0xE5 || status == 0xE9 || status == 0xEC) {
            checkbumpswitch(status);
          }
		  
        while(mode_en == 1){
			status = Bump_Read_Input();
            Motor_LeftSimple(500,100);
            Motor_ForwardSimple(500,250); //moves forwards

            if(interrupting == 1){
                EnableInterrupts();
                  //PORT4_IRQHandler();
              }
			  
            else if(polling == 1){
                DisableInterrupts();
                checkbumpswitch(status);
              }
        }

      

        while(mode_en==2){
			status = Bump_Read_Input();
			Motor_ForwardSimple(500,250); //moves forwards

			if(interrupting == 1){
				EnableInterrupts();
              
            }
          
			else if(polling == 1){
              DisableInterrupts();
              checkbumpswitch(status);
            }

      }
 
    }


