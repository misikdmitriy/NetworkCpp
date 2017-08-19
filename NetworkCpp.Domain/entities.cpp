#include "entities.h"

entities::Message::Message()
	: Identifiable() {
}

entities::Message::Message(const Message& message) noexcept
	: Identifiable(message) {
}
