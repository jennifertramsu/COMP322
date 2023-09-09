/*
 *   COMP322 - Assignment 3
 *   Jennifer Tram Su (260923530)
 * 
 *  file we are tracking will always be called file.txt
 * 
 *  Part One
 *      Create a class called Git322. All functions from A2 are public methods.
 *      Any global variable is a private data member
 *      Any helper function is a private method
 *      The linked list previously defined is a class called LinkedList
 *      The Git322 class will use the LinkedList through a member object called mylist
 * 
 *  PLEASE READ:    I added an additional element where the program prompts you to type the filename of the file to be tracked.
 *                  For this assignment, please type 'file.txt' when prompted!
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>

using namespace std;

// Class Node
class Node { 
    public:
        // Default constructor
        Node() {
            this->content = "";
            this->next = NULL;
            this->vNum = 0;
        }

        // Overloaded constructor
        Node(string content, int vNum, size_t hash) { // Node Constructor
            this->content = content;
            this->next = NULL; // Initialize pointer to NULL 
            this->vNum = vNum;
            this->hash = hash;
        }   

        // Getters
        Node *getNext() {
            return this->next;
        }
        string getContent() {
            return this->content;
        }
        int getVNum() {
            return this->vNum;
        }
        size_t getHash() {
            return this->hash;
        }

        // Setters
        void setNext(Node *next) {
            this->next = next;
        }
        void setContent(string content) {
            this->content = content;
        }
        void setVNum(int vNum) {
            this->vNum = vNum;
        }
    private:
        // Attributes
        Node *next;
        string content;
        int vNum;    
        size_t hash;
};

// Class LinkedList
class LinkedList {
    public:
        // Constructor
        LinkedList() {
            this->head = NULL;
            this->length = 0;
            this->vTotal = 0;
        }

        // Destructor
        ~LinkedList() {
            while (this->head != NULL) {
                Node *n = this->head;

                if (this->head->getNext() == NULL) {
                    cout << "Deleting node" << endl;
                    delete n;
                    break;
                }

                this->head = (*n).getNext();
                cout << "Deleting node" << endl;
                delete n; // Delete node
            }
        }

        // Methods
        void push(string content, int i, size_t hash);
        void removeNode(Node *n);
        void printList(); // for debugging

        // Getters
        Node *getHead() {
            return this->head;
        }
        int getLength() {
            return this->length;
        }
        int getVTotal() {
            return this->vTotal;
        }

        // Setters
        void setHead(Node *head) {
            this->head = head;
        }
        void setLength(int length) {
            this->length = length;
        }
        void setVTotal(int vTotal) {
            this->vTotal = vTotal;
        }

        size_t hash_it(string someString) {
            /*
            Returns the hashed value of a string.
            */
            return hash<string>{}(someString);
        }

    private:
        // Attributes
        Node *head;
        int length;
        int vTotal;
};

    // LinkedList methods
void LinkedList::push(string content, int i, size_t hash) {
    // Traverse through linked list to find last node
    Node *n = this->head;

    if (n == NULL) { // no head node yet
        this->head = new Node(content, 1, hash_it(content));
        this->length = 1;
        this->vTotal = 1;

    } else {
        while ((*n).getNext() != NULL) {
            n = (*n).getNext();
        }

        // Create new node
        (*n).setNext(new Node(content, i, hash_it(content)));
        this->length++; // Increase length of linked list
        this->vTotal++; // Increase total version number
    }
};

void LinkedList::removeNode(Node *n) {
    Node *tracker = this->head;

    if (n == this->head) { // removing head node
        this->head = (*n).getNext();
        delete n;
    } else {
        while ((*tracker).getNext() != n) {
            tracker = (*tracker).getNext();
        }
        (*tracker).setNext((*n).getNext());
        delete n;
    }
    this->length--;
}

void LinkedList::printList() { // for debugging
    Node *n = this->head;

    while (n != NULL) {
        cout << "Node: " << (*n).getVNum() << endl;
        cout << (*n).getContent() << endl;
        cout << endl;
        
        n = (*n).getNext();
    }
}

// Class Git322
class Git322 {
    public:
        Git322(){ // Constructor
            cout << "Which file would you like to track?" << endl;
            cin >> this->path;
            cout << endl;

            this->mylist = new LinkedList();
        };
        virtual ~Git322() {
            if (mylist != NULL) { // If no inheritance, so that it is functional in part 1
                cout << "Deleting LinkedList" << endl;
                delete mylist;
                mylist = NULL; // Set to NULL
            }
        }

        // Methods
        void add(string content);                   // press a
        void remove(int version);                   // press r
        void load(int version);                     // press l
        void print(void);                           // press p
        void compare(int version1, int version2);   // press c
        void search(string keyword);                // press s

        // Getter
        string getPath() {
            return this->path;
        }

    protected:
        LinkedList *mylist;
        void printVersion(Node *n);
        void check_lines(istringstream *c1, istringstream *c2, streampos p1, streampos p2, int lc);
        string path; // No setter, don't want this to be updated 
};

