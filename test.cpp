#include <iostream>
#include <vector>
#include <map>
#include <variant>
#include <string>


// JSON Value 类型
using JsonValue = std::variant<std::nullptr_t, bool, int, double, std::string, std::vector<JsonValue>, std::map<std::string, JsonValue>>;

class Json {
public:
    Json() = default;

    // 解析 JSON 字符串
    static Json parse(const std::string& jsonString);

    // 生成 JSON 字符串
    std::string stringify() const;

    // 获取 JSON 值
    JsonValue getValue() const {
        return value;
    }

private:
    // 私有构造函数，通过 parse() 静态方法创建对象
    Json(JsonValue val) : value(val) {}

    // 解析 JSON 值
    static Json parseValue(const std::string& jsonString, size_t& index);

    // 解析字符串
    static std::string parseString(const std::string& jsonString, size_t& index);

    // 解析数组
    static std::vector<JsonValue> parseArray(const std::string& jsonString, size_t& index);

    // 解析对象
    static std::map<std::string, JsonValue> parseObject(const std::string& jsonString, size_t& index);

    // 生成 JSON 字符串
    static std::string stringifyValue(const JsonValue& value);

private:
    JsonValue value;
};

// 解析 JSON 字符串
Json Json::parse(const std::string& jsonString) {
    size_t index = 0;
    return Json(parseValue(jsonString, index));
}

// 生成 JSON 字符串
std::string Json::stringify() const {
    return stringifyValue(value);
}

// 解析 JSON 值
JsonValue Json::parseValue(const std::string& jsonString, size_t& index) {
    char currentChar = jsonString[index];

    if (currentChar == '{') {
        // 解析对象
        return parseObject(jsonString, index);
    } else if (currentChar == '[') {
        // 解析数组
        return parseArray(jsonString, index);
    } else if (currentChar == '"') {
        // 解析字符串
        return parseString(jsonString, index);
    } else if (currentChar == 't' || currentChar == 'f') {
        // 解析布尔值
        return jsonString.substr(index, 4) == "true" ? true : false;
    } else if (currentChar == 'n') {
        // 解析 null
        return nullptr;
    } else {
        // 解析数字
        size_t endIndex;
        try {
            std::stod(jsonString.substr(index), &endIndex);
        } catch (...) {
            throw std::runtime_error("Failed to parse number.");
        }
        index += endIndex - 1; // 移动索引
        return std::stod(jsonString.substr(index));
    }
}

// 解析字符串
std::string Json::parseString(const std::string& jsonString, size_t& index) {
    index++; // 跳过初始的双引号
    size_t endIndex = jsonString.find('"', index);
    if (endIndex == std::string::npos) {
        throw std::runtime_error("Failed to parse string.");
    }
    std::string result = jsonString.substr(index, endIndex - index);
    index = endIndex + 1; // 移动索引到字符串结束的双引号
    return result;
}

// 解析数组
std::vector<JsonValue> Json::parseArray(const std::string& jsonString, size_t& index) {
    index++; // 跳过初始的左方括号
    std::vector<JsonValue> result;

    while (jsonString[index] != ']') {
        result.push_back(parseValue(jsonString, index));
        index++;
        if (jsonString[index] == ',') {
            index++;
        }
    }

    index++; // 跳过结束的右方括号
    return result;
}

// 解析对象
std::map<std::string, JsonValue> Json::parseObject(const std::string& jsonString, size_t& index) {
    index++; // 跳过初始的左花括号
    std::map<std::string, JsonValue> result;

    while (jsonString[index] != '}') {
        // 解析键
        std::string key = parseString(jsonString, index);

        // 跳过冒号
        index++;

        // 解析值
        result[key] = parseValue(jsonString, index);

        index++;
        if (jsonString[index] == ',') {
            index++;
        }
    }

    index++; // 跳过结束的右花括号
    return result;
}

// 生成 JSON 字符串
std::string Json::stringifyValue(const JsonValue& value) {
    if (std::holds_alternative<std::nullptr_t>(value)) {
        return "null";
    } else if (std::holds_alternative<bool>(value)) {
        return std::get<bool>(value) ? "true" : "false";
    } else if (std::holds_alternative<int>(value)) {
        return std::to_string(std::get<int>(value));
    } else if (std::holds_alternative<double>(value)) {
        return std::to_string(std::get<double>(value));
    } else if (std::holds_alternative<std::string>(value)) {
        return '"' + std::get<std::string>(value) + '"';
    } else if (std::holds_alternative<std::vector<JsonValue>>(value)) {
        const auto& arr = std::get<std::vector<JsonValue>>(value);
        std::string result = "[";
        for (size_t i = 0; i < arr.size(); ++i) {
            result += stringifyValue(arr[i]);
            if (i != arr.size() - 1) {
                result += ",";
            }
        }
        result += "]";
        return result;
    } else if (std::holds_alternative<std::map<std::string, JsonValue>>(value)) {
        const auto& obj = std::get<std::map<std::string, JsonValue>>(value);
        std::string result = "{";
        size_t i = 0;
        for (const auto& [key, val] : obj) {
            result += '"' + key + "\":" + stringifyValue(val);
            if (++i != obj.size()) {
                result += ",";
            }
        }
        result += "}";
        return result;
    }

    return ""; // 未知类型
}

int main() {
    std::string jsonString = R"(
        {
            "name": "John",
            "age": 25,
            "isStudent": true,
            "grades": [95, 80, 90],
            "address": {
                "city": "New York",
                "zip": "10001"
            },
            "isSpecial": null
        }
    )";

    try {
        Json json = Json::parse(jsonString);
        std::cout << "Parsed JSON:\n" << json.stringify() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
