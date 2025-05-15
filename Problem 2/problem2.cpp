#include <bits/stdc++.h>
#include "Vector.hpp"
#include "Queue.hpp"

using namespace std;
#define endl '\n'

struct Contact
{
    string name;
    string phoneNumber;
    string email;
};

class AVLNode
{
public:
    int id;
    Contact contact;
    AVLNode *left;
    AVLNode *right;
    int height;
    AVLNode(int k, Contact c) : id(k), contact(c), left(nullptr), right(nullptr), height(1){}
};

class AVLTree
{
private:
    AVLNode *root;
    static int maxLevel;

    void updateMaxLevel() {
        maxLevel = calculateMaxLevel(root);
    }

    int calculateMaxLevel(AVLNode* node) {
        if (!node) return -1; // Base case: empty tree
        return 1 + max(calculateMaxLevel(node->left), calculateMaxLevel(node->right));
    }

    int height(AVLNode *node)
    {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    int balanceFactor(AVLNode *node)
    {
        if (node == nullptr)
            return 0;
        return height(node->left) - height(node->right);
    }

    AVLNode *rightRotate(AVLNode *y)
    {
        AVLNode *x = y->left;
        AVLNode *T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    AVLNode *leftRotate(AVLNode *x)
    {
        AVLNode *y = x->right;
        AVLNode *T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    AVLNode *insert(AVLNode *node, int id, Contact &c)
    {
        if (node == nullptr)
            return new AVLNode(id, c);

        if (id < node->id)
            node->left = insert(node->left, id, c);
        else if (id > node->id)
            node->right = insert(node->right, id, c);
        else
            return node;

        node->height = 1 + max(height(node->left), height(node->right));

        int balance = balanceFactor(node);

        if (balance > 1 && id < node->left->id)
            return rightRotate(node);

        if (balance < -1 && id > node->right->id)
            return leftRotate(node);

        if (balance > 1 && id > node->left->id)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        if (balance < -1 && id < node->right->id)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        updateMaxLevel(); // Update max level after insertion
        return node;
    }

    AVLNode *minValueNode(AVLNode *node)
    {
        AVLNode *current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    AVLNode *deleteNode(AVLNode *root, int id)
    {
        if (root == nullptr)
            return root;

        if (id < root->id)
            root->left = deleteNode(root->left, id);
        else if (id > root->id)
            root->right = deleteNode(root->right, id);
        else
        {
            if ((root->left == nullptr) || (root->right == nullptr))
            {
                AVLNode *temp = root->left ? root->left : root->right;
                if (temp == nullptr)
                {
                    temp = root;
                    root = nullptr;
                }
                else
                    *root = *temp;
                delete temp;
            }
            else
            {

                AVLNode *temp = minValueNode(root->right);
                root->id = temp->id;
                root->contact = temp->contact;
                root->right = deleteNode(root->right, temp->id);
            }
        }

        if (root == nullptr)
            return root;

        root->height = 1 + max(height(root->left), height(root->right));

        int balance = balanceFactor(root);

        if (balance > 1 && balanceFactor(root->left) >= 0)
            return rightRotate(root);

        if (balance > 1 && balanceFactor(root->left) < 0)
        {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        if (balance < -1 && balanceFactor(root->right) <= 0)
            return leftRotate(root);

        if (balance < -1 && balanceFactor(root->right) > 0)
        {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        updateMaxLevel(); // Update max level after deletion
        return root;
    }

    void inorder(AVLNode *root)
    {
        if (root != nullptr)
        {
            inorder(root->left);
            cout << "ID: " << root->id << ", Name: " << root->contact.name << ", Phone: " << root->contact.phoneNumber << ", Email: " << root->contact.email << endl;
            inorder(root->right);
        }
    }

    AVLNode* searchHelper(AVLNode *root, int id)
    {
        if (root == nullptr)
            return nullptr;
        if (root->id == id)
            return root;
        if (id < root->id)
            return searchHelper(root->left, id);
        return searchHelper(root->right, id);
    }

    Vector<Vector<int>> levelOrderTraversal() {
        Vector<Vector<int>> levels;
        if (!root) return levels;

        Queue<AVLNode*> q;
        q.push(root);
        while (!q.empty()) {
            int size = q.size();
            Vector<int> level;
            for (int i = 0; i < size; ++i) {
                AVLNode* node = q.front();
                q.pop();
                if (node) {
                    level.push_back(node->id);
                    q.push(node->left);
                    q.push(node->right);
                } else {
                    level.push_back(-1); // Placeholder for null nodes
                }
            }
            levels.push_back(level);
        }
        if(levels.size() > maxLevel) levels.pop_back();
        return levels;
    }

public:
    AVLTree(): root(nullptr){}

    bool insert(int id, Contact &c) {
        AVLNode *temp = searchHelper(root, id);
        if (temp != nullptr)
        {
            return false; 
        }
        root = insert(root, id, c); 
        return true;
    }

    bool remove(int id) {
        AVLNode *temp = searchHelper(root, id);
        if (temp == nullptr)
        {
            return false; 
        }
        root = deleteNode(root, id);
        return true;
    }

    AVLNode* search(int id) { 
        return searchHelper(root, id);
        
    }

    void listContacts()
    {
        if(root == nullptr)
        {
            cout << "Address Book is empty!" << endl;
            return;
        }
        cout << "Contacts in Address Book (Sorted by ID)" << endl;
        inorder(root);
    }

    void displayTree() {
        Vector<Vector<int>> levels = levelOrderTraversal();
        int depth = levels.size();
    
        for (int i = 0; i < depth; ++i) {
            int betweenSpaces = (1 << (depth - i + 1)); // Calculate spaces between nodes
            int leadingSpaces = (1 << (depth - i)) - 1; // Calculate leading spaces
    
            for (int j = 0; j < leadingSpaces; ++j) cout << " ";
    
            for (int j = 0; j < levels[i].size(); ++j) {
                if (levels[i][j] != -1) {
                    cout << levels[i][j];
                } else {
                    cout << " ";
                }
                if (j < levels[i].size() - 1) {
                    for (int k = 0; k < betweenSpaces; ++k) cout << " ";
                }
            }
    
            cout << endl;
        }
    }
};

int AVLTree::maxLevel = -1;

class AddressBook
{
private:
    AVLTree avlTree;
public:
    void displayMenu()
    {
        cout << "Address Book Application" << endl;
        cout << "------------------------" << endl;
        cout << "1. Add New Contact" << endl;
        cout << "2. Search for Contact" << endl;
        cout << "3. Delete Contact" << endl;
        cout << "4. List All Contacts (Sorted by ID)" << endl;
        cout << "5. Display AVL Tree" << endl;
        cout << "6. Exit" << endl;
        cout << "------------------------" << endl;
        cout << "Enter operation (1-6): ";
    }
    void addContact()
    {
        Contact c;
        cout << "Enter unique ID (integer): ";
        int id;
        cin >> id;
        cout << "Enter Name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, c.name);
        cout << "Enter Phone: ";
        getline(cin, c.phoneNumber);
        cout << "Enter Email: ";
        getline(cin, c.email);

        if (avlTree.insert(id, c))
            cout << "Contact added successfully!" << endl;
        else
            cout << "Error: Contact with this ID " << id << " already exists!" << endl;
    }
    void searchContact()
    {
        cout << "Enter ID to search: ";
        int id;
        cin >> id;
        AVLNode *result = avlTree.search(id);
        if (result != nullptr)
            cout << "Contact found: ID: " << result->id << ", Name: " << result->contact.name << ", Phone: " << result->contact.phoneNumber << ", Email: " << result->contact.email << endl;
        else
            cout << "Contact with ID " << id << " not found!" << endl;
    }
    void deleteContact()
    {
        cout << "Enter ID to delete: ";
        int id;
        cin >> id;
        if (avlTree.remove(id))
            cout << "Contact deleted successfully!" << endl;
        else
            cout << "Error: Contact with ID " << id << " not found!" << endl;
    }
    void listContacts()
    {
        avlTree.listContacts();
    }
    void run()
    {
        int choice;
        do
        {
            displayMenu();
            cin >> choice;
            switch (choice)
            {
            case 1:
                addContact();
                break;
            case 2:
                searchContact();
                break;
            case 3:
                deleteContact();
                break;
            case 4:
                listContacts();
                break;
            case 5:
                avlTree.displayTree();
                cout << endl;
                break;
            case 6:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
            }
        } while (choice != 6);
    }
};
int main()
{
    AddressBook addressBook;
    addressBook.run();
    return 0;
}