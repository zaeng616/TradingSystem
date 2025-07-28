#pragma once
#include <iostream>
#include "gmock/gmock.h"

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