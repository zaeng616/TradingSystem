#pragma once
#include <stdexcept>

class UnknownDriverException : public std::exception
{
public:
	char const* what() const override
	{
		return "No driver selected!";
	}
};

class UnknownCodeException : public std::exception
{
public:
	char const* what() const override
	{
		return "Invalid StockCode selected!";
	}
};

class InsufficientSharesException : public std::exception
{
	char const* what() const override
	{
		return "You don't have enough shares!";
	}
};

class InsufficientBalanceException : public std::exception
{
public:
	char const* what() const override
	{
		return "Insufficient Balance!";
	}
};