#pragma once
#include <iostream>
#include "gmock/gmock.h"
#include "kiwer_api.cpp"
#include "nemo_api.cpp"

#define interface struct

using std::string;

interface DriverInterface{
	virtual bool login(const string& ID, const string& pass) = 0;
	virtual int getPrice(const string& code) = 0;
	virtual void buy(const string& code, int price, int quantity) = 0;
	virtual void sell(const string& code, int price, int quantity) = 0;
	virtual int getAvailableCash() = 0;
	virtual void depositCash(int cash) = 0;
	virtual int getAvailableShares(const string& code) = 0;
	std::string UNKNOWN_STOCK_CODE = "Unknown";
};

class MockDriver : public DriverInterface {
public:
	MOCK_METHOD(bool, login, (const string& ID, const string& pass), (override));
	MOCK_METHOD(int, getPrice, (const string& code), (override));
	MOCK_METHOD(void, buy, (const string& code, int price, int quantity), (override));
	MOCK_METHOD(void, sell, (const string& code, int price, int quantity), (override));
	MOCK_METHOD(int, getAvailableCash, (), (override));
	MOCK_METHOD(void, depositCash, (int cash), (override));
	MOCK_METHOD(int, getAvailableShares, (const string& code), (override));
};

class KiwerDriver : public DriverInterface {
public:
	bool login(const string& ID, const string& pass) override {
		if (ID == UNKNOWN_STOCK_CODE) return false;
		api.login(ID, pass);
		return true;
	}
	int getPrice(const string& code) override {
		return api.currentPrice(code);
	}
	void buy(const string& code, int price, int quantity) override {
		api.buy(code, quantity, price);
	}
	void sell(const string& code, int price, int quantity) override {
		api.sell(code, quantity, price);
	}
	int getAvailableCash() override {
		return budget;
	}
	void depositCash(int cash) override {
		budget += cash;
	}
	int getAvailableShares(const string& code) override {
		return 100;
	}
private:
	KiwerAPI api;
	int budget;

};

class NemoDriver : public DriverInterface {
public:
	bool login(const string& ID, const string& pass) override {
		if (ID == UNKNOWN_STOCK_CODE) return false;
		api.certification(ID, pass);
		return true;
	}
	int getPrice(const string& code) override {
		return api.getMarketPrice(code, 0);
	}
	void buy(const string& code, int price, int quantity) override {
		api.purchasingStock(code, price, quantity);
	}
	void sell(const string& code, int price, int quantity) override {
		api.sellingStock(code, price, quantity);
	}
	int getAvailableCash() override {
		return budget;
	}
	void depositCash(int cash) override {
		budget += cash;
	}
	int getAvailableShares(const string& code) override {
		return 100;
	}
private:
	NemoAPI api;
	int budget;
};