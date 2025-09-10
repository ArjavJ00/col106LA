#include "tree.h"
#include "file.h"
#include "map.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include <cctype>
#include <cassert>
#include "heap.h"

HashMap<File*, int> fileTorecentFilesIdx;
HashMap<File*, int> fileToBiggestTreeIdx;
// Use pair<time_t, size_t> for RECENT_FILES priority
#include <utility>
Heap<std::pair<time_t, size_t>, File*> recentFilesHeap(fileTorecentFilesIdx);
Heap<int, File*> BiggestTreesHeap(fileToBiggestTreeIdx);

size_t global_update_counter = 0; // For RECENT_FILES tie-breaker
bool isInteger(const std::string& s) {
    if (s.empty()) {
        return false; 
    }
    for (char c : s) {
        if (!std::isdigit(c)) {
            return false; 
        }
    }
    return true;
}
void updateRecentFilesHeap(File* file) {
    // Set/update the file's update_counter
    file->setUpdateCounter(++global_update_counter);
    std::pair<time_t, size_t> priority = {file->getLastModified(), file->getUpdateCounter()};
    if (fileTorecentFilesIdx.contains(file)) {
        int idx = fileTorecentFilesIdx.get(file);
        recentFilesHeap.updateval(idx, priority);
    }
    else {
        recentFilesHeap.insert(file, priority);
    }
}
void updateBiggestTreesHeap(File* file) {
    int priority = file->getTreeSize();
    if (fileToBiggestTreeIdx.contains(file)) {
        int idx = fileToBiggestTreeIdx.get(file);
        BiggestTreesHeap.updateval(idx, priority);
    } else {
        BiggestTreesHeap.insert(file, priority);
    }
}
std::string to_upper(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}
int main(){
    HashMap<std::string, File*> fileMap;
    std::vector<File*> temp_Files;
    int total_files = 0;
    while (true){
        std::cout << "> ";
        std::string line;
        std::getline(std::cin, line);
        std::stringstream ss(line);
        std::vector<std::string> args;
        std::string arg;
        while (ss >> arg) {
            args.push_back(arg);
        }
        if (args.empty()) continue;
        std::string cmd = to_upper(args[0]);
        
        if (cmd == "EXIT") {
            break;
        }
        else if (cmd == "CURRENT"){
            // print version ID of active_version of filename
            if (args.size() < 2 || args[1].empty()){
                std::cout << "Error: No filename specified." << std::endl;
                continue;
            }
            std::string filename = args[1];
            if (!fileMap.contains(filename)) {
                std::cout << "Error: File not found: " << filename << std::endl;
                continue;
            }
            File* file = fileMap.get(filename);
            std::cout << "Current version ID of " << filename << ": " << file->getActiveVersionID() << std::endl;
        }
        else if (cmd == "CREATE"){
            if (args.size() < 2 || args[1].empty()) {
                std::cout << "Error: No filename specified." << std::endl;
                continue;
            }
            if (args.size() > 2) {
                std::cout << "Error: Too many arguments for CREATE command." << std::endl;
                continue;
            }
            std::string filename = args[1];
            if (fileMap.contains(filename)) {
                std::cout << "Error: File already exists: " << filename << std::endl;
                continue;
            }
            File* newFile = new File(filename);
            fileMap.insert(filename, newFile);
            std::cout << "File created successfully: " << filename << std::endl;
            updateRecentFilesHeap(newFile);
            updateBiggestTreesHeap(newFile);
            total_files++;
        }
        else if (cmd == "INSERT" || cmd == "UPDATE" || cmd == "SNAPSHOT") {
            if (args.size() < 2 || args[1].empty()) {
                std::cout << "Error: Insufficient arguments for " << args[0] << " command." << std::endl;
                continue;
            }
            std::string filename = args[1];
            if (!fileMap.contains(filename)) {
                std::cout << "Error: File not found: " << filename << std::endl;
                continue;
            }
            int pos = line.find(filename);
            int content_start = pos + filename.length() + 1;
            if (content_start >= static_cast<int>(line.length())) {
                std::cout << "Error: Insufficient arguments for " << args[0] << " command." << std::endl;
                continue;
            }
            std::string content = line.substr(content_start);            
            File* file = fileMap.get(filename);
            if (cmd == "INSERT") {
                file->insert(content);
                updateRecentFilesHeap(file);
                updateBiggestTreesHeap(file);
            } 
            else if (cmd == "UPDATE") {
                file->update(content);
                updateRecentFilesHeap(file);
                updateBiggestTreesHeap(file);
            } 
            else if (cmd == "SNAPSHOT") {
                try {
                    file->snapshot(content);
                } catch (const char* e) {
                    std::cout << "Error: " << e << std::endl;
                    continue;
                }
                std::cout << "Success Creating Snapshot for File: " << filename << std::endl;
            }
        }
        
        else if (cmd == "READ"){
            if (args.size() < 2 || args[1].empty()) {
                std::cout << "Error: No filename specified." << std::endl;
                continue;
            }
            if (args.size() > 2) {
                std::cout << "Error: Too many arguments for READ command." << std::endl;
                continue;
            }
            std::string filename = args[1];
            if (!fileMap.contains(filename)) {
                std::cout << "Error: File not found: " << filename << std::endl;
                continue;
            }
            File* file = fileMap.get(filename);
            std::cout << file->read() << std::endl;
            std::cout << "Success Reading File: " << filename  << std::endl;
        }
        else if (cmd == "ROLLBACK"){
            if (args.size() < 2 || args[1].empty()) {
                std::cout << "Error: No filename specified." << std::endl;
                continue;
            }
            if (args.size() > 3) {
                std::cout << "Error: Too many arguments for ROLLBACK command." << std::endl;
                continue;
            }
            std::string filename = args[1];
            if (!fileMap.contains(filename)) {
                std::cout << "Error: File not found: " << filename << std::endl;
                continue;
            }
            File* file = fileMap.get(filename);
            if (args.size() == 2) {
                try {
                    file->rollback();
                    std::cout << "Success Rolling Back File: " << filename  << std::endl;
                } catch (const char* e) {
                    std::cout << "Error: " << e << std::endl;
                    continue;
                }
            }
            else {
                int version_id;
                // make try catch blocks, rollback could throw strings
                if (isInteger(args[2])) {
                    version_id = std::stoi(args[2]);
                    try {
                        file->rollback(version_id);
                        std::cout << "Success Rolling Back File: " << filename  << std::endl;
                    } 
                    catch (const char* e) {
                        std::cout << "Error: " << e << std::endl;
                        //continue;
                    }
                } else {
                    std::cout << "Error: Invalid version ID format." << std::endl;
                    continue;
                }
            }
        }
        else if (cmd == "HISTORY"){
            if (args.size() < 2 || args[1].empty()) {
                std::cout << "Error: No filename specified." << std::endl;
                continue;
            }
            if (args.size() > 2) {
                std::cout << "Error: Too many arguments for HISTORY command." << std::endl;
                continue;
            }
            std::string filename = args[1];
            if (!fileMap.contains(filename)) {
                std::cout << "Error: File not found: " << filename << std::endl;
                continue;
            }
            File* file = fileMap.get(filename);
            if (file) {
                file->history();
            }
        }
        else if (cmd == "RECENT_FILES") {
            if (args.size() > 3){
                std::cout << "Error: Too many arguments for RECENT_FILES command." << std::endl;
                continue;
            }
            int num = total_files;
            if (args.size() == 2) {
                if (isInteger(args[1])) {
                    num = std::stoi(args[1]);
                    if (num > total_files) {
                        std::cout << "Error: Number exceeds total files." << std::endl;
                        continue;
                    }
                } 
                else {
                    std::cout << "Error: Invalid number format." << std::endl;
                    continue;
                }
            }
            int cnt = 0;
            std::vector<File*> temp_Files;
            std::vector<std::pair<time_t, size_t>> temp_Priorities;
            while (cnt < num && !recentFilesHeap.isEmpty()) {
                std::pair<std::pair<time_t, size_t>, File*> temp = recentFilesHeap.extract();
                File* F = temp.second;
                time_t t = F->getLastModified();
                std::cout << "File: " << F->getName() << " | Last Modified: " << ctime(&t);
                temp_Files.push_back(F);
                temp_Priorities.push_back({t, F->getUpdateCounter()});
                cnt++;
            }
            for (size_t i = 0; i < temp_Files.size(); ++i) {
                recentFilesHeap.insert(temp_Files[i], temp_Priorities[i]);
            }
        }
        else if (cmd == "BIGGEST_TREES") {
            if (args.size() > 3){
                std::cout << "Error: Too many arguments for BIGGEST_TREES command." << std::endl;
                continue;
            }
            int num = total_files;
            if (args.size() == 2) {
                if (isInteger(args[1])) {
                    num = std::stoi(args[1]);
                    if (num > total_files) {
                        std::cout << "Error: Number exceeds total files." << std::endl;
                        continue;
                    }
                } 
                else {
                    std::cout << "Error: Invalid number format." << std::endl;
                    continue;
                }
            }
            int cnt = 0;
            std::vector<std::pair<File*, int>> temp_Files;
            while (cnt < num && !BiggestTreesHeap.isEmpty()) {
                std::pair<int, File*> temp = BiggestTreesHeap.extract();
                int t = temp.first;
                std::cout << "File: " << temp.second->getName() << " | Tree Size: " << t << std::endl;
                temp_Files.push_back({temp.second, t});
                cnt++;
            }
            for (auto& entry : temp_Files) {
                BiggestTreesHeap.insert(entry.first, entry.second);
            }
        }
        else {
            std::cout << "Error: Unknown command: " << args[0] << std::endl;
        }
    }
}