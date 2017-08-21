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

entities::Observable::Observable() {
}

entities::Observable::Observable(const Observable& observable)
	: m_observers(observable.m_observers) {
}

entities::Observable::~Observable() {
	m_observers.clear();
}

void entities::Observable::addObserver(const Observer const * observer) {
	m_observers.push_back(observer);
}

void entities::Observable::removeObserver(const Observer* observer) {
	auto iterator = find(m_observers.cbegin(), m_observers.cend(), observer);
	if (iterator == m_observers.cend()) {
		return;
	}
	m_observers.erase(iterator);
}

entities::Observer::Observer(Observable& observable) {
	m_observable = std::make_shared<Observable>(observable);
	m_observable->addObserver(this);
}

entities::Observer::Observer(const Observer& observer) {
	m_observable = std::make_shared<Observable>(*observer.m_observable);
	m_observable->addObserver(this);
}

entities::Observer::~Observer() {
	m_observable->removeObserver(this);
}

entities::MessageContainerObserver::~MessageContainerObserver() {
}

void entities::MessageContainerObservable::onAdd(const Message& added) {
	for (MessageContainerObserver* listener : m_observers) {
		listener->addListener(this, added);
	}
}

void entities::MessageContainerObservable::onClear() {
	for (MessageContainerObserver* listener : m_observers) {
		listener->clearListener(this);
	}
}

void entities::MessageContainerObservable::onRemove(const Message& removed) {
	for (auto listener : m_observers) {
		auto converted = (MessageContainerObserver *)listener;
		listener->removeListener(this, removed);
	}
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

entities::MessageBuffer<>& entities::Node::receivedMessages() {
	return *m_receivedMessages;
}

entities::MessageBuffer<>& entities::Node::buffer() {
	return *m_buffer;
}

const bool& entities::Node::isUnactive() const {
	return m_isUnactive;
}

void entities::Node::setIsUnactive(const bool is_unactive) {
	m_isUnactive = is_unactive;
}

entities::Channel::Channel() 
	: Observable() {
	m_busy = false;
}

entities::Channel::Channel(const Channel& channel)
	: Observable(channel) {
	m_busy = false;
}

entities::Channel::~Channel() {
}

entities::OneWayChannel::OneWayChannel()
	: Channel(), MessageContainerObserver(static_cast<Observable&>(*this)), m_message(nullptr) {
}

entities::OneWayChannel::OneWayChannel(const OneWayChannel& channel)
	: Channel(channel), MessageContainerObserver(static_cast<Observable&>(*this)), m_message(channel.m_message) {
}

entities::OneWayChannel::~OneWayChannel() {
}

void entities::OneWayChannel::add(const Message& message) {
	m_message = std::make_shared<const Message&>(message);
}

const entities::Message& entities::OneWayChannel::get() {
	auto message = *m_message;
	m_message = nullptr;
	return message;
}

void entities::OneWayChannel::addListener(void*, const Message&) {
	m_busy = true;
}

void entities::OneWayChannel::removeListener(void*, const Message&) {
	m_busy = false;
}

void entities::OneWayChannel::clearListener(void*) {
	m_busy = false;
}

entities::NodesPair::NodesPair(const Node& node, const Node& node1, const Channel& channel)
	: Identifiable(), m_first(node), m_second(node1), m_channel(channel) {
}

entities::MessageContainerObserver::MessageContainerObserver(Observable& observable) 
	: Observer(observable) {
}

entities::MessageContainerObserver::MessageContainerObserver(const MessageContainerObserver& observer) 
	: Observer(observer) {
}
