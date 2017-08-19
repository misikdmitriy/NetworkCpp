#include "generators.h"

entities::Node generators::NodeGenerator::Generate() {
	auto node = entities::Node();
	return node;
}

generators::MessageGenerator::MessageGenerator() {
	m_nodeGenerator = new NodeGenerator();

	m_rng = new boost::random::mt19937(time(nullptr));
	m_distribution = new boost::random::uniform_int_distribution<>();
}

generators::MessageGenerator::~MessageGenerator() {
	delete m_nodeGenerator;
	delete m_rng;
	delete m_distribution;
}

entities::Message generators::MessageGenerator::Generate() {
	auto message = entities::Message((*m_distribution)(*m_rng),
		m_nodeGenerator->Generate(),
		m_nodeGenerator->Generate());

	return message;
}
