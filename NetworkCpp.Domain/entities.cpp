#include "entities.h"

entities::Message::Message(const int size, const Node& sender, const Node& receiver) 
	: Identifiable(), m_size(size), m_sender(sender), m_receiver(receiver) {
}

entities::Message::Message(const Message& message) noexcept
	: Identifiable(message), m_size(message.m_size), m_sender(message.m_sender), m_receiver(message.m_receiver) {
}

const entities::Message& entities::Message::operator=(const Message& message) {
	Identifiable::operator=(message);

	if (this != &message) {
		*this = Message(message);
	}

	return *this;
}

int entities::Message::size() const {
	return m_size;
}

entities::Node entities::Message::sender() const {
	return m_sender;
}

entities::Node entities::Message::receiver() const {
	return m_receiver;
}
