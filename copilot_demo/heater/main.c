#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "heater.h"

uint8_t temperature;

void heaton(float temp) {
    printf("Heater ON (temp = %.2f C)\n", temp);
}

void heatoff(float temp) {
    printf("Heater OFF (temp = %.2f C)\n", temp);
}

int main() {

    uint8_t values[] = {100,120,140,160,180,200,220,240,200,180,150,120,100};
    int n = sizeof(values)/sizeof(values[0]);

    for(int i=0;i<n;i++) {

        temperature = values[i];

        printf("Sensor reading = %d\n", temperature);

        step();   // run Copilot monitor

        sleep(1);
    }

    return 0;
}
