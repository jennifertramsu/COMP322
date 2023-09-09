/*
 *   COMP322 - Assignment 1
 *   Jennifer Tram Su (260923530)
 */

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Question 1
bool word_diff(string word1, string word2) {
    /*
    Compares two strings and returns true if they were identical, false otherwise.
    */

    int comp = word1.compare(word2); // returns 0 when equal

    if (comp == 0)
    {
        return true; // return 1
    }
    else
    {
        return false; // return 2
    }
}

// Question 2
bool classical_file_diff(string file1, string file2) {
    /*
    Returns a boolean indicating whether two files are identical or not.
    */

    fstream f1(file1);
    fstream f2(file2);

    string l1, l2;
    bool comp; // boolean to store comparison result

    do
    {
        getline(f1, l1); // extract line from file
        getline(f2, l2);

        comp = word_diff(l1, l2); // comparing lines from each file

        if (comp == 0) { // return false for any single line that doesn't match
            f1.close();
            f2.close();
            return false;
        }

    } while (getline(f1, l1) && getline(f2, l2)); // continue until at least one file reaches eof

    if (!f1.good() == !f2.good()) { // both files reached eof
        f1.close();
        f2.close();
        return true;
    } else { // one file has remaining lines, not identical
        f1.close();
        f2.close();
        return false;
    };
}

// Question 3
size_t hash_it(string someString) {
    /*
    Returns the hashed value of a string.
    */
    return hash<string>{}(someString);
}

// Question 4
bool enhanced_file_diff(string file1, string file2) {
    /*
    Checks whether files are identical without using content comparison.
    */

    fstream f1(file1);
    fstream f2(file2);

    ostringstream b1, b2;
    string c1, c2;

    b1 << f1.rdbuf(); // reading entire file contents into buffer
    b2 << f2.rdbuf();

    c1 = b1.str(); // converting from buffer to string
    c2 = b2.str();

    size_t h1, h2;

    h1 = hash_it(c1); // hash all file contents
    h2 = hash_it(c2);

    return h1 == h2;
}

// Q6 helper
void check_words(stringstream *iss1, stringstream *iss2, streampos ip1, streampos ip2, string fn1, string fn2, int lc) {
    /*
    Helper function for list_mismatched_words.
    */
    (*iss1).seekg(ip1); // move cursor to last known position in line
    (*iss2).seekg(ip2);

    if (!(*iss1).good() || !(*iss2).good()) // at least one of the lines has ended
    {
        if ((*iss1).good()) // line1 has remaining words
        {
            string w1;
            *iss1 >> w1; // extract next word in line

            cout << fn1 << ": " << w1 << " (line " << lc << ")" << endl;
            cout << fn2 << ": (line " << lc << ")\n" << endl;

            streampos ip1n = (*iss1).tellg(); // save this position
            
            check_words(iss1, iss2, ip1n, ip2, fn1, fn2, lc); // recursive call to check next word
        }
        else if ((*iss2).good()) // line2 has remaining words
        {
            string w2;
            *iss2 >> w2; // extract next word in line

            cout << fn1 << ": (line " << lc << ")" << endl;
            cout << fn2 << ": " << w2 << " (line " << lc << ")\n" << endl;

            streampos ip2n = (*iss2).tellg(); // save this position

            check_words(iss1, iss2, ip1, ip2n, fn1, fn2, lc); // recursive call to check next word
        } else { // both lines have ended
            return; // end of call
        }
    } else { // neither line has ended
        string w1, w2;

        *iss1 >> w1; // extract next word in line
        *iss2 >> w2;

        streampos ip1n = (*iss1).tellg(); // save cursor position after reading line
        streampos ip2n = (*iss2).tellg();

        // compare hashes
        size_t h1 = hash_it(w1);
        size_t h2 = hash_it(w2);

        if (h1 != h2) { // different words, print mismatch
            cout << fn1 << ": " << w1 << " (line " << lc << ")" << endl;
            cout << fn2 << ": " << w2 << " (line " << lc << ")\n" << endl;
        }

        check_words(iss1, iss2, ip1n, ip2n, fn1, fn2, lc); // recursive call to check next set of words
    }
}

