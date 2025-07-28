#include "gmock/gmock.h"

using namespace testing;

class Driver {
public:
	virtual void login(std::string ID, std::string pass) = 0;
<<<<<<< HEAD
	virtual void buy(std::string stockPrice, int count, int price) = 0;

=======
 	virtual int getPrice(int code) = 0;
>>>>>>> master
};

class MockDriver : public Driver {
public:
	MOCK_METHOD(void, login, (std::string ID, std::string pass), (override));
	MOCK_METHOD(void, buy, (std::string stockPrice, int count, int price), (override));
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
	bool buy(std::string stockPrice, int count, int price) {
		driver->buy(stockPrice, count, price);
		return true;
	}
private:
	Driver* driver = nullptr;
};

<<<<<<< HEAD
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

TEST_F(TradingFixture, TestMockLogin) {
	EXPECT_CALL(mock, login(id, password)).Times(1);
	stockerBrocker.selectStockBrocker(mock);
	bool ret = stockerBrocker.login(id, password);
	EXPECT_TRUE(ret);
}

//TEST_F(TradingFixture, TestMockGetPrice) {
//	EXPECT_CALL(mock, getPrice(code)).WillRepeatedly(testing::Return(price));
//	stockerBrocker.selectStockBrocker(mock);
//	EXPECT_TRUE(stockerBrocker.login(id, password));
//	int ret = stockerBrocker.getPrice(code);
//	EXPECT_EQ(ret, price);
//}

TEST_F(TradingFixture, TestMockBuy) {
	EXPECT_CALL(mock, buy(code, price, quantity)).Times(1);
	stockerBrocker.selectStockBrocker(mock);
	EXPECT_TRUE(stockerBrocker.login(id, password));
	stockerBrocker.buy(code, price, quantity);
}

//TEST_F(TradingFixture, TestMockSell) {
//	EXPECT_CALL(mock, sell(code, price, quantity)).Times(1);
//	stockerBrocker.selectStockBrocker(mock);
//	EXPECT_TRUE(stockerBrocker.login(id, password));
//	bool ret = stockerBrocker.sell(code, price, quantity);
//	EXPECT_TRUE(ret);
//}
=======
TEST(TradingSystemTest, AlwaysTrueTC) {
	EXPECT_EQ(1, 1);
}

class MockDriver : public Driver {
public:
	MOCK_METHOD(void, login, (std::string ID, std::string pass), (override));
	MOCK_METHOD(int, getPrice, (int code), (override));
};

class StockBrockerDriverInterface {
public:
	void selectStockBrocker(Driver& driver) {
		this->driver = &driver;
	}
	bool login(std::string ID, std::string pass) {
		driver->login(ID, pass);
		return true;
	}
	int getPrice(int code) {
		return 10000;
	}
private:
	Driver* driver = nullptr;
};

class TradingSystemFixture : public Test {
public:
	MockDriver mock;
	StockBrockerDriverInterface stockBrocker;
	std::string id = "id1234";
	std::string password = "password56";
	int code = 987654;
};

TEST_F(TradingSystemFixture, AlwaysTrueTC) {
	EXPECT_EQ(1, 1);
}

TEST_F(TradingSystemFixture, TestMockLogin) {
	EXPECT_CALL(mock, login(id, password)).Times(1);
	stockBrocker.selectStockBrocker(mock);
	bool ret = stockBrocker.login(id, password);
	EXPECT_TRUE(ret);
}

TEST_F(TradingSystemFixture, TestMockGetPrice) {
	EXPECT_CALL(mock, getPrice(code)).WillRepeatedly(testing::Return(10000));
	stockBrocker.selectStockBrocker(mock);
	EXPECT_TRUE(stockBrocker.login(id, password));
	int ret = stockBrocker.getPrice(code);
	EXPECT_EQ(ret, 10000);
}
>>>>>>> master
