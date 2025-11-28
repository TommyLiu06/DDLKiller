#include "DatabaseManager.h"
#include <stdexcept>

DatabaseManager::DatabaseManager(const std::string& dbPath)
    : db(dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE),
    insert(db, "INSERT INTO modifications ("
        "uuid, "
        "last_modified, "
        "title, "
        "description, "
        "due_date, "
        "complete_flag"
        ") " 
        "VALUES (?, ?, ?, ?, ?, ?)"
    ),
    select(db, "SELECT * FROM modifications")
{
    try {
        db.exec(
            "CREATE TABLE IF NOT EXISTS modifications ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "uuid INTEGER, "           // timestamp of creation  
            "last_modified INTEGER, "  // timestamp of last modification
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

// void DatabaseManager::addModification(
//     const std::string& operation,
//     const int& targetId = 0,
//     const std::string& title = "",
//     const std::string& description = "",
//     const std::string& dueDate = "",
//     const int& deleteFlag = 0,
//     const int& completeFlag = 0
// ) {
//     insert.bind(1, operation);
//     insert.bind(2, targetId);
//     insert.bind(3, title);
//     insert.bind(4, description);
//     insert.bind(5, dueDate);
//     insert.bind(6, deleteFlag);
//     insert.bind(7, completeFlag);
    
//     insert.exec();
//     insert.reset();
// }

// std::vector<Modification> DatabaseManager::getModificationsAfterId(const int& lastProcessedId) {
//     std::vector<Modification> modifications;

//     select.bind(1, lastProcessedId);

//     while (select.executeStep()) {
//         Modification mod;
//         mod.operation = select.getColumn("operation").getString();
//         mod.targetId = select.getColumn("target_id").isNull() ? 0 : select.getColumn("target_id").getInt();
//         mod.title = select.getColumn("title").isNull() ? "" : select.getColumn("title").getString();
//         mod.description = select.getColumn("description").isNull() ? "" : select.getColumn("description").getString();
//         mod.dueDate = select.getColumn("due_date").isNull() ? "" : select.getColumn("due_date").getString();
//         mod.deleteFlag = select.getColumn("delete_flag").isNull() ? 0 : select.getColumn("delete_flag").getInt();
//         mod.completeFlag = select.getColumn("complete_flag").isNull() ? 0 : select.getColumn("complete_flag").getInt();

//         modifications.push_back(mod);
//     }
//     select.reset();

//     return modifications;
// }
