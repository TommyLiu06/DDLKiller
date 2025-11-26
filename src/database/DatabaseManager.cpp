#include "DatabaseManager.h"
#include <stdexcept>

DatabaseManager::DatabaseManager(const std::string& dbPath)
    : db(dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE),
    insert(db, "INSERT INTO modifications ("
        "user_id, "
        "operation_id, "
        "operation, "
        "target_id, "
        "title, "
        "description, "
        "due_date, "
        "delete_flag, "
        "completed_flag"
        ") " 
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)"
    ),
    select(db, "SELECT * FROM modifications WHERE user_id = ?")
{
    try {
        db.exec(
            "CREATE TABLE IF NOT EXISTS modifications ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "user_id TEXT NOT NULL, "
            "operation_id INTEGER NOT NULL, "
            "operation TEXT NOT NULL, "  // "add", "modify", "delete", "complete", "incomplete"
            "target_id INTEGER, "        // operation is "modify" or "delete"
            "title TEXT, "               // operation is "add" or "modify"
            "description TEXT, "         // operation is "add" or "modify"
            "due_date TEXT, "            // operation is "add" or "modify"
            "delete_flag INTEGER DEFAULT 0,"
            "completed_flag INTEGER DEFAULT 0"
            ");"
        );

    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to open database: ") + e.what());
    }
}

void DatabaseManager::addModification(
    const std::string& userId,
    const int& operationId,
    const std::string& operation,
    const int& targetId = 0,
    const std::string& title = "",
    const std::string& description = "",
    const std::string& dueDate = "",
    int deleteFlag = 0,
    int completedFlag = 0
) {
    insert.bind(1, userId);
    insert.bind(2, operationId);
    insert.bind(3, operation);
    insert.bind(4, targetId);
    insert.bind(5, title);
    insert.bind(6, description);
    insert.bind(7, dueDate);
    insert.bind(8, deleteFlag);
    insert.bind(9, completedFlag);
    
    insert.exec();
    insert.reset();
}

std::vector<Modification> DatabaseManager::getModificationsByUser(const std::string& userId) {
    std::vector<Modification> modifications;

    select.bind(1, userId);

    while (select.executeStep()) {
        Modification mod;
        mod.userId = select.getColumn("user_id").getString();
        mod.operationId = select.getColumn("operation_id").getInt();
        mod.operation = select.getColumn("operation").getString();
        mod.targetId = select.getColumn("target_id").isNull() ? 0 : select.getColumn("target_id").getInt();
        mod.title = select.getColumn("title").isNull() ? "" : select.getColumn("title").getString();
        mod.description = select.getColumn("description").isNull() ? "" : select.getColumn("description").getString();
        mod.dueDate = select.getColumn("due_date").isNull() ? "" : select.getColumn("due_date").getString();
        mod.deleteFlag = select.getColumn("delete_flag").getInt();
        mod.completedFlag = select.getColumn("completed_flag").getInt();

        modifications.push_back(mod);
    }

    select.reset();

    return modifications;
}
