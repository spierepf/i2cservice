#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>

#include <FastDelegate.h>

using namespace fastdelegate;

#define SLAVE_ADDRESS 0x29

namespace I2C {
	class Register {
	public:
		Register() {}

		virtual size_t write(size_t remaining) = 0;
		virtual void read() = 0;

		virtual ~Register() {}
	};

	template <class X>
	class DelegateRegister : public Register {
		typedef FastDelegate0<X> getter;
		typedef FastDelegate1<X> setter;

		getter get;
		setter set;

	public:
		DelegateRegister(getter g, setter s) : get(g), set(s) {
		}

		virtual size_t write(size_t remaining) {
			if(remaining >= sizeof(X)) {
				X value = 0;
				for(size_t i = 0; i < sizeof(X); i++) {
					value |= ((X)Wire.read()) << (8 * i);
				}
				set(value);
				return remaining - sizeof(X);
			} else {
				return 0;
			}
		}

		virtual void read() {
			X value = get();
			Wire.write((const uint8_t*)&value, sizeof(X));
		}
	};

	template <class X>
	class MemoryRegister : public Register {
		X* value;

	public:
		MemoryRegister(X* v) : value(v) {
		}

		virtual size_t write(size_t remaining) {
			if(remaining >= sizeof(X)) {
				for(size_t i = 0; i < sizeof(X); i++) {
					*value |= ((X)Wire.read()) << (8 * i);
				}
				return remaining - sizeof(X);
			} else {
				return 0;
			}
		}

		virtual void read() {
			Wire.write((const uint8_t*)value, sizeof(X));
		}
	};
}

typedef uint8_t addr_t;
typedef uint8_t reg_t;

const reg_t regCount = 2;
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

	registers[0] = new I2C::DelegateRegister<int>(MakeDelegate(&servo, &Servo::readMicroseconds), MakeDelegate(&servo, &Servo::writeMicroseconds));
	registers[1] = new I2C::MemoryRegister<int>(&someValue);

	I2CService.begin(SLAVE_ADDRESS);
}

void loop() {
}
