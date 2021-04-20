#include "app_adc.h"

/**
 * @brief I2S ADC/DAC mode init.
 */
void example_i2s_init(void)
{
    int i2s_num = EXAMPLE_I2S_NUM;
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_TX | I2S_MODE_ADC_BUILT_IN,
        .sample_rate =  EXAMPLE_I2S_SAMPLE_RATE,
        .bits_per_sample = EXAMPLE_I2S_SAMPLE_BITS,
        .communication_format = I2S_COMM_FORMAT_STAND_MSB, 
        .channel_format = EXAMPLE_I2S_FORMAT,
        .intr_alloc_flags = 0,
        .dma_buf_count = 10,
        .dma_buf_len = 1024,
        .use_apll = 0,
    };
    adc1_config_channel_atten(ADC1_TEST_CHANNEL, ADC_ATTEN_11db);
    adc1_config_width(ADC_WIDTH_12Bit);
     //install and start i2s driver
    i2s_driver_install(i2s_num, &i2s_config, 0, NULL);
     //init ADC pad
    i2s_set_adc_mode(I2S_ADC_UNIT, I2S_ADC_CHANNEL);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    //i2s_adc_enable(EXAMPLE_I2S_NUM);
}

/* old function used to read the data from the i2s
void getDataLoopTask(void * pvParameters){

  uint16_t* i2s_read_buff = (uint16_t*) calloc(i2s_buffer_len, sizeof(uint16_t));

  for( ;; ){
    vTaskDelay(1);          // REQUIRED TO RESET THE WATCH DOG TIMER IF WORKFLOW DOES NOT CONTAIN ANY OTHER DELAY
    if(1){  
      i2s_read(i2s_port, (void*)i2s_read_buff, i2s_buffer_len*sizeof(uint16_t), &bytes_read, portMAX_DELAY);
      if(I2S_EVENT_RX_DONE && bytes_read>0){
      for(int i=0;i<i2s_buffer_len;i++)
        printf("%04x\n",i2s_read_buff[i]);
      }  
      vTaskDelay(100/portTICK_RATE_MS); // needed less than 1-tick (1ms) delay
      //esp_task_wdt_reset();  // reset watchdog timer
    } else {
      vTaskDelay(1);  // resets watchdog with 1-tick (1ms) delay
    }
  }
}
*/

/**
 * @brief I2S ADC taking samples
 */
void example_i2s_adc_dac(void*arg)
{
    
    int i2s_read_len = EXAMPLE_I2S_READ_LEN;
    size_t bytes_read,;


    uint16_t* i2s_read_buff = (uint16_t*) calloc(i2s_read_len, sizeof(uint16_t));

    i2s_adc_enable(EXAMPLE_I2S_NUM);

    while(1){
        //read data from I2S bus, in this case, from ADC.

        if(do_sending){
        
        i2s_read(EXAMPLE_I2S_NUM, (void*) i2s_read_buff, i2s_read_len, &bytes_read, portMAX_DELAY);
        //example_disp_buf((uint8_t*) i2s_read_buff, 64);
          for(int i=0; i<i2s_read_len; i+=2){   // checking if i got the right data from the i2s_read()
            ets_printf("%04x\n",i2s_read_buff[i] & 0x0FFF);
          }

        }
        else {
            vTaskDelay(1);
        }
        
    }
    //i2s_adc_disable(EXAMPLE_I2S_NUM);

}
