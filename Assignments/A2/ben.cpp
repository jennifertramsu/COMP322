#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct FileLinkedList{
    struct Node{

        // Node attributes
        int versionNumber;
        size_t hashValue;
        string contents;
        Node *prev;
        Node *next;


        // Method for printing the contents of a node
        void toStringNode() {
            cout << "Version number: " << versionNumber << endl;
            cout << "Hash value: " << hashValue << endl;
            cout << "Content: " << contents << endl << endl;
        };

    };

    // Linked List attributes
    int length;
    Node *head;
    Node *tail;
        
    // Add a new node to the end of the list
    void add(string content) {
        // Creating the new node
        Node *node = new Node;
        if(tail == nullptr){ // Checks if there are already nodes in the list
            node->versionNumber = 1;
        } else {
            node->versionNumber = (tail->versionNumber + 1);
        }  
        // Initializing attributes of the node
        node->contents = content;
        node->next = nullptr;
        hash<string> hasher;
        node->hashValue = hasher(content);

        // Linking the node to the existing list
        if(tail == nullptr) { // If there is no existing list
            head = node;
            tail = node;
        } else { // Else link normally
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
        length++;

    };

    // Remove a specific version number from the list
    void remove(int version){
        Node *curNode = head;

        while(curNode != nullptr) { // Loop through the list looking for the correct node to remove
            if(curNode->versionNumber == version) {
                if(length == 1) { // Case where there's only one node in the list
                    head = nullptr;
                    tail = nullptr;
                } else if(curNode == head) { // Case where the node to be removed is the current head
                    head = curNode->next;
                    curNode->next->prev = nullptr;
                } else if (curNode == tail) { // Case where the node to be removed is the current tail
                    tail = curNode->prev;
                    curNode->prev->next = nullptr;
                } else { // Case where the node to be removed is in the middle of the list
                    curNode->prev->next = curNode->next;
                    curNode->next->prev = curNode->prev;
                }
                // freeing memory
                delete curNode;

                length--;
                cout << "Version " << version << " deleted successfully." << endl << endl;

                return;
            }
            curNode = (*curNode).next;
        };

        cout << "Please enter a valid version number." << endl << endl;

    };

    // Helper function to get a string of the contents of a certain version of the file
    string getFileContent(int version) {
        Node *curNode = head;
        while(curNode != nullptr) { // searches the list for the indicated version
            if(curNode->versionNumber == version) {
                return curNode->contents; // returns a string of the contents
            };
            curNode = (*curNode).next;
        }
        cout << "Version not found." << endl << endl; // If the version indicated isn't found
        return NULL;
    }

    // Helper function to check if the hash of a node is identical to that of the current nodes in the list
    bool checkContent(string content) {
        Node *curNode = head;
        while(curNode != nullptr) { // loop through the list
            if(curNode->contents == content) { // check if the contents of each node is identical to the method argument
                return true;
            };
            curNode = (*curNode).next;
        }

        return false;
    }

    // Helper function to check if a specific version number exists in the list in its current state
    bool checkValidVersion(int version) {
        Node *curNode = head;
        while(curNode != nullptr) { // loop through the list
            if(curNode->versionNumber == version) { // check the argument version number against the version number of each node
                return true;
            };
            curNode = (*curNode).next;
        }

        return false;
    }

    // Helper function to find and print all the nodes that contain a keyword
    bool checkForWord(string keyword) {
        Node *curNode = head;
        bool found = false; // Variable indicating whether the keyword has been found in ANY version yet
        while(curNode != nullptr) { // Loop through the list
            if(curNode->contents.find(keyword) != string::npos) { // If the keyword exists in the contents of the node
                if(!found) { // If this is the first node in which the keyword has been found
                    found = true;
                    cout << "The keyword '" << keyword << "' has been found in the following versions:" << endl;
                }
                curNode->toStringNode(); // Print the node attributes if the keyword was found
            }
            curNode = (*curNode).next;
        }

        return found;
    }

    // Method for printing the contents of the linked list
    void toStringList() {
        Node *curNode = head;
        while(curNode != nullptr) {
            (*curNode).toStringNode();
            curNode = (*curNode).next;
        }
    };

    // Helper function that deletes all the remaining nodes in the list
    void deleteList() {
        Node *curNode = head;
        while(head != nullptr) { // Loops through the list
            
            curNode = (*curNode).next;
            delete head;
            head = curNode;
        }
    }

};

// Global linked list
FileLinkedList *list = new FileLinkedList;

// Function that adds a node to the list with specified contents
void add(string content) {
    if(list->checkContent(content)) { // Checks if there's already a node in the list with the same contents, and doesn't add if found
        cout << "git322 did not detect any change to your file and will not create a new version." << endl << endl;
        return;
    }

    // Otherwise, adds a new node to the list
    list->add(content);
    cout << "Your content has been added successfully" << endl << endl;
    return;
};

// Prints out each element of the list and all their attributes
void print() {

    if(list->head == nullptr) { // Check to make sure there are versions saved
        cout << "There are no versions of the file saved." << endl << endl;
    } else { // Print the versions of the file
        cout << "Number of versions: " << list->length << endl;
        list->toStringList();
    }
    return;
};

