#pragma once

void initMQTT();
void mqttTask(void *pvParameters);
void mqttLoopTask(void *pvParameters);