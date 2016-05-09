#include "common.h"
#include "include.h"
#include "math.h"



void Init_All();
void Init_Key();
void boma_key();
void  main(void)
{
    
    /*��ʼ��*/
    Init_All();
    Init_Key();
//    boma_key();
//    speed_set();
    while(1)
    { 
       camera_get_img(); 
//       shanwai_sendimg(imgbuff, CAMERA_SIZE);                  //���͵���λ��
       Encoder_Compute();
//      if(obstacle_flag)
       Turn();
       checkKey();
       speed_limit();
       OLED_show();
//        simular();
//       uart_tx_irq_en(UART4);
    }
}
void Init_All()
{ 
    OLED_Init();                                   //��ʼ��OLED
    motor_init();                                  //��ʼ�����
    uart_init(UART4,115200);                       //��ʼ��UART4����������
    FTM_PWM_init(SD5_FTM,SD5_CH,SD5_HZ,STEER_MID);        //��ʼ�����
    Ultrasonic_Init();                             //��ʼ��������ģ��
    camera_init(imgbuff);                          //��ʼ������ͷ
    
    /*************������****************/
    DMA_count_Init(DMA_CH1, PTC5, 0x7FFF, DMA_falling_keepon);
    /*************������****************/
    gpio_init(PTC15,GPO,0);
    /*************����ͷ****************/
    set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);
    enable_irq (PORTA_IRQn);
    /*************PIT��ʱ�ж�**************/
    pit_init_ms(PIT0,50);                     //////////////////////////////////////////////////////
    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);
    enable_irq(PIT0_IRQn);
//    pit_init_ms(PIT2,200);
//    set_vector_handler(PIT2_VECTORn ,PIT2_IRQHandler);
//    enable_irq(PIT2_IRQn);
    /*************�ж�����***************/
//    NVIC_SetPriority(PIT2_IRQn,0);
    NVIC_SetPriority(UART4_RX_TX_IRQn,0);          //�������ȼ�
    NVIC_SetPriority(PIT0_IRQn,1);
    NVIC_SetPriority(PORTA_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),2,1));
    NVIC_SetPriority(DMA0_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),2,2));
    set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //����LPTMR���жϸ�λ����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA1_VECTORn ,DMA1_IRQHandler); 
//    set_vector_handler(UART4_RX_TX_VECTORn,uart4_handler);
//    uart_tx_irq_en(UART4);
    /***************���Ź�**************/
//    wdog_init_ms(5000);
//    wdog_enable();
}
void Init_Key()
{
    /********���뿪��*********/
    gpio_init(PTD0,GPI,0);
    gpio_init(PTD1,GPI,0);
    gpio_init(PTD2,GPI,0);
    gpio_init(PTD3,GPI,0);
    gpio_init(PTD4,GPI,0);
    gpio_init(PTD5,GPI,0);
    gpio_init(PTD6,GPI,0);
    gpio_init(PTD7,GPI,0);
    /*********��������********/
    key_init(KEY_U);
    key_init(KEY_D);
    key_init(KEY_L);
    key_init(KEY_R);
    key_init(KEY_A);
}