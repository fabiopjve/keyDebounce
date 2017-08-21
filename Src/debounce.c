/*
 * Key Debouncing and Auto-repeat library
 *
 * Author :		Fabio Pereira	www.sctec.com.br/blog
 * Date :		04/06/2017
 *
 */

#include "stm32l4xx_hal.h"
#include "debounce.h"


/*
 * Function		:	InitKeyAutoRepeat
 * Description	:	initializes the given key property structure
 * Input		:	AutoRepeatKey *keyProp - a pointer to structure with all key properties
 * 					GPIO_TypeDef* GPIOx - the I/O port where the key is connected
 * 					uint16_t GPIO_Pin - GPIO pin where the key is connected
 * 					GPIO_PinState targetState - target state when key is pressed
 * 					uint8_t debounceTimeMs - debounce time in ms
 * 					uint16_t autoRepeatMult - time multiplier for starting auto repeating
 * 					(total time to start repeating = debounce time * autoRepeatMult)
 * Output		:	uint8_t - 0 if key is not pressed, 1 if key is pressed or repeating
 */
void initKeyAutoRepeat(	AutoRepeatKey *keyProp, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,
						GPIO_PinState targetState, uint8_t debounceTimeMs, uint16_t autoRepeatMult)
{
	keyProp->GPIOx = GPIOx;
	keyProp->GPIO_Pin = GPIO_Pin;
	keyProp->targetState = targetState;
	keyProp->debounceTimeMs = debounceTimeMs;
	keyProp->autoRepeatMult = autoRepeatMult;
	keyProp->autoRepeatTarget = 0;
	keyProp->endTime = 0;
	keyProp->keyPressed = 0;
	keyProp->tick = 0;
}

/*
 * Function		:	readKeyAutoRepeat
 * Description	:	performs the debounce of a given pin and includes auto repeat
 * 					This function is non-blocking!!!
 * 					******* Call initKeyAutoRepeat() before using it!!! **************
 * Input		:	AutoRepeatKey *keyProp - a structure with all key properties
 * Output		:	uint8_t - 0 if key is not pressed, 1 if key is pressed or repeating
 */
uint8_t readKeyAutoRepeat(AutoRepeatKey *keyProp)
{
	uint8_t result = 0;
	if (!keyProp->endTime) {
		keyProp->tick = HAL_GetTick();
		keyProp->endTime = keyProp->tick + keyProp->debounceTimeMs;
	}
	if (HAL_GPIO_ReadPin(keyProp->GPIOx,keyProp->GPIO_Pin) == keyProp->targetState){
		if (keyProp->tick < keyProp->endTime){
			keyProp->tick = HAL_GetTick();
			if (HAL_GetTick() < keyProp->tick) {
				keyProp->endTime = keyProp->tick + keyProp->debounceTimeMs;
			}
		} else {
			keyProp->endTime = keyProp->tick + keyProp->debounceTimeMs;
			if (!keyProp->keyPressed) {
				result = 1;
				keyProp->keyPressed = 1;
			} else {
				if (keyProp->autoRepeatMult) {
					keyProp->autoRepeatTarget++;
					if (keyProp->autoRepeatTarget >= keyProp->autoRepeatMult) {
						if ((keyProp->autoRepeatMult - keyProp->autoRepeatStart)>1) {
							keyProp->autoRepeatStart += (keyProp->autoRepeatMult - keyProp->autoRepeatStart) / 8;
						}
						keyProp->autoRepeatTarget = keyProp->autoRepeatStart;
						result = 1;
					}
				}
			}
		}
	} else {
		keyProp->tick = HAL_GetTick();
		keyProp->endTime = keyProp->tick + keyProp->debounceTimeMs;
		keyProp->keyPressed = 0;
		keyProp->autoRepeatStart = 0;
		keyProp->autoRepeatTarget = 0;
	}
	return result;
}
