#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_bt_main.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"  // <-- necesario para vTaskDelay

static const char *TAG = "BLE_SCANNER";

static void gap_callback(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch (event)
    {
    case ESP_GAP_BLE_SCAN_RESULT_EVT:
    {
        esp_ble_gap_cb_param_t *scan_result = param;

        if (scan_result->scan_rst.search_evt == ESP_GAP_SEARCH_INQ_RES_EVT)
        {
            char bda_str[18];
            sprintf(bda_str, "%02x:%02x:%02x:%02x:%02x:%02x",
                    scan_result->scan_rst.bda[0],
                    scan_result->scan_rst.bda[1],
                    scan_result->scan_rst.bda[2],
                    scan_result->scan_rst.bda[3],
                    scan_result->scan_rst.bda[4],
                    scan_result->scan_rst.bda[5]);

            int rssi = scan_result->scan_rst.rssi;

            ESP_LOGI(TAG, "Dispositivo encontrado: %s | RSSI: %d dBm", bda_str, rssi);

            uint8_t adv_name_len = 0;
            uint8_t *adv_name = esp_ble_resolve_adv_data(
                scan_result->scan_rst.ble_adv,
                ESP_BLE_AD_TYPE_NAME_CMPL,
                &adv_name_len);

            if (adv_name != NULL && adv_name_len > 0)
            {
                char name[32];
                snprintf(name, sizeof(name), "%.*s", adv_name_len, (char *)adv_name);
                ESP_LOGI(TAG, "Nombre: %s", name);
            }

            // 🔹 Delay de 500 ms entre dispositivos
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        break;
    }

    case ESP_GAP_BLE_SCAN_START_COMPLETE_EVT:
        if (param->scan_start_cmpl.status == ESP_BT_STATUS_SUCCESS)
        {
            ESP_LOGI(TAG, "Escaneo BLE iniciado correctamente.");
        }
        else
        {
            ESP_LOGE(TAG, "Error al iniciar escaneo BLE.");
        }
        break;

    default:
        break;
    }
}

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_BLE));

    ESP_ERROR_CHECK(esp_bluedroid_init());
    ESP_ERROR_CHECK(esp_bluedroid_enable());

    ESP_ERROR_CHECK(esp_ble_gap_register_callback(gap_callback));

    esp_ble_scan_params_t ble_scan_params = {
        .scan_type = BLE_SCAN_TYPE_PASSIVE,
        .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
        .scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ALL,
        .scan_interval = 0x50,
        .scan_window = 0x30,
        .scan_duplicate = BLE_SCAN_DUPLICATE_DISABLE};

    ESP_ERROR_CHECK(esp_ble_gap_set_scan_params(&ble_scan_params));
    ESP_ERROR_CHECK(esp_ble_gap_start_scanning(0));

    ESP_LOGI(TAG, "Escaneo en curso... Se mostrarán los dispositivos detectados.");
}