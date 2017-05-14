#include "AT89X51.h"

int delay();
void inti_lcd();
void show_lcd(int);
void cmd_wr();
void ShowState();
void clock(unsigned int Delay) ;
void DoSpeed();       //�����ٶ�
//��תֵ
#define RIGHT_RUN 1
//��תֵ
#define LEFT_RUN 0
sbit RS=0xA0;
sbit RW=0xA1;
sbit E=0xA2;

char SpeedChar[]="SPEED(n/min):";
char StateChar[]="RUN STATE:";
char STATE_CW[]="CW";
char STATE_CCW[]="CCW";
char SPEED[3]="050";
unsigned int RunSpeed=50;		//�ٶ�
unsigned char RunState=RIGHT_RUN;  //����״̬
main()
{
	
	/*��ʱ������*/
	TMOD=0x66;    	//��ʱ��0��1��Ϊ������ʽ����ʽ2��
	EA=1;			//���ж�
	
	TH0=0xff;		//��ʱ��0��ֵFFH��
	TL0=0xff;
	ET0=1;
	TR0=1;	
	
	TH1=0xff;		//��ʱ��1��ֵFFH��
	TL1=0xff;
	ET1=1;
	TR1=1;
   	
	IT0=1;			//���巽ʽ
	EX0=1;			//���ⲿ�ж�0:����
	IT1=1;			//���巽ʽ
	EX1=1;			//���ⲿ�ж�1:����
    
    inti_lcd();
    DoSpeed();       
    ShowState();

    while(1)
    { 
      clock(RunSpeed);//��ʱRunSpeed ms
      P0_1=P0_1^0x01;
    }
    
}

//��ʱ��0�жϳ���:��ת
void t_0(void) interrupt 1
{
	RunState=RIGHT_RUN;	
    P0_0=1;
    P1=0x01;
    cmd_wr();
    ShowState();
} 


//��ʱ��1�ж�:��ת
void t_1(void) interrupt 3
{
	RunState=LEFT_RUN;
    P0_0=0;
    P1=0x01;
    cmd_wr();
    ShowState();
   
} 

//�ж�0:���ٳ���
void SpeedUp() interrupt 0
{   
       if(RunSpeed>=12)
          RunSpeed=RunSpeed-2; 
       DoSpeed();
       P1=0x01;
       cmd_wr();
       ShowState();
              
}

//�ж�1:���ٳ���
void SpeedDowm() interrupt 2
{   
    
    if(RunSpeed<=100)
        RunSpeed=RunSpeed+2; 
    DoSpeed();
    P1=0x01;
    cmd_wr();
    ShowState();
        
 
}

int delay()         //�ж�LCD�Ƿ�æ
{   
    int a;
start:
    
    RS=0;
    RW=1;
    E=0;
    for(a=0;a<2;a++);
    E=1;
    P1=0xff;
    if(P1_7==0)
       return 0;
    else        
       goto start;

}

void inti_lcd()  //����LCD��ʽ
{

   P1=0x38;
   cmd_wr();
   delay();

   P1=0x01;     //���
   cmd_wr();
   delay();

   P1=0x0f;
   cmd_wr();
   delay();

   P1=0x06;
   cmd_wr();
   delay(); 

   P1=0x0c;
   cmd_wr();
   delay();
}

void cmd_wr()           //д������
{
   RS=0;
   RW=0;
   E=0;
   E=1;
}

void show_lcd(int i)   //LCD��ʾ�ӳ���
{  
   P1=i;
   RS=1;
   RW=0;
   E=0;
   E=1;

}

void ShowState()    //��ʾ״̬���ٶ�
{
    int i=0;
    while(SpeedChar[i]!='\0')
    {
       delay();
       show_lcd(SpeedChar[i]);
       i++;
    }
    
    delay();
    P1=0x80 | 0x0d;
    cmd_wr();

    i=0;
    while(SPEED[i]!='\0')
    {
       delay();
       show_lcd(SPEED[i]);
       i++;
    }

    delay();
    P1=0xC0;
    cmd_wr();

    i=0;
    while(StateChar[i]!='\0')
    {
       delay();
       show_lcd(StateChar[i]);
       i++;
    }

    delay();
    P1=0xC0 | 0x0A;
    cmd_wr();

    i=0;
    if(RunState==RIGHT_RUN)
        while(STATE_CW[i]!='\0')
        {
          delay();
          show_lcd(STATE_CW[i]);
          i++;
        }
    else
       while(STATE_CCW[i]!='\0')
        {
          delay();
          show_lcd(STATE_CCW[i]);
          i++;
        }

}

//1ms��ʱ����
void clock(unsigned int Delay)
{  
	unsigned int i; 
   for(;Delay>0;Delay--) 
    for(i=0;i<124;i++); 
     
}

void DoSpeed()
{
    SPEED[0]=(1000*6/RunSpeed/100)+48;
    SPEED[1]=1000*6/RunSpeed%100/10+48;
    SPEED[2]=1000*6/RunSpeed%10+48;
}