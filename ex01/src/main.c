/*
1_ Cree un programa usando FreeRTos que cumpla con las siguientes consignas:
 - Posea dos tareas, Tarea1 y Tarea2, con los niveles de prioridad correspondientes.
 - La Tarea1 debe ejecutarse de manera continua, indicando este estado a traves el puerto serie.
 - La Tarea2 debe ser periodica, con periodo de 1 seg y debe cambiar el estado del led verde e indicarlo a traes del puerto serie.
Adicionalmente debe indicar los ticks del reloj de FreeRTos para asegurarse el periodo solicitado.
*/

#include "board.h"
#include "chip.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#define PRIO_TAREA1 1
#define PRIO_TAREA2 2
#define TAM 150


const char *pcTextoTarea1 = "Tarea1 Continua\r\n";
const char *pcTextoTarea2 = "Tarea2 Periodica\r\n";

static void vTarea1(void *pcParameters){
	portTickType xLastWakeTime;
	portTickType xPeriodo;

	char *pcTaskName;

	pcTaskName = (char *)pcParameters;

	xPeriodo = 10/ConfigTICK_RATE_MS; //Periodo de 10ms.

	xLastWakeTime = xTaskGetTickCount(); //Inicializo el xLastWakeTime.

	for(;;){
		vTaskDelayUntil(xLastWakeTime,xPeriodo);
		printf(pcTaskName);
	}

}

static void vTarea2(void *pcParameters){
	const TickType_t xDelay1s = pdMS_to_TICKS(1000UL);
	char *pcTaskName;

	pcTaskName = (char *)pcParameters;

	while(1){
		printf(pcTaskName);
		Board_LED_Toggle(5); //Parpadeo del led VERDE.
		printf("TICKS: ",portTick_RATE_MS); //Cantidad de Ticks.
	}
}

int main(void){
	//Crear tareas.
	xTaskCreate(vTarea1,(const char *)"Tarea1",TAM,(void *)pcTextoTarea1,PRIO_TAREA1,NULL);
	xTaskCreate(vTarea2,(const char *)"Tarea2",TAM,(void *)pcTextoTarea2,PRIO_TAREA2,NULL);

	//Arrancanr planificador.
	vTaskStartScheduler();

	for(;;);
	return(0);
}
