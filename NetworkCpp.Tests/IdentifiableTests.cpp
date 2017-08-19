#include <gtest/gtest.h>

#include "interfaces.h"

using namespace interfaces;

class IdentifiableTest : public testing::Test {
};

TEST(IdentifiableTest, CheckId) {
	// arrange
	Identifiable identifiable;
	boost::uuids::uuid empty;
	
	// act
	// assert
	EXPECT_NE(identifiable.id(), empty);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}