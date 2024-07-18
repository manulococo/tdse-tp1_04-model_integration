/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @file   : task_actuator.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
/* Project includes. */
#include "main.h"

/* Demo includes. */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes. */
#include "board.h"
#include "app.h"
#include "task_actuator_attribute.h"
#include "task_actuator_interface.h"

/********************** macros and definitions *******************************/
#define G_TASK_ACT_CNT_INIT	0u

#define DELAY_INI	0u
#define DELAY		100u
#define DELAY_MED	25u
#define DELAY_MAX	50u

#define LED_XX_DEL_PUL	DELAY_MAX
#define LED_XX_DEL_BLI	DELAY_MED
#define LED_XX_DEL_MIN	DELAY_INI

/********************** internal data declaration ****************************/

const task_actuator_cfg_t task_actuator_cfg_list[] =

	{{ID_LED_D1, LED_D1_PORT, LED_D1_PIN,
	  LED_ON, LED_OFF,  LED_XX_DEL_BLI, LED_XX_DEL_PUL},

	 {ID_LED_D2, LED_D2_PORT, LED_D2_PIN,
	  LED_ON, LED_OFF,  LED_XX_DEL_BLI, LED_XX_DEL_PUL},

     {ID_LED_A,  LED_A_PORT,  LED_A_PIN,
	  LED_ON,  LED_OFF,   LED_XX_DEL_BLI, LED_XX_DEL_PUL}};

#define ACTUATOR_CFG_QTY	(sizeof(task_actuator_cfg_list)/sizeof(task_actuator_cfg_t))

task_actuator_dta_t task_actuator_dta_list[] =

	{{LED_XX_DEL_MIN, ST_LED_XX_OFF, EV_LED_XX_NOT_BLINK, false},

	 {LED_XX_DEL_MIN, ST_LED_XX_OFF, EV_LED_XX_NOT_BLINK, false},

     {LED_XX_DEL_MIN, ST_LED_XX_OFF, EV_LED_XX_NOT_BLINK, false}};

#define ACTUATOR_DTA_QTY	(sizeof(task_actuator_dta_list)/sizeof(task_actuator_dta_t))
/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
const char *p_task_actuator 		= "Task Actuator (Actuator Modeling)";
const char *p_task_actuator_ 		= "Non-Blocking & Update By Time Code";
/********************** external data declaration *****************************/
uint32_t g_task_actuator_cnt;
volatile uint32_t g_task_actuator_tick;
/********************** external functions definition ************************/
void task_actuator_init(void *parameters)
{
	uint32_t index;
	const task_actuator_cfg_t *p_task_actuator_cfg;
	task_actuator_dta_t *p_task_actuator_dta;
	task_actuator_st_t state;
	task_actuator_ev_t event;
	bool b_event;

	/* Print out: Task Initialized */
	LOGGER_LOG("  %s is running - %s\r\n", GET_NAME(task_actuator_init), p_task_actuator);
	LOGGER_LOG("  %s is a %s\r\n", GET_NAME(task_actuator), p_task_actuator_);

	g_task_actuator_cnt = G_TASK_ACT_CNT_INIT;

	/* Print out: Task execution counter */
	LOGGER_LOG("   %s = %d\r\n", GET_NAME(g_task_actuator_cnt), (int)g_task_actuator_cnt);

	for (index = 0; index < ACTUATOR_DTA_QTY; index++)
	{
		/* Update Task Actuator Configuration & Data Pointer */
		p_task_actuator_cfg = &task_actuator_cfg_list[index];
		p_task_actuator_dta = &task_actuator_dta_list[index];

		/* Print out: Index & Task execution FSM */
		LOGGER_LOG("   %s = %d", GET_NAME(index), (int)index);

		state = p_task_actuator_dta->state;
		LOGGER_LOG("   %s = %d", GET_NAME(state), (int)state);

		event = p_task_actuator_dta->event;
		LOGGER_LOG("   %s = %d", GET_NAME(event), (int)event);

		b_event = p_task_actuator_dta->flag;
		LOGGER_LOG("   %s = %d\r\n", GET_NAME(b_event), (int)b_event);

		HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_off);
	}

	g_task_actuator_tick = DELAY_INI;
}

