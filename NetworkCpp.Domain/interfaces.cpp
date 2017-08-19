#include "interfaces.h"

#include <boost/uuid/uuid_generators.hpp>

interfaces::Identifiable::Identifiable() 
	: m_id(boost::uuids::random_generator()()) {
}

interfaces::Identifiable::Identifiable(const Identifiable& obj) noexcept
	: m_id(obj.m_id) {
}

boost::uuids::uuid interfaces::Identifiable::id() const {
	return m_id;
}

bool interfaces::operator==(const interfaces::Identifiable& lhs, const interfaces::Identifiable& rhs) {
	return lhs.m_id == rhs.m_id;
}

bool interfaces::operator!=(const interfaces::Identifiable& lhs, const interfaces::Identifiable& rhs) {
	return !(lhs == rhs);
}