// Q5+6 helper
void check_lines(fstream *f1, fstream *f2, streampos p1, streampos p2, string fn1, string fn2, bool wc, int lc) {
    /*
    Recursive function in list_mismatched_lines and list_mismatched_words that displays all mismatched lines between two files.

    If list_mismatched_lines, wc = false and lc = 0.
    Else if list_mismatched_words, wc = true and lc = 1.
    */
    (*f1).seekg(p1); // move cursor to last known position in file
    (*f2).seekg(p2);

    if (!(*f1).good() || !(*f2).good()) // at least one of the files has reached eof
    {
        if ((*f1).good()) // file1 has remaining lines
        {
            string l1;
            getline(*f1, l1); // extract line from file

            streampos p1n = (*f1).tellg(); // save position in file

            if (wc) { // for Q6, print mismatching words
                stringstream iss1(l1); // creating stringstream from line
                stringstream iss2("");

                stringstream *issp1 = &iss1; // creating pointer reference for function call
                stringstream *issp2 = &iss2;
                                
                check_words(issp1, issp2, 0, 0, fn1, fn2, lc); // send lines to check_words to print mismatched words
            } else { // for Q5, print mismatching lines
                cout << fn1 << ": " << l1 << endl;
                cout << fn2 << ": (eof)" << "\n" << endl; // (eof) to indicate file2 has already reached eof
            }
            
            lc++; // increase line counter for Q6
            check_lines(f1, f2, p1n, p2, fn1, fn2, wc, lc); // recursive call to check next line
        }
        else if ((*f2).good()) // file2 has remaining lines
        {
            string l2;
            getline(*f2, l2); // extract line from file

            streampos p2n = (*f2).tellg(); // save position in file

            if (wc) { // for Q6, print mismatching words
                stringstream iss1(""); // creating stringstream from line
                stringstream iss2(l2);

                stringstream *issp1 = &iss1; // creating pointer reference for function call
                stringstream *issp2 = &iss2;

                check_words(issp1, issp2, 0, 0, fn1, fn2, lc); // send lines to check_words to print mismatching words
            } else { // for Q5, print mismatching lines
                cout << fn1 << ": (eof)" << endl; // (eof) to indicate file1 has already reached eof
                cout << fn2 << ": " << l2 << "\n" << endl;
            }

            lc++; // increase line counter for Q6
            check_lines(f1, f2, p1, p2n, fn1, fn2, wc, lc); // recursive call to check next line
        } else { // both files have reached eof
            return; // end of function call
        }
    } else { // neither file has reached eof
        string l1, l2;

        getline(*f1, l1); // extract line from file
        getline(*f2, l2);

        streampos p1n = (*f1).tellg(); // save cursor position after reading line
        streampos p2n = (*f2).tellg();

        // compare hashes
        size_t h1 = hash_it(l1);
        size_t h2 = hash_it(l2);

        if (h1 != h2) {
            if (wc) { // for Q6, print mismatching words   
                stringstream iss1(l1); // creating stringstream from line
                stringstream iss2(l2);

                stringstream *issp1 = &iss1; // creating pointer reference for function call
                stringstream *issp2 = &iss2;

                check_words(issp1, issp2, 0, 0, fn1, fn2, lc); // send lines to check_words to print mismatching words
            } else { // for Q5, print mismatching lines
                cout << fn1 << ": " << l1 << endl;
                cout << fn2 << ": " << l2 << "\n" << endl;
            }
        }

        lc++; // increase line counter for Q6
        check_lines(f1, f2, p1n, p2n, fn1, fn2, wc, lc); // recursive call to check next lines
    }
}

// Question 5
void list_mismatched_lines(string file1, string file2) {
    /*
    Parent function for recursive call that prints all mismatching lines between two files.
    */
    // Grabbing basenames
    string fn1 = file1.substr(file1.find_last_of("/") + 1);
    string fn2 = file2.substr(file2.find_last_of("/") + 1);

    fstream f1(file1);
    fstream f2(file2);

    fstream *f1p = &f1; // creating pointer reference for function call
    fstream *f2p = &f2;

    check_lines(f1p, f2p, 0, 0, fn1, fn2, false, 0); // start of recursive call

    return;
}

// Question 6
void list_mismatched_words(string file1, string file2) {
    /*
    Parent function for recursive call that prints all mismatching words.
    */
    // grabbing basenames
    string fn1 = file1.substr(file1.find_last_of("/") + 1);
    string fn2 = file2.substr(file2.find_last_of("/") + 1);

    fstream f1(file1);
    fstream f2(file2);

    fstream *f1p = &f1; // creating pointer reference for function call
    fstream *f2p = &f2;

    check_lines(f1p, f2p, 0, 0, fn1, fn2, true, 1); // start of recursive call

    return;
}

int main()
{
    // Q1
    std::string str1 = "Hello World";
    std::string str2 = "hEllO World";
    std::string str3 = "World";
    std::string str4 = "Hello World";

    bool result1 = word_diff(str1, str2); // False
    bool result2 = word_diff(str1, str3); // False
    bool result3 = word_diff(str1, str4); // True

    cout << result1 << endl;
    cout << result2 << endl;
    cout << result3 << endl;

    // Q2
    std::string file1 = "./txt_folder/file1.txt";
    std::string file2 = "./txt_folder/file2.txt";
    bool result4 = classical_file_diff(file1, file2); // False

    cout << result4 << endl;

    // Q3
    std::string mystr = "I love this assignment";
    std::size_t h1 = hash_it (mystr);
    std::cout << h1 << std::endl;

    // Q4
    bool result5 = enhanced_file_diff(file1, file2); // False

    cout << result5 << endl;

    // Q5
    list_mismatched_lines(file1, file2); // This should print to the screen the mismatched lines

    // Q6
    list_mismatched_words(file1, file2); // This should print to the screen the mismatched words
}
