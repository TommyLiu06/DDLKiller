/**
 * Build：
 *   g++ DatabaseManager.cpp TestDatabase.cpp \
 *       -lSQLiteCpp -lsqlite3 -lpthread -std=c++17 \
 *       -o TestDatabase.exec
 */

#include "DatabaseManager.h"
#include <stdexcept>
#include <unordered_set>

DatabaseManager::DatabaseManager(const char* dbPath)
    : db(dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE)
{
    try {
        db.exec(
            "CREATE TABLE IF NOT EXISTS items ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "uuid TEXT, "              // timestamp of creation  
            "last_modified TEXT, "     // timestamp of last modification
            "title TEXT, "             // operation is "add" or "modify"
            "description TEXT, "       // operation is "add" or "modify"
            "due_date TEXT, "          // operation is "add" or "modify"
            "complete_flag INTEGER DEFAULT 0"
            ");"
        );

    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to open database: ") + e.what());
    }
}

void DatabaseManager::updateTodoItems(const std::vector<TodoItem>& clientTodoItems) {
    std::vector<TodoItem> serverTodoItems = getTodoItems();

    // 找出客户端有但服务器没有的项，添加到服务器
    std::vector<TodoItem> itemsToAdd = getUnique(clientTodoItems, serverTodoItems);
    for (const auto& item : itemsToAdd) {
        addTodoItem(item);
    }

    // 找出服务器有但客户端没有的项，从服务器删除
    std::vector<TodoItem> itemsToDelete = getUnique(serverTodoItems, clientTodoItems);
    for (const auto& item : itemsToDelete) {
        deleteTodoItem(item.uuid);
    }
}

std::vector<TodoItem> DatabaseManager::getUnique(const std::vector<TodoItem>& source,
                                                    const std::vector<TodoItem>& exclude)
{
    std::vector<TodoItem> uniqueItems;

    std::unordered_set<std::string> excludeSet;
    for (const auto& item : exclude)
        excludeSet.insert(item.uuid);

    for (const auto& item : source) {
        if (excludeSet.find(item.uuid) == excludeSet.end()) {
            uniqueItems.push_back(item); // only in A
        }
    }

    return uniqueItems;
}

void DatabaseManager::addTodoItem(const TodoItem& item) {
    SQLite::Statement insertStmt(db, 
        "INSERT INTO items (uuid, last_modified, title, description, due_date, complete_flag) "
        "VALUES (?, ?, ?, ?, ?, ?)"
    );

    insertStmt.bind(1, item.uuid);
    insertStmt.bind(2, item.lastModified);
    insertStmt.bind(3, item.title);
    insertStmt.bind(4, item.description);
    insertStmt.bind(5, item.dueDate);
    insertStmt.bind(6, item.completeFlag);

    if (insertStmt.exec() != 1) {
        throw std::runtime_error("Failed to add todo item");
    }
    insertStmt.reset();
}

void DatabaseManager::deleteTodoItem(const std::string& uuid) {
    SQLite::Statement deleteStmt(db, 
        "DELETE FROM items WHERE uuid = ?"
    );

    deleteStmt.bind(1, uuid);

    if (deleteStmt.exec() != 1) {
        throw std::runtime_error("Failed to delete todo item");
    }
    deleteStmt.reset();
}

void DatabaseManager::moidfyTodoItem(const TodoItem& item) {
    SQLite::Statement updateStmt(db, 
        "UPDATE items SET last_modified = ?, title = ?, description = ?, due_date = ?, complete_flag = ? "
        "WHERE uuid = ?"
    );

    updateStmt.bind(1, item.lastModified);
    updateStmt.bind(2, item.title);
    updateStmt.bind(3, item.description);
    updateStmt.bind(4, item.dueDate);
    updateStmt.bind(5, item.completeFlag);
    updateStmt.bind(6, item.uuid);

    if (updateStmt.exec() != 1) {
        throw std::runtime_error("Failed to modify todo item");
    }
    updateStmt.reset();
}

std::vector<TodoItem> DatabaseManager::getTodoItems() {
    SQLite::Statement selectStmt(db, 
        "SELECT uuid, last_modified, title, description, due_date, complete_flag FROM items"
    );
    
    std::vector<TodoItem> todoItems;

    while (selectStmt.executeStep()) {
        TodoItem item;
        item.uuid = selectStmt.getColumn("uuid").getString();
        item.lastModified = selectStmt.getColumn("last_modified").getString();
        item.title = selectStmt.getColumn("title").getString();
        item.description = selectStmt.getColumn("description").getString();
        item.dueDate = selectStmt.getColumn("due_date").getString();
        item.completeFlag = selectStmt.getColumn("complete_flag").getInt();

        todoItems.push_back(item);
    }
    selectStmt.reset();

    return todoItems;
}
