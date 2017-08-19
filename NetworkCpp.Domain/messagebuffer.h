#ifndef _MESSAGEBUFFER_H_
#define _MESSAGEBUFFER_H_

#include <vector>
#include <functional>

#include "entities.h"

namespace entities {
	template<int size = INT_MAX>
	class MessageBuffer {
		static_assert(size > 0, "Size should non-negative and not zero");
	public:
		MessageBuffer();

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
		m_buffer = std::vector<Message>();

		m_addListeners = std::vector<onAddListener>();
		m_removeListeners = std::vector<onRemoveListener>();
		m_clearListeners = std::vector<onClearListener>();
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
		auto pointer = std::find(m_buffer.cbegin(), m_buffer.cend(), message);
		if (pointer == m_buffer.cend()) {
			return;
		}

		m_buffer.erase(std::find(m_buffer.cbegin(), m_buffer.cend(), message));
		onRemove(message);
	}

	template <int size>
	bool MessageBuffer<size>::contains(const Message& message) const {
		return std::find(m_buffer.cbegin(), m_buffer.cend(), message) != m_buffer.cend();
	}

	template <int size>
	int MessageBuffer<size>::indexOf(const Message& message) const {
		return std::find(m_buffer.cbegin(), m_buffer.cend(), message) - m_buffer.cbegin();
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
}

#endif
