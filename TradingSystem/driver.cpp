#include <string>
#include "custom_exception.cpp"

class Driver {
public:
	virtual bool login(std::string ID, std::string pass) = 0;
	virtual int getPrice(std::string code) = 0;
};

class AutoTradingSystem {
public:
	void selectStockBrocker(Driver& driver) {
		this->driver = &driver;
	}
	bool login(std::string ID, std::string pass) {
		if (driver == nullptr) {
			throw UnknownDriverException();
		}

		driver->login(ID, pass);
		return true;
	}
	int getPrice(std::string code) {
		return 10000;
	}
private:
	Driver* driver = nullptr;
};