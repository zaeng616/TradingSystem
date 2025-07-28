#pragma once
#include <iostream>
#include "gmock/gmock.h"

#define interface struct

interface DriverInterface{
	virtual bool login(std::string ID, std::string pass) = 0;
	virtual int getPrice(std::string code) = 0;
	virtual void buy(std::string code, int price, int count) = 0;
	virtual void sell(std::string code, int price, int quantity) = 0;
	virtual int getAvailableCash() = 0;
	virtual void depositCash(int cash) = 0;
	virtual int getAvailableShares(std::string code) = 0;
};

class MockDriver : public DriverInterface {
public:
	MOCK_METHOD(bool, login, (std::string ID, std::string pass), (override));
	MOCK_METHOD(int, getPrice, (std::string code), (override));
	MOCK_METHOD(void, buy, (std::string code, int price, int count), (override));
	MOCK_METHOD(void, sell, (std::string code, int price, int quantity), (override));
	MOCK_METHOD(int, getAvailableCash, (), (override));
	MOCK_METHOD(void, depositCash, (int cash), (override));
	MOCK_METHOD(int, getAvailableShares, (std::string code), (override));
};