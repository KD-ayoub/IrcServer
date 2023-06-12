#include <iostream>
#include <string>
#include <vector>
#include <sstream>

std::vector<std::string> splitString(const std::string& input, char delimiter)
{
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    std::string token;

    while (std::getline(iss, token, delimiter))
    {
        if (!token.empty())
            tokens.push_back(token);
    }

    return tokens;
}

int main()
{
    std::string input = ",,,,,,,,,,,Hello,World,,,,,,,,,,,,,,Split,,,,,,,,Example,";
    char delimiter = ',';

    std::vector<std::string> tokens = splitString(input, delimiter);

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        std::cout << tokens[i] << std::endl;
    }

    return 0;
}
