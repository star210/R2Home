// Telemetry
unsigned long tlm = 0;
int telemetryPacketCount = 0;

void setupTelemetry() {
  //   serialTelem.begin(TELEM_BAUD);         //Telemetry
#ifdef DEVMODE
  Serial.println("Telemetry setup complete");
#endif
}

void setTelemetry(int t) {
  if (millis() - tlm >= t) {
    sendTelemetry();
  }
}
void sendTelemetry() {
  telemetryPacketCount ++;
#ifdef DEVMODE
  Serial.println("Telemetry Packet Sent");
#endif
}
