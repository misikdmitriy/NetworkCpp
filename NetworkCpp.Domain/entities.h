#ifndef _ENTITIES_H_
#define _ENTITIES_H_

#include "interfaces.h"

namespace entities {
	class Node;

	class Message : public interfaces::Identifiable	{
	public:
		Message(const int size, const Node& sender, const Node& receiver);
		Message(const Message& message) noexcept;

		const Message&					operator=(const Message&);

		virtual int						size() const;
		virtual Node					sender() const;
		virtual Node					receiver() const;

	private:
		const int						m_size;
		const Node&						m_sender;
		const Node&						m_receiver;
	};

	class Node : public interfaces::Identifiable {
	public:
	private:

	};
}

#endif
