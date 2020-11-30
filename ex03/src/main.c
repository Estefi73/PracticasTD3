/*
3_Cree un programa usando FreeRTOS que cumpla con las siguientes consignas:
 - Posea dos tareas, Tarea1 y Tarea2.
 - La Tarea1 de mayor prioridad, de mayor prioridad, debe indicar por el puerto serie que esta ejecuntando un bucle busy-waiting.
 - Periodicamente debe aumentar la prioridad de la Tarea2, para que esta pueda ejecutarse.
 - La Tarea2 debe cambiar el estado del ledverde, indicarlo por puerto serie y disminuir su prioridad para detener su ejecucion.
*/

#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"
#include "board.h"

//#define PRIO_TAREA1 3
//#define PRIO_TAREA2 1
#define TAM 150

TaskHandle_t xTask2Handle; //Guarda el id de la Tarea2.

static vTarea1(void *pcParameters){

	UBaseType_t uxPriority;
	uxPriority = uxTaskPriorityGet(NULL); //Retorna la propia prioridad.

	for(;;){
		printf("Tarea1 esta corriendo\r\n");

		printf("Aumentado la prioridad de la Tarea2...\r\n");
		vTaskPrioritySet(&xTask2Handle,(uxPriority + 1)); //Tarea2 con prioridad mas alta.
		Board_LED_Set(5,FALSE); //Led Verde OFF.
	}
}

static vTarea2(void *pcParameters){

	UBaseType_t uxPriority;
	uxPriority = uxTaskPriorityGet(NULL);

	for(;;){
		printf("Tarea2 esta corriendo\r\n");

		printf("Disminuir la prioridad de la Tarea2...\r\n");
		vTaskPrioritySet(NULL,(uxPriority - 2)); //Tarea2 es la de menor prioridad.
		Board_LED_Set(5,TRUE); //Led Verde ON.
	}
}

int main(void){
	//Crear tareas.
	xTaskCreate(vTarea1,"Tarea1",TAM,NULL,tskIDLE_PRIORITY+2,NULL);
	xTaskCreate(vTarea2,"Tarea2",TAM,NULL,tskIDLE_PRIORITY+1,&xTask2Handle);

	//Arrancar planificador.
	xTaskStartScheduler();

	for(;;);
	return(0);
}
