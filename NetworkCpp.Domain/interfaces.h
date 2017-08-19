#ifndef _INTERFACES_H_
#define _INTERFACES_H_

#include <boost/uuid/uuid.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <ostream>
#include <string>

namespace interfaces {
	class Identifiable {
	public:
		Identifiable();
		Identifiable(Identifiable &&) noexcept;

		virtual ~Identifiable() = default;

		friend bool operator==(const Identifiable& lhs, const Identifiable& rhs);
		friend bool operator!=(const Identifiable& lhs, const Identifiable& rhs);

		virtual boost::uuids::uuid id() const;

		friend std::ostream& operator<<(std::ostream& os, const Identifiable& obj) {
			return os << "id: " << boost::lexical_cast<std::string>(obj.m_id);
		}

	private:
		boost::uuids::uuid m_id;
	};
}

#endif
