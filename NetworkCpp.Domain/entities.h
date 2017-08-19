#ifndef _ENTITIES_H_
#define _ENTITIES_H_

#include "interfaces.h"

namespace entities {
	class Message : public interfaces::Identifiable	{
	public:
		Message();
		Message(const Message& message) noexcept;

	private:
	};
}

#endif
