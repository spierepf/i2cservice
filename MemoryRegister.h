#ifndef MEMORY_REGISTER_H_
#define MEMORY_REGISTER_H_

#include <Wire.h>

#include "Register.h"

namespace I2C {
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

#endif /* MEMORY_REGISTER_H_ */
