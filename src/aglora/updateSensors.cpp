#include "aglora.h"

/// Udpate sensors in loraDataPacket
void AGLORA::updateSensors(DATA *loraDataPacket)
{
    loraDataPacket->battery = 100; // not implemented: battery is not measured, hardcoded 100

#if DEBUG_MODE && DEBUG_AGLORA
    Serial.print(F("🟢[AGLoRa sensors: "));
    Serial.print(F("🔋 - "));
    Serial.print(loraDataPacket->battery);
    Serial.println(F("]"));
#endif
}