void task_actuator_update(void *parameters)
{
	uint32_t index;
	const task_actuator_cfg_t *p_task_actuator_cfg;
	task_actuator_dta_t *p_task_actuator_dta;
	bool b_time_update_required = false;

	/* Update Task Actuator Counter */
	g_task_actuator_cnt++;

	/* Protect shared resource (g_task_actuator_tick) */
	__asm("CPSID i");	/* disable interrupts*/
	if (DELAY == g_task_actuator_tick)
	{
		b_time_update_required = true;
	}
	if (!g_task_actuator_tick)
	{
		g_task_actuator_tick = DELAY;
	}


    __asm("CPSIE i");	/* enable interrupts*/

    while (b_time_update_required)
    {
		/* Protect shared resource (g_task_actuator_tick) */
		__asm("CPSID i");	/* disable interrupts*/

		if (1 == g_task_actuator_tick)
		{
			b_time_update_required = false;
		}
		g_task_actuator_tick--;


		__asm("CPSIE i");	/* enable interrupts*/

    	for (index = 0; index < ACTUATOR_DTA_QTY; index++)
		{
    		/* Update Task Actuator Configuration & Data Pointer */
			p_task_actuator_cfg = &task_actuator_cfg_list[index];
			p_task_actuator_dta = &task_actuator_dta_list[index];

			switch (p_task_actuator_dta->state)
			{
				case ST_LED_XX_OFF:

					if ((true == p_task_actuator_dta->flag) && (EV_LED_XX_BLINK == p_task_actuator_dta->event))
					{
						p_task_actuator_dta->flag = false;
						p_task_actuator_dta->tick = p_task_actuator_cfg->tick_blink;
						HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_on);
						p_task_actuator_dta->state = ST_LED_XX_BLINK_ON;
					}

					if ((true == p_task_actuator_dta->flag) && (EV_LED_XX_PULSE == p_task_actuator_dta->event))
					{
						p_task_actuator_dta->flag = false;
						p_task_actuator_dta->tick = p_task_actuator_cfg->tick_pulse;
						HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_on);
						p_task_actuator_dta->state = ST_LED_XX_PULSE;
					}

					break;

				case ST_LED_XX_BLINK_ON:

					if ((true == p_task_actuator_dta->flag) && (EV_LED_XX_NOT_BLINK == p_task_actuator_dta->event))
					{
						p_task_actuator_dta->flag = false;
						HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_off);
						p_task_actuator_dta->state = ST_LED_XX_OFF;
					}

					p_task_actuator_dta->tick--;
					if (LED_XX_DEL_MIN == p_task_actuator_dta->tick)
					{
						p_task_actuator_dta->tick = p_task_actuator_cfg->tick_blink;
						HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_off);
						p_task_actuator_dta->state = ST_LED_XX_BLINK_OFF;
					}

					break;

				case ST_LED_XX_BLINK_OFF:

					p_task_actuator_dta->tick--;
					if (LED_XX_DEL_MIN == p_task_actuator_dta->tick)
					{
						p_task_actuator_dta->tick = p_task_actuator_cfg->tick_blink;
						HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_on);
						p_task_actuator_dta->state = ST_LED_XX_BLINK_ON;
					}
					break;

				case ST_LED_XX_PULSE:

					p_task_actuator_dta->tick--;
					if (LED_XX_DEL_MIN == p_task_actuator_dta->tick)
					{
						p_task_actuator_dta->tick = p_task_actuator_cfg->tick_pulse;
						HAL_GPIO_WritePin(p_task_actuator_cfg->gpio_port, p_task_actuator_cfg->pin, p_task_actuator_cfg->led_off);
						p_task_actuator_dta->state = ST_LED_XX_OFF;
					}

					break;

				default:

					break;
			}
		}
    }
}

/********************** end of file ******************************************/
