#include <gtest/gtest.h>

#include "generators.h"
#include "entities.h"

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
	auto messageGenerator = generators::MessageGenerator();
	auto message = messageGenerator();

	// act
	auto result = entities::Message(message);

	// assert
	EXPECT_EQ(result.id(), message.id()) << "ID isn't equal. Expected " << to_string(message.id()) 
		<< " but was " << to_string(result.id());
	EXPECT_EQ(result.sender(), message.sender()) << "Senders aren't equal. Expected " << to_string(message.sender().id())
		<< " but was " << to_string(result.sender().id());
	EXPECT_EQ(result.receiver(), message.receiver()) << "Receivers aren't equal. Expected " << to_string(message.receiver().id())
		<< " but was " << to_string(result.receiver().id());
	EXPECT_EQ(result.size(), message.size()) << "Message size isn't equal. Expecter" << message.size() 
		<< " but was " << result.size();
}

TEST(EntitiesTests, CopiedMessageShouldContaintSameSenderAndReceiver) {
	// arrange
	// arrange 
	auto messageGenerator = generators::MessageGenerator();
	auto message = messageGenerator();

	// act
	auto result = entities::Message(message);

	result.sender().buffer().add(messageGenerator());
	result.receiver().buffer().add(messageGenerator());

	// assert
	EXPECT_EQ(result.sender().buffer().count(), message.sender().buffer().count());
	EXPECT_EQ(result.receiver().buffer().count(), message.receiver().buffer().count());
}

TEST(EntitiesTests, NodeShouldBeCreatedWithCorrectInfo) {
	// arrange
	// act
	auto node = entities::Node();
	auto emptyUuid = boost::uuids::uuid();

	// assert
	EXPECT_EQ(node.isUnactive(), false);
	EXPECT_EQ(node.buffer().count(), 0);
	EXPECT_EQ(node.receivedMessages().count(), 0);
	EXPECT_NE(node.id(), emptyUuid);
}

TEST(EntitiesTest, NodeShouldBeCopied) {
	// arrange
	auto messageGenerator = generators::MessageGenerator();

	auto nodeGenerator = generators::NodeGenerator();
	auto node = nodeGenerator();

	node.buffer().add(messageGenerator());
	node.receivedMessages().add(messageGenerator());

	// act
	auto result = entities::Node(node);

	// assert
	EXPECT_EQ(result.buffer().count(), node.buffer().count());
	EXPECT_EQ(result.buffer()[0], node.buffer()[0]);
	EXPECT_EQ(result.receivedMessages().count(), node.receivedMessages().count());
	EXPECT_EQ(result.receivedMessages()[0], node.receivedMessages()[0]);
	EXPECT_EQ(result.isUnactive(), node.isUnactive());
	EXPECT_EQ(result.id(), node.id());
}

TEST(EntitiesTest, NodeShouldBeCopiedIndependently) {
	// arrange
	auto messageGenerator = generators::MessageGenerator();

	auto nodeGenerator = generators::NodeGenerator();
	auto node = nodeGenerator();

	// act
	auto result = entities::Node(node);

	result.buffer().add(messageGenerator());
	result.receivedMessages().add(messageGenerator());
	result.setIsUnactive(true);

	// assert
	EXPECT_NE(result.buffer().count(), node.buffer().count());
	EXPECT_NE(result.receivedMessages().count(), node.receivedMessages().count());
	EXPECT_NE(result.isUnactive(), node.isUnactive());
	EXPECT_EQ(result.id(), node.id());
}