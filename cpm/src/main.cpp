#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main()
{
    std::cout << json::parse(R"({"happy": true, "pi": 3.141})").dump() << std::endl;
    return 0;
}
