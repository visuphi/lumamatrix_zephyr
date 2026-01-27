/*
 * Copyright (c) 2026 Institute of Embedded Systems ZHAW
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <string.h>

#define LOG_LEVEL 4
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main);

#include <zephyr/kernel.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/sys/util.h>

#define STRIP_NODE		DT_ALIAS(led_strip)

#if DT_NODE_HAS_PROP(DT_ALIAS(led_strip), chain_length)
#define STRIP_NUM_PIXELS	DT_PROP(DT_ALIAS(led_strip), chain_length)
#else
#error Unable to determine length of LED strip
#endif

#define DELAY_TIME K_MSEC(CONFIG_SAMPLE_LED_UPDATE_DELAY)

#define RGB(_r, _g, _b) { .r = (_r), .g = (_g), .b = (_b) }


static const struct gpio_dt_spec joystick_up = GPIO_DT_SPEC_GET_OR(DT_ALIAS(sw0), gpios,
							      {0});
static const struct gpio_dt_spec joystick_down = GPIO_DT_SPEC_GET_OR(DT_ALIAS(sw1), gpios,
							      {0});
static const struct gpio_dt_spec joystick_left = GPIO_DT_SPEC_GET_OR(DT_ALIAS(sw2), gpios,
							      {0});
static const struct gpio_dt_spec joystick_right = GPIO_DT_SPEC_GET_OR(DT_ALIAS(sw3), gpios,
							      {0});
static const struct gpio_dt_spec joystick_center = GPIO_DT_SPEC_GET_OR(DT_ALIAS(sw4), gpios,
							      {0});
static const struct gpio_dt_spec switcher = GPIO_DT_SPEC_GET_OR(DT_ALIAS(sw5), gpios,
							      {0});

static struct gpio_callback joystick_up_cb_data;
static struct gpio_callback joystick_down_cb_data;
static struct gpio_callback joystick_left_cb_data;
static struct gpio_callback joystick_right_cb_data;
static struct gpio_callback joystick_center_cb_data;

int cursor = 0;
static const struct led_rgb colors[] = {
	RGB(CONFIG_SAMPLE_LED_BRIGHTNESS, 0x00, 0x00), /* red */
	RGB(0x00, CONFIG_SAMPLE_LED_BRIGHTNESS, 0x00), /* green */
	RGB(0x00, 0x00, CONFIG_SAMPLE_LED_BRIGHTNESS), /* blue */
};

static struct led_rgb pixels[STRIP_NUM_PIXELS];

static const struct device *const strip = DEVICE_DT_GET(STRIP_NODE);
size_t color = 0;

void joystick_up_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	int rc;
	if (cursor !=64){
		cursor++;
	}
	else {
		cursor = 0;
	}
	memset(&pixels, 0x00, sizeof(pixels));
	memcpy(&pixels[cursor], &colors[color], sizeof(struct led_rgb));
	rc = led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);
	if (rc) {
		LOG_ERR("couldn't update strip: %d", rc);
	}
	color = (color + 1) % ARRAY_SIZE(colors);
}

void joystick_down_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	LOG_INF("joystick down callback");
}

void joystick_left_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	LOG_INF("joystick left callback");
}

void joystick_right_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	LOG_INF("joystick right callback");
}

void joystick_center_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	LOG_INF("joystick center callback");
}




static int init_hw(void) {
	int rc;
	if (device_is_ready(strip)) {
		LOG_INF("Found LED strip device %s", strip->name);
	} else {
		LOG_ERR("LED strip device %s is not ready", strip->name);
		return -1;
	}

	if (!gpio_is_ready_dt(&joystick_up)) {
		printk("Error: button device %s is not ready\n",
		       joystick_up.port->name);
		return -1;
	}

	rc = gpio_pin_configure_dt(&joystick_up, GPIO_INPUT);
	if (rc != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       rc, joystick_up.port->name, joystick_up.pin);
		return -1;
	}

	rc = gpio_pin_interrupt_configure_dt(&joystick_up,
					      GPIO_INT_EDGE_TO_ACTIVE);
	if (rc != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			rc, joystick_up.port->name, joystick_up.pin);
		return 0;
	}

	gpio_init_callback(&joystick_up_cb_data, joystick_up_pressed, BIT(joystick_up.pin));
	gpio_add_callback(joystick_up.port, &joystick_up_cb_data);

	if (!gpio_is_ready_dt(&joystick_down)) {
		printk("Error: button device %s is not ready\n",
		       joystick_down.port->name);
		return -1;
	}

	rc = gpio_pin_configure_dt(&joystick_down, GPIO_INPUT);
	if (rc != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       rc, joystick_down.port->name, joystick_down.pin);
		return -1;
	}

	gpio_init_callback(&joystick_down_cb_data, joystick_down_pressed, BIT(joystick_down.pin));
	gpio_add_callback(joystick_down.port, &joystick_down_cb_data);


	if (!gpio_is_ready_dt(&joystick_left)) {
		printk("Error: button device %s is not ready\n",
		       joystick_left.port->name);
		return -1;
	}

	rc = gpio_pin_configure_dt(&joystick_left, GPIO_INPUT);
	if (rc != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       rc, joystick_left.port->name, joystick_left.pin);
		return -1;
	}

	gpio_init_callback(&joystick_left_cb_data, joystick_left_pressed, BIT(joystick_left.pin));
	gpio_add_callback(joystick_left.port, &joystick_left_cb_data);

	if (!gpio_is_ready_dt(&joystick_right)) {
		printk("Error: button device %s is not ready\n",
		       joystick_right.port->name);
		return -1;
	}

	rc = gpio_pin_configure_dt(&joystick_right, GPIO_INPUT);
	if (rc != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       rc, joystick_right.port->name, joystick_right.pin);
		return -1;
	}

	gpio_init_callback(&joystick_right_cb_data, joystick_right_pressed, BIT(joystick_right.pin));
	gpio_add_callback(joystick_right.port, &joystick_right_cb_data);

	if (!gpio_is_ready_dt(&joystick_center)) {
		printk("Error: button device %s is not ready\n",
		       joystick_center.port->name);
		return -1;
	}

	rc = gpio_pin_configure_dt(&joystick_center, GPIO_INPUT);
	if (rc != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       rc, joystick_center.port->name, joystick_center.pin);
		return -1;
	}

	gpio_init_callback(&joystick_center_cb_data, joystick_center_pressed, BIT(joystick_center.pin));
	gpio_add_callback(joystick_center.port, &joystick_center_cb_data);
	return 0;
}

int main(void)
{

	int rc;

	rc = init_hw();
	if (!rc) {
		LOG_ERR("failed to init hw");
	}

	return 0;
}