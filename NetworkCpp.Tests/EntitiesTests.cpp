#include <gtest/gtest.h>

#include "generators.h"

class EntitiesTests : public testing::Test {
};

TEST(EntitiesTests, MessageShouldBeCreatedWithCorrectInfo) {
	// arrange
	auto nodeGenerator = generators::NodeGenerator();
	auto sender = nodeGenerator();
	auto receiver = nodeGenerator();
	auto emptyUuid = boost::uuids::uuid();

	// act
	auto message = entities::Message(10, sender, receiver);

	// assert
	EXPECT_EQ(message.receiver(), receiver);
	EXPECT_EQ(message.sender(), sender);
	EXPECT_EQ(message.size(), 10);
	EXPECT_NE(message.id(), emptyUuid);
}

TEST(EntitiesTests, MessageShouldBeCopied) {
	// arrange 
	auto nodeGenerator = generators::MessageGenerator();
	auto message = nodeGenerator();

	// act
	auto result = entities::Message(message);

	// assert
	EXPECT_EQ(result.id(), message.id());
	EXPECT_EQ(result.sender(), message.sender());
	EXPECT_EQ(result.receiver(), message.receiver());
	EXPECT_EQ(result.size(), message.size());
}