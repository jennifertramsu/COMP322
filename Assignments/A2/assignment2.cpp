/*
 *   COMP322 - Assignment 2
 *   Jennifer Tram Su (260923530)
 * 
 *  git322 will track one file and will keep all versions of this file in memory
 *  the user decides when to take a snapshot of his/her modification
 *  the user should be able to load a specific version to make it the current version or can also decide to delete any version of the file
 *  allowed operations:
 *      adding a version of the file when the user finds suitable (a)
 *      removing a version (any version that was being tracked) (r)
 *      loading a specific version and making it the current active one (l)
 *      printing to the screen the list of all versions of the file (p)
 *      comparing any 2 versions of the file (c)
 *      searching and displaying all the versions having a specific keyword (s)
 *      exiting a program (e)
 * 
 *  git322 has to keep all versions of the file in memory in a linked list using dynamic memory allocation
 *  every element in the list holds information about one version of the file
 * 
 *  file we are tracking will always be called file.txt
 */

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Class Definitions

// Helper
size_t hash_it(string someString) {
    /*
    Returns the hashed value of a string.
    */
    return hash<string>{}(someString);
}

    // Node
class Node { 
    public:
        // Attributes
        Node *next;
        string content;
        int vNum;

        // Default constructor
        Node() {
            this->content = "";
            this->next = NULL;
            this->vNum = 0;
        }

        // Overloaded constructor
        Node(string content, int vNum) { // Node Constructor
            this->content = content;
            this->next = NULL; // Initialize pointer to NULL 
            this->vNum = vNum;
        }                
};

    // Linked List
class LinkedList {
    public:
        // Attributes
        Node *head;
        int length;
        int vTotal;

        // Default Constructor
        LinkedList() {
            this->head = NULL;
            this->length = 0;
            this->vTotal = 0;
        }

        // Methods
        void push(string content);
        void removeNode(Node *n);
        void printList(); // for debugging
};

    // Linked List Methods
void LinkedList::push(string content) {
    // Traverse through linked list to find last node
    Node *n = this->head;

    if (n == NULL) { // no head node yet
        this->head = new Node(content, 1);
        this->length = 1;
        this->vTotal = 1;

    } else {
        while (n->next != NULL) {
            n = n->next;
        }

        // Create new node
        n->next = new Node(content, ++this->vTotal);
        this->length++; // Increase length of linked list
    }
};

void LinkedList::removeNode(Node *n) {
    Node *tracker = this->head;

    if (n == this->head) { // removing head node
        this->head = n->next;
        delete n;
    } else {
        while (tracker->next != n) {
            tracker = tracker->next;
        }
        tracker->next = n->next;
        delete n;
    }
    this->length--;
}

void LinkedList::printList() { // for debugging
    Node *n = this->head;

    while (n != NULL) {
        cout << "Node: " << n->vNum << endl;
        cout << n->content << endl;
        cout << endl;
        
        n = n->next;
    }
}

// Function Declarations

void add(string content);                   // press a
void remove(int version);                   // press r
void load(int version);                     // press l
void print(void);                           // press p
void compare(int version1, int version2);   // press c
void search(string keyword);                // press s
void exit();

// Declaring global variables

    // Initialize global linked list

LinkedList *ll = new LinkedList();
string path = "file.txt";

int main() {
    // Welcome message

    cout << "Welcome to the Comp322 file versioning system!\n" << endl;

    // Declarations
    char command;
    string content;

    int version;
    int v1, v2;
    string keyword;

    while (true) {
        cout << "To add the content of your file to version control press 'a'" << endl;
        cout << "To remove a version press 'r'" << endl;
        cout << "To load a version press 'l'" << endl;
        cout << "To print to the screen the detailed list of all versions press 'p'" << endl;
        cout << "To compare any 2 versions press 'c'" << endl;
        cout << "To search versions for a keyword press 's'" << endl;
        cout << "To exit press 'e'\n" << endl;

        // Take in user input
        cin >> command;
        cout << endl;

        // Read in file
        fstream f(path);
        stringstream b;
        
        b << f.rdbuf();
        content = b.str();        

        switch (command) {
            case 'a':
                add(content);
                break;

            case 'r':
                cout << "Enter the number of the version that you want to delete: ";
                cin >> version;
                cout << endl;

                remove(version);
                cout << endl;
                break;

            case 'l':
                cout << "Which version would you like to load? " ;
                cin >> version; // take user input for version number
                cout << endl;
                load(version);
                break;

            case 'p':
                print();
                break;

            case 'c':
                cout << "Please enter the number of the first version to compare: ";
                cin >> v1;
                cout << endl;

                cout << "Please enter the number of the second version to compare: ";
                cin >> v2;
                cout << endl;

                compare(v1, v2);
                cout << endl;
                break;
            
            case 's':
                cout << "Please enter the keyword that you are looking for: ";
                cin >> keyword;
                cout << endl;

                search(keyword);
                break;

            case 'e':
                // Clear all dynamically allocated memory
                exit();
                f.close();
                return 0;
        }
    }

}

