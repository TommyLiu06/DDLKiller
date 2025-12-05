#include "DatabaseManager.h"
#include <stdexcept>
#include <unordered_set>

DatabaseManager::DatabaseManager(const char* dbPath)
    // 打开数据库文件，若不存在就创建
    : db(dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE)
{
    try {
        // 创建 item 表（若不存在）
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

void DatabaseManager::updateTodoItems(const std::vector<TodoItem>& clientTodoItems)
{
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

    // 找出双方都有的项，在服务器上保存修改时间较新的版本
    std::vector<TodoItem> clientCommonItems = getCommon(clientTodoItems, serverTodoItems);
    std::vector<TodoItem> serverCommonItems = getCommon(serverTodoItems, clientTodoItems);

    for (size_t i = 0; i < clientCommonItems.size(); ++i) {
        const TodoItem& clientItem = clientCommonItems[i];
        const TodoItem& serverItem = serverCommonItems[i];
        if (clientItem.lastModified > serverItem.lastModified) {
            moidfyTodoItem(clientItem);
        }
    }
}

std::vector<TodoItem> DatabaseManager::getUnique(const std::vector<TodoItem>& source,
                                                    const std::vector<TodoItem>& exclude)
{
    std::vector<TodoItem> uniqueItems;

    // 将 exclude 中所有项的 uuid 添加到 unordered_set (unordered_set 查找的时间复杂度为 O(1))
    std::unordered_set<std::string> excludeSet;
    for (const auto& item : exclude)
        excludeSet.insert(item.uuid);

    // 对于 source 中的每一项, 如果没有在 exclude 中找到相同的 uuid, 则这一项是 source 独有的
    for (const auto& item : source) {
        if (excludeSet.find(item.uuid) == excludeSet.end()) {  // 没有找到时返回 excludeSet.end()
            uniqueItems.push_back(item); // only in source
        }
    }

    return uniqueItems;
}

std::vector<TodoItem> DatabaseManager::getCommon(const std::vector<TodoItem>& aItems,
                                                    const std::vector<TodoItem>& bItems)
{
    std::vector<TodoItem> commonItems;

    // 将 bItems 中所有项的 uuid 添加到 unordered_set (unordered_set 查找的时间复杂度为 O(1))
    std::unordered_set<std::string> bSet;
    for (const auto& item : bItems)
        bSet.insert(item.uuid);

    // 对于 aItem 中的每一项, 如果在 bItem 中找到相同的 uuid, 则这一项是共有的
    for (const auto& item : aItems) {
        if (bSet.find(item.uuid) != bSet.end()) {  // 找到时会返回位置, 所以用 != bSet.end() 判断
            commonItems.push_back(item); // in both a and b
        }
    }

    return commonItems;
}

void DatabaseManager::addTodoItem(const TodoItem& item)
{
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

    try {
        insertStmt.exec();
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to add todo item: ") + e.what());
    }

    insertStmt.reset();
}

void DatabaseManager::deleteTodoItem(const std::string& uuid)
{
    SQLite::Statement deleteStmt(db, 
        "DELETE FROM items WHERE uuid = ?"
    );

    deleteStmt.bind(1, uuid);

    try {
        deleteStmt.exec();
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to delete todo item: ") + e.what());
    }

    deleteStmt.reset();
}

void DatabaseManager::moidfyTodoItem(const TodoItem& item)
{
    SQLite::Statement modifyStmt(db, 
        "UPDATE items SET last_modified = ?, title = ?, description = ?, due_date = ?, complete_flag = ? "
        "WHERE uuid = ?"
    );

    modifyStmt.bind(1, item.lastModified);
    modifyStmt.bind(2, item.title);
    modifyStmt.bind(3, item.description);
    modifyStmt.bind(4, item.dueDate);
    modifyStmt.bind(5, item.completeFlag);
    modifyStmt.bind(6, item.uuid);

    try {
        modifyStmt.exec();
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to modify todo item: ") + e.what());
    }

    modifyStmt.reset();
}

std::vector<TodoItem> DatabaseManager::getTodoItems()
{
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
