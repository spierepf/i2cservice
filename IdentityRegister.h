#ifndef IDENTITY_REGISTER_H_
#define IDENTITY_REGISTER_H_

#include <Wire.h>

#include "Register.h"

namespace I2C {
	template <class X>
	class IdentityRegister : public Register {
		X value;

	public:
		IdentityRegister(X v) : value(v) {
		}

		virtual size_t write(size_t remaining) {
			if(remaining >= sizeof(X)) {
				for(size_t i = 0; i < sizeof(X); i++) {
					Wire.read();
				}
				return remaining - sizeof(X);
			} else {
				return 0;
			}
		}

		virtual void read() {
			Wire.write((const uint8_t*)&value, sizeof(X));
		}
	};
}

#endif /* IDENTITY_REGISTER_H_ */
