#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#define UNICODE 0
#undef UNICODE
#include <windows.h>
#include <conio.h>
using namespace std;
vector<string> recentFiles;
#define MAX_RECENT 5

// add to recent files list
void addRecent(string fname) {
    // remove if already exists
    for (int i = 0; i < (int)recentFiles.size(); i++) {
        if (recentFiles[i] == fname) {
            recentFiles.erase(recentFiles.begin() + i);
            break;
        }
    }
    recentFiles.insert(recentFiles.begin(), fname);
    if ((int)recentFiles.size() > MAX_RECENT)
        recentFiles.pop_back();
}

// ---- FILE OPERATIONS (Part A) ----

// save a file
void saveFile(string fname, vector<string>& lines) {
    ofstream f(fname);
    if (!f.is_open()) {
        cout << "Error: could not save " << fname << endl;
        return;
    }
    for (auto& line : lines)
        f << line << "\n";
    f.close();
    addRecent(fname);
    cout << "Saved: " << fname << endl;
}

// open a file
vector<string> openFile(string fname) {
    vector<string> lines;
    ifstream f(fname);
    if (!f.is_open()) {
        cout << "Error: file not found - " << fname << endl;
        return lines;
    }
    string line;
    while (getline(f, line))
        lines.push_back(line);
    if (lines.empty())
        lines.push_back("");
    addRecent(fname);
    cout << "Opened: " << fname << endl;
    return lines;
}

// show recent files
void showRecent() {
    if (recentFiles.empty()) {
        cout << "No recent files." << endl;
        return;
    }
    cout << "\nRecent files:" << endl;
    for (int i = 0; i < (int)recentFiles.size(); i++)
        cout << "  " << i+1 << ". " << recentFiles[i] << endl;
}

// find and replace in a vector of lines
int findReplace(vector<string>& lines, string find, string replace) {
    int count = 0;
    for (auto& line : lines) {
        size_t pos = 0;
        while ((pos = line.find(find, pos)) != string::npos) {
            line.replace(pos, find.size(), replace);
            pos += replace.size();
            count++;
        }
    }
    return count;
}

int main() {
    // set console title
    SetConsoleTitleA("Text Editor - Q2 Launcher");

    cout << "=== Text Editor - Q2 ===" << endl;
    cout << "Opening editor UI in browser..." << endl;
    cout << endl;

    // check if HTML file exists first
    ifstream check("editor_ui.html");
    if (!check.is_open()) {
        cout << "Error: editor_ui.html not found!" << endl;
        cout << "Make sure editor_ui.html is in the same folder as editor.exe" << endl;
        cout << "Press any key to exit..." << endl;
        _getch();
        return 1;
    }
    check.close();

    // open the HTML editor in default browser
    system("start editor_ui.html");

    cout << "Editor opened in browser!" << endl;
    cout << endl;
    cout << "This console window shows C++ file operations running in background." << endl;
    cout << "Both the HTML UI and this C++ code handle the same file format." << endl;
    cout << endl;

    vector<string> currentLines;
    currentLines.push_back("");
    string currentFile = "untitled.txt";
    bool modified = false;

    int choice;
    while (true) {
        cout << "\n--- C++ Backend Console ---" << endl;
        cout << "Current file: " << currentFile;
        if (modified) cout << " [modified]";
        cout << endl;
        cout << "1. New file" << endl;
        cout << "2. Open file" << endl;
        cout << "3. Save file" << endl;
        cout << "4. Save As" << endl;
        cout << "5. Show recent files" << endl;
        cout << "6. Find and Replace" << endl;
        cout << "7. Show file contents" << endl;
        cout << "8. Reopen HTML editor" << endl;
        cout << "0. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 0) {
            if (modified) {
                cout << "Unsaved changes! Save before exit? (y/n): ";
                string ans; getline(cin, ans);
                if (ans == "y" || ans == "Y")
                    saveFile(currentFile, currentLines);
            }
            break;
        }
        else if (choice == 1) {
            if (modified) {
                cout << "Unsaved changes! Save first? (y/n): ";
                string ans; getline(cin, ans);
                if (ans == "y" || ans == "Y")
                    saveFile(currentFile, currentLines);
            }
            currentLines.clear();
            currentLines.push_back("");
            currentFile = "untitled.txt";
            modified = false;
            cout << "New file created." << endl;
        }
        else if (choice == 2) {
            cout << "Enter filename: ";
            string fname; getline(cin, fname);
            if (!fname.empty()) {
                currentLines = openFile(fname);
                if (!currentLines.empty()) {
                    currentFile = fname;
                    modified = false;
                }
            }
        }
        else if (choice == 3) {
            if (currentFile == "untitled.txt") {
                cout << "Enter filename: ";
                string fname; getline(cin, fname);
                if (!fname.empty()) currentFile = fname;
            }
            saveFile(currentFile, currentLines);
            modified = false;
        }
        else if (choice == 4) {
            cout << "Save as (filename): ";
            string fname; getline(cin, fname);
            if (!fname.empty()) {
                saveFile(fname, currentLines);
                currentFile = fname;
                modified = false;
            }
        }
        else if (choice == 5) {
            showRecent();
        }
        else if (choice == 6) {
            cout << "Find: ";
            string find; getline(cin, find);
            cout << "Replace with: ";
            string replace; getline(cin, replace);
            int count = findReplace(currentLines, find, replace);
            if (count > 0) {
                modified = true;
                cout << "Replaced " << count << " occurrence(s)." << endl;
            } else {
                cout << "'" << find << "' not found." << endl;
            }
        }
        else if (choice == 7) {
            cout << "\nFile contents (" << currentLines.size() << " lines):" << endl;
            cout << string(40, '-') << endl;
            for (int i = 0; i < (int)currentLines.size(); i++)
                cout << i+1 << ": " << currentLines[i] << endl;
        }
        else if (choice == 8) {
            system("start editor_ui.html");
            cout << "HTML editor reopened." << endl;
        }
    }

    cout << "\nGoodbye!" << endl;
    return 0;
}