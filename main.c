/*-----------------------------------------------
  ���ƣ�595����4���ֵ�����
  ��̳��www.doflye.net
  ��д��shifang
  ���ڣ�2009.5
  �޸ģ��� 51��Ƭ��
  ���ݣ���+5V��GND���ӵ�Դ5V
        P0.0-P0.3�ֱ�����A��B��C��D�����ڿ���16�У�ͨ��2·HC138����
        SCKʱ������P2.5
        LATCH��������P2.6
        R1��ɫ�����ź�������P0.4
        G1��ɫ�����ź�������P2.4�����ʹ�õ���ɫ�����ź���������
        OEʹ�ܶ�һ���GND��������IO����PWM��������
        ����ʹ�ô����ſ�����β����
  ����ʹ����ͨ�ٶ�51ϵ�е�Ƭ����1��������ʾ��������������ʾ��仰���ظ�ѭ����ʾ��
------------------------------------------------*/
#include<reg52.h>

sbit T_STR = P2^6;       //�������ţ����ϱ�ʶλSTR
sbit R1    = P0^4;       //�������ţ����ϱ�ʶR1
sbit G1    = P2^4;       //�������ţ����ϱ�ʶG1
sbit T_CLK = P2^5;       //ʱ�����ţ����ϱ�ʶλCLK
sbit OE    = P2^1;       //ʹ�����ţ����ϱ�ʶEN/OE

unsigned char MoveBitNum,MoveBitNumtemp,IncNum;//�ƶ�λ��,��ʱ�ƶ�λ��������һ���ֽ������ֽ���
unsigned int HzNum;//���ָ���
unsigned char buff[10];

