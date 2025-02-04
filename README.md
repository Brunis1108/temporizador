# Temporizador de Um Disparo (One-Shot) com Botão

## Descrição
Este projeto implementa um sistema de temporização acionado por um botão no microcontrolador RP2040 com a Pico SDK. Quando o botão é pressionado, os LEDs acendem em sequência e são desligados após um intervalo de tempo.

## Componentes Utilizados
- Microcontrolador Raspberry Pi Pico W
- 3 LEDs (Azul, Vermelho e Verde)
- 3 Resistores de 330Ω
- 1 Botão (Pushbutton)

## Esquema de Funcionamento
1. O usuário pressiona o botão.
2. Todos os LEDs acendem simultaneamente.
3. Após 3 segundos, o LED azul apaga.
4. Após mais 3 segundos, o LED vermelho apaga.
5. Após mais 3 segundos, o LED verde apaga.
6. O botão só pode ser pressionado novamente após o último LED apagar.

## Código
```c
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define LED_AZUL 12
#define LED_VERMELHO 13
#define LED_VERDE 11
#define BOTAO 5

bool leds_ativos = false;

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
    
    gpio_init(LED_AZUL);
    gpio_init(LED_VERMELHO);
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_AZUL, GPIO_OUT);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    
    gpio_init(BOTAO);
    gpio_set_dir(BOTAO, GPIO_IN);
    gpio_pull_up(BOTAO);
    gpio_set_irq_enabled_with_callback(BOTAO, GPIO_IRQ_EDGE_RISE, true, &botao_callback);
    
    while (true) {
        printf("Aguardando botão...\n");
        sleep_ms(1000);
    }
}
```

## Como Executar
1. Configure o ambiente de desenvolvimento com a Pico SDK.
2. Compile o código e faça o upload para a Raspberry Pi Pico.
3. Conecte os LEDs, resistores e o botão conforme especificado.
4. Pressione o botão e observe o funcionamento do sistema de temporização.

## 
Projeto desenvolvido para experimentação com temporizadores e interrupções no RP2040.
