#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>

//#define SLEEP_TIME_MS 1000

/* The devicetree node identifier for the "led0" alias. */
//#define LED_NODE DT_ALIAS(app_led)
//static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static const struct device *const led_sensor =
    DEVICE_DT_GET(DT_NODELABEL(led_sensor));

int main(void)
{

    if (!device_is_ready(led_sensor)) {
        LOG_ERR("LED sensor initialization failed");
        return 0;
    }

    LOG_INF("LED sensor ready");

    struct sensor_value state;

    while (true) {
        int ret = sensor_sample_fetch(led_sensor);

        if (ret < 0) {
            LOG_ERR("Failed to turn LED on: %d", ret);
            return 0;
        }

        LOG_INF("LED ON");
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);

        ret = sensor_channel_get(
            led_sensor, SENSOR_CHAN_PRIV_START, &state);

        if (ret < 0) {
            LOG_ERR("Failed to turn LED off: %d", ret);
            return 0;
        }

        LOG_INF("LED OFF, state: %d", state.val1);
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }

    return 0;


    // bool led_state = true;

    // if (!gpio_is_ready_dt(&led)) return 0;

    // if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    // LOG_INF("Hello World!");

    // while (1) {
    //     if (gpio_pin_toggle_dt(&led) < 0) return 0;

    //     led_state = !led_state;
    //     //LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
    //     k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    // }
    // return 0;
}
