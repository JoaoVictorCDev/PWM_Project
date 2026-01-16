#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

#define LED_PIN 23
#define BUTTON_ON 5

#define LEDC_TIMER LEDC_TIMER_0        // escolher o timer
#define LEDC_MODE LEDC_HIGH_SPEED_MODE // Define como o PWM Funciona (NO CASO TA FALANDO QUE VAI SER 100% POR HARDWARE)
#define LEDC_CHANNEL LEDC_CHANNEL_0    // O canal liga o PWM a um pino GPIO
#define LEDC_FREQ_HZ 5000              // Frequência do PWM: 5000 Hz = 5000 pulsos por segundo
#define LEDC_RES LEDC_TIMER_8_BIT      // Resolução do PWM: 8 bits → valores de 0 a 255

void pwm_timer_init(void)
{
    ledc_timer_config_t timer = {
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER,
        .duty_resolution = LEDC_RES,
        .freq_hz = LEDC_FREQ_HZ,
        .clk_cfg = LEDC_AUTO_CLK};

    ledc_timer_config(&timer);
}

void pwm_channel_init(void)
{
    ledc_channel_config_t channel = {
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = LED_PIN,
        .duty = 0,
        .hpoint = 0};

    ledc_channel_config(&channel);
}

void set_brilho(uint8_t duty)
{
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty); // ledc_set_duty → prepara o novo valor
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);    // ledc_update_duty → aplica no hardware
}

void app_main(void)
{
    // Aqui você liga o PWM.
    pwm_timer_init();
    pwm_channel_init();
    // Aqui você liga o PWM.

    gpio_reset_pin(BUTTON_ON);
    gpio_set_direction(BUTTON_ON, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_ON, GPIO_PULLUP_ONLY);

    vTaskDelay(pdMS_TO_TICKS(1000));
    printf("SISTEMA ON\n");

    while (1)
    {
        int ver_but = gpio_get_level(BUTTON_ON);
        vTaskDelay(pdMS_TO_TICKS(10));

        if (ver_but == 0)
        {
            printf("acionando!");
            for (int i = 0; i <= 255; i++)
            {
                set_brilho(i);
                vTaskDelay(pdMS_TO_TICKS(10));
            }

            for (int i = 255; i >= 0; i--)
            {
                set_brilho(i);
                vTaskDelay(pdMS_TO_TICKS(10));
            }
        }
    }
}