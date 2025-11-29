/*
 * Build:
 *   g++ TestJson.cpp JsonParser.cpp JsonSender.cpp \
 *      --std=c++17 \
 *      -o TestJson.exec
 */

#include <iostream>
#include "JsonParser.h"
#include "JsonSender.h"

int main() {
    // 创建一些 Modification 类型的 JSON 字符串进行测试
    std::string addJson = R"({
        "type": "modification",
        "content": {
            "operation": "add",
            "uuid": "20251129030444123",
            "title": "New Task",
            "description": "This is a new task.",
            "due_date": "2025-12-31"
        }
    })";

    std::string deleteJson = R"({
        "type": "modification",
        "content": {
            "operation": "delete",
            "target_uuid": "20251129030444123"
        }
    })";

    std::string modifyJson = R"({
        "type": "modification",
        "content": {
            "operation": "modify",
            "target_uuid": "20251129030444123",
            "last_modified": "20251130091530123",
            "title": "Updated Task",
            "description": "This task has been updated.",
            "due_date": "2026-01-15",
            "complete_flag": 1
        }
    })";

    // 测试 JsonParser 解析功能
    std::cout << "Testing JsonParser:\n";
    {
        std::string modType = JsonParser::getModificationType(addJson);
        AddOperation addOp = JsonParser::parseAddOperation(addJson);
        std::cout << "Add Operation:\n"
                  << "Type: " << modType << "\n"
                  << "UUID: " << addOp.uuid << "\n"
                  << "Title: " << addOp.title << "\n"
                  << "Description: " << addOp.description << "\n"
                  << "Due Date: " << addOp.dueDate << "\n\n";
    }
    {
        std::string modType = JsonParser::getModificationType(deleteJson);
        DeleteOperation delOp = JsonParser::parseDeleteOperation(deleteJson);
        std::cout << "Delete Operation:\n"
                  << "Type: " << modType << "\n"
                  << "Target UUID: " << delOp.targetUuid << "\n\n";
    }
    {
        std::string modType = JsonParser::getModificationType(modifyJson);
        ModifyOperation modOp = JsonParser::parseModifyOperation(modifyJson);
        std::cout << "Modify Operation:\n"
                  << "Type: " << modType << "\n"
                  << "Target UUID: " << modOp.targetUuid << "\n"
                  << "Last Modified: " << modOp.lastModified << "\n"
                  << "Title: " << modOp.title << "\n"
                  << "Description: " << modOp.description << "\n"
                  << "Due Date: " << modOp.dueDate << "\n"
                  << "Complete Flag: " << modOp.completeFlag << "\n\n";
    }

    // 创建一些 TodoItem 进行测试
    std::vector<TodoItem> items = {
        {"20251129030444123", "20251129030444123", "Test Todo 1", "This is a test todo item 1.", "2025-12-31", 0},
        {"20251129030444124", "20251129030444124", "Test Todo 2", "This is a test todo item 2.", "2025-11-30", 1}
    };

    // 使用 JsonSender 创建完整更新消息
    std::string jsonMsg = JsonSender::createFullUpdateMessage(items);

    // 输出生成的 JSON 消息
    std::cout << "Generated JSON Message:\n" << jsonMsg << std::endl;

    // 使用 JsonParser 解析生成的 JSON 消息
    std::vector<TodoItem> parsedItems = JsonParser::parseTodoItems(jsonMsg);

    // 输出解析得到的 TodoItem 列表
    std::cout << "\nParsed TodoItems:\n";
    for (const auto& item : parsedItems) {
        std::cout << "UUID: " << item.uuid << "\n"
                  << "Last Modified: " << item.lastModified << "\n"
                  << "Title: " << item.title << "\n"
                  << "Description: " << item.description << "\n"
                  << "Due Date: " << item.dueDate << "\n"
                  << "Complete Flag: " << item.completeFlag << "\n"
                  << "-------------------------\n";
    }

    return 0;
}
