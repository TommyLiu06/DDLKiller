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
    void moidfyTodoItem(const TodoItem& item);
    
    void updateTodoItems(const std::vector<TodoItem>& clientTodoItems);
    std::vector<TodoItem> getTodoItems();

private:
    SQLite::Database db;

    std::vector<TodoItem> getUnique(const std::vector<TodoItem>& source,
                                    const std::vector<TodoItem>& exclude);
    std::vector<TodoItem> getCommon(const std::vector<TodoItem>& clientItems,
                                    const std::vector<TodoItem>& serverItems);
    
};
