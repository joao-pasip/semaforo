#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

// Definição dos pinos dos LEDs
#define LED_RED 11
#define LED_YELLOW 12
#define LED_GREEN 13

// Variável para controlar o estado do semáforo
typedef enum
{
    RED,
    YELLOW,
    GREEN
} TrafficLightState;

TrafficLightState current_state = RED;

// Função de callback do temporizador
bool repeating_timer_callback(struct repeating_timer *t)
{
    // Desliga todos os LEDs
    gpio_put(LED_RED, 0);
    gpio_put(LED_YELLOW, 0);
    gpio_put(LED_GREEN, 0);

    // Altera o estado do semáforo
    switch (current_state)
    {
    case RED:
        gpio_put(LED_YELLOW, 1);
        current_state = YELLOW;
        break;
    case YELLOW:
        gpio_put(LED_GREEN, 1);
        current_state = GREEN;
        break;
    case GREEN:
        gpio_put(LED_RED, 1);
        current_state = RED;
        break;
    }

    return true; // Continua o temporizador
}

int main()
{
    // Inicializa a porta serial
    stdio_init_all();

    // Configura os pinos dos LEDs como saída
    gpio_init(LED_RED);
    gpio_init(LED_YELLOW);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_YELLOW, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);

    // Inicia o semáforo com o LED vermelho aceso
    gpio_put(LED_RED, 1);

    // Configura o temporizador para chamar a função de callback a cada 3 segundos
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);

    // Loop principal
    while (true)
    {
        // Envia uma mensagem pela porta serial a cada segundo
        printf("Estado atual do semáforo: %d\n", current_state);
        sleep_ms(1000);
    }

    return 0;
}