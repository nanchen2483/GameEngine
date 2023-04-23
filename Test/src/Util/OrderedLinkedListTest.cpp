#include <gtest/gtest.h>
#include "Engine.h"
#include "Engine/Util/OrderedLinkedList.h"

namespace EngineTest
{
	struct TestElement : Engine::IComparable<TestElement>
	{
		int value;
		TestElement(int value) : value(value) {}

		// Inherited via IComparable
		virtual bool operator==(const TestElement& other) const override { return value == other.value; }
		virtual bool operator!=(const TestElement& other) const override { return value != other.value; }
		virtual bool operator<(const TestElement& other) const override { return value < other.value; }
		virtual bool operator<=(const TestElement& other) const override { return value <= other.value; }
		virtual bool operator>(const TestElement& other) const override { return value > other.value; }
		virtual bool operator>=(const TestElement& other) const override { return value >= other.value; }
	};

	struct OrderedLinkedListTest : testing::Test
	{
		Engine::OrderedLinkedList<Engine::Ptr<TestElement>> linkedList{ 10 };
		Engine::Ptr<TestElement> element1 = Engine::CreatePtr<TestElement>(1);
		Engine::Ptr<TestElement> element2 = Engine::CreatePtr<TestElement>(2);
		Engine::Ptr<TestElement> element3 = Engine::CreatePtr<TestElement>(3);
		Engine::Ptr<TestElement> element4 = Engine::CreatePtr<TestElement>(4);
		Engine::Ptr<TestElement> element5 = Engine::CreatePtr<TestElement>(5);

		OrderedLinkedListTest()
		{
			linkedList.Add(element3);
			linkedList.Add(element1);
			linkedList.Add(element5);
			linkedList.Add(element2);
			linkedList.Add(element4);
		}
	};

	TEST_F(OrderedLinkedListTest, AddElementSuccessfully)
	{
		// Arrange
		// Act
		linkedList.Add(Engine::CreatePtr<TestElement>(0));

		// Assert
		ASSERT_EQ(linkedList.GetSize(), 6);
	}

	TEST_F(OrderedLinkedListTest, SortElementCorrectly)
	{
		// Arrange
		// Act
		Engine::Ptr<TestElement> newElement1 = Engine::CreatePtr<TestElement>(1);
		linkedList.Add(newElement1);

		// Assert
		auto it = linkedList.begin();
		ASSERT_EQ(linkedList.GetSize(), 6);
		ASSERT_EQ((*it), element1);
		ASSERT_EQ((*++it), newElement1);
		ASSERT_EQ((*++it), element2);
		ASSERT_EQ((*++it), element3);
		ASSERT_EQ((*++it), element4);
		ASSERT_EQ((*++it), element5);
	}

	TEST_F(OrderedLinkedListTest, GetHeadValueReturnsFirstValueInList)
	{
		// Arrange
		Engine::Ptr<TestElement> newElement0 = Engine::CreatePtr<TestElement>(0);

		// Act
		const Engine::Ptr<TestElement>& headValue = linkedList.GetHeadValue();
		linkedList.Delete(headValue);
		const Engine::Ptr<TestElement>& newHeadValue = linkedList.GetHeadValue();
		linkedList.Add(newElement0);
		const Engine::Ptr<TestElement>& newHeadValue2 = linkedList.GetHeadValue();

		// Assert
		ASSERT_EQ(linkedList.GetSize(), 5);
		ASSERT_EQ(headValue, element1);
		ASSERT_EQ(newHeadValue, element2);
		ASSERT_EQ(newHeadValue2, newElement0);
	}

	TEST_F(OrderedLinkedListTest, GetTailValueReturnsLastValueInList)
	{
		// Arrange
		Engine::Ptr<TestElement> newElement6 = Engine::CreatePtr<TestElement>(6);

		// Act
		const Engine::Ptr<TestElement>& tailValue = linkedList.GetTailValue();
		linkedList.Delete(tailValue);
		const Engine::Ptr<TestElement>& newTailValue = linkedList.GetTailValue();
		linkedList.Add(newElement6);
		const Engine::Ptr<TestElement>& newTailValue2 = linkedList.GetTailValue();

		// Assert
		ASSERT_EQ(linkedList.GetSize(), 5);
		ASSERT_EQ(tailValue, element5);
		ASSERT_EQ(newTailValue, element4);
		ASSERT_EQ(newTailValue2, newElement6);
	}

	TEST_F(OrderedLinkedListTest, DeleteElementSuccessfully)
	{
		// Arrange
		// Act
		linkedList.Delete(element3);
		linkedList.Delete(element4);
		linkedList.Delete(element5);
		linkedList.Delete(element2);
		linkedList.Delete(element1);

		// Assert
		ASSERT_EQ(linkedList.GetSize(), 0);
		ASSERT_FALSE(linkedList.begin() != linkedList.end());
	}
}