void Git322::add(string content) {
    // Check if this version already exists
    Node *n = (*mylist).getHead();
    size_t h2 = (*mylist).hash_it(content);

    if (n == NULL) {
        (*mylist).push(content, (*mylist).getVTotal() + 1, h2);
        cout << "Your content has been added successfully.\n" << endl;
    } else {
        while (n != NULL) {
            size_t h1 = (*n).getHash();

            if (h1 == h2) {
                cout << "git322 did not detect any change to your file and will not create a new version.\n" << endl;
                return;
            } else {
                n = (*n).getNext();
            }
        }
        (*mylist).push(content, (*mylist).getVTotal() + 1, h2);
        cout << "Your content has been added successfully.\n" << endl;
    }
};

// Helper for print and search
void Git322::printVersion(Node *n) {
        cout << "Version number: " << (*n).getVNum() << endl;
        cout << "Hash value: " << (*n).getHash() << endl;
        cout << "Content: " << (*n).getContent() << endl;
}

void Git322::print() {
    cout << "Number of versions: " << (*mylist).getLength() << endl;

    Node *n = (*mylist).getHead();

    while (n != NULL) {
        printVersion(n);
        cout << endl;

        n = (*n).getNext();
    }
}

void Git322::load(int version) {
    Node *n = (*mylist).getHead();
    bool found = false;

    if (version > (*mylist).getLength()) {
        cout << "Please enter a valid version number. If you are not sure please press 'p' to list all valid version numbers.\n" << endl;
        return;
    } else {
        while (n != NULL && version != (*n).getVNum()) {
            n = (*n).getNext();

            if (n == NULL) {
                cout << "Please enter a valid version number. If you are not sure please press 'p' to list all valid version numbers.\n" << endl;
                return;
            }
        }

        ofstream w(path);

        w << (*n).getContent();
        w.close();
        cout << "Version " << version << " loaded successfully. Please refresh your text editor to see the changes.\n" << endl;
        return;
    }    
}

// Helper for compare
void Git322::check_lines(istringstream *c1, istringstream *c2, streampos p1, streampos p2, int lc) {

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
        size_t h1 = (*mylist).hash_it(l1);
        size_t h2 = (*mylist).hash_it(l2);

        if (h1 != h2) {
            cout << "Line " << lc << ": " << l1 << " <<>> " << l2 << endl;
        } else {
            cout << "Line " << lc << ": <Identical>" << endl;
        }

        lc++; // Increment line counter
        check_lines(c1, c2, p1n, p2n, lc);
    }
}

void Git322::compare(int version1, int version2) {
    // Find version 1
    Node *n1 = (*mylist).getHead();

    while (n1 != NULL && version1 != (*n1).getVNum()) {
        n1 = (*n1).getNext();
    }

    if (n1 == NULL) {
        cout << "Please enter a valid version number for the first version." << endl;
        return;
    }

    // Find version 2
    Node *n2 = (*mylist).getHead();

    while (n2 != NULL && version2 != (*n2).getVNum()) {
        n2 = (*n2).getNext();
    }

    if (n2 == NULL) {
        cout << "Please enter a valid version number for the second version." << endl;
        return;
    }

    // Compare lines
    istringstream c1((*n1).getContent());
    istringstream c2((*n2).getContent());

    istringstream *cp1 = &c1;
    istringstream *cp2 = &c2;

    check_lines(cp1, cp2, 0, 0, 1);
}

void Git322::search(string keyword) {
    Node *n = (*mylist).getHead();
    bool found = false;

    while (n != NULL) {
        istringstream c((*n).getContent());
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
        n = (*n).getNext();
    }
    if (!found) { // no matches found in linked list
        cout << "Your keyword '" << keyword << "' was not found in any version.\n" << endl;
    }
}

void Git322::remove(int version) {
    Node *n = (*mylist).getHead();
    bool found = false;

    while (n != NULL) {
        if (version == (*n).getVNum()) {
            found = true;
            (*mylist).removeNode(n);
            cout << "Version " << version << " deleted successfully." << endl;
        }
        n = (*n).getNext();
    }
    if (!found) {
        cout << "Please enter a valid version number." << endl;
    }  
}

