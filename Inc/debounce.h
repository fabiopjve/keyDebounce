/*
 * Key Debouncing and Auto-repeat library
 *
 * Author :		Fabio Pereira	www.sctec.com.br/blog
 * Date :		04/06/2017
 *
 */

#ifndef DEBOUNCE_H_
#define DEBOUNCE_H_

typedef struct {
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
	GPIO_PinState targetState;
	uint8_t debounceTimeMs;
	uint16_t autoRepeatMult;
	uint32_t tick;
	uint32_t endTime;
	uint16_t autoRepeatTarget;
	uint16_t autoRepeatStart;
	uint8_t keyPressed;
} AutoRepeatKey;

void initKeyAutoRepeat(AutoRepeatKey *keyProp, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState targetState, uint8_t debounceTimeMs, uint16_t autoRepeatMult);
uint8_t readKeyAutoRepeat(AutoRepeatKey *keyProp);

#endif /* DEBOUNCE_H_ */
