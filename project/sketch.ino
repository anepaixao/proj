#include<arduino.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define BOTAO1 2
#define BOTAO2 3
#define BOTAO3 4
#define LED1 5
#define LED2 6
#define LED3 7
#define BUZZER 8

const char *frases[] = {"OLA", "AGUA", "AJUDA"};
int fraseIndex = 0;

Adafruit_SSD1306 display(128, 64, &Wire, -1); // Configuração do display

void atualizar_display() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 20);
    display.print(frases[fraseIndex]);
    display.display();
}

void emitir_som() {
    gpio_put(BUZZER, 1);
    sleep_ms(200);
    gpio_put(BUZZER, 0);
}

void selecionar_frase() {
    gpio_put(LED1, fraseIndex == 0);
    gpio_put(LED2, fraseIndex == 1);
    gpio_put(LED3, fraseIndex == 2);
    atualizar_display();
}

int main() {
    Serial.begin(115200);  // Inicializa a comunicação serial
    
    // Inicializa os GPIOs
    gpio_init(BOTAO1);
    gpio_init(BOTAO2);
    gpio_init(BOTAO3);
    gpio_init(LED1);
    gpio_init(LED2);
    gpio_init(LED3);
    gpio_init(BUZZER);
    
    gpio_set_dir(BOTAO1, GPIO_IN);
    gpio_set_dir(BOTAO2, GPIO_IN);
    gpio_set_dir(BOTAO3, GPIO_IN);
    gpio_set_dir(LED1, GPIO_OUT);
    gpio_set_dir(LED2, GPIO_OUT);
    gpio_set_dir(LED3, GPIO_OUT);
    gpio_set_dir(BUZZER, GPIO_OUT);
    
    gpio_pull_up(BOTAO1);
    gpio_pull_up(BOTAO2);
    gpio_pull_up(BOTAO3);
    
    // Inicializa o display
    i2c_init(i2c0, 400 * 1000);  // Configura a velocidade do I2C
    gpio_set_function(4, GPIO_FUNC_I2C);  // Pino SDA (GPIO 4)
    gpio_set_function(5, GPIO_FUNC_I2C);  // Pino SCL (GPIO 5)
    gpio_pull_up(4);  // Ativa o pull-up no pino SDA
    gpio_pull_up(5);  // Ativa o pull-up no pino SCL

    // Inicializa o display (endereço 0x3C)
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.printf("Falha ao inicializar o display!\n");
        return 1;
    }

    selecionar_frase();

    while (1) {
        if (!gpio_get(BOTAO1)) {
            fraseIndex = 0;
            selecionar_frase();
            sleep_ms(300);
        }
        if (!gpio_get(BOTAO2)) {
            fraseIndex = 1;
            selecionar_frase();
            sleep_ms(300);
        }
        if (!gpio_get(BOTAO3)) {
            fraseIndex = 2;
            selecionar_frase();
            emitir_som();
            sleep_ms(300);
        }
    }
}
