#include "interfaces.h"

#include <boost/uuid/uuid_generators.hpp>

interfaces::IdentifiableInterface::IdentifiableInterface() 
	: id(boost::uuids::random_generator()()) {
}

interfaces::IdentifiableInterface::IdentifiableInterface(IdentifiableInterface&& obj) noexcept
	: id(obj.id) {
}

bool interfaces::operator==(const interfaces::IdentifiableInterface& lhs, const interfaces::IdentifiableInterface& rhs) {
	return lhs.id == rhs.id;
}

bool interfaces::operator!=(const interfaces::IdentifiableInterface& lhs, const interfaces::IdentifiableInterface& rhs) {
	return !(lhs == rhs);
}