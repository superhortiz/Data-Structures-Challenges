#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::string;
using std::vector;
using std::cin;

struct Query {
    string type, name;
    int number;
};

vector<Query> read_queries() {
    int n;
    cin >> n;
    vector<Query> queries(n);
    for (int i = 0; i < n; ++i) {
        cin >> queries[i].type;
        if (queries[i].type == "add")
            cin >> queries[i].number >> queries[i].name;
        else
            cin >> queries[i].number;
    }
    return queries;
}

void write_responses(const vector<string>& result) {
    for (size_t i = 0; i < result.size(); ++i)
        std::cout << result[i] << "\n";
}

vector<string> process_queries(const vector<Query>& queries) {
    vector<string> result;
    std::unordered_map<int, string> contacts;

    for (size_t i = 0; i < queries.size(); ++i)
        // Add to contacts
        if (queries[i].type == "add") {
            contacts[queries[i].number] = queries[i].name;
            
        // Remove from contacts
        } else if (queries[i].type == "del") {
            contacts.erase(queries[i].number);

        // Find in contacts
        } else {
            string response = "not found";
            if (contacts.count(queries[i].number) > 0) {
                response = contacts[queries[i].number];
            }
            result.push_back(response);
        }
    return result;
}

int main() {
    write_responses(process_queries(read_queries()));
    return 0;
}
