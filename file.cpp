#include "file.h"
#include "tree.h"
#include "map.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <stack>

// Helper function to recursively delete all nodes in the tree


File::File(const std::string& name) : name(name) {
    root = new TreeNode(nullptr, "", 0);
    root->setMessage("Root Snapshot");
    root->setSnapshotTimestamp(time(nullptr));
    active_version = root;
    version_map.insert(0, root);
    total_versions = 1;
    last_modified = time(nullptr);
    update_counter = 0;
}
std::string File::read(){
    return active_version->getContent();
}
void File::insert(const std::string& content) {
    if (active_version->isSnapshot()) {
        std::string temp = active_version->getContent();
        if (!temp.empty()) temp += "\n";
        temp += content;
        TreeNode* new_version = new TreeNode(active_version, temp, total_versions);
        active_version->addChild(new_version);
        active_version = new_version;
        version_map.insert(total_versions, new_version);
        total_versions++;
        std::cout << "Success creating new version and inserting content in File: " << name << std::endl;
    }
    else {
        std::string temp = active_version->getContent();
        if (!temp.empty()) temp += "\n";
        temp += content;
        active_version->setContent(temp);
        std::cout << "Success Inserting into File: " << name << std::endl;        
    }
    last_modified = time(nullptr);
    // update_counter will be set from main.cpp
}
std::string File::getName() const {
    return name;
}
void File::update(const std::string& content) {
    if (active_version->isSnapshot()) {
        TreeNode* new_version = new TreeNode(active_version, content, total_versions);
        active_version->addChild(new_version);
        active_version = new_version;
        version_map.insert(total_versions, new_version);
        total_versions++;
        std::cout << "Success creating new version and updating content in File: " << name << std::endl;
    }
    else {
        active_version->setContent(content);
        std::cout << "Success updating content in File: " << name << std::endl;
    }
    last_modified = time(nullptr);
    // update_counter will be set from main.cpp
}
void File::snapshot(const std::string& message){
    if (active_version->isSnapshot()) {
        throw "Cannot create snapshot of a snapshot";
    }
    active_version->setMessage(message);
    time_t cur_time = time(nullptr);
    active_version->setSnapshotTimestamp(cur_time);
}
void File::rollback(){
    if (active_version->getParent() != nullptr) {
        active_version = active_version->getParent();
    }
    else {
        throw "No parent version to rollback to";
    }
    last_modified = time(nullptr);
    // update_counter will be set from main.cpp
}
void File::rollback(int version_id){
    if (version_id < 0) {
        throw "Invalid version ID";
    }
    if (version_id >= total_versions) {
        throw "Version ID out of range";
    }
    if (version_map.contains(version_id)) {
        active_version = version_map.get(version_id);
    }
    else {
        throw "Version ID not found";
    }
    last_modified = time(nullptr);
    // update_counter will be set from main.cpp
}
void File::setUpdateCounter(size_t counter) {
    update_counter = counter;
}

size_t File::getUpdateCounter() const {
    return update_counter;
}
void File::history(){
    std::stack<TreeNode*> history_stack;
    TreeNode* temp = active_version;
    while (temp != nullptr){
        if (temp->isSnapshot()) {
            history_stack.push(temp);
        }
        temp = temp->getParent();
    }
    while (!history_stack.empty()) {
        TreeNode* snapshot = history_stack.top();
        history_stack.pop();
        time_t t = snapshot->getSnapshotTimestamp();
        std::cout << "[Ver: " << snapshot->getVersionID() << "] " << "[Msg: " << snapshot->getMessage() << "] " << "Time: " << ctime(&t) << "";
    }
}
time_t File::getLastModified() const {
    return last_modified;
}
int File::getTreeSize() const {
    return total_versions;
}
int File::getActiveVersionID() const {
    return active_version->getVersionID();
}
void File::deleteTree(TreeNode* node) {
    if (!node) return;
    for (TreeNode* child : node->children) {
        deleteTree(child);
    }
    delete node;
}
File::~File() {
    deleteTree(root);
}
