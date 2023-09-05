#include <iostream>
#include <map>

struct Compare {
    int extra_param;

    Compare(int param) : extra_param(param) {}

    bool operator()(const int& a, const int& b) const {
        return a + extra_param > b + extra_param;
    }
};

int main() {
    std::map<int, std::string, Compare> myMap(10); // 初始化 Compare 对象并传递额外的参数
    myMap[3] = "apple";
    myMap[1] = "banana";
    myMap[2] = "carrot";

    // 输出排序后的 map
    for (const auto& pair : myMap) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return 0;
}

