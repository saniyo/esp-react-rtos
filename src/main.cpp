#include <ESP8266React.h>
#include <LightMqttSettingsService.h>
#include <LightStateService.h>
// #include <freertos/FreeRTOS.h>
// #include <freertos/task.h>
// #include <Arduino_FreeRTOS.h>
//#include <FreeRTOS.h>
// #include <task.h>


#define SERIAL_BAUD_RATE 115200

AsyncWebServer server(80);
ESP8266React esp8266React(&server);
LightMqttSettingsService lightMqttSettingsService =
                            LightMqttSettingsService(&server, esp8266React.getFS(), esp8266React.getSecurityManager());
                            LightStateService lightStateService = LightStateService(&server,
                                                                                    esp8266React.getSecurityManager(),
                                                                                    esp8266React.getMqttClient(),
                                                                                    &lightMqttSettingsService);

void espTask(void* pvParameters);
TaskHandle_t espTaskHandle = NULL;

void setup() {
    // start serial and filesystem
    Serial.begin(SERIAL_BAUD_RATE);

    // start the framework and demo project
    esp8266React.begin();

    // load the initial light settings
    lightStateService.begin();

    // start the light service
    lightMqttSettingsService.begin();

    // start the server
    server.begin();

    // create task for ESPAsyncWebServer event loop
    xTaskCreatePinnedToCore(espTask, "ESPAsyncWebServerTask", 4096, NULL, 1, &espTaskHandle, 0);
}

void loop() {
    // nothing to do here
}

void espTask(void* pvParameters) {
    // loop forever
    while (true) {
      // run the ESPAsyncWebServer event loop
      esp8266React.loop();

      // wait a short period to allow other tasks to run
      vTaskDelay(pdMS_TO_TICKS(1));
      // vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}