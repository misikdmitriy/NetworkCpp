#include <gtest/gtest.h>

#include "interfaces.h"

using namespace interfaces;

class IdentifiableTest : public testing::Test {
};

TEST(IdentifiableTest, IdShouldBeNotNullable) {
	// arrange
	Identifiable identifiable;
	boost::uuids::uuid empty;
	
	// act
	// assert
	EXPECT_NE(identifiable.id(), empty);
}

TEST(IdentifiableTest, IdShouldBeUnique) {
	// arrange
	Identifiable identifiable1, identifiable2;

	// Act
	// Assert
	EXPECT_NE(identifiable1.id(), identifiable2.id());
}