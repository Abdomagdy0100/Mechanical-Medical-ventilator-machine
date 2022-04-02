/*
 CURRENTLY USED PIN IN VENTILATOR:-

 PIN (PORT_C2) STEPPER MOTOR 1 STEP
 PIN (PORT_C1) STEPPER MOTOR 2 STEP 
 PIN (PORT_B1) STEPPER MOTOR 1 DIRECTION
 PIN (PORT_B0) STEPPER MOTOR 2 DIRECTION 
 PINS(AN0,AN1,AN2)INTERFACE POTENTIOMETER
 PIN (PORT_D1) WRITE EEPROM TIME IE,LENGTH
 PIN (PORT_D7,PORT_D6) LIMIT SWITCH STEPPER MOTOR
 PINS(PORT_B7,PORT_B6,PORT_B5,PORT_B4,PORT_B3,PORT_B2) LCD
 PIN (PORT_D4) CHANGE VALUE(I:E,IE_PER_MIN,VOLUME) EN
 */
 char character1[] = {8,12,14,15,14,12,8,0};
 char character2[] = {27,27,27,27,27,27,27,0};
void CustomChar(char pos_row, char pos_char, char *custom_char) {
  char i;
    Lcd_Cmd(64);
    for (i = 0; i<=7; i++){
     Lcd_Chr_CP(custom_char[i]);
    }
    Lcd_Cmd(_LCD_RETURN_HOME);
    Lcd_Chr(pos_row, pos_char, 0);
}
sbit LCD_RS at RB7_bit;
sbit LCD_EN at RB6_bit;
sbit LCD_D4 at RB5_bit;
sbit LCD_D5 at RB4_bit;
sbit LCD_D6 at RB3_bit;
sbit LCD_D7 at RB2_bit;

sbit LCD_RS_Direction at TRISB7_bit;
sbit LCD_EN_Direction at TRISB6_bit;
sbit LCD_D4_Direction at TRISB5_bit;
sbit LCD_D5_Direction at TRISB4_bit;
sbit LCD_D6_Direction at TRISB3_bit;
sbit LCD_D7_Direction at TRISB2_bit;

