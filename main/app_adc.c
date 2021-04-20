#include "app_adc.h"

/**
 * @brief I2S ADC/DAC mode init.
 */
/*
void configure_i2s(){

  i2s_config_t i2s_config = 
    {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),  // I2S receive mode with ADC
    .sample_rate = adc_sample_freq,                                               // set I2S ADC sample rate
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,                                 // 16 bit I2S (even though ADC is 12 bit)
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,                                 // handle adc data as single channel (right)
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S),               // I2S format
    .intr_alloc_flags = 0,                                                        // 
    .dma_buf_count = 10,                                                           // number of DMA buffers >=2 for fastness
    .dma_buf_len = dma_buffer_len,                                                // number of samples per buffer
    .use_apll = 0,                                                                // no Audio PLL - buggy and not well documented
  };
  adc1_config_channel_atten(adc_channel, ADC_ATTEN_11db); //checking later
  adc1_config_width(ADC_WIDTH_12Bit);
  i2s_driver_install(i2s_port, &i2s_config, 0, NULL);
 
  i2s_set_adc_mode(ADC_UNIT_1, adc_channel);
  i2s_adc_enable(i2s_port);
  vTaskDelay(1000);
  //uint16_t* i2s_read_buff = (uint16_t*) calloc(i2s_buffer_len, sizeof(uint16_t));
}
*/

void example_i2s_init(void)
{
    int i2s_num = EXAMPLE_I2S_NUM;
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_TX | I2S_MODE_ADC_BUILT_IN,
        .sample_rate =  EXAMPLE_I2S_SAMPLE_RATE,
        .bits_per_sample = EXAMPLE_I2S_SAMPLE_BITS,
        .communication_format = I2S_COMM_FORMAT_STAND_MSB, //_STAND_MSB
        .channel_format = EXAMPLE_I2S_FORMAT,
        .intr_alloc_flags = 0,
        .dma_buf_count = 2,
        .dma_buf_len = 1024,
        .use_apll = 0,
    };
    //adc1_config_channel_atten(ADC1_TEST_CHANNEL, ADC_ATTEN_11db);
    //adc1_config_width(ADC_WIDTH_12Bit);
     //install and start i2s driver
    i2s_driver_install(i2s_num, &i2s_config, 0, NULL);
     //init DAC pad
     //i2s_set_dac_mode(I2S_DAC_CHANNEL_BOTH_EN);
     //init ADC pad
    i2s_set_adc_mode(I2S_ADC_UNIT, I2S_ADC_CHANNEL);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    //i2s_adc_enable(EXAMPLE_I2S_NUM);
}

/*
 * @brief erase flash for recording
 */


/**
 * @brief debug buffer data
 */
void example_disp_buf(uint8_t* buf, int length)
{
//#if EXAMPLE_I2S_BUF_DEBUG
    ets_printf("======\n");
    for (int i = 0; i < length; i++) {
        ets_printf("%02x ", buf[i]);
        if ((i + 1) % 8 == 0) {
            ets_printf("\n");
        }
    }
    ets_printf("======\n");
//#endif
}
/*
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
void getSamples(){

}


/**
 * @brief I2S ADC/DAC example
 *        1. Erase flash
 *        2. Record audio from ADC and save in flash
 *        3. Read flash and replay the sound via DAC
 *        4. Play an example audio file(file format: 8bit/8khz/single channel)
 *        5. Loop back to step 3
 */
void example_i2s_adc_dac(void*arg)
{
    
    //1. Erase flash
    int i2s_read_len = EXAMPLE_I2S_READ_LEN;
    int flash_wr_size = 0;
    size_t bytes_read, bytes_written;

    //2. Record audio from ADC and save in flash

    uint16_t* i2s_read_buff = (uint16_t*) calloc(i2s_read_len, sizeof(uint16_t));
    //uint8_t* flash_write_buff = (uint8_t*) calloc(i2s_read_len, sizeof(char));

    i2s_adc_enable(EXAMPLE_I2S_NUM);

    while(1){
        //read data from I2S bus, in this case, from ADC.
        //ets_printf("%d : ",do_sending);

        if(do_sending){

        
        i2s_read(EXAMPLE_I2S_NUM, (void*) i2s_read_buff, i2s_read_len, &bytes_read, portMAX_DELAY);
        //example_disp_buf((uint8_t*) i2s_read_buff, 64);
        for(int i=0; i<i2s_read_len; i+=2){  // caution: this is not robust to odd buffer lens

            //ws_send_buffer[i] = i2s_read_buff[i+1] & 0x0FFF;
            //ws_send_buffer[i+1] = i2s_read_buff[i] & 0x0FFF;  
            ets_printf("%04x\n",i2s_read_buff[i] & 0x0FFF);

        }
        //esp_task_wdt_reset();
        }
        else {
            vTaskDelay(1);
        }
        //example_disp_buf((uint8_t*) i2s_read_buff, 64);
        //save original data from I2S(ADC) into flash.
        //esp_partition_write(data_partition, flash_wr_size, i2s_read_buff, i2s_read_len);
        //flash_wr_size += i2s_read_len;
        //ets_printf("Sound recording %u%%\n", flash_wr_size * 100 / FLASH_RECORD_SIZE);
        
    }
    //i2s_adc_disable(EXAMPLE_I2S_NUM);

    //free(i2s_read_buff);
   // i2s_read_buff = NULL;
    //free(flash_write_buff);
    //flash_write_buff = NULL;


    //uint8_t* flash_read_buff = (uint8_t*) calloc(i2s_read_len, sizeof(char));
    //uint8_t* i2s_write_buff = (uint8_t*) calloc(i2s_read_len, sizeof(char));
    
    //free(flash_read_buff);
    //free(i2s_write_buff);*/
    //****//vTaskDelete(NULL);
}
/*
void adc_read_task(void* arg)
{
    esp_adc_cal_characteristics_t characteristics;
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, V_REF, &characteristics);
    while(1) {
        uint32_t voltage;
        esp_adc_cal_get_voltage(ADC1_TEST_CHANNEL, &characteristics, &voltage);
        ESP_LOGI(TAG, "%d mV", voltage);
        vTaskDelay(200 / portTICK_RATE_MS);
    }
}*/
