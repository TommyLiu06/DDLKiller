/**
 * @file DatabaseManager.cpp
 * @brief Implementation of the DatabaseManager class.
 *
 * Build：
 *   g++ DatabaseManager.cpp TestDatabase.cpp \
 *       -lSQLiteCpp -lsqlite3 -lpthread -std=c++17 \
 *       -o TestDatabase
 */


#include "DatabaseManager.h"
#include <stdexcept>

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
