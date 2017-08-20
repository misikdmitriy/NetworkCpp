#ifndef _NODE_H_
#define _NODE_H_

#include <memory>
#include <vector>

#include "interfaces.h"

namespace entities {
	class Node;

	class Message : public interfaces::Identifiable {
	public:
		Message(const int size, const Node& sender, const Node& receiver);
		Message(const Message& message) noexcept;

		const Message&							operator=(const Message&);

		virtual int								size() const;
		virtual const Node&						sender() const;
		virtual const Node&						receiver() const;

	private:
		const int								m_size;
		const std::shared_ptr<const Node>		m_sender;
		const std::shared_ptr<const Node>		m_receiver;
	};

	template<int size = INT_MAX>
	class MessageBuffer {
		static_assert(size > 0, "Size should non-negative and not zero");
	public:
		MessageBuffer();
		MessageBuffer(const MessageBuffer&);
		template<int copySize>
		MessageBuffer(const MessageBuffer<copySize>&);

		typedef std::function<void(void*, const Message&)>		onAddListener;
		typedef std::function<void(void*, const Message&)>		onRemoveListener;
		typedef std::function<void(void*)>						onClearListener;

		bool										isFilled() const;

		void										addOnAddListener(const onAddListener&);
		void										addRemoveListener(const onRemoveListener&);
		void										addOnClearListener(const onClearListener&);

		std::vector<Message>::iterator				begin();
		std::vector<Message>::iterator				end();

		std::vector<Message>::const_iterator		cbegin() const;
		std::vector<Message>::const_iterator		cend() const;

		void										add(const Message&);
		void										clear();
		void										remove(const Message&);

		bool										contains(const Message&) const;
		int											indexOf(const Message&) const;

		int											count() const;

		const Message&								operator[](size_t index);
		const MessageBuffer&						operator=(const MessageBuffer&);
		template<int copySize>
		const MessageBuffer&						operator=(const MessageBuffer<copySize>&);
	private:
		std::vector<Message>						m_buffer;
		std::vector<onAddListener>					m_addListeners;
		std::vector<onRemoveListener>				m_removeListeners;
		std::vector<onClearListener>				m_clearListeners;

		void										onAdd(const Message&);
		void										onClear();
		void										onRemove(const Message&);
	};

	template <int size>
	MessageBuffer<size>::MessageBuffer() {
	}

	template <int size>
	MessageBuffer<size>::MessageBuffer(const MessageBuffer<size>& buffer)
		: MessageBuffer() {
		*this = buffer;
	}

	template <int size>
	template <int copySize>
	MessageBuffer<size>::MessageBuffer(const MessageBuffer<copySize>& buffer)
		: MessageBuffer() {
		*this = buffer;
	}

	template <int size>
	bool MessageBuffer<size>::isFilled() const {
		return m_buffer.size() >= size;
	}

	template <int size>
	void MessageBuffer<size>::addOnAddListener(const onAddListener& listener) {
		m_addListeners.push_back(listener);
	}

	template <int size>
	void MessageBuffer<size>::addRemoveListener(const onRemoveListener& listener) {
		m_removeListeners.push_back(listener);
	}

	template <int size>
	void MessageBuffer<size>::addOnClearListener(const onClearListener& listener) {
		m_clearListeners.push_back(listener);
	}

	template <int size>
	std::vector<Message>::iterator MessageBuffer<size>::begin() {
		return m_buffer.begin();
	}

	template <int size>
	std::vector<Message>::iterator MessageBuffer<size>::end() {
		return m_buffer.end();
	}

	template <int size>
	std::vector<Message>::const_iterator MessageBuffer<size>::cbegin() const {
		return m_buffer.cbegin();
	}

	template <int size>
	std::vector<Message>::const_iterator MessageBuffer<size>::cend() const {
		return m_buffer.cend();
	}

	template <int size>
	void MessageBuffer<size>::add(const Message& message) {
		if (isFilled()) {
			return;
		}

		m_buffer.push_back(message);
		onAdd(message);
	}

	template <int size>
	void MessageBuffer<size>::clear() {
		m_buffer.clear();
		onClear();
	}

	template <int size>
	void MessageBuffer<size>::remove(const Message& message) {
		auto pointer = find(m_buffer.cbegin(), m_buffer.cend(), message);
		if (pointer == m_buffer.cend()) {
			return;
		}

		m_buffer.erase(find(m_buffer.cbegin(), m_buffer.cend(), message));
		onRemove(message);
	}

