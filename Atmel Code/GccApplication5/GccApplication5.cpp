#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>


#define BAUD 9600 //Default Baud Rate for bluetooth device is 9600 
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

void uart_init (void)
{
	UBRRL=BAUDRATE;	
	UBRRH=(BAUDRATE>>8);
	
	UCSRB|=(1<<TXEN)|(1<<RXEN); //enable Tx and Rx
	UCSRC|=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
	UCSRC &= ~(1<<UMSEL);		//Asynchronous mode communication 
	UCSRC &= ~(1<<USBS);         //Set one stop bit
}

unsigned char uart_recieve (void)
{
	//RXC flag is one when unread data in UDR buffer and zero when UDR is empty.
	while(!(UCSRA) & (1<<RXC));//wait until received buffer UDR load new data.
	return UDR; //Return loaded data.
	
}
//FIFO Buffer Clear function
void USART_Flush( void )
{
	/*If the buffer has to be flushed during normal operation, due to for instance 
	an error condition, read the UDR I/O location until the RXC Flag is cleared.
	when buffer is empty RXC flag will be zero and break the while loop */
	unsigned char dummy;
	while ( UCSRA & (1<<RXC) ) dummy = UDR;
	
}



int main(void)
{	TCCR1A|=(1<<WGM11)|(1<<COM1A1);
	TCCR1B|=(1<<WGM12)|(1<<WGM13)|(1<<CS11);
	ICR1=19999;
	DDRD|=(1<<PD4)|(1<<PD5);
	DDRB=0xff;
	OCR1A=800;
	unsigned char byte_received;
	
	uart_init(); //Initialize UART communication
	
	
	while(1)
	{
		if ((UCSRA) & (1<<RXC))
		{
			byte_received=uart_recieve();
			USART_Flush(); //After receiving one byte FIFO buffer is cleared 
			
		switch (byte_received)
		{			
			case 1: 
			{
				PORTB =0b00000010;
			//	anticlock forwrard
			
			
			}
				break;
			case 2 :
			{
				PORTB =0b00001000;//right
				
			}
				break;
			case 3 :
			{
					PORTB=0b00001010; //right forward	
								
										
			}
				break;
			case 4 ://left
			{
				PORTB =0b00000100;
				
			}
				break;
			case 5 ://left forward
			{
						
				PORTB =0b00000110;
				
			}
				break;
			case 8://back
			{
				
				PORTB =0b00000001;	
			}
				break;
			case 10://right backward
			{
				PORTB =0b00001001;		
			}
				break;
			case 12 ://left back
			{
				PORTB =0b00000101;
			}
				break;
			case 16 :
			{
				OCR1A+=50;
				
			}
				break;
			case 32:
			{	
				OCR1A-=50;	
			}
				break;
			default:
			{
				PORTB =0b00000000;
			}
			break;
	
		}//end of switch 
		
	}//end of if
	
	
  }//end of while
  
return 0;
}