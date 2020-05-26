#include <driver/gpio.h>
#include <driver/dac.h>
#include <Arduino.h>

void blinkTask(void *pvParam) {
  uint8_t dacValue = 0;
  int16_t dacInc = 128;

  dac_output_enable(DAC_CHANNEL_1);
  while (true) {
    dac_output_voltage(DAC_CHANNEL_1, dacValue);  // dacWrite(DAC1, dacValue);
    dacValue += dacInc;
    if (dacInc > 0) {
      dacInc /= 2;
      if (! dacInc)
        dacInc = -1;
    } else {
      dacInc *= 2;
      if (dacInc < -128)
        dacInc = 128;
    }
    vTaskDelay(pdMS_TO_TICKS(111));
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  if (xTaskCreate(blinkTask, "blinkTask", 1024, NULL, 1, NULL) != pdPASS) {
    Serial.println("Unable to create blink task!");
    Serial.flush();
    esp_deep_sleep_start();
  }
}

void loop() {
  vTaskDelete(NULL);
}
