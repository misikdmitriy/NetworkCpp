#ifndef _GENERATORS_H_
#define _GENERATORS_H_

#include "entities.h"

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

namespace generators {
	template<typename T>
	class Generator {
	public:
		virtual ~Generator() = default;

		virtual T					Generate() = 0;
		T							operator()();
	};

	template <typename T>
	T Generator<T>::operator()() {
		return Generate();
	}

	class NodeGenerator : public Generator<entities::Node> {

	public:
		entities::Node				Generate() override;
	};

	class MessageGenerator : public Generator<entities::Message> {
	public:
		MessageGenerator();

		~MessageGenerator() override;

		entities::Message							Generate() override;
	private:
		Generator<entities::Node>*					m_nodeGenerator;
		boost::random::mt19937*						m_rng;
		boost::random::uniform_int_distribution<>*	m_distribution;
	};
}

#endif
