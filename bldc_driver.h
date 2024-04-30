#define DEBUG_ENABLE 1

#if DEBUG_ENABLE
#define debugPrint(x) Serial.print(x)
#define debugPrintln(x) Serial.println(x)
#endif

#if (DRIVER_TYPE == ZLAC8030L)
#define ADDR_CONTROL_WORD 0x2031
#define ADDR_OPERATING_MODE 0x2032
#define ADDR_TARGET_VELOCITY 0x203A
#define ADDR_ACTUAL_VELOCITY 0x202C
#define ADDR_BUS_VOLTAGE 0x2029
#define ADDR_MOTOR_TEMP 0x2026
#define ADDR_DRIVER_TEMP 0x2024

#define DRIVER_ENABLE 0x08
#define DRIVER_CLEAR_ALARM 0x06
#define DRIVER_VELOCITY_MODE 0x03

bool driver__write_single_register(ModbusMaster node, int16_t address, int16_t value) {
  delayMicroseconds(t3_5);
  uint8_t result = node.writeSingleRegister(address, value);
  bool ret = (result == node.ku8MBSuccess);
  if (!ret) {
    debugPrintln(result);
  }
  return ret;
}

bool driver__read_single_holding_register(ModbusMaster node, int16_t address, int16_t* dest) {
  delayMicroseconds(t3_5);
  uint8_t result = node.readHoldingRegisters(address, 1);
  if (result == node.ku8MBSuccess) {
    *dest = node.getResponseBuffer(0);
    return true;
  } else {
    debugPrint("GET ERROR: ");
    debugPrintln(result);
    return false;
  }
}

void driver__init(ModbusMaster node, String node_name = "default node") {
  debugPrintln("==== Initialize " + node_name + " ====");
  bool driver_initiation_success = true;
  debugPrint("Clear Driver Alarm -> ");
  if (driver__write_single_register(node, ADDR_CONTROL_WORD, DRIVER_CLEAR_ALARM)) {
    debugPrintln("SUCCESS");
  } else {
    debugPrintln("FAILED");
    driver_initiation_success = false;
  }

  debugPrint("Enable Driver -> ");
  if (driver__write_single_register(node, ADDR_CONTROL_WORD, DRIVER_ENABLE)) {
    debugPrintln("SUCCESS");
  } else {
    debugPrintln("FAILED");
    driver_initiation_success = false;
  }

  debugPrint("Driver Set Velocity Mode -> ");
  if (driver__write_single_register(node, ADDR_OPERATING_MODE, DRIVER_VELOCITY_MODE)) {
    debugPrintln("SUCCESS");
  } else {
    debugPrintln("FAILED");
    driver_initiation_success = false;
  }
  if (!driver_initiation_success) {
    debugPrintln("Driver Init Failed");
    while (1) {}
  }
}
#endif
