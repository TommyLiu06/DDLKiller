#pragma once

#include <string>
#include "JsonParser.h"

class JsonSender
{
public:
    // 创建 std::string 类型的全量更新消息，可以指定是否不要求回复
    static std::string createFullUpdateMessage(const std::vector<TodoItem>& items, const bool& noresponse = false);

    // 创建成功返回消息
    static std::string createSuccessMessage(const std::string& operationType = "", const std::string& uuid = "");
};
