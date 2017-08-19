#include <gtest/gtest.h>

#include "entities.h"
#include "messagebuffer.h"

entities::Message testMessages[] = { entities::Message(), entities::Message(), entities::Message() };

class MessageBufferTests : public testing::Test {
};

TEST(MessageBufferTests, BufferShouldBeLimited) {
	// arrange
	auto buffer = entities::MessageBuffer<2>();

	// act
	buffer.add(testMessages[0]);
	buffer.add(testMessages[1]);
	buffer.add(testMessages[2]);

	// assert
	EXPECT_TRUE(buffer.isFilled());
	EXPECT_EQ(buffer.count(), 2);
	EXPECT_EQ(buffer[0], testMessages[0]);
	EXPECT_EQ(buffer[1], testMessages[1]);
}

TEST(MessageBufferTests, BufferShouldRaiseEventOnAdd) {
	// arrange
	auto buffer = entities::MessageBuffer<1>();
	auto counter = 0;
	auto listener = [&](void* sender, const entities::Message& message) {
		EXPECT_EQ(message, testMessages[0]);
		EXPECT_EQ(sender, &buffer);
		counter++;
	};

	buffer.addOnAddListener(listener);

	// act
	buffer.add(testMessages[0]);

	// assert
	EXPECT_EQ(counter, 1);
}

TEST(MessageBufferTests, BufferShouldNotRaiseEventIfBufferOverfilled) {
	// arrange
	auto buffer = entities::MessageBuffer<1>();
	auto counter = 0;
	auto listener = [&](void* sender, const entities::Message& message) {
		counter++;
	};

	buffer.addOnAddListener(listener);

	// act
	buffer.add(testMessages[0]);
	buffer.add(testMessages[1]);

	// assert
	EXPECT_EQ(counter, 1);
}

TEST(MessageBufferTests, BufferShouldRaiseEventOnRemove) {
	// arrange
	auto buffer = entities::MessageBuffer<1>();
	auto counter = 0;
	auto listener = [&](void* sender, const entities::Message& message) {
		EXPECT_EQ(message, testMessages[0]);
		EXPECT_EQ(sender, &buffer);
		counter++;
	};

	buffer.addRemoveListener(listener);
	buffer.add(testMessages[0]);

	// act
	buffer.remove(testMessages[0]);

	// assert
	EXPECT_EQ(counter, 1);
}

TEST(MessageBufferTests, BufferShouldNotFailIfMessageIsAbsent) {
	// arrange
	auto buffer = entities::MessageBuffer<1>();

	// act
	// assert
	buffer.remove(testMessages[0]);
}

TEST(MessageBufferTests, BufferShouldNotRaiseEventIfMessageIsAbsent) {
	// arrange
	auto buffer = entities::MessageBuffer<1>();
	auto counter = 0;
	auto listener = [&](void* sender, const entities::Message& message) {
		counter++;
	};

	buffer.addRemoveListener(listener);

	// act
	buffer.remove(testMessages[0]);

	// assert
	EXPECT_EQ(counter, 0);
}

// ToDo: finish tests