#include "JsonSender.h"

std::string JsonSender::createFullUpdateMessage(const std::vector<TodoItem>& items, const bool& noresponse) {
    nlohmann::json jsonObj;
    jsonObj["type"] = noresponse ? "full_update_noresponse" : "full_update";
    jsonObj["content"] = nlohmann::json::array();

    for (const auto& item : items) {
        nlohmann::json itemJson;
        itemJson["uuid"] = item.uuid;
        itemJson["last_modified"] = item.lastModified;
        itemJson["title"] = item.title;
        itemJson["description"] = item.description;
        itemJson["due_date"] = item.dueDate;
        itemJson["complete_flag"] = item.completeFlag;
        jsonObj["content"].push_back(itemJson);
    }

    return jsonObj.dump();
}

std::string JsonSender::createSuccessMessage(const std::string& operationType, const std::string& uuid) {
    nlohmann::json jsonObj;
    jsonObj["type"] = "response";
    
    if (!operationType.empty()) {
        jsonObj["content"] = {
            {"operation", operationType},
            {"uuid", uuid},
            {"status", "success"}
        };

    } else {
        jsonObj["content"] = {
            {"status", "success"}
        };
    }

    return jsonObj.dump();
}
