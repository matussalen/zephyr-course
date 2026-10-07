#ifndef LED_SENSOR_H_
#define LED_SENSOR_H_

#include <stdbool.h>
#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

int led_sensor_set_state(const struct device *dev, bool on);

#ifdef __cplusplus
}
#endif

#endif