void add(string content) {
    // Check if this version already exists
    Node *n = ll->head;
    size_t h2 = hash_it(content);

    if (n == NULL) {
        (*ll).push(content);
        cout << "Your content has been added successfully.\n" << endl;
    } else {
        while (n != NULL) {
            size_t h1 = hash_it(n->content);

            if (h1 == h2) {
                cout << "git322 did not detect any change to your file and will not create a new version.\n" << endl;
                return;
            } else {
                n = n->next;
            }
        }
        (*ll).push(content);
        cout << "Your content has been added successfully.\n" << endl;
    }
};

// Helper for print and search
void printVersion(Node *n) {
        cout << "Version number: " << n->vNum << endl;
        cout << "Hash value: " << hash_it(n->content) << endl;
        cout << "Content: " << n->content << endl;
}

void print() {
    cout << "Number of versions: " << ll->length << endl;

    Node *n = ll->head;

    while (n != NULL) {
        printVersion(n);
        cout << endl;

        n = n->next;
    }
}

void load(int version) {
    Node *n = ll->head;
    bool found = false;

    if (version > ll->length) {
        cout << "Please enter a valid version number. If you are not sure please press 'p' to list all valid version numbers.\n" << endl;
        return;
    } else {
        while (n != NULL && version != n->vNum) {
            n = n->next;

            if (n == NULL) {
                cout << "Please enter a valid version number. If you are not sure please press 'p' to list all valid version numbers.\n" << endl;
                return;
            }
        }

        ofstream w(path);

        w << n->content;
        w.close();
        cout << "Version " << version << " loaded successfully. Please refresh your text editor to see the changes.\n" << endl;
        return;
    }    
}

// Helper for compare
void check_lines(istringstream *c1, istringstream *c2, streampos p1, streampos p2, int lc) {

    (*c1).seekg(p1);
    (*c2).seekg(p2);

    if (!(*c1).good() || !(*c2).good()) { // at least one of the versions has reached eof
        if ((*c1).good()) { // version 1 has remaining lines
            string l1;
            getline(*c1, l1); // extract line

            streampos p1n = (*c1).tellg(); // save position

            cout << "Line " << lc << ": " << l1 << " <<>> <Empty line>" << endl;

            lc++;
            check_lines(c1, c2, p1n, p2, lc);
        }
        else if ((*c2).good()) { // version 2 has remaining lines
            string l2;
            getline(*c2, l2); // extract line

            streampos p2n = (*c2).tellg(); // save position

            cout << "Line " << lc << ": " << "<Empty line>" << " <<>> " << l2 << endl;

            lc++;
            check_lines(c1, c2, p1, p2n, lc);
        }
    } else { // neither file has reached eof
        string l1, l2;

        getline(*c1, l1); // extract line from file
        getline(*c2, l2);

        streampos p1n = (*c1).tellg(); // save cursor position after reading line
        streampos p2n = (*c2).tellg();

        // compare hashes
        size_t h1 = hash_it(l1);
        size_t h2 = hash_it(l2);

        if (h1 != h2) {
            cout << "Line " << lc << ": " << l1 << " <<>> " << l2 << endl;
        } else {
            cout << "Line " << lc << ": <Identical>" << endl;
        }

        lc++; // Increment line counter
        check_lines(c1, c2, p1n, p2n, lc);
    }
}

void compare(int version1, int version2) {
    // Find version 1
    Node *n1 = ll->head;

    while (n1 != NULL && version1 != n1->vNum) {
        n1 = n1->next;
    }

    if (n1 == NULL) {
        cout << "Please enter a valid version number for the first version." << endl;
        return;
    }

    // Find version 2
    Node *n2 = ll-> head;

    while (n2 != NULL && version2 != n2->vNum) {
        n2 = n2->next;
    }

    if (n2 == NULL) {
        cout << "Please enter a valid version number for the second version." << endl;
        return;
    }

    // Compare lines
    istringstream c1(n1->content);
    istringstream c2(n2->content);

    istringstream *cp1 = &c1;
    istringstream *cp2 = &c2;

    check_lines(cp1, cp2, 0, 0, 1);
}

void search(string keyword) {
    Node *n = ll->head;
    bool found = false;

    while (n != NULL) {
        istringstream c(n->content);
        string l;

        while (getline(c, l)) {
            if (l.find(keyword) != string::npos) { // match found
                if (!found) { // first match found
                    found = true;
                    cout << "The keyword '" << keyword << "' has been found in the following versions:" << endl;
                }
                printVersion(n);
                cout << endl;
                break;
            }
        }
        n = n->next;
    }
    if (!found) { // no matches found in linked list
        cout << "Your keyword '" << keyword << "' was not found in any version.\n" << endl;
    }
}

void remove(int version) {
    Node *n = ll->head;
    bool found = false;

    while (n != NULL) {
        if (version == n->vNum) {
            found = true;
            (*ll).removeNode(n);
            cout << "Version " << version << " deleted successfully." << endl;
        }
        n = n->next;
    }
    if (!found) {
        cout << "Please enter a valid version number." << endl;
    }  
}

void exit() {
    // Delete all dynamically allocated memory
    while (ll->head != NULL) {
        Node *n = ll->head;
        ll->head = n->next;
        delete n; // Delete node
    }

    delete ll; // Delete LinkedList
}