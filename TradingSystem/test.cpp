#include "gmock/gmock.h"
#include "custom_exception.cpp"
#include <windows.h>

using namespace testing;

class Driver {
public:
	virtual bool login(std::string ID, std::string pass) = 0;
	virtual int getPrice(std::string code) = 0;
	virtual void buy(std::string stockPrice, int count, int price) = 0;
	virtual void sell(std::string code, int price, int quantity) = 0;
	virtual int getAvailableCash() = 0;
	virtual void depositCash(int cash) = 0;
};

class MockDriver : public Driver {
public:
	MOCK_METHOD(bool, login, (std::string ID, std::string pass), (override));
	MOCK_METHOD(int, getPrice, (std::string code), (override));
	MOCK_METHOD(void, buy, (std::string stockPrice, int count, int price), (override));
	MOCK_METHOD(void, sell, (std::string code, int price, int quantity), (override));
	MOCK_METHOD(int, getAvailableCash, (), (override));
	MOCK_METHOD(void, depositCash, (int cash), (override));
};

class StockBrockerDriverInterface {
public:
	void selectStockBrocker(Driver& driver) {
		this->driver = &driver;
	}
	bool login(std::string ID, std::string pass) {
		if (driver == nullptr) {
			throw UnknownDriverException();
		}

		return driver->login(ID, pass);
	}
	int getPrice(std::string code) {
		return driver->getPrice(code);
	}
	bool buy(std::string stockPrice, int count, int price) {
		driver->buy(stockPrice, count, price);
		return true;
	}
	bool sell(std::string code, int price, int quantity) {
		driver->sell(code, price, quantity);
		return true;
	}
	int getAvailableCash() {
		return driver->getAvailableCash();
	}
	void depositCash(int cash) {
		driver->depositCash(cash);
	}
	bool buyNiceTiming(std::string code, int totalPrice) {
		int price1 = getPrice(code);

		Sleep(200);
		int price2 = getPrice(code);

		Sleep(200);
		int price3 = getPrice(code);

		if (price2 <= price1 || price3 <= price2) return false;

		return buy(code, price3, totalPrice / price3);
	}

private:
	Driver* driver = nullptr;
};

class TradingFixture :public testing::Test {
public:
	MockDriver mock;
	StockBrockerDriverInterface stockerBrocker;
	std::string id = "id1234";
	std::string UNKNOWN = "Unknown";
	std::string password = "password56";
	std::string code = "987654";
	int price = 10000;
	int quantity = 100;
};

TEST_F(TradingFixture, TestMockLogin) {
	EXPECT_CALL(mock, login(id, password))
		.Times(1)
		.WillRepeatedly(Return(true));
	stockerBrocker.selectStockBrocker(mock);
	bool ret = stockerBrocker.login(id, password);
	EXPECT_TRUE(ret);
}

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

TEST_F(TradingFixture, TestMockGetPrice) {
	EXPECT_CALL(mock, getPrice(code)).WillRepeatedly(testing::Return(price));
	EXPECT_CALL(mock, login(id, password)).WillRepeatedly(Return(true));
	stockerBrocker.selectStockBrocker(mock);

	int ret = stockerBrocker.getPrice(code);
	EXPECT_EQ(ret, price);
}
/*
TEST_F(TradingFixture, TestMockUnknownStockCode1) {
	stockerBrocker.selectStockBrocker(mock);
	stockerBrocker.login(id, password);
	try {
		stockerBrocker.getPrice(UNKNOWN);
		FAIL();
	}
	catch (UnknownCodeException& e) {}
}

TEST_F(TradingFixture, TestMockUnknownStockCode2) {
	stockerBrocker.selectStockBrocker(mock);
	stockerBrocker.login(id, password);
	try {
		stockerBrocker.buy(UNKNOWN, price, quantity);
		FAIL();
	}
	catch (UnknownCodeException& e) {}
}

TEST_F(TradingFixture, TestMockUnknownStockCode3) {
	stockerBrocker.selectStockBrocker(mock);
	stockerBrocker.login(id, password);
	try {
		stockerBrocker.sell(UNKNOWN, price, quantity);
		FAIL();
	}
	catch (UnknownCodeException& e) {}
}
*/
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

TEST_F(TradingFixture, TestMockBuy) {
	EXPECT_CALL(mock, buy(code, price, quantity)).Times(1);
	EXPECT_CALL(mock, login(id, password)).WillRepeatedly(Return(true));
	stockerBrocker.selectStockBrocker(mock);

	stockerBrocker.buy(code, price, quantity);
}

TEST_F(TradingFixture, TestMockSell) {
	EXPECT_CALL(mock, sell(code, price, quantity)).Times(1);
	EXPECT_CALL(mock, login(id, password)).WillRepeatedly(Return(true));
	stockerBrocker.selectStockBrocker(mock);

	bool ret = stockerBrocker.sell(code, price, quantity);
	EXPECT_TRUE(ret);
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
	//stockerBrocker.login(id, password);
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
	//stockerBrocker.login(id, password);
	stockerBrocker.buyNiceTiming(code, cash);
}