/*-----------------------------------------------
             16x16����ȡģ����
------------------------------------------------*/
unsigned char code hztest[][32]=    //ȡģѡ���ֽڵ��� ������ó������Ƿ��ģ������������������Զ�����
{


0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //ǰ��һ����С�Ŀ��ַ�������ʾ������������Ĵ�С�޸ģ������1��������
                                                                                 //����д��1������
/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0xC0,0x1F,0x40,0x00,
0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0xFF,0x7F,0x00,0x00,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x80,0x00,0x84,0x00,0x88,0x3F,0x48,0x00,0xA1,0x1F,0x82,0x10,0x92,0x12,0x90,0x14,
0xE8,0x7F,0x88,0x10,0x47,0x12,0x44,0x14,0xC4,0x3F,0x04,0x10,0x04,0x0A,0x00,0x04,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x08,0x00,0x10,0x3E,0x7E,0x22,0x42,0x22,0x42,0x22,0x7E,0x3E,0x42,0x22,0x42,0x22,
0x7E,0x22,0x02,0x3E,0x12,0x22,0x22,0x22,0x5A,0x21,0x46,0x21,0x82,0x28,0x40,0x10,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x00,0xC4,0x3F,0x88,0x20,0x08,0x11,0x00,0x0A,0x00,0x04,0x0F,0x1B,0xC8,0x64,
0x08,0x04,0x88,0x3F,0x08,0x04,0x28,0x04,0xD8,0x7F,0x08,0x04,0x00,0x04,0x00,0x04,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x80,0x00,0x80,0x00,0x80,0x00,0xFC,0x1F,0x84,0x10,0x84,0x10,0x84,0x10,0xFC,0x1F,
0x84,0x10,0x84,0x10,0x84,0x10,0xFC,0x1F,0x84,0x50,0x80,0x40,0x80,0x40,0x00,0x7F,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x00,0xFE,0x1F,0x00,0x08,0x00,0x04,0x00,0x02,0x80,0x01,0x80,0x00,0xFF,0x7F,
0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0xA0,0x00,0x40,0x00,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x10,0x08,0xB8,0x08,0x0F,0x09,0x08,0x09,0x08,0x08,0xBF,0x08,0x08,0x09,0x1C,0x09,
0x2C,0x08,0x0A,0x78,0xCA,0x0F,0x09,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x08,0x04,0x08,0x04,0x08,0x04,0xC8,0x7F,0x3F,0x04,0x08,0x04,0x08,0x04,0xA8,0x3F,
0x18,0x21,0x0C,0x11,0x0B,0x12,0x08,0x0A,0x08,0x04,0x08,0x0A,0x8A,0x11,0x64,0x60,

/*--  ����:     --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ����:     --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x08,0x02,0x08,0x02,0xF4,0x7F,0x02,0x02,0xE9,0x3F,0x28,0x25,0x24,0x25,0xE6,0x3F,
0x05,0x00,0xF4,0x7F,0x04,0x02,0x04,0x24,0xA4,0x44,0xA4,0x50,0x94,0x10,0x04,0x1F,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x00,0xFF,0x03,0x00,0x02,0x00,0x22,0x00,0x12,0x00,0x0A,0x00,0x06,0x00,0x0A,
0x00,0x12,0x00,0x22,0x00,0x04,0x00,0x04,0x00,0x48,0x00,0x50,0x00,0x60,0x00,0x40,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x10,0x04,0x10,0x04,0xFF,0x7F,0x90,0x04,0x80,0x00,0xFC,0x1F,0x80,0x00,0x88,0x08,
0x90,0x04,0xFE,0x3F,0xC0,0x01,0xA0,0x02,0x90,0x04,0x8C,0x18,0x83,0x60,0x80,0x00,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x00,0xFE,0x3F,0x10,0x04,0x10,0x04,0x10,0x04,0x10,0x04,0x10,0x04,0xFF,0x7F,
0x10,0x04,0x10,0x04,0x10,0x04,0x10,0x04,0x08,0x04,0x08,0x04,0x04,0x04,0x02,0x04,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x80,0x00,0x88,0x08,0x88,0x10,0x44,0x00,0xFC,0x3F,0x40,0x00,0x20,0x00,0xE0,0x1F,
0x50,0x10,0x90,0x10,0x88,0x08,0x08,0x05,0x04,0x02,0x02,0x05,0xC0,0x18,0x38,0x60,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x08,0x10,0x08,0x38,0x88,0x07,0x88,0x00,0xBF,0x00,0x88,0x3F,0x8C,0x22,0x9C,0x22,
0xAA,0x22,0xAA,0x14,0x89,0x14,0x88,0x08,0x88,0x08,0x48,0x14,0x48,0x22,0x28,0x41,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x04,0x00,0x18,0x00,0x30,0x00,0x20,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  ʵ  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x40,0x00,0x80,0x00,0xFE,0x7F,0x02,0x40,0x11,0x21,0x20,0x01,0x20,0x01,0x08,0x01,
0x10,0x01,0x10,0x01,0xFF,0x7F,0x80,0x02,0x40,0x04,0x20,0x08,0x18,0x10,0x06,0x20,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x04,0x1F,0x04,0x10,0x0A,0x12,0x0A,0x12,0x11,0x92,0x20,0x52,0x5F,0x3E,0x00,
0x20,0x22,0x20,0x24,0xB8,0x24,0x27,0x15,0x22,0x11,0x20,0x08,0xD4,0x7F,0x08,0x00,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x10,0x01,0x10,0x12,0x50,0x12,0x48,0x10,0x48,0x10,0x8C,0x08,0x8C,0x08,0x8A,0x08,
0x09,0x05,0x08,0x05,0x08,0x02,0x08,0x02,0x08,0x05,0x88,0x08,0x48,0x10,0x28,0x60,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x00,0x7C,0x3E,0x44,0x22,0x44,0x22,0x7C,0x3E,0x80,0x04,0x80,0x08,0xFF,0x7F,
0x40,0x01,0x30,0x06,0x0C,0x18,0x03,0x60,0x7C,0x3E,0x44,0x22,0x44,0x22,0x7C,0x3E,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x04,0x00,0x18,0x00,0x30,0x00,0x20,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  Ԫ  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x00,0xFC,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x7F,0x20,0x02,0x20,0x02,
0x20,0x02,0x20,0x02,0x10,0x02,0x10,0x42,0x08,0x42,0x04,0x42,0x02,0x7C,0x01,0x00,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x00,0x7C,0x3E,0x44,0x22,0x44,0x22,0x7C,0x3E,0x80,0x04,0x80,0x08,0xFF,0x7F,
0x40,0x01,0x30,0x06,0x0C,0x18,0x03,0x60,0x7C,0x3E,0x44,0x22,0x44,0x22,0x7C,0x3E,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x10,0x04,0x10,0x04,0x90,0x04,0x88,0x04,0x88,0x3F,0x4C,0x04,0x4C,0x04,0x2A,0x04,
0x09,0x04,0xE8,0x7F,0x08,0x04,0x08,0x04,0x08,0x04,0x08,0x04,0x08,0x04,0x08,0x04,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x0C,0x00,0x08,0x00,0x04,0x00,0x00,0x00,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x00,0xFF,0x7F,0x80,0x00,0x80,0x00,0xFC,0x1F,0x84,0x10,0x84,0x10,0xFC,0x1F,
0x84,0x10,0x84,0x10,0xFC,0x1F,0x88,0x00,0x50,0x00,0x60,0x00,0x98,0x03,0x07,0x7C,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x40,0x00,0x40,0x00,0xE0,0x0F,0x10,0x04,0x1C,0x02,0x20,0x01,0xC0,0x02,0x30,0x01,
0x8E,0x1F,0x40,0x10,0x30,0x08,0x4C,0x04,0x80,0x02,0x80,0x01,0x70,0x00,0x0E,0x00,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x10,0x02,0x10,0x04,0xD0,0x7F,0x08,0x00,0x08,0x00,0x8C,0x3F,0x0C,0x00,0x0A,0x00,
0x89,0x3F,0x08,0x00,0x08,0x00,0x88,0x3F,0x88,0x20,0x88,0x20,0x88,0x3F,0x88,0x20,

/*--  ����:  Ϣ  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x80,0x00,0x40,0x00,0xF8,0x0F,0x08,0x08,0xF8,0x0F,0x08,0x08,0xF8,0x0F,0x08,0x08,
0xF8,0x0F,0x08,0x08,0x80,0x00,0x10,0x21,0x12,0x49,0x12,0x48,0xE1,0x0F,0x00,0x00,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x02,0x02,0x02,0xE4,0x3F,0x04,0x02,0xC0,0x1F,0x00,0x02,0xE7,0x7F,0x04,0x00,
0xC4,0x1F,0x44,0x10,0xC4,0x1F,0x44,0x10,0xD4,0x1F,0x4C,0x10,0x44,0x14,0x40,0x08,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x08,0x05,0x88,0x34,0x88,0x24,0x88,0x24,0xBF,0x35,0x88,0x24,0x88,0x24,0xA8,0x3F,
0x18,0x04,0x8C,0x3F,0x0B,0x11,0x08,0x0A,0x08,0x04,0x08,0x0A,0x0A,0x11,0xC4,0x60,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x80,0x00,0x80,0x00,0xFC,0x1F,0x80,0x00,0x80,0x00,0xFE,0x7F,0x42,0x40,0x21,0x24,
0xF8,0x03,0x80,0x01,0x60,0x08,0xFC,0x1F,0x80,0x10,0x88,0x04,0xA4,0x08,0x42,0x10,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x00,0x00,0x11,0x80,0x08,0x40,0x04,0xC0,0x0C,0xC0,0x0C,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x08,0x02,0x08,0x02,0xF4,0x7F,0x02,0x02,0xE9,0x3F,0x28,0x25,0x24,0x25,0xE6,0x3F,
0x05,0x00,0xF4,0x7F,0x04,0x02,0x04,0x24,0xA4,0x44,0xA4,0x50,0x94,0x10,0x04,0x1F,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x00,0xFF,0x03,0x00,0x02,0x00,0x22,0x00,0x12,0x00,0x0A,0x00,0x06,0x00,0x0A,
0x00,0x12,0x00,0x22,0x00,0x04,0x00,0x04,0x00,0x48,0x00,0x50,0x00,0x60,0x00,0x40,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x10,0x04,0x10,0x04,0xFF,0x7F,0x90,0x04,0x80,0x00,0xFC,0x1F,0x80,0x00,0x88,0x08,
0x90,0x04,0xFE,0x3F,0xC0,0x01,0xA0,0x02,0x90,0x04,0x8C,0x18,0x83,0x60,0x80,0x00,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x00,0x98,0x01,0x98,0x01,0x10,0x01,0x88,0x00,0x44,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x01,0x7C,0x09,0x40,0x25,0x24,0x12,0x28,0x0A,0x10,0x04,0xE8,0x0B,0x04,0x10,
0xFB,0x6F,0x08,0x08,0x08,0x08,0xF8,0x0F,0x10,0x04,0x20,0x02,0xFE,0x3F,0x00,0x00,

/*--  ����:  ¼  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x00,0xFC,0x0F,0x00,0x08,0x00,0x08,0xF8,0x0F,0x00,0x08,0x00,0x08,0xFF,0x7F,
0x80,0x00,0x84,0x10,0x88,0x09,0xA0,0x06,0x90,0x04,0x8C,0x18,0xA3,0x60,0x40,0x00,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x08,0x08,0x10,0x04,0x20,0x02,0xFC,0x1F,0x84,0x10,0x84,0x10,0xFC,0x1F,0x84,0x10,
0x84,0x10,0xFC,0x1F,0x80,0x00,0x80,0x00,0xFF,0x7F,0x80,0x00,0x80,0x00,0x80,0x00,

/*--  ����:  Ƭ  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x02,0x08,0x02,0x08,0x02,0x08,0x02,0x08,0x02,0xF8,0x3F,0x08,0x00,0x08,0x00,
0x08,0x00,0xF8,0x07,0x08,0x04,0x08,0x04,0x08,0x04,0x04,0x04,0x04,0x04,0x02,0x04,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x08,0x00,0x88,0x0F,0x88,0x08,0x88,0x08,0xBF,0x08,0x88,0x08,0x8C,0x08,0x9C,0x08,
0xAA,0x08,0xAA,0x08,0x89,0x08,0x88,0x48,0x88,0x48,0x48,0x48,0x48,0x70,0x28,0x00,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x10,0x80,0x3F,0x7E,0x08,0x44,0x08,0x88,0x04,0xFE,0x7F,0x42,0x40,0x41,0x20,
0xFE,0x1F,0x20,0x00,0xE0,0x0F,0x50,0x08,0x88,0x04,0x04,0x03,0xC2,0x0C,0x38,0x70,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x08,0x00,0x08,0x3F,0x08,0x20,0x08,0x10,0x3F,0x08,0x24,0x04,0x24,0x04,0xA4,0x7F,
0x24,0x04,0x12,0x04,0x14,0x04,0x08,0x04,0x14,0x04,0x22,0x04,0x21,0x05,0x00,0x02,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x40,0x00,0x40,0x10,0xFC,0x0B,0x40,0x04,0x40,0x02,0xFF,0x7F,0x80,0x00,0x40,0x00,
0xF0,0x0F,0x18,0x08,0x14,0x08,0xF2,0x0F,0x11,0x08,0x10,0x08,0xF0,0x0F,0x10,0x08,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x02,0x04,0x02,0x08,0x05,0x08,0x05,0x80,0x08,0x40,0x10,0x2F,0x60,0x88,0x08,
0x88,0x04,0x88,0x02,0x88,0x01,0x88,0x00,0xA8,0x20,0x98,0x20,0x08,0x3F,0x00,0x00,

/*--  ����:  ̳  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x08,0x00,0x08,0x00,0x88,0x3F,0x08,0x00,0x08,0x00,0x3F,0x00,0xC8,0x7F,0x08,0x04,
0x08,0x04,0x08,0x02,0x08,0x02,0x38,0x11,0x87,0x20,0xC2,0x7F,0x80,0x40,0x00,0x00,

/*--  ����:  ��  --*/
/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x18,0x00,0x24,0x00,0x24,0x00,0x18,0x00,0x00,0x00,0x00,0x00,



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //β����һ���Ŀ��ַ�����������������������С�йء�
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,


};
/*-----------------------------------------------
             ��595д��һ���ֽ� ����ɫ
------------------------------------------------*/
void InputByte( unsigned  char DataR1) //дһ���ֽ�
{ 
unsigned  char i;
for(i=8; i>0; i--)
{
 R1 = !(DataR1&0x01);                   
 T_CLK = 0;
 T_CLK = 1;
 DataR1 = DataR1 >> 1; 
 } 
}
/*-----------------------------------------------
             ��595д�������ֽ� ˫ɫ
------------------------------------------------*/
void Input2Byte( unsigned  char DataR1,unsigned  char DataG1) //дһ���ֽ�
{ 
unsigned  char i;
for(i=8; i>0; i--)
{
 R1 = !(DataR1&0x01);    
 G1 = !(DataG1&0x01);                
 T_CLK = 0;
 T_CLK = 1;
 DataR1 = DataR1 >> 1; 
 DataG1 = DataG1 >> 1; 
 } 
}