void main() {
  float vol,x,x2,x3,Length,DELAY_PULSE_I,DELAY_PULSE_E
  ,ITOE,IEPerM,counter_pulse,er[2];
  unsigned short c,con,er12[1];
  char f1[17];
  char txt1[]="Running..";
  char txt2[]="Vol=";
  char txt3[]="I:E= 1:";
  char txt4[]="IEPerM=";
  char txt5[]="Stop...  ";
  char txt6[]="T=";
  IEPerM=12;
  ITOE=1;
  x=0;

  con=0;
  length=6724;
  counter_pulse=0;
  trisd.b7=1;
  trisd.b6=1;
  trisd.b1=1;
  trisd.b5=0;
  trisd.b4=1;
  trisb.b0=0;
  trisb.b1=0;
  trisc.b1=0;
  trisc.b2=0;
  adc_init();
  Lcd_Init();
  lcd_cmd(_lcd_clear);
  lcd_cmd(_lcd_cursor_off);
while(portd.b1==0)
{
  Lcd_out(2, 2, txt2);
  Lcd_out(3, 2, txt6);
  if(portd.b4==1)
   {
    CustomChar(2, 1,character1);
    Lcd_Chr(3, 1, ' ');
    vol=adc_read(0);
    itoe=adc_read(1);
    ieperm=adc_read(2);
    x=vol+ieperm+itoe;
    x=(x/(1021*3))*100;
    floatTostr(x,f1);
    f1[4]='%';
    for(c=5;c<=12;c=c++)
    {
     f1[c]=' ';
    }
    Lcd_out(2, 9, f1);
    er[0]=x;
    delay_ms(250);
   }else
   {
    Lcd_Chr(2, 1, ' ');
    CustomChar(3, 1,character1);
    vol=adc_read(0);
    itoe=adc_read(1);
    ieperm=adc_read(2);
    x=vol+ieperm+itoe;
    x=(x/(1024*3))*100;
    floatTostr(x,f1);
    f1[4]='%';
    for(c=5;c<=12;c=c++)
    {
     f1[c]=' ';
    }
    Lcd_out(3, 9, f1);
    er[1]=x;
    delay_ms(250);
   }
   c=1;
  }
  if(c==1)
  {
   er12[0]=er[0];
   er12[1]=er[1];
   delay_ms(25);
   eeprom_write(0,er12[0]);
   delay_ms(25);
   eeprom_write(1,er12[1]);
   delay_ms(25);
  }
  delay_ms(25);
  er[0]=eeprom_read(0);
  delay_ms(25);
  er[1]=eeprom_read(1);
  delay_ms(25);
  x=(er[0]*length)/100; //****
  length=x+length;
  if(er[1]>100||er[0]>100)
  {
   er[0]=0;
   er[1]=0;
  }  
  x=0;
  c=0;
  lcd_cmd(_lcd_clear);
  CustomChar(1, 1,character1);
  LCD_OUT(1,2,txt1);
  LCD_OUT(2,1,txt2);
  LCD_OUT(3,1,txt3);
  LCD_OUT(4,1,txt4);
  portd.b5=1;
  delay_ms(10);
  portd.b5=0;
  portb.b0=0;
  portb.b1=0;
  portc.b1=0;
  portc.b2=0;
  IEPerM=adc_read(2);
     if(IEPerM>=0&&IEPerM<=102)
     {
       IEPerM=12.1;
     }
     if(IEPerM>=103&&IEPerM<=204)
     {
       IEPerM=16;
     }
     if(IEPerM>=205&&IEPerM<=306)
     {
       IEPerM=18.1;
     }
     if(IEPerM>=307&&IEPerM<=408)
     {
       IEPerM=22.1;
     }
     if(IEPerM>=409&&IEPerM<=510)
     {
       IEPerM=24.1;
     }
     if(IEPerM>=511&&IEPerM<=612)
     {
       IEPerM=30;
     }
     if(IEPerM>=613&&IEPerM<=714)
     {
       IEPerM=34.1;
     }
     if(IEPerM>=715&&IEPerM<=816)
     {
       IEPerM=40;
     }
     if(IEPerM>=817)
     {
       IEPerM=60;
     }
     FloatToStr(IEPerM,f1);
     for(c=2;c<15;c=c++)
     {
      f1[c]=' ';
     }
     LCD_chr(4,8,f1[0]);
     LCD_chr(4,9,f1[1]);
     vol=adc_read(0);
     if(vol<256)
     {
       f1[0]='0';
       f1[1]='.';
       f1[2]='5';
       x2=length/4;
     }
     if(vol>=257&&vol<=512)
     {
       f1[0]='1';
       f1[1]='.';
       f1[2]='0';
       x2=length/2;
     }
     if(vol>=513&&vol<=768)
     {
       f1[0]='1';
       f1[1]='.';
       f1[2]='5';
       x2=(75*length)/100;
     }
     if(vol>=769)
     {
       f1[0]='2';
       f1[1]='.';
       f1[2]='0';
       x2=length;
     }
      for(c=4;c<=10;c=c++)
      {
       f1[c]=' ';
      }
      f1[3]='L';
      LCD_chr(2,6,f1[0]);
      LCD_chr(2,7,f1[1]);
      LCD_chr(2,8,f1[2]);
      LCD_chr(2,9,f1[3]);

     ItoE=adc_read(1);
     if(ItoE>=0&&ItoE<=146)
     {
       ItoE=1;
     }
     if(ItoE>=147&&ItoE<=293)
     {
       ItoE=1.5;
     }
     if(ItoE>=294&&ItoE<=439)
     {
       ItoE=2;
     }
     if(ItoE>=440&&ItoE<=585)
     {
       ItoE=2.5;
     }
     if(ItoE>=586&&ItoE<=731)
     {
       ItoE=3;
     }
     if(ItoE>=732&&ItoE<=877)
     {
       ItoE=3.5;
     }
     if(ItoE>=878)
     {
       ItoE=4;
     }
     floattostr(ItoE,f1);
     if(itoe==1||itoe==2||itoe==3||itoe==4)
     {
      for(c=1;((itoe==1||itoe==2||itoe==3||itoe==4)&&(c<15));c=c++)
      {
       f1[c]=' ';
      }
     }
     LCD_chr(3,8,f1[0]);
     LCD_chr(3,9,f1[1]);
     LCD_chr(3,10,f1[2]);
     DELAY_PULSE_i=(((60/IEPerM)/2)*10)+((er[1]*(((60/IEPerM)/2)*10))/100);
     DELAY_PULSE_E=(DELAY_PULSE_i/itoe);
     x3=x2;
  while(1)
  {
            //||
    while(portd.B7==1||portd.B6==1)      //<<<<<<<<<<E
    {
     if(con==1)
     {
      CustomChar(1, 1,character1);
      LCD_OUT(1,2,txt1);
      con=0;
     }
     while(portd.b4==0)
     {
      con=1;
      CustomChar(1, 1,character2);
      LCD_OUT(1,2,txt5);
      IEPerM=adc_read(2);
      if(IEPerM>=0&&IEPerM<=102)
      {
       IEPerM=12.1;
      }
      if(IEPerM>=103&&IEPerM<=204)
      {
       IEPerM=16;
      }
      if(IEPerM>=205&&IEPerM<=306)
      {
       IEPerM=18.1;
      }
      if(IEPerM>=307&&IEPerM<=408)
      {
       IEPerM=22.1;
      }
      if(IEPerM>=409&&IEPerM<=510)
      {
       IEPerM=24.1;
      }
      if(IEPerM>=511&&IEPerM<=612)
      {
       IEPerM=30;
      }
      if(IEPerM>=613&&IEPerM<=714)
      {
       IEPerM=34.1;
      }
      if(IEPerM>=715&&IEPerM<=816)
      {
       IEPerM=40;
      }
      if(IEPerM>=817)
      {
       IEPerM=60;
      }
     FloatToStr(IEPerM,f1);
     for(c=2;c<15;c=c++)
     {
      f1[c]=' ';
     }
     LCD_chr(4,8,f1[0]);
     LCD_chr(4,9,f1[1]);

      vol=adc_read(0);
     if(vol<256)
     {
       f1[0]='0';
       f1[1]='.';
       f1[2]='5';
       x2=length/4;
     }
     if(vol>=257&&vol<=512)
     {
       f1[0]='1';
       f1[1]='.';
       f1[2]='0';
       x2=length/2;
     }
     if(vol>=513&&vol<=768)
     {
       f1[0]='1';
       f1[1]='.';
       f1[2]='5';
       x2=(75*length)/100;
     }
     if(vol>=769)
     {
       f1[0]='2';
       f1[1]='.';
       f1[2]='0';
       x2=length;
     }
      for(c=4;c<=10;c=c++)
      {
       f1[c]=' ';
      }
      f1[3]='L';
      LCD_chr(2,6,f1[0]);
      LCD_chr(2,7,f1[1]);
      LCD_chr(2,8,f1[2]);
      LCD_chr(2,9,f1[3]);
      ItoE=adc_read(1);
      if(ItoE>=0&&ItoE<=146)
      {
       ItoE=1;
      }
      if(ItoE>=147&&ItoE<=293)
      {
       ItoE=1.5;
      }
      if(ItoE>=294&&ItoE<=439)
      {
       ItoE=2;
      }
      if(ItoE>=440&&ItoE<=585)
      {
       ItoE=2.5;
      }
      if(ItoE>=586&&ItoE<=731)
      {
       ItoE=3;
      }
      if(ItoE>=732&&ItoE<=877)
      {
       ItoE=3.5;
      }
      if(ItoE>=878)
      {
       ItoE=4;
      }
      floattostr(ItoE,F1);
      if(itoe==1||itoe==2||itoe==3||itoe==4)
      {
       for(c=1;((itoe==1||itoe==2||itoe==3||itoe==4)&&(c<=10));c=c++)
       {
        f1[c]=' ';
       }
      }
      LCD_chr(3,8,f1[0]);
      LCD_chr(3,9,f1[1]);
      LCD_chr(3,10,f1[2]);
     DELAY_PULSE_i=(((60/IEPerM)/2)*10)+((er[1]*(((60/IEPerM)/2)*10))/100);
     DELAY_PULSE_E=(DELAY_PULSE_i/itoe);
     FloatToStr(DELAY_PULSE_E,f1);
     lcd_out(1,12,f1);
      x3=x2;
     }
     if(portd.B7==1)
     {
      portc.b1=1;
     }
     if(portd.B6==1)
     {
      portc.b2=1;
     }
     while(counter_pulse<=DELAY_PULSE_e)
     {
       //delay_us(1);
       counter_pulse=counter_pulse+1;
     }
     counter_pulse=0;
     portc.b1=0;
     portc.b2=0;
     while(counter_pulse<=DELAY_PULSE_e)
     {
       //delay_us(1);
       counter_pulse=counter_pulse+1;
     }
     counter_pulse=0;
    }

    portb.b0=1;
    portb.b1=1;
    if(portd.b7==0&&portd.b6==0)
    {
      delay_ms(1);
     }
          //VOL
          //||
    while(x<=x3)                      //<<<<<<<<<<I
    {
     if(con==1)
     {
      CustomChar(1, 1,character1);
      LCD_OUT(1,2,txt1);
      con=0;
     }
     while(portd.B4==0)
     {
      con=1;
      CustomChar(1, 1,character2);
      LCD_OUT(1,2,txt5);
      IEPerM=adc_read(2);
      if(IEPerM>=0&&IEPerM<=102)
      {
       IEPerM=12.1;
      }
      if(IEPerM>=103&&IEPerM<=204)
      {
       IEPerM=16;
      }
      if(IEPerM>=205&&IEPerM<=306)
      {
       IEPerM=18.1;
      }
      if(IEPerM>=307&&IEPerM<=408)
      {
       IEPerM=22.1;
      }
      if(IEPerM>=409&&IEPerM<=510)
      {
       IEPerM=24.1;
      }
      if(IEPerM>=511&&IEPerM<=612)
      {
       IEPerM=30;
      }
      if(IEPerM>=613&&IEPerM<=714)
      {
       IEPerM=34.1;
      }
      if(IEPerM>=715&&IEPerM<=816)
      {
       IEPerM=40;
      }
      if(IEPerM>=817)
      {
       IEPerM=60;
      }
     FloatToStr(IEPerM,f1);
     for(c=2;c<15;c=c++)
     {
      f1[c]=' ';
     }
     LCD_chr(4,8,f1[0]);
     LCD_chr(4,9,f1[1]);

     vol=adc_read(0);
     if(vol<256)
     {
       f1[0]='0';
       f1[1]='.';
       f1[2]='5';
       x2=length/4;
     }
     if(vol>=257&&vol<=512)
     {
       f1[0]='1';
       f1[1]='.';
       f1[2]='0';
       x2=length/2;
     }
     if(vol>=513&&vol<=768)
     {
       f1[0]='1';
       f1[1]='.';
       f1[2]='5';
       x2=(75*length)/100;
     }
     if(vol>=769)
     {
       f1[0]='2';
       f1[1]='.';
       f1[2]='0';
       x2=length;
     }
      for(c=4;c<=10;c=c++)
      {
       f1[c]=' ';
      }
      f1[3]='L';
      LCD_chr(2,6,f1[0]);
      LCD_chr(2,7,f1[1]);
      LCD_chr(2,8,f1[2]);
      LCD_chr(2,9,f1[3]);
      ItoE=adc_read(1);
      if(ItoE>=0&&ItoE<=146)
      {
       ItoE=1;
      }
      if(ItoE>=147&&ItoE<=293)
      {
       ItoE=1.5;
      }
      if(ItoE>=294&&ItoE<=439)
      {
       ItoE=2;
      }
      if(ItoE>=440&&ItoE<=585)
      {
       ItoE=2.5;
      }
      if(ItoE>=586&&ItoE<=731)
      {
       ItoE=3;
      }
      if(ItoE>=732&&ItoE<=877)
      {
       ItoE=3.5;
      }
      if(ItoE>=878)
      {
       ItoE=4;
      }
      floattostr(ItoE,F1);
      if(itoe==1||itoe==2||itoe==3||itoe==4)
      {
       for(c=1;((itoe==1||itoe==2||itoe==3||itoe==4)&&(c<=10));c=c++)
       {
        f1[c]=' ';
       }
      }
     LCD_chr(3,8,f1[0]);
     LCD_chr(3,9,f1[1]);
     LCD_chr(3,10,f1[2]);
     DELAY_PULSE_i=(((60/IEPerM)/2)*10)+((er[1]*(((60/IEPerM)/2)*10))/100);
     DELAY_PULSE_E=(DELAY_PULSE_i/itoe);

     }
     portc.b1=1;
     portc.b2=1;
     while(counter_pulse<=DELAY_PULSE_i)
     {
       //delay_us(1);
       counter_pulse=counter_pulse+1;
     }
     counter_pulse=0;
     portc.b1=0;
     portc.b2=0;
     while(counter_pulse<=DELAY_PULSE_i)
     {
       //delay_us(1);
       counter_pulse=counter_pulse+1;
     }
     counter_pulse=0;
     x=x+1;
   }
   x=0;
   x3=x2;
   portb.b0=0;
   portb.b1=0;
   delay_ms(1);
  }
}