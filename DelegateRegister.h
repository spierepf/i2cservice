#ifndef DELEGATE_REGISTER_H_
#define DELEGATE_REGISTER_H_

#include <Wire.h>

#include <FastDelegate.h>

#include "Register.h"

using namespace fastdelegate;

#define SLAVE_ADDRESS 0x29

namespace I2C {
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
}

#endif /* DELEGATE_REGISTER_H_ */
