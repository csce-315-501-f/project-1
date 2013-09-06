#ifndef ATTRIBUTE
#define ATTRIBUTE

#include <vector>
#include <queue>
#include <algorithm>
#include <string>
using namespace std;


class Attribute{
    vector<string> values;
    
    // available types are string and integer
    string type;
    int length;

    queue<int> freeKeys;

  public:
    Attribute() : type(""), length(0) {}
    Attribute(string attrType, int len) {
        transform(attrType.begin(), attrType.end(), attrType.begin(), ::tolower);
        if (attrType != "string" || attrType != "integer") {
            // error, type can only be string or integer
        }
        type = attrType;
        length = len;
    }

    // checks if the Attribute is empty if no key given
    // check if values[key] is empty is key given
    // returns false if `key` is not empty, or does not exist
    bool isEmpty(int key = -1) {
        if (key == -1) {
            return length == 0 && type == "";
        }
        else {
            if (values.size() <= key) return false;
            if (values[key] == "") return true;
            else return false;
        }
    }

    string getType() { return type; }
    int getSize() { return values.size(); }
    
    string getValue(int key) {
        return values[key];
    }
    
    /*
     * Returns key for found `value`
     * or -1 if not found
     */
    int findValue(string value) {
        for (int i = 0; i < values.size(); ++i) {
            if (values[i] == value) {
                return i;
            }
        }
        return -1;
    }

    void removeValue(int key) {
        // Check if key exists in `values`
        if (values.size() > key) {
            values[key] = "";
            freeKeys.push(key);
        }
        else {
            // throw an error for key not being in `values`
        }
    }

    void removeLast() {
        values.pop_back();
    }

    /*
     * Adds a new value to the `values` vector at `key`
     * if `key` is not passed, then `val` is insterted at the first
     * free key of `values`
     * Returns key of where `val` was inserted
     * Return value of -1 means `val` was not inserted
     */
    int addValue(string val, int key = -1) {
        int insertedKey = key;

        if (type == "string" && val.size() > length) {
            // error, trying to insert string larger than `length`
            return -1;
        }

        if (key == -1) {
            if (freeKeys.empty()) {
                // There are no free keys, insert new `val` at end
                insertedKey = values.size();
                values.push_back(val);
                return insertedKey;
            }
            else {
                // There is a free key, insert `val` at that key
                insertedKey = freeKeys.front();
                freeKeys.pop();
                values[insertedKey] = val;
                return insertedKey;
            }
        }
        else {
            if (values.size() <= key) {
                //throw exception because inserting at key not in vector
                return -1;
            }
            else {
                values[insertedKey] = val;
                return insertedKey;
            }
        }
    }
    
    bool updateValue(int key, string val) {
        if (type == "string" && val.size() > length) {
            // error, trying to insert string larger than `length`
            return false;
        }
        if (values.size() <= key) {
            // error, trying to update non-existant key
            return false;
        }
        values[key] = val;
    }

    void setLength(int len) { length = len; }
};

#endif