	template <int size>
	bool MessageBuffer<size>::contains(const Message& message) const {
		return std::find(m_buffer.cbegin(), m_buffer.cend(), message) != m_buffer.cend();
	}

	template <int size>
	int MessageBuffer<size>::indexOf(const Message& message) const {
		auto iterator = find(m_buffer.cbegin(), m_buffer.cend(), message);
		if (iterator == m_buffer.cend())
		{
			return -1;
		}
		return iterator - m_buffer.cbegin();
	}

	template <int size>
	int MessageBuffer<size>::count() const {
		return m_buffer.size();
	}

	template <int size>
	const Message& MessageBuffer<size>::operator[](size_t index) {
		return m_buffer[index];
	}

	template <int size>
	const MessageBuffer<size>& MessageBuffer<size>::operator=(const MessageBuffer& buffer) {
		if (this != &buffer) {
			this->clear();
			this->m_clearListeners.clear();
			this->m_addListeners.clear();
			this->m_removeListeners.clear();

			this->m_buffer.insert(this->m_buffer.cbegin(), buffer.m_buffer.cbegin(), buffer.m_buffer.cend());
			this->m_addListeners.insert(this->m_addListeners.cbegin(), buffer.m_addListeners.cbegin(), buffer.m_addListeners.cend());
			this->m_removeListeners.insert(this->m_removeListeners.cbegin(), buffer.m_removeListeners.cbegin(), buffer.m_removeListeners.cend());
			this->m_clearListeners.insert(this->m_clearListeners.cbegin(), buffer.m_clearListeners.cbegin(), buffer.m_clearListeners.cend());
		}

		return *this;
	}

	template <int size>
	template <int copySize>
	const MessageBuffer<size>& MessageBuffer<size>::operator=(const MessageBuffer<copySize>& buffer) {
		if (this != &buffer) {
			if (size > copySize)
			{
				throw std::logic_error("cannot copy bigger to smaller buffer");
			}

			this->clear();
			this->m_clearListeners.clear();
			this->m_addListeners.clear();
			this->m_removeListeners.clear();

			this->m_buffer.insert(this->m_buffer.cbegin(), buffer.m_buffer.cbegin(), buffer.m_buffer.cend());
			this->m_addListeners.insert(this->m_addListeners.cbegin(), buffer.m_addListeners.cbegin(), buffer.m_addListeners.cend());
			this->m_removeListeners.insert(this->m_removeListeners.cbegin(), buffer.m_removeListeners.cbegin(), buffer.m_removeListeners.cend());
			this->m_clearListeners.insert(this->m_clearListeners.cbegin(), buffer.m_clearListeners.cbegin(), buffer.m_clearListeners.cend());
		}

		return *this;
	}

	template <int size>
	void MessageBuffer<size>::onAdd(const Message& added) {
		for (auto listener : m_addListeners) {
			listener(this, added);
		}
	}

	template <int size>
	void MessageBuffer<size>::onClear() {
		for (auto listener : m_clearListeners) {
			listener(this);
		}
	}

	template <int size>
	void MessageBuffer<size>::onRemove(const Message& removed) {
		for (auto listener : m_removeListeners) {
			listener(this, removed);
		}
	}

	class Node : public interfaces::Identifiable {
	public:
		Node();
		Node(const Node& node);

		~Node() override;

		const Node&									operator=(const Node&);

		virtual MessageBuffer<>& receivedMessages() const;
		virtual MessageBuffer<>& buffer() const;

		virtual const bool& isUnactive() const;
		virtual void setIsUnactive(const bool is_unactive);
	private:
		std::shared_ptr<MessageBuffer<>>			m_receivedMessages;
		std::shared_ptr<MessageBuffer<>>			m_buffer;
		bool										m_isUnactive;
	};

	class Channel : public interfaces::Identifiable {
	public:
		Channel();
		Channel(const int size);
		Channel(const Channel&);

	private:
		MessageBuffer<>								m_buffer;
		bool										m_busy;
	};

	class NodesPair : public interfaces::Identifiable {
	public:

	private:
		const Node&								m_first;
		const Node&								m_second;
		const Channel&							m_channel;
	};
}

#endif