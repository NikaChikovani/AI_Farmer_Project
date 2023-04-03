#include "main.h"

QueueHandle_t sensor_data_queue;


void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);
    GPIO_Init();

    wifi_init_sta();


    sensor_data_queue = xQueueCreate(2, sizeof(sensor_paramtypedef));

    xTaskCreatePinnedToCore(main_task, "main_task", 10072, NULL, 12, NULL, app_cpu);
    xTaskCreatePinnedToCore(sensor_monitoring_task, "sensor_monitoring_task", 10072, NULL, 12, NULL, app_cpu);
    xTaskCreatePinnedToCore(wifi_monitoring_task, "wifi_monitoring_task", 10072, NULL, 12, NULL, app_cpu);

}


