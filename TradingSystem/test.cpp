#pragma once
#include "gmock/gmock.h"
#include "custom_exception.cpp"
#include "driver.cpp"
#include "auto_trading_system.cpp"

using namespace testing;

class TradingFixture :public Test {
public:
	MockDriver mock;
	KiwerDriver kiwer;
	NemoDriver nemo;
	AutoTradingSystem stockBrocker;
	std::string id = "id1234";
	std::string UNKNOWN = stockBrocker.UNKNOWN_STOCK_CODE;
	std::string password = "password56";
	std::string code = "987654";
	int price = 10000;
	int quantity = 100;
};

TEST_F(TradingFixture, TestNotSelectDriver) {
	try {
		stockBrocker.login(id, password);
		FAIL();
	}
	catch (const UnknownDriverException& e) {}
}

TEST_F(TradingFixture, TestMockLoginFail) {
	EXPECT_CALL(mock, login(UNKNOWN, password))
		.WillRepeatedly(Return(false));

	stockBrocker.selectStockBrocker(mock);
	bool ret = stockBrocker.login(UNKNOWN, password);
	EXPECT_FALSE(ret);

	stockBrocker.selectStockBrocker(kiwer);
	ret = stockBrocker.login(UNKNOWN, password);
	EXPECT_FALSE(ret);

	stockBrocker.selectStockBrocker(nemo);
	ret = stockBrocker.login(UNKNOWN, password);
	EXPECT_FALSE(ret);
}

TEST_F(TradingFixture, TestMockLogin) {
	EXPECT_CALL(mock, login(id, password))
		.Times(1)
		.WillRepeatedly(Return(true));

	stockBrocker.selectStockBrocker(mock);
	bool ret = stockBrocker.login(id, password);
	EXPECT_TRUE(ret);

	stockBrocker.selectStockBrocker(kiwer);
	ret = stockBrocker.login(id, password);
	EXPECT_TRUE(ret);

	stockBrocker.selectStockBrocker(nemo);
	ret = stockBrocker.login(id, password);
	EXPECT_TRUE(ret);
}

TEST_F(TradingFixture, TestMockGetPrice) {
	EXPECT_CALL(mock, getPrice(code))
		.WillRepeatedly(Return(price));
	EXPECT_CALL(mock, login(id, password))
		.WillRepeatedly(Return(true));
	stockBrocker.selectStockBrocker(mock);

	int ret = stockBrocker.getPrice(code);
	EXPECT_EQ(ret, price);
}

TEST_F(TradingFixture, TestMockUnknownStockCode1) {
	stockBrocker.selectStockBrocker(mock);
	try {
		stockBrocker.getPrice(UNKNOWN);
		FAIL();
	}
	catch (UnknownCodeException& e) {}
}
TEST_F(TradingFixture, TestMockUnknownStockCode2) {
	stockBrocker.selectStockBrocker(mock);
	try {
		stockBrocker.buy(UNKNOWN, price, quantity);
		FAIL();
	}
	catch (UnknownCodeException& e) {}
}

TEST_F(TradingFixture, TestMockUnknownStockCode3) {
	stockBrocker.selectStockBrocker(mock);
	try {
		stockBrocker.sell(UNKNOWN, price, quantity);
		FAIL();
	}
	catch (UnknownCodeException& e) {}
}

TEST_F(TradingFixture, TestMockDepositCash) {
	int cash = 100000;
	
	EXPECT_CALL(mock, getAvailableCash())
		.WillOnce(Return(0))
		.WillOnce(Return(cash));
	EXPECT_CALL(mock, depositCash(cash))
		.Times(1);
	
	stockBrocker.selectStockBrocker(mock);
	EXPECT_EQ(stockBrocker.getAvailableCash(), 0);
	stockBrocker.depositCash(cash);
	EXPECT_EQ(stockBrocker.getAvailableCash(), cash);
}

