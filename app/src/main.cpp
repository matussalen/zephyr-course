#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include <led_sensor/led_sensor.h>
#include <zephyr/shell/shell.h>
#include <zephyr/sys/util.h>
#include <errno.h>


//#define SLEEP_TIME_MS 1000

/* The devicetree node identifier for the "led0" alias. */
//#define LED_NODE DT_ALIAS(app_led)
//static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static const struct device *const led_sensor =
    DEVICE_DT_GET(DT_NODELABEL(led_sensor));

static int cmd_sensor_info(
    const struct shell *sh,
    size_t argc,
    char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    shell_print(sh, "Device: %s", led_sensor->name);
    shell_print(sh, "Ready: %s",
                device_is_ready(led_sensor) ? "yes" : "no");

    return 0;
}

static int cmd_sensor_fetch(
    const struct shell *sh,
    size_t argc,
    char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    if (!device_is_ready(led_sensor)) {
        shell_error(sh, "LED sensor is not ready");
        return -ENODEV;
    }

    int ret = sensor_sample_fetch(led_sensor);

    if (ret < 0) {
        shell_error(sh, "Fetch failed: %d", ret);
        return ret;
    }

    shell_print(sh, "Fetch completed: LED ON");

    return 0;
}

static int cmd_sensor_read(
    const struct shell *sh,
    size_t argc,
    char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    if (!device_is_ready(led_sensor)) {
        shell_error(sh, "LED sensor is not ready");
        return -ENODEV;
    }

    struct sensor_value state;

    int ret = sensor_channel_get(
        led_sensor, SENSOR_CHAN_PRIV_START, &state);

    if (ret < 0) {
        shell_error(sh, "Read failed: %d", ret);
        return ret;
    }

    shell_print(sh, "Read completed: LED OFF, state: %d",
                state.val1);

    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
    sub_sensor,
    SHELL_CMD(info, NULL,
              "Show device name and ready state",
              cmd_sensor_info),
    SHELL_CMD(fetch, NULL,
              "Fetch a sample and turn LED on",
              cmd_sensor_fetch),
    SHELL_CMD(read, NULL,
              "Read the result and turn LED off",
              cmd_sensor_read),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(
    sensor,
    &sub_sensor,
    "LED sensor commands",
    NULL
);



int main(void)
{

    if (!device_is_ready(led_sensor)) {
        LOG_ERR("LED sensor initialization failed");
        return 0;
    }

    LOG_INF("LED sensor ready");
  
    int ret = led_sensor_set_state(led_sensor, true);

    if (ret < 0) {
    LOG_ERR("Custom API failed: %d", ret);
    return 0;
}

    LOG_INF("Custom API: LED ON");
    k_msleep(2000);

    ret = led_sensor_set_state(led_sensor, false);

    if (ret < 0) {
        LOG_ERR("Custom API failed: %d", ret);
        return 0;
    }

    LOG_INF("Custom API: LED OFF");
    k_msleep(2000);

    // struct sensor_value state;
    // while (true) {
    //     ret = sensor_sample_fetch(led_sensor);

    //     if (ret < 0) {
    //         LOG_ERR("Failed to turn LED on: %d", ret);
    //         return 0;
    //     }

    //     LOG_INF("LED ON");
    //     k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);

    //     ret = sensor_channel_get(
    //         led_sensor, SENSOR_CHAN_PRIV_START, &state);

    //     if (ret < 0) {
    //         LOG_ERR("Failed to turn LED off: %d", ret);
    //         return 0;
    //     }

    //     LOG_INF("LED OFF, state: %d", state.val1);
    //     k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    // }

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
