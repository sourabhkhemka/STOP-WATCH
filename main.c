/*
 * receiver.c
 *
 * Created: 05-11-2017 22:46:55
 * Author : USER -DELL
 */ 

#include <avr/io.h>
#include <util/delay.h>
/* Prototypes */
void USART_Init (unsigned int baud);
void USART_Transmit (unsigned char data);
unsigned char USART_Receive (void);
#define SETBIT(ADDRESS, BIT) (ADDRESS |= (1 << BIT))
#define CLEARBIT(ADDRESS, BIT) (ADDRESS &= ~(1 << BIT))
#define CHECKBIT(ADDRESS, BIT) (!(ADDRESS & (1 << BIT)))
#define LED1 PE6 //Connected to PORTE
#define LED2 PE7 //Connected to PORTE


void timer_init()
{
	TCCR1B |= (1<<CS12) | (1<<CS10);
	TCNT1 = 0;
}

void transmit(int num)
{
  if (num == 0)
  {
	  USART_Transmit_Number(48);
	  USART_Transmit_Number(48);
  }
  	
 
  int mod=1,temp, numb;
  numb = num;

  while(num != 0)
  {
	  num = num / 10;
	  mod *= 10;
  }
  mod /= 10;

  while(mod > 0)
  {
	  USART_Transmit_Number((numb/mod)+48);
	  numb = numb % mod;
	  mod = mod/10;
  }
}

int main (void)
{
	//char message[] = "WELCOME MONITOR TO ROBOLUTION's AVR!\r\n";
	
	int i=0, sec=0, min=0, hour=0;
	
	/* Set the baudrate to 19.2 kbps using a 16.00 MHz crystal
	Fuse bits set to handle CPU with external Crystal Oscillator */
	unsigned char a=100;
	
	USART_Init (12);//12 :: 9600
	DDRE = 0xC0;
	
	timer_init();
	
	while (1)
	{	 
		if(TCNT1 > 1952)
		{
			
	 			char message3[] = "\r\n";
	 			while(message3[i] != '\0')
	 			{
		 			USART_Transmit(message3[i++]);
		 			//_delay_ms(500);
	 			}
	 			i = 0;
	 			
	 			char message4[] = "\033[1A";
	 			while(message4[i] != '\0')
	 			{
		 			USART_Transmit(message4[i++]);
		 			//_delay_ms(500);
	 			}
	 			i = 0;
	 			
	 			char message5[] = "\033[K";
	 			while(message5[i] != '\0')
	 			{
		 			USART_Transmit(message5[i++]);
		 			//_delay_ms(500);
	 			}
	 			i = 0;
				 
			TCNT1 = 0;
			sec++;
			PORTE |= (1<<PE7)|(1<<PE6);
			if(sec == 60)
			{
				sec = 0;
				min++;
				
				if (min == 60)
				{
					hour++;
					min = 0;
				}
			}
			
			
			
			transmit(hour);
			char message1[] = "::";
			
			while(message1[i] != '\0')
			{
				USART_Transmit(message1[i++]);
				//_delay_ms(500);
			}
			i = 0;
			
			transmit(min);
			char message2[] = "::";
			
			while(message2[i] != '\0')
			{
				USART_Transmit(message2[i++]);
				//_delay_ms(500);
			}
			i = 0;


			transmit(sec);
			
		}

/////////		
		
		
	}
	
	return 0;
}
//Initialize USART
void USART_Init (unsigned int baud)
{

	UCSR1B = 0x00;							//disable while setting baud rate
	UCSR1A = 0x00;
	UCSR1C = 0x06;
	UBRR1L = 0x0C; 							//9600BPS at 14745600Hz
	UBRR1H = 0x00;
	UCSR1B = 0x98;
}
//Read and Write functions
void USART_Transmit (unsigned char data)
{
	/* Wait for empty transmit buffer */
	while (!(UCSR1A & (1 << UDRE1)));
	/* Put data into buffer, transmits the data */
	UDR1 = data;
}

void USART_Transmit_Number (unsigned char data)
{
	/* Wait for empty transmit buffer */
	while (!(UCSR1A & (1 << UDRE1)));
	/* Put data into buffer, transmits the data */
	UDR1 = data;
}

unsigned char USART_Receive (void)
{
	/* Wait for data to be received */
	while (!(UCSR1A & (1 << RXC1)));
	/* Get and return received data from buffer */
	return UDR1;
}

// 		while(message[i] != '\0')
// 		{
	// 			USART_Transmit(message[i++]);
	// 			//_delay_ms(500);
// 		}
// 		i = 0;
