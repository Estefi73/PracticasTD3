/*
2_ Cree un programa usando FreeRTOS que cumpla con las siguientes confinas:
 - Posea dos tareas (Tarea1 y Tarea2) y una rutina de atencion de intrrupcion de timer RIT (Repetitive Interrupt Timer)
 - La Tarea1 debe producir elementos.
 - La Tarea2 debe esperar de la interrupcion del timer RIT (que sera de 3s)
 - Una vez que expire el timer, consumir un elemento, informar por puerto serie el elemento consumido y cambiar el estado del led
amarillo.
*/

#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "board.h"
#include "queue.h"

#define TAM_COLA 20
#define TAM_MSG 8

SemaphoreHandle_t sem_serie;
QueueHandle_t cola;

static void InitTimer(void){

	Chip_RIT_Init(LPC_RITIMER);
	Chip_RIT_SetTimerInterval(LCP_RITIMER,3000);//3s.
}

void Rit_IRQHandler(void){

	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	xSemaphoreGiveFromISR(sem_serie,&xHigherPriorityTaskWoken); // xHigherPriorityTaskWoken toma el valor pdTRUE o pdFALSE si
								   //  todo  o no el semaforo respectivamente.
	if(xHigherPriorityTaskWoken == pdTRUE){
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
	Chip_RIT_ClearInt(LPC_RITIMER);
}

static void vTarea1(void *pcParameters){

	const char *pcTaskName = "Tarea1 corriendo\r\n";
	const char *pcEnviado = "Mensaje enviado\r\n";
	char cad[4] = {'T','D','3','\0'};
	volatile uint32_t i;
	const TickType_t xDelay = pd_MS_TO_TICKS(50UL);

	while(1){
		vPrintString(pcTaskName);
		for(i = 0;i < 4;i ++){
			if(xQueueSend(cola,(void *)cad[i],xDelay) == pdPASS){
				vPrintString(pcEnviado);
			}else{
				Board_LED_Toggle(0); //RGB.
			}
		}
	}
}

static void vTarea2(void *pcParameters){

	char cad;
	const char *pcTaskName = "Tarea2 corriendo\r\n";
	const TickType_t xDelay = pdMS_TO_TICKS(50UL);

	while(1){
		vPrintString(pcTaskName);
		if(xSemaphoreTake(sem_serie,portMAX_DELAY) == pdTRUE){
			if(xQueueReceive(cola,(void *)cad,xDelay) == pdTRUE){
				vPrintString(cad);
				Board_LED_Toggle(3); //Led Amarillo.
			}else{
				Board_LED_Toggle(0); //RGB.
			}
		}
	}
}

int main(void){

	InitTimer(); //RIT.

	sem_serie = xSemaphoreCreateBinary(); //Inicio semaforo en 0.

	cola = xQueueCreate(TAM_COLA,TAM_MSG); //Creo cola de mensajes.

	//Creo tareas.
	xTaskCreate(vTarea1,(const char *)"Tarea1",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 1,NULL);
	xTaskCreate(vTarea2,(const char *)"Tarea2",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 1,NULL);

	NVIC_EnableIRQ(RITIMER_IRQn); //RIT.

	//Arranco planificador.
	xTaskStartScheduler();

	for(;;);
	return(0);
}


