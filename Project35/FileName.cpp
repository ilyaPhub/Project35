#include <iostream>

using namespace std;

struct Node {
    char word[50];
    char translation[50];
    int accessCount;
    Node* left;
    Node* right;

    Node(const char* w, const char* t, int count = 0) : accessCount(count), left(nullptr), right(nullptr) {
        copyString(word, w);
        copyString(translation, t);
    }

    void copyString(char* dest, const char* src) {
        int i = 0;
        while (src[i] != '\0' && i < 49) {
            dest[i] = src[i];
            i++;
        }
        dest[i] = '\0'; 
    }

    int compareStrings(const char* str1, const char* str2) {
        int i = 0;
        while (str1[i] != '\0' && str2[i] != '\0') {
            if (str1[i] != str2[i]) {
                return str1[i] - str2[i];
            }
            i++;
        }
        return str1[i] - str2[i];
    }
};

class Dictionary {
private:
    Node* root;

    Node* insert(Node* node, const char* word, const char* translation, int count) {
        if (!node)
            return new Node(word, translation, count);
        if (node->compareStrings(word, node->word) < 0)
            node->left = insert(node->left, word, translation, count);
        else if (node->compareStrings(word, node->word) > 0)
            node->right = insert(node->right, word, translation, count);
        return node;
    }

    Node* find(Node* node, const char* word) {
        if (!node || node->compareStrings(word, node->word) == 0) {
            if (node) node->accessCount++; 
            return node;
        }
        if (node->compareStrings(word, node->word) < 0)
            return find(node->left, word);
        return find(node->right, word);
    }

    void findPopularUnpopular(Node* node, Node*& popular, Node*& unpopular) {
        if (!node) return;
        if (!popular || node->accessCount > popular->accessCount)
            popular = node;
        if (!unpopular || node->accessCount < unpopular->accessCount)
            unpopular = node;
        findPopularUnpopular(node->left, popular, unpopular);
        findPopularUnpopular(node->right, popular, unpopular);
    }

public:
    Dictionary() : root(nullptr) {}

    void addWord(const char* word, const char* translation, int count = 0) {
        root = insert(root, word, translation, count);
    }

    void showTranslation(const char* word) {
        Node* node = find(root, word);
        if (node) {
            cout << "Translation: " << node->translation << endl;
        }
        else {
            cout << "Word not found." << endl;
        }
    }

    void updateTranslation(const char* word, const char* newTranslation) {
        Node* node = find(root, word);
        if (node) {
            node->copyString(node->translation, newTranslation);
            cout << "Translation updated." << endl;
        }
        else {
            cout << "Word not found." << endl;
        }
    }

    void deleteWord(Node*& node, const char* word) {
        if (!node) return;

        if (node->compareStrings(word, node->word) < 0)
            deleteWord(node->left, word);
        else if (node->compareStrings(word, node->word) > 0)
            deleteWord(node->right, word);
        else {
            if (!node->left) {
                Node* temp = node->right;
                delete node;
                node = temp;
            }
            else if (!node->right) {
                Node* temp = node->left;
                delete node;
                node = temp;
            }
            else {
                Node* temp = node->right;
                while (temp->left) temp = temp->left;
                node->copyString(node->word, temp->word);
                node->copyString(node->translation, temp->translation);
                node->accessCount = temp->accessCount;
                deleteWord(node->right, temp->word);
            }
            cout << "Word deleted." << endl;
        }
    }

    void deleteWord(const char* word) {
        deleteWord(root, word);
    }

    void showMostPopularWord() {
        Node* popular = nullptr;
        Node* unpopular = nullptr;
        findPopularUnpopular(root, popular, unpopular);
        if (popular) {
            cout << "Most popular word: " << popular->word << " (access count: " << popular->accessCount << ")" << endl;
        }
    }

    void showLeastPopularWord() {
        Node* popular = nullptr;
        Node* unpopular = nullptr;
        findPopularUnpopular(root, popular, unpopular);
        if (unpopular) {
            cout << "Least popular word: " << unpopular->word << " (access count: " << unpopular->accessCount << ")" << endl;
        }
    }
};

int main() {
    Dictionary dict;
    dict.addWord("hello", "привет", 5);
    dict.addWord("world", "мир", 3);
    dict.addWord("apple", "€блоко", 1);

    dict.showTranslation("hello");
    dict.showMostPopularWord();
    dict.showLeastPopularWord();

    dict.updateTranslation("world", "земл€");
    dict.deleteWord("apple");

    return 0;
}
