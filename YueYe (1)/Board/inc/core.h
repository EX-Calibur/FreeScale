#include "common.h"
#include "include.h"

/************��������*************/
/*ͼ�����*/
extern uint8 imgbuff[CAMERA_SIZE];                             //����洢����ͼ�������[ͼ��ռ�ÿռ��С]
extern uint8 img[CAMERA_W*CAMERA_H];                           //����ӥ������ͷ��һ�ֽ�8�����أ������Ҫ��ѹΪ 1�ֽ�1�����أ����㴦��
extern uint8 img_OLED[CAMERA_H][CAMERA_W];                      //ͼ����
/*�ٶȲ���*/
#define SPEED_STABLE 1500
#define SPEED_SLOW 800
extern uint8 speed_flag;
extern uint16 speed_out;
/*�������*/
#define SD5_FTM   FTM0
#define SD5_CH    FTM_CH2
#define SD5_HZ    (200)
#define STEER_MID 3150
#define STEER_RIGHT_MAX 2650
#define STEER_LEFT_MAX 3650
extern uint16 steer_mid_debug;
extern uint16 steer_mid;
extern uint16 limit_speed;
extern uint16 turn_distance;
extern uint16 steer_wucha;
extern int debug_num;
/*�ж�*/
extern uint8 ud;
extern uint8 limit_flag;
/*�������*/
/*������ģ�����*/
#define TRIG PTE4
#define ECHG PTE5
#define TRIG_LEFT PTE2
#define ECHG_LEFT PTE3
#define TRIG_RIGHT PTE0
#define ECHG_RIGHT PTE1
//#define UL_FLAGWAIT    0xFFFFF    
extern uint32 ultrasonic_timevar;
extern uint32 ultrasonic_flag;
extern uint16 ultrasonic_distance;
/************��������*************/
/*������ģ��*/ 
void Ultrasonic_Init();
void Ultrasonic_Compute();
/*���*/
void motor_init(void); 
/*�ٶȿ���*/
void speed_set(void);
void speed_limit();
/*OLED*/
extern uint8 menu_choose;
extern uint8 parameter_choose;
extern uint8 scan_choose;
extern uint8 menu_model;
extern uint8 oled_show_img_flag;
void OLED_Init();
void OLED_show();
void OLED_model();
void OLED_scan_model();
void OLED_show_img();
void OLED_Parameter();
/*ͼ��*/
extern uint8 size;
extern uint8 model;
#define WHIRL_SIZE 100
void shanwai_sendimg(uint8 *imgaddr, uint32 imgsize);          //����ͼ����λ��
void img_extract(uint8 *dst, uint8 *src, uint32 srclen);       //��ȡͼ���ֵ��ͼ���ѹ
__RAMFUNC void Scan(); 
__RAMFUNC void go_s();
__RAMFUNC void left_s(uint8);
__RAMFUNC void right_s(uint8);
__RAMFUNC void turn_s(uint8);
/*���*/
void Steer_Init();
/*ת��*/
extern uint16 stop_time;
extern uint8 turn_direction;
void Turn();
__RAMFUNC void go();
void back();
void stop();
__RAMFUNC void left();
__RAMFUNC void right();
void slow();
void front();
/*������*/
extern uint8 pit0_flag;
extern uint32 speed_count;
void Encoder_Compute();
/*���뿪��*/
#define boma_1  PTD0            //�˵�
#define boma_2  PTD1            //����ͼ����λ��
#define boma_3  PTD2            //�����ջ�
#define boma_4  PTD3            //����
#define boma_5  PTD4            //�����ٶ�
#define boma_6  PTD5            //���ٶ�
#define boma_7  PTD6            //���ٶ�
#define boma_8  PTD7            //�����ٶ�
void boma_key();
/*����*/
void checkKey();
/*�жϺ���*/
void PIT0_IRQHandler();
void PIT2_IRQHandler();
void PORTA_IRQHandler();
void DMA0_IRQHandler();
void DMA1_IRQHandler();
void uart4_handler();
/*��λ��*/
void simular();