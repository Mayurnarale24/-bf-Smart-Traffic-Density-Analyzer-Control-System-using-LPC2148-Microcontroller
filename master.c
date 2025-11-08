#include<lpc214x.h>
void lcd_data(unsigned char);
void lcd_init(void);
void lcd_cmd(unsigned char);
void lcd_string(unsigned char *p);
void uart0_init(void);
void uart1_init(void);
void uart0_tx(unsigned char);
void uart1_tx(unsigned char);
unsigned char uart0_rx(void);
unsigned char uart1_rx(void);
void delay_seconds(unsigned int);
void delay (void);

int main()
{
   unsigned char node1, node2;
    lcd_init();
    lcd_string("MASTER ACTIVE");
    uart0_init();  
    uart1_init();

    while(1)
    {
          node1 = uart0_rx();   // Node 1 -> UART0
        node2 = uart1_rx();   // Node 2 -> UART1

        lcd_cmd(0xC0);
        lcd_string("N1:");
        lcd_data(node1);
        lcd_string(" N2:");
        lcd_data(node2);
        delay();
        if(node1 == 'H' && node2 == 'L')
        {
            uart0_tx('G');  
            uart1_tx('R');  
            lcd_cmd(0xC0);
            lcd_string("Node1 Green ");
        }


        else if(node1 == 'L' && node2 == 'H')
        {
            uart0_tx('R');  
            uart1_tx('G');  
            lcd_cmd(0xC0);
            lcd_string("Node2 Green ");
        }


        else if(node1 == 'H' && node2 == 'H')
        {
            lcd_cmd(0xC0);
            lcd_string("Alternate Mode");
            uart0_tx('G');
            uart1_tx('R');
            delay_seconds(10);
            uart0_tx('R');
            uart1_tx('G');
            delay_seconds(10);
        }
        else if(node1 == 'L' && node2 == 'L')
        {
            uart0_tx('R');
            uart1_tx('R');
            lcd_cmd(0xC0);
            lcd_string("No Traffic");
        }
    }
}

void lcd_cmd(unsigned char x)
{
    IO0CLR = (0XFF<<16);
    IO0SET = (x<<16);
    IO0CLR|=(1<<10);
    IO0CLR|=(1<<11);
    IO0SET|=(1<<12);
    delay();
    IO0CLR|=(1<<12);
}

void lcd_data(unsigned char x)
{
    IO0CLR = (0XFF<<16);
    IO0SET = (x<<16);
    IO0SET|=(1<<10);
    IO0CLR|=(1<<11);
    IO0SET|=(1<<12);
    delay();
    IO0CLR|=(1<<12);
}

void lcd_init(void)
{
    IO0DIR|=(1<<10)|(1<<11)|(1<<12);
    IO0DIR|=(0XFF<<16);

    lcd_cmd(0x38);
    lcd_cmd(0x0C);
    lcd_cmd(0x01);
    lcd_cmd(0x80);
}

void lcd_string(unsigned char *p)
{
    while(*p!='\0')
    {
        lcd_data(*p);
        p++;
    }
}

void uart0_init(void)
{
    PINSEL0 |= 0x00000005; 
    U0LCR = 0x83;
    U0DLL = 0x3B;
    U0DLM = 0x00;
    U0LCR = 0x03;
    U0FCR = 0x07;
}

void uart1_init(void)
{
    PINSEL0 |= 0x00050000;  
    U1LCR = 0x83;
    U1DLL = 0x3B;
    U1DLM = 0x00;
    U1LCR = 0x03;
    U1FCR = 0x07;
}

void uart0_tx(unsigned char c)
{
    while(!(U0LSR & (1<<5)));
    U0THR = c;
}

void uart1_tx(unsigned char c)
{
    while(!(U1LSR & (1<<5)));
    U1THR = c;
}

unsigned char uart0_rx(void)
{
    while(!(U0LSR & 0x01));
    return U0RBR;
}

unsigned char uart1_rx(void)
{
    while(!(U1LSR & 0x01));
    return U1RBR;
}

void delay_seconds(unsigned int s)
{
    unsigned int i;
    for(i=0;i<s;i++)
        delay();
}
void delay(void)
{
    int i,j;
    for(i=0;i<200;i++)
        for(j=0;j<400;j++);
}
