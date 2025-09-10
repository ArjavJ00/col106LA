#ifndef TREE_H
#define TREE_H
#include <string>
#include <vector>
#include <ctime>

class TreeNode {
    int version_id;
    std::string content;
    std::string message;
    time_t created_timestamp;
    time_t snapshot_timestamp;
    TreeNode* parent;
public:
    std::vector<TreeNode*> children;
    TreeNode(TreeNode* parent, const std::string& content, int version_id);
    bool isSnapshot() const;
    void addChild(TreeNode* child);
    std::string getContent() const;
    void setContent(const std::string& content);
    void setMessage(const std::string& message);
    void setSnapshotTimestamp(time_t snapshot_timestamp);
    int getVersionID() const;
    TreeNode* getParent() const;
    std::string getMessage() const;
    time_t getSnapshotTimestamp() const;
};

#endif