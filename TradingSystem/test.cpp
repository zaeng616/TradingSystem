#include "gmock/gmock.h"

TEST(TradingSystemTest, AlwaysTrueTC) {
	EXPECT_EQ(1, 1);
}

TEST(TradingSystemTest, TestMockLogin) {
	MockDriver mock;
	StockerBrockerDriverInterface stockerBrocker;
	std::string id = "id1234";
	std::string password = "password56";
	EXPECT_CALL(mock, login(id, password)).Times(1);
	stockerBrocker.selectStockBrocker(mock);
	bool ret = stockerBrocker.login(id, password);
	EXPECT_TRUE(ret);
}

TEST(TradingSystemTest, TestMockGetPrice) {
	MockDriver mock;
	StockerBrockerDriverInterface stockerBrocker;
	std::string id = "id1234";
	std::string password = "password56";
	int code = 987654;
	EXPECT_CALL(mock, getPrice(code)).WillRepeatedly(testing::Return(10000));
	stockerBrocker.selectStockBrocker(mock);
	EXPECT_TRUE(stockerBrocker.login(id, password));
	int ret = stockerBrocker.getPrice(code);
	EXPECT_EQ(ret, 10000);
}

