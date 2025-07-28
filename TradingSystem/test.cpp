#pragma once
#include "gmock/gmock.h"
#include "custom_exception.cpp"
#include "driver.cpp"
#include "auto_trading_system.cpp"

using namespace testing;

class TradingFixture :public testing::Test {
public:
	MockDriver mock;
	AutoTradingSystem stockerBrocker;
	std::string id = "id1234";
	std::string UNKNOWN = stockerBrocker.UNKNOWN_STOCK_CODE;
	std::string password = "password56";
	std::string code = "987654";
	int price = 10000;
	int quantity = 100;
};

TEST_F(TradingFixture, TestNotSelectDriver) {
	try {
		stockerBrocker.login(id, password);
		FAIL();
	}
	catch (const UnknownDriverException& e) {}
}

TEST_F(TradingFixture, TestMockLoginFail) {
	EXPECT_CALL(mock, login(UNKNOWN, password)).WillRepeatedly(testing::Return(false));
	stockerBrocker.selectStockBrocker(mock);
	bool ret = stockerBrocker.login(UNKNOWN, password);
	EXPECT_FALSE(ret);
}

TEST_F(TradingFixture, TestMockLogin) {
	EXPECT_CALL(mock, login(id, password))
		.Times(1)
		.WillRepeatedly(Return(true));
	stockerBrocker.selectStockBrocker(mock);
	bool ret = stockerBrocker.login(id, password);
	EXPECT_TRUE(ret);
}

TEST_F(TradingFixture, TestMockGetPrice) {
	EXPECT_CALL(mock, getPrice(code)).WillRepeatedly(testing::Return(price));
	EXPECT_CALL(mock, login(id, password)).WillRepeatedly(Return(true));
	stockerBrocker.selectStockBrocker(mock);

	int ret = stockerBrocker.getPrice(code);
	EXPECT_EQ(ret, price);
}

TEST_F(TradingFixture, TestMockUnknownStockCode1) {
	stockerBrocker.selectStockBrocker(mock);
	try {
		stockerBrocker.getPrice(UNKNOWN);
		FAIL();
	}
	catch (UnknownCodeException& e) {}
}
TEST_F(TradingFixture, TestMockUnknownStockCode2) {
	stockerBrocker.selectStockBrocker(mock);
	try {
		stockerBrocker.buy(UNKNOWN, price, quantity);
		FAIL();
	}
	catch (UnknownCodeException& e) {}
}

TEST_F(TradingFixture, TestMockUnknownStockCode3) {
	stockerBrocker.selectStockBrocker(mock);
	try {
		stockerBrocker.sell(UNKNOWN, price, quantity);
		FAIL();
	}
	catch (UnknownCodeException& e) {}
}

TEST_F(TradingFixture, TestMockDepositCash) {
	int cash = 100000;
	EXPECT_CALL(mock, getAvailableCash())
		.WillOnce(testing::Return(0))
		.WillOnce(testing::Return(cash));
	EXPECT_CALL(mock, depositCash(cash)).Times(1);
	stockerBrocker.selectStockBrocker(mock);

	EXPECT_EQ(stockerBrocker.getAvailableCash(), 0);
	stockerBrocker.depositCash(cash);
	EXPECT_EQ(stockerBrocker.getAvailableCash(), cash);
}

TEST_F(TradingFixture, TestMockBuyFail) {
	EXPECT_CALL(mock, getAvailableCash())
		.WillOnce(testing::Return(0));
	stockerBrocker.selectStockBrocker(mock);
	try {
		stockerBrocker.buy(code, price, quantity);
		FAIL();
	}
	catch (InsufficientBalanceException& e) {}
}

TEST_F(TradingFixture, TestMockBuySuccess) {
	int cash = 1000000;
	EXPECT_CALL(mock, getAvailableCash())
		.WillRepeatedly(testing::Return(cash));
	EXPECT_CALL(mock, buy(code, price, quantity)).Times(1);
	stockerBrocker.selectStockBrocker(mock);
	stockerBrocker.buy(code, price, quantity);
}

TEST_F(TradingFixture, TestMockSellFail) {
	EXPECT_CALL(mock, sell(code, price, quantity)).Times(0);
	EXPECT_CALL(mock, getAvailableShares(code)).WillRepeatedly(testing::Return(0));
	EXPECT_CALL(mock, login(id, password)).WillRepeatedly(Return(true));
	stockerBrocker.selectStockBrocker(mock);

	EXPECT_TRUE(stockerBrocker.login(id, password));
	try {
		stockerBrocker.sell(code, price, quantity);
		FAIL();
	}
	catch (InsufficientSharesException& e) {}
}

TEST_F(TradingFixture, TestMockSellSuccess) {
	EXPECT_CALL(mock, sell(code, price, quantity)).Times(1);
	EXPECT_CALL(mock, getAvailableShares(code)).WillRepeatedly(testing::Return(100));
	EXPECT_CALL(mock, login(id, password)).WillRepeatedly(Return(true));
	stockerBrocker.selectStockBrocker(mock);
	EXPECT_TRUE(stockerBrocker.login(id, password));
	stockerBrocker.sell(code, price, quantity);
	int ret = stockerBrocker.getAvailableShares(code);
	EXPECT_EQ(ret, 100);
}

TEST_F(TradingFixture, TestBuyNiceTiming) {
	int cash = 1000000;
	EXPECT_CALL(mock, getAvailableCash())
		.WillRepeatedly(testing::Return(cash));
	EXPECT_CALL(mock, getPrice(code))
		.WillOnce(testing::Return(9000))
		.WillOnce(testing::Return(9500))
		.WillOnce(testing::Return(10000));
	EXPECT_CALL(mock, buy(code, 10000, 100)).Times(1);
	stockerBrocker.selectStockBrocker(mock);
	stockerBrocker.buyNiceTiming(code, cash);
}

TEST_F(TradingFixture, TestBuyNiceTiming2) {
	int cash = 1000000;
	EXPECT_CALL(mock, getAvailableCash())
		.WillRepeatedly(testing::Return(cash));
	EXPECT_CALL(mock, getPrice(code))
		.WillOnce(testing::Return(10000))
		.WillOnce(testing::Return(9500))
		.WillOnce(testing::Return(9000));
	EXPECT_CALL(mock, buy(code, 10000, 100)).Times(0);
	stockerBrocker.selectStockBrocker(mock);
	stockerBrocker.buyNiceTiming(code, cash);
}

TEST_F(TradingFixture, TestSellNiceTiming) {
	EXPECT_CALL(mock, getAvailableShares)
		.WillRepeatedly(testing::Return(100));
	EXPECT_CALL(mock, getPrice(code))
		.WillOnce(testing::Return(10000))
		.WillOnce(testing::Return(9500))
		.WillOnce(testing::Return(9000));
	EXPECT_CALL(mock, sell(code, 9000, 100)).Times(1);
	stockerBrocker.selectStockBrocker(mock);
	stockerBrocker.sellNiceTiming(code, 100);
}

TEST_F(TradingFixture, TestSellNiceTiming2) {
	EXPECT_CALL(mock, getAvailableShares)
		.WillRepeatedly(testing::Return(100));
	EXPECT_CALL(mock, getPrice(code))
		.WillOnce(testing::Return(9000))
		.WillOnce(testing::Return(9500))
		.WillOnce(testing::Return(10000));
	EXPECT_CALL(mock, sell).Times(0);
	stockerBrocker.selectStockBrocker(mock);
	stockerBrocker.sellNiceTiming(code, 100);
}
