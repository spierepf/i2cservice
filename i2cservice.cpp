#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>

#include "Register.h"
#include "MemoryRegister.h"
#include "DelegateRegister.h"
#include "IdentityRegister.h"

using namespace fastdelegate;

#define SLAVE_ADDRESS 0x29

typedef uint8_t addr_t;
typedef uint8_t reg_t;

const reg_t regCount = 3;
I2C::Register* registers[regCount];

reg_t reg;

void requestEvent() {
	registers[reg++] -> read();
}

void receiveEvent(int bytesReceived) {
	reg = Wire.read();

	size_t remaining = bytesReceived - 1;
	while(remaining > 0 && reg < regCount) {
		remaining = registers[reg++] -> write(remaining);
	}
}

class i2c_service {
public:
	void begin(addr_t slave_addr) {
		Wire.begin(SLAVE_ADDRESS);
		Wire.onRequest(requestEvent);
		Wire.onReceive(receiveEvent);
	}
};

i2c_service I2CService;

Servo servo;
int someValue = 0;

void setup() {
	Serial.begin(9600);

	servo.attach(9);
	servo.writeMicroseconds(1500);

	registers[0] = new I2C::IdentityRegister<int>(65);
	registers[1] = new I2C::DelegateRegister<int>(MakeDelegate(&servo, &Servo::readMicroseconds), 0 /*MakeDelegate(&servo, &Servo::writeMicroseconds)*/);
	registers[2] = new I2C::MemoryRegister<int>(&someValue);

	I2CService.begin(SLAVE_ADDRESS);
}

void loop() {
}