// Loads content from an existing node into the file
void load(int version){

    if(list->checkValidVersion(version)){ // Checks that the version indicated has been saved

        ofstream f1("file.txt", ios::trunc); // Opens the file in truncated mode, which clears the contents
        f1.close();
        fstream file("file.txt"); // Opens the file to write to in regular mode
        file << list->getFileContent(version); // Writes the contents of the indicated version into the file
        cout << "Version " << version << " loaded successfully. Please refresh your text editor to see the changes." << endl << endl;
        file.close();
    } else { // If the version indicated does not exist
        cout << "Please enter a valid version number. If you are not sure please press 'p' to list all valid version numbers." << endl << endl;
    }

};

// Function to compare the contents of two files line-by-line
void compare(int version1, int version2) {
    cout << endl;
    // Make sure both versions entered exist in the list
    if(!list->checkValidVersion(version1) || !list->checkValidVersion(version2)) {
        cout << "Please enter two valid version numbers." << endl << endl;
        return;
    }

    // Get the contents from each node
    string content1 = list->getFileContent(version1);
    string content2 = list->getFileContent(version2);
    
    int line = 1; // Tracks line number for print formatting

    // Initialize variables to track character positions
    int start1 = -1, start2 = -1, end1 = -1, end2 = -1;


    do { // loop for each line
        // sets starting character position to the beginning of the line on each loop
        start1 = end1 + 1;
        start2 = end2 + 1;

        // Get the end character position
        end1 = content1.find("\n", start1);
        end2 = content2.find("\n", start2);
        
        // gets a substring from the beginning of the line to the end of the line
        string sub1 = content1.substr(start1, end1 - start1);
        string sub2 = content2.substr(start2, end2 - start2);

        if(sub1 == sub2) { // compares the lines for each file
            cout << "Line " << line << ": <Identical>" << endl; // Case if they're identical
        } else {
            cout << "Line " << line << ": " << sub1 << " <<>> " << sub2 << endl; // Case if they're different
        }

        line++;
        
    } while (end1 != -1 && end2 != -1); // While both files still have more lines

    if(end1 != -1) { // If the first file has more lines but the second doesn't

        do { // Same loop as above, getting the line from the first version and printing it
            start1 = end1 + 1;
            end1 = content1.find("\n", start1);
            string sub1 = content1.substr(start1, end1 - start1);
            cout << "Line " << line << ": " << sub1 << " <<>> " << "<Empty Line>" << endl;
            line++;
        } while(end1 != -1);


    } else if(end2 != -1){ // If the second file has more lines but the first doesn't

        do { // Same loop as above to print the contents of the second version line-by-line
            start2 = end2 + 1;
            end2 = content2.find("\n", start2);
            string sub2 = content2.substr(start2, end2 - start2);
            cout << "Line " << line << ": " << "<Empty Line>" << " <<>> " << sub2 << endl;
            line++;
        } while(end2 != -1);

    }
    cout << endl;
};

// Function to search the contents of each element in the list for a specific string
void search(string keyword) {
    if(!list->checkForWord(keyword)) { // Calls the function to search the list, and prints not found if string is not found
        cout << "Your keyword '" << keyword << "' was not found in any version." << endl << endl;
    }
    return;
};

// Function to remove an element of the list (a saved file version)
void remove(int version) {

    list->remove(version);
};

// Function called when exiting the program, cleans up the list and frees memory
void exit() {
    list->deleteList();
    delete list;

    return;

}


// Main 
int main(){

    // Initializes the global linked list
    list->length = 0;
    list->head = nullptr;
    list->tail = nullptr;



    bool running = true;
    while(running) { // Loop to continue allowing the user to continue interacting with the program.

        cout << "Welcome to the Comp322 file versioning system!" << endl << endl <<
                "To add the content of your file to version control press 'a'" << endl <<
                "To remove a version press 'r'" << endl <<
                "To load a version press 'l'" << endl <<
                "To print to the screen the detailed list of all versions press 'p'" << endl <<
                "To compare any 2 versions press 'c'" << endl <<
                "To search versions for a keyword press 's'" << endl <<
                "To exit press 'e'" << endl << endl;
        char input;
        cin >> input;

        if(input == 'a') { // Case if user wants to add the current version of their file to version control
            // Read contents of the file
            fstream file("file.txt");
            ostringstream buffer;
            string content;

            buffer << file.rdbuf();
            content = buffer.str();
            
            file.close();

            // Save this version of the file
            add(content);

        } else if(input == 'r') { // Case if  user wants to remove a certain version of their file from the system
            cout << "Enter the number of the version that you want to delete: " << endl;
            int ver;
            cin >> ver;
            remove(ver);
        } else if(input == 'l') { // Case if user wants to load a specific saved version of the file
            // Get desired version number from user
            cout << "Which version would you like to load?" << endl;
            int version;
            cin >> version;

            load(version);
            
        } else if(input == 'p') { // Case if user wants to print the versions that are saved
            print();
        } else if(input == 'c') { // Case if user wants to compare two versions of the file
            cout << "Please enter the number of the first version to compare: " << endl;
            int ver1;
            cin >> ver1;

            cout << "Please enter the number of the second version to compare: " << endl;
            int ver2;
            cin >> ver2;

            compare(ver1, ver2);

        } else if(input == 's') { // Case if user wants to search all versions for a specific word
            cout << "Please enter the keyword that you are looking for: " << endl;
            string word;
            cin >> word;
            search(word);

        } else if(input == 'e') { // Case if user wants to exit the program
            exit();
            running = false;
        } else { // Case if user doesn't enter a valid option
            cout << "Please enter a valid input" << endl << endl;
        }
    }

    return 0;
}