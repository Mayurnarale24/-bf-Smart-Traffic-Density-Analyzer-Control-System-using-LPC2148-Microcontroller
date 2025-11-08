#include<lpc214x.h>
void lcd_data(unsigned char);
void lcd_init(void);
void lcd_cmd(unsigned char);
void lcd_string(unsigned char *p);
void uart_init(void);
void trans(unsigned char);
void uart_string(unsigned char *str);
unsigned char receive(void);
void delay (void);
int main()
{
	unsigned char rx;
	PINSEL0=0X00000000;
 IO0DIR  = 0x00000098;      
    IO0CLR  = 0x00000098;
	IO0PIN=0X06000000;
 
	uart_init();
lcd_init();
lcd_string("NODE-1");
	
while(1)
{
if((IO0PIN&0X02000000)==0)
{
lcd_cmd(0XC0);
	lcd_string("HIGH TRAFFIC");
	trans('H');
	delay();
}
 else if ((IO0PIN&0X04000000)==0)
{
lcd_cmd(0XC0);
	lcd_string("LOW TRAFFIC");
		trans('L');
	delay();
}
 if(U0LSR & 0x01)    
        {
            rx = receive();

            if(rx == 'G')   
            {
                IO0SET = 0X00000080;   
                IO0CLR = 0X00000008; 
							lcd_cmd(0X01);
lcd_cmd(0X80);		
				lcd_string("GO");								
                lcd_cmd(0xC0);
                lcd_string("GREEN ON");
            }
            else if(rx == 'R')  
            {
                IO0SET = 0X00000008;  
                IO0CLR = 0X00000080;
							lcd_cmd(0X01);
							lcd_cmd(0X80);		
				lcd_string("STOP");	
                lcd_cmd(0xC0);
                lcd_string("RED ON");
            }
        }
}

}

void lcd_cmd(unsigned char x)
{
IO0CLR|=(0XFF<<16);
IO0SET|=(x<<16);
IO0CLR|=(1<<10);
IO0CLR|=(1<<11);
IO0SET|=(1<<12);
delay();
IO0CLR|=(1<<12);
}
void lcd_data(unsigned char x)
{
IO0CLR|=(0XFF<<16);
IO0SET|=(x<<16);
IO0SET|=(1<<10);
IO0CLR|=(1<<11);
IO0SET|=(1<<12);
delay();
IO0CLR|=(1<<12);
}
void lcd_init(void)
{
IO0DIR|=(1<<10);
IO0DIR|=(1<<11);
IO0DIR|=(1<<12);
IO0DIR|=(0XFF<<16);
lcd_cmd(0X30);
lcd_cmd(0X38);
lcd_cmd(0X01);
lcd_cmd(0X80);
lcd_cmd(0X0f);
}
void lcd_string(unsigned char *p)
{
	while(*p!='\0')
	{
		lcd_data(*p);
		p++;
}
}
void uart_init(void)
{
	PINSEL0=0X00000005;
	U0LCR=0X80;
	U0DLL=0X3B;
	U0DLM=0X00;
	U0FDR=0X00000010;
	U0LCR=0X03;
	U0FCR=0X07;
}
void trans(unsigned char x)
{
	U0THR=x;
	while((U0LSR & (1<<5))==0);
}
void uart_string(unsigned char *str)
{
	while(*str!='\0')
		trans(*str);
	str++;
}
unsigned char receive(void)
{
    while(!(U0LSR & 0x01));   
    return U0RBR;              
}
void delay(void)
{
int i,j;
for(i=0;i<100;i++)
{
for(j=0;j<200;j++)
{}
}
}

