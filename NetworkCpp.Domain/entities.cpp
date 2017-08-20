#include "entities.h"

entities::Message::Message(const int size, const Node& sender, const Node& receiver)
	: Identifiable(), m_size(size), m_sender(std::make_shared<Node>(sender))
		, m_receiver(std::make_shared<Node>(receiver)) {
}

entities::Message::Message(const Message& message) noexcept
	: Identifiable(message), m_size(message.m_size), m_sender(message.m_sender)
		, m_receiver(message.m_receiver) {
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

const entities::Node& entities::Message::sender() const {
	return *m_sender;
}

const entities::Node& entities::Message::receiver() const {
	return *m_receiver;
}

entities::Node::Node()
	: Identifiable() {
	m_buffer = std::make_shared<MessageBuffer<>>();
	m_receivedMessages = std::make_shared<MessageBuffer<>>();

	m_isUnactive = false;
}

entities::Node::Node(const Node& node)
	: Identifiable(node) {
	*this = node;
}

entities::Node::~Node() {
}

const entities::Node& entities::Node::operator=(const Node& node) {
	Identifiable::operator=(node);

	if (this != &node) {
		this->m_buffer = std::make_shared<MessageBuffer<>>(*node.m_buffer);
		this->m_receivedMessages = std::make_shared<MessageBuffer<>>(*node.m_receivedMessages);

		this->m_isUnactive = node.m_isUnactive;
	}

	return *this;
}

entities::MessageBuffer<>& entities::Node::receivedMessages() const {
	return *m_receivedMessages;
}

entities::MessageBuffer<>& entities::Node::buffer() const {
	return *m_buffer;
}

entities::Channel::Channel()
	: Channel(INT_MAX) {
}

entities::Channel::Channel(const Channel& channel) {
	*this = channel;
}

entities::Channel::Channel(const int size)
	: Identifiable() {
	// ToDo: finish
}

const bool& entities::Node::isUnactive() const {
	return m_isUnactive;
}

void entities::Node::setIsUnactive(const bool is_unactive) {
	m_isUnactive = is_unactive;
}