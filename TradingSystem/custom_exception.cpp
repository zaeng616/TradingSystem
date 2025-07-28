#include <stdexcept>

class UnknownDriverException : public std::exception
{
public:
	char const* what() const override
	{
		return "No driver selected!";
	}
};