TEST_F(TradingFixture, TestMockBuyFail) {
	EXPECT_CALL(mock, getAvailableCash())
		.WillOnce(Return(0));
	
	stockBrocker.selectStockBrocker(mock);
	try {
		stockBrocker.buy(code, price, quantity);
		FAIL();
	}
	catch (InsufficientBalanceException& e) {}
}

TEST_F(TradingFixture, TestMockBuySuccess) {
	int cash = 1000000;
	EXPECT_CALL(mock, getAvailableCash())
		.WillRepeatedly(Return(cash));
	EXPECT_CALL(mock, buy(code, price, quantity))
		.Times(1);
	
	stockBrocker.selectStockBrocker(mock);
	stockBrocker.buy(code, price, quantity);
}

TEST_F(TradingFixture, TestMockSellFail) {
	EXPECT_CALL(mock, sell(code, price, quantity))
		.Times(0);
	EXPECT_CALL(mock, getAvailableShares(code))
		.WillRepeatedly(Return(0));
	
	stockBrocker.selectStockBrocker(mock);

	try {
		stockBrocker.sell(code, price, quantity);
		FAIL();
	}
	catch (InsufficientSharesException& e) {}
}

TEST_F(TradingFixture, TestMockSellSuccess) {
	EXPECT_CALL(mock, sell(code, price, quantity))
		.Times(1);
	EXPECT_CALL(mock, getAvailableShares(code))
		.WillRepeatedly(Return(100));
	
	stockBrocker.selectStockBrocker(mock);
	stockBrocker.sell(code, price, quantity);
	
	int ret = stockBrocker.getAvailableShares(code);
	
	EXPECT_EQ(ret, 100);
}

TEST_F(TradingFixture, TestBuyNiceTiming) {
	int cash = 1000000;
	EXPECT_CALL(mock, getAvailableCash())
		.WillRepeatedly(Return(cash));
	EXPECT_CALL(mock, getPrice(code))
		.WillOnce(Return(9000))
		.WillOnce(Return(9500))
		.WillOnce(Return(10000));
	EXPECT_CALL(mock, buy(code, 10000, 100)).Times(1);
	
	stockBrocker.selectStockBrocker(mock);
	stockBrocker.buyNiceTiming(code, cash);
}

TEST_F(TradingFixture, TestBuyNiceTiming2) {
	int cash = 1000000;
	EXPECT_CALL(mock, getAvailableCash())
		.WillRepeatedly(Return(cash));
	EXPECT_CALL(mock, getPrice(code))
		.WillOnce(Return(10000))
		.WillOnce(Return(9500))
		.WillOnce(Return(9000));
	EXPECT_CALL(mock, buy(code, 10000, 100)).Times(0);
	
	stockBrocker.selectStockBrocker(mock);
	stockBrocker.buyNiceTiming(code, cash);
}

TEST_F(TradingFixture, TestSellNiceTiming) {
	EXPECT_CALL(mock, getAvailableShares)
		.WillRepeatedly(Return(100));
	EXPECT_CALL(mock, getPrice(code))
		.WillOnce(Return(10000))
		.WillOnce(Return(9500))
		.WillOnce(Return(9000));
	EXPECT_CALL(mock, sell(code, 9000, 100)).Times(1);
	
	stockBrocker.selectStockBrocker(mock);
	stockBrocker.sellNiceTiming(code, 100);
}

TEST_F(TradingFixture, TestSellNiceTiming2) {
	EXPECT_CALL(mock, getAvailableShares)
		.WillRepeatedly(Return(100));
	EXPECT_CALL(mock, getPrice(code))
		.WillOnce(Return(9000))
		.WillOnce(Return(9500))
		.WillOnce(Return(10000));
	EXPECT_CALL(mock, sell).Times(0);
	
stockBrocker.selectStockBrocker(mock);
	stockBrocker.sellNiceTiming(code, 100);
}
