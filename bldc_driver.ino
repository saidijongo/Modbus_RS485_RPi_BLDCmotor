#define DRIVER_TYPE ZLAC8030L
#include <ModbusMaster.h>

#define MODBUS_BAUD_RATE 115200
#if MODBUS_BAUD_RATE > 19200
uint32_t t3_5 = 1750;
#else
uint32_t t3_5 = (1000000 * 39) / MODBUS_BAUD_RATE + 500;  // +500us : to be safe
#endif

#include "bldc_driver.h"
ModbusMaster driver_node;

int16_t actual_velocity = 0;
int16_t bus_voltage = 0;
int16_t driver_temp = 0;
int16_t motor_temp = 0;

void setup() {
  // Replace with the desired Serial port
  Serial2.begin(MODBUS_BAUD_RATE, SERIAL_8N1, 16, 17);
  driver_node.begin(6, Serial2);

  Serial.begin(9600);

  driver__init(driver_node, "My Driver");
  driver__write_single_register(driver_node, ADDR_TARGET_VELOCITY, 0);
}

void loop() {
  driver__write_single_register(driver_node, ADDR_TARGET_VELOCITY, 50);
  driver__read_single_holding_register(driver_node, ADDR_ACTUAL_VELOCITY, &actual_velocity);
  driver__read_single_holding_register(driver_node, ADDR_BUS_VOLTAGE, &bus_voltage);
  driver__read_single_holding_register(driver_node, ADDR_MOTOR_TEMP, &motor_temp);
  driver__read_single_holding_register(driver_node, ADDR_DRIVER_TEMP, &driver_temp);
  Serial.print(actual_velocity);
  Serial.print(";");
  Serial.print(bus_voltage);
  Serial.print(";");
  Serial.print(motor_temp);
  Serial.print(";");
  Serial.print(driver_temp);
  Serial.println();
}
