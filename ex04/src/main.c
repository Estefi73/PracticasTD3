/*
4_ Cree un programa usando FreeRTOS que cumpla con las siguientes consignas:
 - Posea dos tareas, Tarea1 y Tarea2 de distinta prioridad y que sean implementadas en las misma funcion.
 - Ambas tareas deben indicar por puerto serie que estan en ejecucion y su prioridad, luego bloquearse por 500ms.
 - En el tiempo Idle aprovechar para cambiar el estado del led Azul cada 300ms e indicarlo por puerto serie (Idle Hook).
 - Justifique que sucede con los cambios de estado del led en caso que la tarea Tarea1 pase a ejecutarse de manera continua.
*/

#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"
#include "board.h"

const char *pcTextoTarea1 = "Tarea1 corriendo\r\n";
const char *pcTextoTarea2 = "Tarea2 corriendo\r\n";

static void vTarea(void *pcParameters){

	UBaseType_t uxPriority;
	char *pcTaskName;
	const TickType_t xDelay500ms = pdMS_TO_TICKS(500UL); // Delay de 500ms.

	pcTaskName = (char *)pcParameters;

	uxPriority = xTaskPriorityGet(NULL); //Retorna el id de la tarea.

	for(;;){
		printf(pcTaskName);

		if( pcTaskName == "Tarea1 corriendo'r\n" ){
			Board_LED_Toggle(3); //Parpadea Led 1 amarillo.
		}else{
			Board_LED_Toggle(5); //Parpadea Led 3 verde.
		}

		printf("Prioridad: ",uxPriority);
		vTaskDelay(xDelay500ms);
	}
}

void vApplicationIdleHook(void){

	const char *pcIdleTask = "Tarea Idle corriendo\r\n";
	const TickType_t xDelay300ms = pdMS_TO_TICKS(300UL);

	printf(pcIdleTask);

	Board_LED_Set(2,TRUE); //Led Azul ON.
	vTaskDelay(xDelay300ms); //Delay 300ms.
	Board_LED_Set(2,FALSE); //Led Azul OFF.
}

int main(void){
	//Crear tarea.
	xTaskCreate(vTarea,"Tarea1",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY+2,NULL);
	xTaskCreate(vTarea,"Tarea2",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY+1,NULL):

	//Arrancar planificador.
	xTaskStartScheduler();

	for(;;);
	return(0);
}