// Add persistence so that the file versions are saved between multiple runs of the program
// Class EnhancedGit322
class EnhancedGit322: public Git322 {
    public:
        // Constructor, load persistence
        EnhancedGit322(){
             // Read in file for LinkedList
            string linkedListFileName = folder_dir + "/linkedlist.txt";
            ifstream fll(linkedListFileName);
            int vTotal;

            if (fll) { // File exists
                string sf, line;

                while(getline(fll, line)) {
                    sf += line + "\n";
                }

                vTotal = stoi(sf.substr(sf.rfind(" ") + 1));

                fll.close();
                std::remove(linkedListFileName.c_str());
            
                for (int i = 1; i <= vTotal; i++) {
                    string nodeFileName = folder_dir + "/node_" + to_string(i) + ".txt";
                    ifstream fn(nodeFileName);

                    if (!fn) { // Version doesn't exist
                        continue;
                    } else {
                        string content, c, h, line;
                        size_t hash;

                        // Will always be Hash: 
                        getline(fn, h);
                        h = h.substr(h.rfind(" ") + 1);
                        // Convert to size_t
                        stringstream stream(h);
                        stream >> hash;

                        // Grabbing content
                        while(getline(fn, line)) {
                                content += line;
                                content += "\n";
                        }

                        content = content.substr(content.find_first_of(":") + 2, content.find_last_of("\n") - 1);
                        content.erase(content.length()-1);
                        cout << content << endl;

                        if ((*mylist).getHead() == NULL) { // Initializing head node
                            (*mylist).setHead(new Node(content, i, hash));  
                            (*mylist).setLength(1);                      
                        } else {
                            (*mylist).push(content, i, hash);
                        }
                        fn.close();
                        std::remove(nodeFileName.c_str()); // Remove file
                    }
                }
                (*mylist).setVTotal(vTotal);

                // Remove persistence directory
                rmdir(folder_dir.c_str());
            }
        }; 
        // Destructor, save persistence 
        ~EnhancedGit322(){
            /*
            * Things to save to persistence:
            *
            * LinkedList class
            *       length (length of linked list)
            *       vTotal (total version numbers)
            * Node
            *       content (file contents)
            *       vnum    (version number of node)
            */

            // Making directory for persistence
            mkdir(folder_dir.c_str(), 0777);

            // Save linked list elements
            ofstream fll(folder_dir + "/linkedlist.txt");
            fll << "vTotal: " << (*mylist).getVTotal() << endl;
            fll.close();

            // Save node elements
            ofstream fn;

            Node *n = (*mylist).getHead();

            while (n != NULL) {
                fn.open(folder_dir + "/node_" + to_string((*n).getVNum()) + ".txt");
                stringstream ss;
                ss << (*n).getHash();
                string hash = ss.str();
                fn << "Hash: " << hash << endl;
                fn << "Content: " << (*n).getContent();

                n = (*n).getNext();
                fn.close();
            }           

            cout << "Deleting LinkedList" << endl;
            delete mylist;
            mylist = NULL; // set to NULL
        };  
    private:
        string folder_dir = this->path.substr(0, this->path.find_last_of("."));
};

/*
// Part 1
int main() {
    
    cout << "Welcome to the Comp322 file versioning system!\n" << endl;
    char command;
    string content;

    int version;
    int v1, v2;
    string keyword;

    Git322 *git = new Git322();

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
        fstream f((*git).getPath());
        stringstream b;
        
        b << f.rdbuf();
        content = b.str();        

        switch (command) {
            case 'a':
                (*git).add(content);
                break;

            case 'r':
                cout << "Enter the number of the version that you want to delete: ";
                cin >> version;
                cout << endl;

                (*git).remove(version);
                cout << endl;
                break;

            case 'l':
                cout << "Which version would you like to load? " ;
                cin >> version; // take user input for version number
                cout << endl;
                (*git).load(version);
                break;

            case 'p':
                (*git).print();
                break;

            case 'c':
                cout << "Please enter the number of the first version to compare: ";
                cin >> v1;
                cout << endl;

                cout << "Please enter the number of the second version to compare: ";
                cin >> v2;
                cout << endl;

                (*git).compare(v1, v2);
                cout << endl;
                break;
            
            case 's':
                cout << "Please enter the keyword that you are looking for: ";
                cin >> keyword;
                cout << endl;

                (*git).search(keyword);
                break;

            case 'e':
                // Clear all dynamically allocated memory
                f.close();
                delete git;
                return 0;
        }
    }
}*/

// Part 2
int main() {
    
    cout << "Welcome to the Comp322 file versioning system!\n" << endl;
    char command;
    string content;

    int version;
    int v1, v2;
    string keyword;

    EnhancedGit322 *git = new EnhancedGit322();

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
        fstream f((*git).getPath());
        stringstream b;
        
        b << f.rdbuf();
        content = b.str();        

        switch (command) {
            case 'a':
                (*git).add(content);
                break;

            case 'r':
                cout << "Enter the number of the version that you want to delete: ";
                cin >> version;
                cout << endl;

                (*git).remove(version);
                cout << endl;
                break;

            case 'l':
                cout << "Which version would you like to load? " ;
                cin >> version; // take user input for version number
                cout << endl;
                (*git).load(version);
                break;

            case 'p':
                (*git).print();
                break;

            case 'c':
                cout << "Please enter the number of the first version to compare: ";
                cin >> v1;
                cout << endl;

                cout << "Please enter the number of the second version to compare: ";
                cin >> v2;
                cout << endl;

                (*git).compare(v1, v2);
                cout << endl;
                break;
            
            case 's':
                cout << "Please enter the keyword that you are looking for: ";
                cin >> keyword;
                cout << endl;

                (*git).search(keyword);
                break;

            case 'e':
                // Clear all dynamically allocated memory
                f.close();
                delete git;
                return 0;
        }
    }
}
