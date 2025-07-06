#include "bluetooth.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID "12345678-1234-1234-1234-123456789abc"
#define CHARACTERISTIC_UUID "abcd1234-5678-90ab-cdef-1234567890ab"

BLECharacteristic *sensorCharacteristic;

void initBLE()
{
    BLEDevice::init("TH_DEVICE");
    BLEServer *server = BLEDevice::createServer();
    BLEService *service = server->createService(SERVICE_UUID);

    sensorCharacteristic = service->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY);

    sensorCharacteristic->addDescriptor(new BLE2902());

    service->start();
    BLEAdvertising *advertising = BLEDevice::getAdvertising();
    advertising->start();

    Serial.println("📡 BLE başlatıldı, reklam yapılıyor...");
}

void bleSendSensorData(float temp, float hum)
{
    if (sensorCharacteristic)
    {
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "T:%.1fC H:%.0f%%", temp, hum);
        sensorCharacteristic->setValue(buffer);
        sensorCharacteristic->notify();
        Serial.println(String("BLE notify: ") + buffer);
    }
}

void bluetoothTask(void *pvParameters)
{
    extern float getTemperature();
    extern float getHumidity();

    while (1)
    {
        float temp = getTemperature();
        float hum = getHumidity();

        bleSendSensorData(temp, hum);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}