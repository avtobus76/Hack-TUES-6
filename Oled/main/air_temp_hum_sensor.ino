#include "Common.h"

void _initAirHumSensor()
{
  dht.begin();
}

void _getAirTempHum(int* temp, int* hum)
{
  *temp = dht.readTemperature();
  *hum = dht.readHumidity();
}
