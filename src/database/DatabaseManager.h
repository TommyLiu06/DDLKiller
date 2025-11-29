#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include <vector>

struct TodoItem {
    std::string uuid;
    std::string lastModified;
    std::string title;
    std::string description;
    std::string dueDate;
    int completeFlag;
};

class DatabaseManager {
public:
    DatabaseManager(const char* dbPath);

    // SQLite::Database 不可拷贝
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    void addTodoItem(const TodoItem& item);
    void deleteTodoItem(const std::string& uuid);
    std::vector<TodoItem> getTodoItems();

private:
    SQLite::Database db;
    // SQLite::Statement insertStmt;
    // SQLite::Statement selectStmt;
    // SQLite::Statement deleteStmt;

};
