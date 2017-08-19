#ifndef _INTERFACES_H_
#define _INTERFACES_H_

#include <boost/uuid/uuid.hpp>

namespace interfaces {
	class Identifiable {
	public:
		Identifiable();
		Identifiable(Identifiable &&) noexcept;

		virtual ~Identifiable() = default;

		friend bool operator==(const Identifiable& lhs, const Identifiable& rhs);
		friend bool operator!=(const Identifiable& lhs, const Identifiable& rhs);


		virtual boost::uuids::uuid m_id1() const;
		virtual void set_id(const boost::uuids::uuid& id);

	private:
		boost::uuids::uuid m_id;
	};
}

#endif
