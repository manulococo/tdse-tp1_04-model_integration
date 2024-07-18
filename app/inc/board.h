/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar>.
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
 * @file   : board.h
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/*
 * Se alimenta con 3.3V
 * El botón vale 0 al presionar
 * El led se prende con un 0 lógico
 *
 *--------------|-------|-------|-------|--------|
 * CON Arduino 	| MCU	|  CON	|  NOM	|  Tipo	 |
 *--------------|-------|-------|-------|--------|
 * 		D2		|  PA10	|  S1	|  S1	| Sensor |
 *--------------|-------|-------|-------|--------|
 * 		D4		|  PB5	|  S2	|  S2	| Sensor |
 *--------------|-------|-------|-------|--------|
 * 		D5		|  PB4	|  S3	|  S3	| Sensor |
 *--------------|-------|-------|-------|--------|
 * 		D6		|  PB10	|  S4	|  S4	| Sensor |
 *--------------|-------|-------|-------|--------|
 * 		D7		|  PA8	|  R1	|  K1	| Sensor |
 *--------------|-------|-------|-------|--------|
 * 		D8		|  PA9	|  R2	|  K2	| Sensor |
 *--------------|-------|-------|-------|--------|
 * 		D15		|  PB8	|LED_D1	|LED_D1	|Actuador|
 *--------------|-------|-------|-------|--------|
 * 		D14		|  PB9	|LED_D2	|LED_D2 |Actuador|
 *--------------|-------|-------|-------|--------|
 * 		D12		|  PA6	|LED_D3	|LED_D3	|Actuador|
 *--------------|-------|-------|-------|--------|
 *
 */


#ifndef BOARD_INC_BOARD_H_
#define BOARD_INC_BOARD_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/
#define NUCLEO_F103RC		(0)
#define NUCLEO_F401RE		(1)
#define NUCLEO_F446RE		(2)
#define NUCLEO_F429ZI		(3)
#define NUCLEO_F413ZH		(4)
#define STM32F429I_DISCO1	(5)

#define BOARD (NUCLEO_F103RC)

/* STM32 Nucleo Boards - 64 Pins */
#if ((BOARD == NUCLEO_F103RC) || (BOARD == NUCLEO_F401RE) || (BOARD == NUCLEO_F446RE))

#define BUTTON_A_PIN	B1_Pin
#define BUTTON_A_PORT	B1_GPIO_Port
#define BUTTON_B_PIN	B1_Pin
#define BUTTON_B_PORT	B1_GPIO_Port
#define BUTTON_C_PIN	B1_Pin
#define BUTTON_C_PORT	B1_GPIO_Port

#define BUTTON_PRESSED	GPIO_PIN_RESET
#define BUTTON_HOVER	GPIO_PIN_SET

#define LED_A_PIN		LD2_Pin
#define LED_A_PORT		LD2_GPIO_Port
#define LED_B_PIN		LD2_Pin
#define LED_B_PORT		LD2_GPIO_Port
#define LED_C_PIN		LD2_Pin
#define LED_C_PORT		LD2_GPIO_Port

#define LED_ON			GPIO_PIN_SET
#define LED_OFF			GPIO_PIN_RESET

#define BUTTON_S1_PIN	D2_Pin
#define BUTTON_S1_PORT	D2_GPIO_Port
#define BUTTON_S2_PIN	D4_Pin
#define BUTTON_S2_PORT	D4_GPIO_Port
#define BUTTON_S3_PIN	D5_Pin
#define BUTTON_S3_PORT	D5_GPIO_Port
#define BUTTON_S4_PIN	D6_Pin
#define BUTTON_S4_PORT	D6_GPIO_Port
#define BUTTON_K1_PIN	D7_Pin
#define BUTTON_K1_PORT	D7_GPIO_Port
#define BUTTON_K2_PIN	D8_Pin
#define BUTTON_K2_PORT	D8_GPIO_Port

#define LED_D1_PIN		D15_Pin
#define LED_D1_PORT		D15_GPIO_Port
#define LED_D2_PIN		D14_Pin
#define LED_D2_PORT		D14_GPIO_Port
#define LED_D3_PIN		D12_Pin
#define LED_D3_PORT		D12_GPIO_Port

#endif/* STM32 Nucleo Boards - 144 Pins */

#if ((BOARD == NUCLEO_F429ZI) || (BOARD == NUCLEO_F413ZH))

#define BUTTON_A_PIN	USER_Btn_Pin
#define BUTTON_A_PORT	USER_Btn_GPIO_Port
#define BUTTON_B_PIN	USER_Btn_Pin
#define BUTTON_B_PORT	USER_Btn_GPIO_Port
#define BUTTON_C_PIN	USER_Btn_Pin
#define BUTTON_C_PORT	USER_Btn_GPIO_Port

#define BUTTON_PRESSED	GPIO_PIN_SET
#define BUTTON_HOVER	GPIO_PIN_RESET

#define LED_A_PIN		LD1_Pin
#define LED_A_PORT		LD1_GPIO_Port
#define LED_B_PIN		LD2_Pin
#define LED_B_PORT		LD2_GPIO_Port
#define LED_C_PIN		LD3_Pin
#define LED_C_PORT		LD3_GPIO_Port

#define LED_ON			GPIO_PIN_SET
#define LED_OFF			GPIO_PIN_RESET

#endif

/* STM32 Discovery Kits */
#if (BOARD == STM32F429I_DISCO1)

#define BUTTON_A_PIN	B1_Pin
#define BUTTON_A_PORT	B1_GPIO_Port
#define BUTTON_B_PIN	B2_Pin
#define BUTTON_B_PORT	B2_GPIO_Port
#define BUTTON_C_PIN	B3_Pin
#define BUTTON_C_PORT	B3_GPIO_Port

#define BUTTON_PRESSED	GPIO_PIN_SET
#define BUTTON_HOVER	GPIO_PIN_RESET

#define LED_A_PIN		LD3_Pin
#define LED_A_PORT		LD3_GPIO_Port
#define LED_B_PIN		LD4_Pin
#define LED_B_PORT		LD4_GPIO_Port
#define LED_C_PIN		LD4_Pin
#define LED_C_PORT		LD4_GPIO_Port

#define LED_ON			GPIO_PIN_SET
#define LED_OFF			GPIO_PIN_RESET

#endif

/********************** typedef **********************************************/

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* BOARD_INC_BOARD_H_ */

/********************** end of file ******************************************/
