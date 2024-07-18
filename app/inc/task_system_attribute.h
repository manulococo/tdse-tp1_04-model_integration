#ifndef TASK_INC_TASK_SYSTEM_ATTRIBUTE_H_
#define TASK_INC_TASK_SYSTEM_ATTRIBUTE_H_
/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif
/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/
/* Finite State Machine Task System Table */
/* 	------------------------+-----------------------+-----------------------------------+--------------------------------------------
 * 	| Current               |                       | Event                 			| Next                  |           		|
 * 	| State                 | [Guard]               | (Parameters)         				| State                 | Actions   		|
 * 	|=======================+=======================+===============================================================================|
 * 	|						|						| EV_SYSTEM_AUTO_POSICION_ON		| ST_SYSTEM_STATE_1		|					|
 * 	|S_TASK_SYSTEM_STATE_0	|						|-----------------------------------+-----------------------+-------------------|
 * 	|						|						| EV_SYSTEM_AUTO_POSICION_OFF		| ST_SYSTEM_STATE_0		|					|
 * 	|-----------------------+-----------------------+-----------------------------------+-----------------------+-------------------|
 * 	|S_TASK_SYSTEM_STATE_1  |						| EV_SYSTEM_TICKET_IMPRESO_ON		| ST_SYSTEM_STATE_2		|imprimir_ticket	|
 * 	|-----------------------+-----------------------+-----------------------------------+-----------------------+-------------------|
 * 	|  						|						| EV_SYSTEM_TICKET_RETIRADO_ON		| ST_SYSTEM_STATE_3		|subir_barrera		|
 * 	|S_TASK_SYSTEM_STATE_2	|						|-----------------------------------+-----------------------+-------------------|
 * 	|						|						| EV_SYSTEM_TICKET_RETIRADO_OFF		| ST_SYSTEM_STATE_0		|recuperar_ticket	|
 * 	|-----------------------+-----------------------+-----------------------------------+-----------------------+-------------------|
 * 	|S_TASK_SYSTEM_STATE_3  |						| EV_SYSTEM_BARRERA_VERTICAL_ON		| ST_SYSTEM_STATE_4		|stop_barrera		|
 * 	|-----------------------+-----------------------+-----------------------------------+-----------------------+-------------------|
 * 	|S_TASK_SYSTEM_STATE_4  |						| EV_SYSTEM_AUTO_ENTRANTE_ON		| ST_SYSTEM_STATE_5		|					|
 * 	|-----------------------+-----------------------+-----------------------------------+-----------------------+-------------------|
 * 	|S_TASK_SYSTEM_STATE_5  |						| EV_SYSTEM_AUTO_ENTRANTE_OFF		| ST_SYSTEM_STATE_6		|bajar_barrera		|
 * 	|-----------------------+-----------------------+-----------------------------------+-----------------------+-------------------|
 * 	|S_TASK_SYSTEM_STATE_6  |						| EV_SYSTEM_BARRERA_HORIZONTAL_ON	| ST_SYSTEM_STATE_0 	|stop_barrera		|
 * 	|-----------------------+-----------------------+-----------------------------------+-----------------------+-------------------|
 */
typedef enum task_system_st {
	ST_SYSTEM_STATE_0,
	ST_SYSTEM_STATE_1,
	ST_SYSTEM_STATE_2,
	ST_SYSTEM_STATE_3,
	ST_SYSTEM_STATE_4,
	ST_SYSTEM_STATE_5,
	ST_SYSTEM_STATE_6
} task_system_st_t;


typedef enum task_system_ev {
	EV_SYSTEM_AUTO_POSICION_ON,
	EV_SYSTEM_AUTO_POSICION_OFF,
	EV_SYSTEM_AUTO_ENTRANTE_ON,
	EV_SYSTEM_AUTO_ENTRANTE_OFF,
	EV_SYSTEM_TICKET_IMPRESO_ON,
	EV_SYSTEM_TICKET_IMPRESO_OFF,
	EV_SYSTEM_TICKET_RETIRADO_ON,
	EV_SYSTEM_TICKET_RETIRADO_OFF,
	EV_SYSTEM_BARRERA_VERTICAL_ON,
	EV_SYSTEM_BARRERA_VERTICAL_OFF,
	EV_SYSTEM_BARRERA_HORIZONTAL_ON,
	EV_SYSTEM_BARRERA_HORIZONTAL_OFF
} task_system_ev_t;

/* Signals of Task Sensor */
typedef enum task_system_sig {SIG_SYS_XX_ACTIVE, SIG_SYS_XX_IDLE} task_system_sig_t;

typedef struct
{
	uint32_t			tick;
	task_system_st_t	state;
	task_system_ev_t	event;
	bool				flag;
} task_system_dta_t;

/********************** external data declaration ****************************/
extern task_system_dta_t task_system_dta;
/********************** external functions definition ************************/
/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_SYSTEM_ATTRIBUTE_H_ */

/********************** end of file ******************************************/
