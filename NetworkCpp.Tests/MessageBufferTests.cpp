#include <gtest/gtest.h>

#include "entities.h"
#include "generators.h"

generators::MessageGenerator messageGenerator;
entities::Message testMessages[] = { messageGenerator(), messageGenerator(), messageGenerator() };

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
	auto buffer = entities::MessageBuffer<>();
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
	auto buffer = entities::MessageBuffer<>();
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
	auto buffer = entities::MessageBuffer<>();

	// act
	// assert
	buffer.remove(testMessages[0]);
}

TEST(MessageBufferTests, BufferShouldNotRaiseEventIfMessageIsAbsent) {
	// arrange
	auto buffer = entities::MessageBuffer<>();
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

TEST(MessageBufferTests, BufferShouldRaiseEventOnClear) {
	// arrange
	auto buffer = entities::MessageBuffer<>();
	auto counter = 0;
	auto listener = [&](void* sender) {
		EXPECT_EQ(sender, &buffer);
		counter++;
	};

	buffer.addOnClearListener(listener);

	// act
	buffer.clear();

	// assert
	EXPECT_EQ(counter, 1);
}

TEST(MessageBufferTests, ContainsShouldReturnTrue) {
	// arrange
	auto buffer = entities::MessageBuffer<>();
	buffer.add(testMessages[0]);

	// act
	auto result = buffer.contains(testMessages[0]);

	// assert
	EXPECT_TRUE(result);
}

TEST(MessageBufferTests, ContainsShouldReturnFalse) {
	// arrange
	auto buffer = entities::MessageBuffer<>();
	buffer.add(testMessages[0]);

	// act
	auto result = buffer.contains(testMessages[1]);

	// assert
	EXPECT_FALSE(result);
}

TEST(MessageBufferTests, ContainsShouldReturnFalseIfBufferIsEmpty) {
	// arrange
	auto buffer = entities::MessageBuffer<>();

	// act
	auto result = buffer.contains(testMessages[1]);

	// assert
	EXPECT_FALSE(result);
}

TEST(MessageBufferTests, IndexOfShouldReturnCorrectIndex) {
	// arrange
	auto buffer = entities::MessageBuffer<>();
	buffer.add(testMessages[0]);
	buffer.add(testMessages[1]);

	// act
	auto result = buffer.indexOf(testMessages[1]);

	// assert
	EXPECT_EQ(result, 1);
}

TEST(MessageBufferTests, IndexOfShouldReturnCorrectMinusOneIfElementIsAbsent) {
	// arrange
	auto buffer = entities::MessageBuffer<>();
	buffer.add(testMessages[0]);
	buffer.add(testMessages[1]);

	// act
	auto result = buffer.indexOf(testMessages[2]);

	// assert
	EXPECT_EQ(result, -1);
}

TEST(MessageBufferTests, IndexOfShouldReturnCorrectMinusOneIfArrayIsEmpty) {
	// arrange
	auto buffer = entities::MessageBuffer<>();

	// act
	auto result = buffer.indexOf(testMessages[2]);

	// assert
	EXPECT_EQ(result, -1);
}

TEST(MessageBufferTests, CountShouldReturnZero) {
	// arrange
	auto buffer = entities::MessageBuffer<>();

	// act
	auto result = buffer.count();

	// assert
	EXPECT_EQ(result, 0);
}

TEST(MessageBufferTests, CountShouldReturnCorrectSize) {
	// arrange
	auto buffer = entities::MessageBuffer<>();
	buffer.add(testMessages[0]);

	// act
	auto result = buffer.count();

	// assert
	EXPECT_EQ(result, 1);
}

TEST(MessageBufferTests, CopyConstructorShouldCopyBufferAndListeners) {
	// arrange
	auto buffer = entities::MessageBuffer<>();
	auto counter = 0;

	buffer.add(testMessages[0]);
	buffer.add(testMessages[1]);
	buffer.addOnAddListener([&](void*, const entities::Message&) { counter++; });

	// act
	auto result = entities::MessageBuffer<>(buffer);

	result.add(testMessages[2]);

	// assert
	EXPECT_EQ(result.count(), 3);
	EXPECT_EQ(counter, 1);
}
