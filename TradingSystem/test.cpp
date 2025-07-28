#include "gmock/gmock.h"

<<<<<<< HEAD
class Driver {
public:
	virtual void login(std::string ID, std::string pass) = 0;
};

class MockDriver : public Driver {
public:
	MOCK_METHOD(void, login, (std::string ID, std::string pass), (override));
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
private:
	Driver* driver = nullptr;
};

TEST(TradingSystemTest, AlwaysTrueTC) {
	EXPECT_EQ(1, 1);
}

TEST(TradingSystemTest, TestMockLogin) {
=======
class TradingFixture :public testing::Test {
public:
>>>>>>> unittest
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