/*-----------------------------------------------
                    ��ʱ����
------------------------------------------------*/
void Delay(unsigned int t)
{
 while(--t);
}


/*-----------------------------------------------
                      ������
------------------------------------------------*/
main()
{
 unsigned char count;//16��ɨ�����ݣ���Χ0-15
 unsigned int i,  j;
 unsigned char temp;

while(1)
{
 

 i++;
 if(i==60)//���������ٶ�,1T��Ƭ����12T��Ƭ���ٶȴ�Լ5-8����ע����Ĳ���
   {
   i=0;
   MoveBitNum++;
 if(MoveBitNum==16)//ÿ���ƶ���һ�����ִ�С��ѭ��
    {
      MoveBitNum=0;
	  HzNum+=1;    //������һ������
	  if(HzNum>=50)//��Ҫ��ʾ�ĺ��ָ���������ǰ���һ�����ַ��ĸ��������������Ŀ��ַ�����������,������(���ָ���+1)
	     HzNum=0;   //��ȫ��ʾ���ѭ������
    }
  }


 Delay(1);//����ɨ��Ƶ��
 for(j=0;j<2;j++) //ȡÿ�����ֵ�ǰ2���ֽڣ�
	{             //���ָ���+1
	 buff[j+j]=hztest[HzNum+j][count+count];  //ÿ���ƶ���һ�����ֺ�ѡ����һ������
	 buff[j+j+1]=hztest[HzNum+j][count+count+1];
	}

  if(MoveBitNum<8)                   //  �ж��ƶ������Ǵ���һ���ֽڻ���С��һ���ֽڣ���Ϊһ���ֽ������������ֻ��8λ
	  { IncNum=0; MoveBitNumtemp=MoveBitNum; }
  else
	  { IncNum=1; MoveBitNumtemp=MoveBitNum-8;}//����8�ͼ�ȥ8�õ�����ֵ����С��8

 T_STR=0;       //������Ч
  for(j=0;j<2;j++)          //��bit�ķ�ʽ�ƶ������������ݣ�Ȼ�������595����ȡ������ֵÿ���ֽ�����һ����λ����
	  {                                     //�������Ƴ����������ϵ�ǰ����ֽ��У��������ݵ�������	     
	   temp=(buff[j+IncNum]>>MoveBitNumtemp)|(buff[j+1+IncNum]<<(8-MoveBitNumtemp));//���Ƚ���Ҫ����Ҫ�����ó�2���ֽڵ�����ģ�����
       InputByte(temp);//�����595
	  }//8���ֽڴ������������
  OE  = 1;
  P0=15-count;//��P0�ڵ�ǰ4λ����16�У����ڲ�ͨ��4-16������������ѭ��ɨ��16��
  T_STR=1;      //������Ч����ʱһ�е�������ʾ������
  OE = 0;

 
 count++;
 if(count==16)
    count=0;
 }
}


