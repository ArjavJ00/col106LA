#include "tree.h"
#include <string>
#include <vector>
#include <ctime>

TreeNode::TreeNode(TreeNode* parent, const std::string& content, int version_id) {
    this->parent = parent;
    this->content = content;
    this->version_id = version_id;
    this->created_timestamp = time(nullptr);
    this->snapshot_timestamp = -1;
    //this->is_snapshot = false;
}

bool TreeNode::isSnapshot() const {
    return (snapshot_timestamp != -1);
}
void TreeNode::addChild(TreeNode* child) {
    children.push_back(child);
}
std::string TreeNode::getContent() const {
    return content;
}
void TreeNode::setContent(const std::string& content) {
    this->content = content;
}
void TreeNode::setMessage(const std::string& message) {
    this->message = message;
}
void TreeNode::setSnapshotTimestamp(time_t snapshot_timestamp) {
    this->snapshot_timestamp = snapshot_timestamp;
}
int TreeNode::getVersionID() const {
    return version_id;
}
TreeNode* TreeNode::getParent() const {
    return parent;
}
std::string TreeNode::getMessage() const {
    return message;
}
time_t TreeNode::getSnapshotTimestamp() const {
    return snapshot_timestamp;
}
