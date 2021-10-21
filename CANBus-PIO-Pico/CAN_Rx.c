#include "CAN_RX_D.h"


 

int LastReceived = -1;

int Send_Tx_bool = 0;



void core2_main()
{                        //second Core for Data Transmission

    PIO pio2 = pio1;     // used PIO
    uint offset2 = pio_add_program(pio2, &CAN_TX_program);  //memory where the Programm will run
    uint sm2 = pio_claim_unused_sm(pio2,true);              //find unused State Machine
    CAN_TX_program_init(pio2,sm2,offset2,CAN_PIO_TX_PIN,CAN_BAUD_Bit_Per_Second); // to make all needed setting for the Satate Machine , the function is defined in .pio file

    gpio_init(GPIO_LED_PIN);
    gpio_set_dir(GPIO_LED_PIN,GPIO_OUT);

    txhcan.Message_Identifier = 0x446;            //choosed Message ID to be Sent , it can be changed
    txhcan.DLC = 8;                               //choosed of The number of Byte to be Sent
    for (int i = 0; i < 8; i++)
        txhcan.RxData[i] = i;                    // 8 Byte to be sent

     txhcan.CRC = CRCCalculator(&txhcan);        //Calculation of the CRC based on the given Message Id , DLC , DATA

    int g = 0;
    
    while(1)
    {                                           
     
    g = multicore_fifo_pop_blocking();  // run forever and block here until data pushed on the second Core . that can be done from ACK acknlogemnt in the function CAN_getc or from the pushing the button for a normal frame

    if (g == Send_Normal_Frame)         // if nomal frame Type is pushed on the FIFO , sent the FRame
     {   

      CAN_Send_Message(&txhcan ,  pio2 ,  sm2);  
      sleep_ms(1000);
       g = 0;

    }

    else if (g == Send_Ack)         // if ACK Type is pushed tahn sent 0 and then 1 for the rest of 128 bits 
    {
        pio_sm_put_blocking(pio2,sm2,0x7FFFFFFF);
        pio_sm_put_blocking(pio2,sm2,0xFFFFFFFF);
        pio_sm_put_blocking(pio2,sm2,0xFFFFFFFF);
        pio_sm_put_blocking(pio2,sm2,0xFFFFFFFF);
        g = 0;
    }
    }
}



void button_callback(uint gpio , uint32_t events)
{
 /*
  gpio Callback interrupt at the Button PIN . 
  -gpio: gpio button 
  -events : falling Edge on the PIN 
 */
   
//Send_Tx_bool = 1;

multicore_fifo_push_blocking((uint32_t) Send_Normal_Frame); // if the button is pushed than a normal frame Type will be pushed into the second core


}




CAN_Type_Def hcan;

int main()
{
    stdio_init_all();  // for Uart use , based on printf()
    PIO pio = pio0;    // pio index to be used
    uint offset = pio_add_program(pio , &CAN_Rx_program); // memory where the programm execute
    uint sm = pio_claim_unused_sm(pio,true); // state Machine
    CAN_Rx_program_init(pio,sm,offset,CAN_PIO_RX_PIN , CAN_BAUD_Bit_Per_Second); // do all setting of the PIO . this function is defined in .pio file

     
  
    multicore_launch_core1(core2_main);  // launch the second core

    gpio_init(GPIO_CALL_BACK__Input_PIN);
    gpio_set_dir(GPIO_CALL_BACK__Input_PIN,GPIO_IN);
    gpio_pull_up(GPIO_CALL_BACK__Input_PIN);

    gpio_set_irq_enabled_with_callback(GPIO_CALL_BACK__Input_PIN,GPIO_IRQ_EDGE_FALL,true,&button_callback); // define a call back interrupt on the Button PIN

 
    while(1)
   {
   

    CAN_Rx_program_getc(pio,sm ,&LastReceived);  //run forever , this function will block here until data existe in the RX FIFO .(see CAN.c for definition)

    sleep_ms(500);

    printf("Id = %d\nDLC= %d\n CRC = %d\n",RX_CAN_Table[LastReceived].Message_Identifier,RX_CAN_Table[LastReceived].DLC,RX_CAN_Table[LastReceived].CRC); // print the received Message id , CRC , DATA

    for (int i = 0; i < RX_CAN_Table[LastReceived].DLC ; i++)
        printf("Data[%d] = %d\n",i,RX_CAN_Table[LastReceived].RxData[i]);


    pio_sm_restart(pio,sm); 

    /*
    blink the LED based on the Received DATA
    */     
     
    for (int i = 0; i < RX_CAN_Table[LastReceived].DLC ; i++)
    {
     
     for (int j = 0; j < RX_CAN_Table[LastReceived].RxData[i] ; j ++)
     {
         gpio_put(GPIO_LED_PIN,1);
         sleep_ms(500);
         gpio_put(GPIO_LED_PIN,0);
         sleep_ms(500);
     }

     sleep_ms(1000);
    }
      
      pio_sm_restart(pio,sm); 
    
    
     sleep_ms(1000);
    tight_loop_contents();

    } 

    return 0; 
}

