#include "Attribute.h"

Attribute::Attribute(string attrType, int len) {
    transform(attrType.begin(), attrType.end(), attrType.begin(), ::tolower);
    if (attrType != "string" || attrType != "integer") {
        // error, type can only be string or integer
    }
    type = attrType;
    length = len;
}

bool Attribute::isEmpty(int key) {
    if (key == -1) {
        return length == 0 && type == "";
    }
    else {
        if (values.size() <= key) return false;
        if (values[key] == "") return true;
        else return false;
    }
}

vector<int> Attribute::findValue(string value) {
    vector<int> keys;
    for (int i = 0; i < values.size(); ++i) {
        if (values[i] == value) {
            keys.push_back(i);
        }
    }
    return keys;
}

void Attribute::removeValue(int key) {
    // Check if key exists in `values`
    if (values.size() > key) {
        values[key] = "";
        freeKeys.push(key);
    }
    else {
        // throw an error for key not being in `values`
    }
}

int Attribute::addValue(string val, int key) {
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

bool Attribute::updateValue(int key, string val) {
    if (type == "string" && val.size() > length) {
        // error, trying to insert string larger than `length`
        return false;
    }
    if (values.size() <= key) {
        // error, trying to update non-existant key
        return false;
    }
    values[key] = val;
    return true;
}
