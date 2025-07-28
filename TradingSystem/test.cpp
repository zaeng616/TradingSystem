#include "gmock/gmock.h"

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


TEST(TradingSystemTest, TestMockBuy) {
	MockDriver mock;
	StockerBrockerDriverInterface stockerBrocker;
	std::string id = "id1234";
	std::string password = "password56";
	int code = 987654;
	int price = 10000;
	int quantity = 100;
	EXPECT_CALL(mock, buy(code, price, quantity)).Times(1);
	stockerBrocker.selectStockBrocker(mock);
	EXPECT_TRUE(stockerBrocker.login(id, password));
	stockerBrocker.buy(code, price, quantity);
}
