#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define LED_AZUL 12
#define LED_VERMELHO 13
#define LED_VERDE 11
#define BOTAO 5

// Variáveis globais
bool leds_ativos = false;

// Callbacks para desligar os LEDs sequencialmente
int64_t desligar_led2_callback(alarm_id_t id, void *user_data) {
    gpio_put(LED_VERDE, 0);
    leds_ativos = false;
    return 0;
}

int64_t desligar_led1_callback(alarm_id_t id, void *user_data) {
    gpio_put(LED_VERMELHO, 0);
    add_alarm_in_ms(3000, desligar_led2_callback, NULL, false);
    return 0;
}

int64_t desligar_led0_callback(alarm_id_t id, void *user_data) {
    gpio_put(LED_AZUL, 0);
    add_alarm_in_ms(3000, desligar_led1_callback, NULL, false);
    return 0;
}

// Callback do botão para iniciar sequência
void botao_callback(uint gpio, uint32_t events) {
    if (!leds_ativos) {
        leds_ativos = true;
        gpio_put(LED_AZUL, 1);
        gpio_put(LED_VERDE, 1);
        gpio_put(LED_VERMELHO, 1);
        add_alarm_in_ms(3000, desligar_led0_callback, NULL, false);
    }
}

int main() {
    stdio_init_all();
    
    // Configuração dos LEDs
    gpio_init(LED_AZUL);
    gpio_init(LED_VERMELHO);
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_AZUL, GPIO_OUT);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    
    // Configuração do botão
    gpio_init(BOTAO);
    gpio_set_dir(BOTAO, GPIO_IN);
    gpio_pull_up(BOTAO);
    gpio_set_irq_enabled_with_callback(BOTAO, GPIO_IRQ_EDGE_RISE, true, &botao_callback);
    
    // Loop principal
    while (true) {
        printf("Aguardando botão...\n");
        sleep_ms(1000);
    }
}
