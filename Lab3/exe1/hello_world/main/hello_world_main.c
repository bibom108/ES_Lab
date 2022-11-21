#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include <stdio.h>


void task1( void * parameter )
{
  printf((char *)parameter);  
  while(1){
    printf("task1 --- running  \r\n");
    for (int i = 0 ; i < 1000; i++);
  }
}

void task2( void * parameter )
{
  printf((char *)parameter);
  while (1){
    printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX \r\n");
    for (int i = 0 ; i < 100000; i++);
  }
} 

void task_idle( void * parameter )
{
  printf((char *)parameter);
  while (1){
    printf("task_idle OOO running \r\n");
    printf(ulIdleCycleCount);
    printf("\r\n");
    if (ulIdleCycleCount > 1000) {
      ulIdleCycleCount = 0;
    }
  }
}

volatile uint32_t ulIdleCycleCount = 0UL;
void vApplicationIdleHook ( void )
{
    ulIdleCycleCount ++;
}

void app_main() {
  char task1Param[12] = "taskParam";
  esp_register_freertos_idle_hook(vApplicationIdleHook);
  xTaskCreate(
      &task1,        // Task function
      "task1",      // name of task
      5000,        // Stack size of task
      (void *)task1Param,  // parameter of the task
      1,            // priority of the task 
      NULL);        // Task handle to keep track of created task 
  xTaskCreate(
      &task2,        // Task function. s
      "task2",      // name of task. 
      5000,        // Stack size of task 
      (void *)task1Param,  // parameter of the task 
      4,            // priority of the task 
      NULL);        // Task handle to keep track of created task 
  xTaskCreate(
      &task_idle,        // Task function. 
      "task_idle",      // name of task. 
      5000,        // Stack size of task 
      (void *)task1Param,  // parameter of the task 
      1,            // priority of the task 
      NULL);        // Task handle to keep track of created task 
}

