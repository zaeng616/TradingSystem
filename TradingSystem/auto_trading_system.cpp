#pragma once
#include <iostream>
#include "driver.cpp"
#include "custom_exception.cpp"
#include "windows.h"

class AutoTradingSystem {
public:
	std::string UNKNOWN_STOCK_CODE = "Unknown";
	void selectStockBrocker(DriverInterface& driver) {
		this->driver = &driver;
	}
	bool login(std::string ID, std::string pass) {
		if (driver == nullptr) {
			throw UnknownDriverException();
		}

		return driver->login(ID, pass);
	}
	int getPrice(std::string code) {
		CheckStockCode(code);
		return driver->getPrice(code);
	}
	bool buy(std::string code, int price, int quantity) {
		CheckStockCode(code);
		if (driver->getAvailableCash() < price) {
			throw InsufficientBalanceException();
		}

		driver->buy(code, price, quantity);
		return true;
	}
	bool sell(std::string code, int price, int quantity) {
		CheckStockCode(code);
		if (quantity > getAvailableShares(code)) {
			throw InsufficientSharesException();
			return false;
		}
		driver->sell(code, price, quantity);
		return true;
	}
	int getAvailableCash() {
		return driver->getAvailableCash();
	}
	void depositCash(int cash) {
		driver->depositCash(cash);
	}
	int getAvailableShares(std::string code) {
		return driver->getAvailableShares(code);
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
	bool sellNiceTiming(std::string code, int quantity) {
		int price1 = getPrice(code);

		Sleep(200);
		int price2 = getPrice(code);

		Sleep(200);
		int price3 = getPrice(code);

		if (price1 <= price2 || price2 <= price3) return false;

		return sell(code, price3, quantity);
	}
private:
	DriverInterface* driver = nullptr;

	void CheckStockCode(std::string& code)
	{
		if (code == UNKNOWN_STOCK_CODE) {
			throw UnknownCodeException();
		}
	}
};

