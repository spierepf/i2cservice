#ifndef REGISTER_H_
#define REGISTER_H_

#include <stddef.h>

namespace I2C {
	class Register {
	public:
		Register() {}

		virtual size_t write(size_t remaining) = 0;
		virtual void read() = 0;

		virtual ~Register() {}
	};
}

#endif /* REGISTER_H_ */
