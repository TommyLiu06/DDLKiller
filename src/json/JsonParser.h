#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include "../database/DatabaseManager.h"

struct AddOperation {
    std::string uuid;
    std::string title;
    std::string description;
    std::string dueDate;
};

struct DeleteOperation {
    std::string targetUuid;
};

struct ModifyOperation {
    std::string targetUuid;
    std::string lastModified;
    std::string title;
    std::string description;
    std::string dueDate;
    int completeFlag;
};

class JsonParser {
public:
    static std::string getMessageType(const std::string& jsonStr);

    static std::string getModificationType(const std::string& jsonStr);
    static AddOperation parseAddOperation(const std::string& jsonStr);
    static DeleteOperation parseDeleteOperation(const std::string& jsonStr);
    static ModifyOperation parseModifyOperation(const std::string& jsonStr);

    static std::vector<TodoItem> parseTodoItems(const std::string& jsonStr);
};
