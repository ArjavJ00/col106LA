#ifndef FILE_H
#define FILE_H
#include "tree.h"
#include "map.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <stack>
#include <iostream>


class File {
private:
    std::string name;
    TreeNode* root;
    TreeNode* active_version;
    HashMap<int, TreeNode*> version_map;
    int total_versions;
    time_t last_modified;
    size_t update_counter; // For RECENT_FILES tie-breaker
public:
    File(const std::string& name);
    std::string read();
    void insert(const std::string& content);
    std::string getName() const;
    void update(const std::string& content);
    void snapshot(const std::string& message);
    void rollback();
    void rollback(int version_id);
    void history();
    time_t getLastModified() const;
    int getTreeSize() const;
    int getActiveVersionID() const;
    void setUpdateCounter(size_t counter);
    size_t getUpdateCounter() const;
    void deleteTree(TreeNode* node);
    ~File();
};

#endif