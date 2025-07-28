#include "gmock/gmock.h"

class Driver {
public:
	virtual void login(std::string ID, std::string pass) = 0;
	virtual int getPrice(std::string code) = 0;
	virtual void buy(std::string stockPrice, int count, int price) = 0;
	virtual void sell(std::string code, int price, int quantity) = 0;
};

class MockDriver : public Driver {
public:
	MOCK_METHOD(void, login, (std::string ID, std::string pass), (override));
	MOCK_METHOD(int, getPrice, (std::string code), (override));
	MOCK_METHOD(void, buy, (std::string stockPrice, int count, int price), (override));
	MOCK_METHOD(void, sell, (std::string code, int price, int quantity), (override));
};

class StockerBrockerDriverInterface {
public:
	void selectStockBrocker(Driver& driver) {
		this->driver = &driver;
	}
	bool login(std::string ID, std::string pass) {
		driver->login(ID, pass);
		return true;
	}
	int getPrice(std::string code) {
		return 10000;
	}
	bool buy(std::string stockPrice, int count, int price) {
		driver->buy(stockPrice, count, price);
		return true;
	}
	bool sell(std::string code, int price, int quantity) {
		driver->sell(code, price, quantity);
		return true;;
	}
private:
	Driver* driver = nullptr;
};

class TradingFixture :public testing::Test {
public:
	MockDriver mock;
	StockerBrockerDriverInterface stockerBrocker;
	std::string id = "id1234";
	std::string password = "password56";
	std::string code = "987654";
	int price = 10000;
	int quantity = 100;

};

TEST(TradingSystemTest, AlwaysTrueTC) {
	EXPECT_EQ(1, 1);
}

TEST(TradingSystemTest, TestMockLogin) {
	MockDriver mock;
	StockerBrockerDriverInterface stockerBrocker;
	std::string id = "id1234";
	std::string password = "password56";
	std::string code = "987654";
	int price = 10000;
	int quantity = 100;

};

TEST_F(TradingFixture, TestMockLogin) {
	EXPECT_CALL(mock, login(id, password)).Times(1);
	stockerBrocker.selectStockBrocker(mock);
	bool ret = stockerBrocker.login(id, password);
	EXPECT_TRUE(ret);
}

TEST_F(TradingFixture, TestMockGetPrice) {
	EXPECT_CALL(mock, getPrice(code)).WillRepeatedly(testing::Return(price));
	stockerBrocker.selectStockBrocker(mock);
	EXPECT_TRUE(stockerBrocker.login(id, password));
	int ret = stockerBrocker.getPrice(code);
	EXPECT_EQ(ret, price);
}

TEST_F(TradingFixture, TestMockBuy) {
	EXPECT_CALL(mock, buy(code, price, quantity)).Times(1);
	stockerBrocker.selectStockBrocker(mock);
	EXPECT_TRUE(stockerBrocker.login(id, password));
	stockerBrocker.buy(code, price, quantity);
}

TEST_F(TradingFixture, TestMockSell) {
	EXPECT_CALL(mock, sell(code, price, quantity)).Times(1);
	stockerBrocker.selectStockBrocker(mock);
	EXPECT_TRUE(stockerBrocker.login(id, password));
	bool ret = stockerBrocker.sell(code, price, quantity);
	EXPECT_TRUE(ret);
}