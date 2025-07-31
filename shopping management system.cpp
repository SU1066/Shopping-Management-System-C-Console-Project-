#include<iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <cctype>
#include <string>

using namespace std;

//Global variables
bool loggedIn = false;
string currentUsername = "";
string currentUserRole = "";

//structs
struct User
{
    string username;
    string password;
    string role;      // "admin" or "user"
    User* next;
};

struct product
{
    string name;
    string category;
    int price;
    int id;
    product *left;
    product *right;
    int height;

    product(string name, string category, int price, int id)
    {
        this->name = name;
        this->category = category;
        this->price = price;
        this->id = id;
        left = right = nullptr;
        height = 1;
    }
};

struct Cart
{
    product* productItem; // pointer to the product
    int quantity = 0;
    Cart *next;
};

struct CartStack
{
    int productId;
    int quantity;
};

const int MAX = 100;
CartStack c[MAX];
int top = -1;

struct Wishlist
{
    product* item;
    Wishlist* next;
};

// Function declarations
void userMenu(product* root, Cart*& cart);
void adminMenu();
void deleteUsers(User*& head);
bool isValidUsername(const string& uname);
bool isValidPassword(const string& pass);
void insertAtEnd(User*& head, string uname, string pass, string role);
void displayUsers(User* head);
void saveusersToFile(User* head, const string& filename);
void loadUsersFromFile(User*& head, const string& filename);
string loginUser(User* head, string uname, string pass);
void registerUser(User*& head);
void login(User*& head);
void deleteUsers(User*& head);
int getHeight(product *n);
int getBalance(product *n);
void updateHeight(product *node);
product* rightRotate(product *y);
product* leftRotate(product *y);
product* insertion(product *root, string name, string category, int price, int id);
product* loadFromFile(string filename, product* root);
void inOrder(product* root);
void addToCart(Cart *&cart, product *productItem, int q);
void deleteFromCart(Cart *&cart, int id);
void editCart(Cart *&cart);
void clearCart(Cart *&cart);
void displayCart(Cart *cart);
void checkoutCart(Cart *&cart);
void writeWishlistToFile(Wishlist* wishlist);
void addToWishlist(Wishlist*& wishlist, product* item);
void displayWishlistFromFile();
bool searchByName(product* root, string name);
product* searchById(product* root, int id);
bool searchByCategory(product* root, string category);
void file(string filename);
void undoCart(Cart *&cart);
void push(Cart *&temp);
void pop(Cart *&temp);
// AVL tree functions
product* insertion(product* root, string name, string category, int price, int id);
product* deleteProductById(product* root, int id, string filename);
product* deleteProduct(product* root, int id);
void updateItemsFile(product* root, string filename);
void saveToFile(product* root, ofstream& file);
void inOrder(product* root);


// Validation for username: all lowercase, no spaces
bool isValidUsername(const string& uname)
{
    if (uname.empty()) return false;
    for (char c : uname)
    {
        if (!islower(c) || isspace(c))
        {
            return false;
        }
    }
    return true;
}

// Validation for password: at least one digit and one uppercase letter
bool isValidPassword(const string& pass)
{
    if (pass.empty()) return false;
    bool hasDigit = false, hasUpper = false;
    for (char c : pass)
    {
        if (isdigit(c)) hasDigit = true;
        if (isupper(c)) hasUpper = true;
    }
    return hasDigit && hasUpper;
}

// Insert a new user at the end of the linked list
void insertAtEnd(User*& head, string uname, string pass, string role)
{
    User* p = new User;
    p->username = uname;
    p->password = pass;
    p->role = role;
    p->next = NULL;

    if (head == NULL)
    {
        head = p;
    }
    else
    {
        User* q = head;
        while (q->next != NULL)
        {
            q = q->next;
        }
        q->next = p;
    }
}

// Display all users (for testing)
void displayUsers(User* head)
{
    User* temp = head;
    if (temp == NULL)
    {
        cout << "\n No users found.\n";
    }
    else
    {
        cout << "\n Registered Users:\n";
        while (temp != NULL)
        {
            cout << "\n Username: " << temp->username << ", Role: " << temp->role << endl;
            temp = temp->next;
        }
    }
}

// Save all users to file
void saveUsersToFile(User* head, const string& filename)
{
    ofstream outFile(filename);
    User* temp = head;
    while (temp != NULL)
    {
        outFile << temp->username << " " << temp->password << " " << temp->role << endl;
        temp = temp->next;
    }
    outFile.close();
}

// Load users from file
void loadUsersFromFile(User*& head, const string& filename)
{
    ifstream inFile(filename);
    string uname, pass, role;
    while (inFile >> uname >> pass >> role)
    {
        insertAtEnd(head, uname, pass, role);
    }
    inFile.close();
}

// Check if username and password match
string loginUser(User* head, string uname, string pass)
{
    User* temp = head;
    while (temp != NULL)
    {
        if (temp->username == uname && temp->password == pass)
        {
            loggedIn = true;
            currentUsername = uname;
            currentUserRole = temp->role;
            return temp->role;
        }
        temp = temp->next;
    }
    return "none";
}

// Register a new user
void registerUser(User*& head)
{
    system("cls");
    string uname, pass, role;
    cout << "\n             REGISTRATION \n";
    cout << "--------------------------------------\n";

    cout << "\n Register as:\n 1. Admin\n 2. User\n\n Enter your role choice: ";
    int r;
    cin >> r;

    if (r == 1) role = "admin";
    else if (r == 2) role = "user";
    else
    {
        cout << " \n Invalid role selected.\n";
        system("pause");
        return;
    }

    cout << " \nEnter new username (lowercase, no spaces): ";
    cin >> uname;
    if (!isValidUsername(uname))
    {
        cout << "\n Invalid username! Must be all lowercase with no spaces.\n";
        system("pause");
        return;
    }

    // Check if username already exists
    User* temp = head;
    while (temp != NULL)
    {
        if (temp->username == uname)
        {
            cout << "  \nUsername already exists! Please choose a different username.\n";
            system("pause");
            return;
        }
        temp = temp->next;
    }

    cout << " \nEnter new password (must contain at least one digit and one uppercase letter): ";
    cin >> pass;
    if (!isValidPassword(pass))
    {
        cout << " \nInvalid password! Must contain at least one digit and one uppercase letter.\n";
        system("pause");
        return;
    }

    insertAtEnd(head, uname, pass, role);
    saveUsersToFile(head, "users.txt");

    cout << "\nUser registered successfully!";
    cout << "\n Press Enter to continue...";
    cin.ignore();
    cin.get();
}

// User login
void login(User*& head)
{
    system("cls");
    string uname, pass;
    cout << "\n             LOGIN \n";
    cout << "--------------------------------------\n";
    cout << "\n Enter username: ";
    cin >> uname;
    cout << "\n Enter password: ";
    cin >> pass;

    string role = loginUser(head, uname, pass);

    if (role == "admin")
    {
        cout << "\n Admin login successful. Welcome, " << uname << "!\n";
        cout << "\n Press Enter to continue...";
        cin.ignore();
        cin.get();

        currentUserRole = "admin";
    }
    else if (role == "user")
    {
        cout << "\n User login successful. Welcome, " << uname << "!\n";
        cout << "\n Press Enter to continue...";
        cin.ignore();
        cin.get();

        currentUserRole = "user";
    }
    else
    {
        cout << "\n Login failed. Invalid credentials.\n";
        cout << "\n Press Enter to continue...";
        cin.ignore();
        cin.get();

    }
}
void deleteUser(User*& head, string name,string password)
{
    User* current = head;
    User* prev = nullptr;
    bool deleted=false;
    while (current != nullptr)
    {

        if ((current->username == name) &&(current->password == password)  && (current->role=="user"))
        {
            if (prev == nullptr)
            {
                // Deleting the head node
                head = current->next;
            }
            else
            {
                prev->next = current->next;
            }

            delete current;
            deleted=true;
            return; // Done
        }

        prev = current;
        current = current->next;
    }
    if (deleted){
             cout<<"\n Deletion complete!"<<endl;

    }else{
        cout<<"\n User not found"<<endl;
    }


}

// Delete all users (cleanup)
// Prevents memory leaks by deleting all nodes in the list
void deleteUsers(User*& head)
{
    User* temp;
    while (head != NULL)
    {
        temp = head; // Store current node
        head = head->next;// Move to next node
        delete temp;// Delete current node
    }
}
// AVL Tree helper function - returns height of a node
// Returns 0 for null nodes, actual height for valid nodes
int getHeight(product *n)
{
    return n ? n->height : 0;
}

// AVL Tree helper function - calculates balance factor
// Balance factor = height(left subtree) - height(right subtree)
int getBalance(product *n)
{
    return n ? getHeight(n->left) - getHeight(n->right) : 0;
}
// AVL Tree helper function - updates height of a node
// Height = 1 + max(left_height, right_height)
//height is updated after every insertion/deletion
void updateHeight(product *node)
{
    if (node)
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
}

// AVL Tree right rotation for balancing
// Used when left subtree is heavier (left-left or left-right case)
product* rightRotate(product *y)
{
    product *x = y->left;   // New root after rotation
    product *t2 = x->right; // Subtree to be moved


    // Perform rotation
    x->right = y;
    y->left = t2;

    // Update heights (order matters: y first, then x)
    updateHeight(y);
    updateHeight(x);
    return x;// Return new root
}

// AVL Tree left rotation for balancing
// Used when right subtree is heavier (right-right or right-left case)
product* leftRotate(product *y)
{
    product *x = y->right;     // New root after rotation
    product *t2 = x->left;     // Subtree to be moved

    // Perform rotation
    x->left = y;
    y->right = t2;

    // Update heights (order matters: y first, then x)
    updateHeight(y);
    updateHeight(x);
    return x; // Return new root
}

// AVL Tree insertion function with automatic balancing
// Inserts new product and maintains AVL tree properties
product* insertion(product *root, string name, string category, int price, int id)
{
    // Base case: create new node if position found
    if (!root)
        return new product(name, category, price, id);

    // Recursive insertion based on ID comparison
    if (id < root->id)
        root->left = insertion(root->left, name, category, price, id);
    else if (id > root->id)
        root->right = insertion(root->right, name, category, price, id);
    else
        return root; // Duplicate IDs not allowed

    // Update height of current node
    updateHeight(root);
    int balance = getBalance(root);

    // Perform rotations if tree becomes unbalanced
    // Left-Left case
    if (balance > 1 && id < root->left->id)
        return rightRotate(root);

    // Right-Right case
    if (balance < -1 && id > root->right->id)
        return leftRotate(root);

    // Left-Right case
    if (balance > 1 && id > root->left->id)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right-Left case
    if (balance < -1 && id < root->right->id)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root; // Return unchanged root if balanced
}


// Function to load products from file into AVL tree
// Creates default products if file doesn't exist
product* loadFromFile(string filename, product* root)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "\n Items file not found! Creating default items..." << endl;
        // Create default items file with sample products
        ofstream defaultFile(filename);
        defaultFile << "ID Category Name Price\n";
        defaultFile << "1 Shirts Cotton_Shirt 1500\n";
        defaultFile << "2 Pants Jeans 2500\n";
        defaultFile << "3 Shoes Sneakers 3000\n";
        defaultFile << "4 Shirts Formal_Shirt 2000\n";
        defaultFile << "5 Pants Trousers 1800\n";
        defaultFile.close();

        // Reopen the newly created file
        file.open(filename);
    }

    int id, price;
    string category, name;

    // Skip header line
    string header;
    getline(file, header);

    // Read each product and insert into AVL tree
    while (file >> id >> category >> name >> price)
    {
        root = insertion(root, name, category, price, id);
    }

    file.close();
    return root;
}

// Function to display all products in sorted order (in-order traversal)
// Shows products sorted by ID in ascending order
void inOrder(product* root)
{
    if (root==nullptr) return;

    inOrder(root->left);  // Visit left subtree

    // Display current node's data
    cout << left << setw(5) << root->id
         << setw(15) << root->category
         << setw(25) << root->name
         << "Rs. " << root->price << endl;

    inOrder(root->right); // Visit right subtree
}

product* minValueNode(product* node)
{
    product* current = node;
    while (current && current->left != nullptr)
        current = current->left;
    return current;
}

product* deleteProduct(product* root, int id)
{
    if (!root) return root;

    if (id < root->id)
        root->left = deleteProduct(root->left, id);
    else if (id > root->id)
        root->right = deleteProduct(root->right, id);
    else
    {
        // Node found
        if (!root->left || !root->right)
        {
            product* temp = root->left ? root->left : root->right;
            delete root;
            return temp;
        }
        else
        {
            product* temp = minValueNode(root->right);
            root->id = temp->id;
            root->name = temp->name;
            root->category = temp->category;
            root->price = temp->price;
            root->right = deleteProduct(root->right, temp->id);
        }
    }

    // Update height and balance
    updateHeight(root);
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}
void saveToFile(product* root, ofstream& file)
{
    if (!root) return;
    saveToFile(root->left, file);
    file << root->id << " " << root->category << " " << root->name << " " << root->price << "\n";
    saveToFile(root->right, file);
}


void updateItemsFile(product* root, string filename)
{
    ofstream file(filename);
    if (!file.is_open())
    {
        cout << "Error opening file for writing.\n";
        return;
    }
    file << "ID Category Name Price\n";
    saveToFile(root, file);
    file.close();
}

product* deleteProductById(product* root, int id, string filename)
{
    root = deleteProduct(root, id);           // Remove from AVL
    updateItemsFile(root, filename);          // Rewrite file with updated tree
    cout << "Product with ID " << id << " deleted successfully.\n";
    return root;
}


// Function to add products to shopping cart
// Updates quantity if product already exists, otherwise adds new item
void addToCart(Cart *&cart, product *productItem, int q)
{
    // Check if product already exists in cart
    Cart* temp = cart;
    while (temp != nullptr)
    {
        if (temp->productItem->id == productItem->id)
        {
            // Update existing item quantity
            temp->quantity += q;
            cout << "\n Product quantity updated in cart!\n";
            return;
        }
        temp = temp->next;
    }

    // Create new cart item for new product
    Cart *p = new Cart;
    p->productItem = productItem;
    p->quantity = q;
    p->next = nullptr;

    // Add to cart (at the end of linked list)
    if (cart == nullptr)
    {
        cart = p; // First item in cart
    }
    else
    {
        // Find end of list and append
        Cart *temp = cart;
        while (temp->next != nullptr)
            temp = temp->next;
        temp->next = p;
    }

    // Push to undo stack for undo functionality
    push(p);
    cout << "\n Product added to cart successfully!\n";
}

// Function to remove specific product from cart by ID
// Handles both first item and middle/end item removal
void deleteFromCart(Cart *&cart, int id)
{
    // Check if cart is empty
    if (cart == nullptr)
    {
        cout << "\n Cart is empty. No items to delete." << endl;
        return;
    }

    // Special case: removing first item
    if (cart->productItem->id == id)
    {
        Cart* temp = cart;
        cart = cart->next;     // Update head pointer
        delete temp;           // Free memory
        cout << "\n Product removed from cart successfully!\n";
        return;
    }

    // Search for item to remove in rest of list
    Cart *current = cart;
    Cart *previous = nullptr;

    while (current != nullptr && current->productItem->id != id)
    {
        previous = current;
        current = current->next;
    }

    // Item not found
    if (current == nullptr)
    {
        cout << "\n Product with ID " << id << " not found in cart.\n";
        return;
    }

    // Remove item from linked list
    previous->next = current->next;
    delete current;
    cout << "\n Product removed from cart successfully!\n";
}

// Function to edit quantity of existing cart items
// Allows users to modify quantities or remove items (by setting quantity to 0)
void editCart(Cart *&cart)
{
    int quantity, pid;

    cout << "\n Editing product quantity in cart" << endl;
    cout << "\n Enter product ID to edit: ";
    cin >> pid;

    // Check if cart is empty
    if (cart == nullptr)
    {
        cout << "\n Cart is empty. Nothing to edit." << endl;
        return;
    }

    // Search for product in cart
    Cart *temp = cart;
    while (temp != nullptr && temp->productItem->id != pid)
    {
        temp = temp->next;
    }

    // Product not found in cart
    if (temp == nullptr)
    {
        cout << "Product with ID " << pid << " not found in cart." << endl;
        return;
    }

    // Show current quantity and get new quantity
    cout << "\n Current quantity: " << temp->quantity << endl;
    cout << "\n Enter new quantity (0 to remove): ";
    cin >> quantity;

    if (quantity <= 0)
    {
        // Remove item if quantity is 0 or negative
        deleteFromCart(cart, pid);
    }
    else
    {
        // Update quantity
        temp->quantity = quantity;
        cout << "\n Quantity updated successfully!" << endl;
    }
}

// Function to push cart item to undo stack
// Enables undo functionality by storing recent cart additions
void push(Cart *&temp)
{
    // Check for stack overflow
    if(top==MAX-1)
    {
        cout<<"\n Stack overflow";
        return;
    }
    else
    {
        top++;
        // Store product ID and quantity for undo purposes
        c[top].productId=temp->productItem->id;
        c[top].quantity=temp->quantity;
    }
}

// Function to pop from undo stack and remove last added item
// Implements undo functionality by removing most recent cart addition
void pop(Cart *&cart)
{
    Cart* prev = nullptr;

    // Check for stack underflow
    if(top==-1)
    {
        cout<<"\n Stack underflow";
    }
    else
    {
        // Find last item in cart
        Cart *temp=cart;
        while (temp->next!=nullptr)
        {
            prev = temp;
            temp=temp->next;
        }

        // Verify that last cart item matches top of stack
        if((temp->productItem->id==c[top].productId)&&(temp->quantity==c[top].quantity))
        {
            cout<<"\n Matches the last item in stack and linkedlist"<<endl;
            cout << "\n Undoing: " << temp->productItem->name << ", Qty: " << temp->quantity << endl;

            // Clear stack entry
            c[top].productId=0;
            c[top].quantity=0;

            // Remove last node from cart
            if (prev == nullptr)
            {
                // Only one item in cart
                delete cart;
                cart = nullptr;
            }
            else
            {
                // Multiple items, remove last one
                delete temp;
                prev->next = nullptr;
            }

            top--; // Pop from stack
        }
        else
        {
            cout << "\n Top of stack and last cart item don't match!\n";
        }
    }
}

// Function to undo last cart action
// Wrapper function that calls pop to remove last added item
void undoCart(Cart *&cart)
{
    pop(cart);
}

// Function to completely clear the shopping cart
// Removes all items and frees memory
void clearCart(Cart *&cart)
{
    Cart *temp;

    // Check if cart is already empty
    if (cart == nullptr)
    {
        cout << "\n Cart is already empty." << endl;
        return;
    }

    // Delete all nodes in the cart
    while(cart != nullptr)
    {
        temp = cart;
        cart = cart->next;
        delete temp;           // Free memory for each node
    }

    cout << "\n Cart cleared successfully!" << endl;
}

// Function to display all items currently in the shopping cart
// Shows detailed information and calculates total cart value
void displayCart(Cart *cart)
{
    cout<<endl;
    // Check if cart is empty
    if (cart == nullptr)
    {
        cout << "Cart is empty." << endl;
        return;
    }

    Cart *q = cart;
    double totalAmount = 0;


    cout << left << setw(5) << "ID" << setw(15) << "Name" << setw(15) << "Category"
         << setw(10) << "Price" << setw(8) << "Qty" << setw(12) << "Total" << endl;
    cout << string(70, '-') << endl;

    // Display each cart item with calculations
    while (q != nullptr)
    {
        double itemTotal = q->quantity * q->productItem->price;
        totalAmount += itemTotal;

        cout << left << setw(5) << q->productItem->id
             << setw(15) << q->productItem->name
             << setw(15) << q->productItem->category
             << setw(10) << q->productItem->price
             << setw(8) << q->quantity
             << setw(12) << itemTotal << endl;
        q = q->next;
    }

    // Display cart summary
    cout << string(70, '-') << endl;
    cout << "\n Total Cart Value: Rs. " << totalAmount << endl;
}

// Function to get current invoice number from file
// Maintains persistent invoice numbering across program runs
int getInvoiceNumberFromFile()
{
    ifstream in("invoice_counter.txt");
    int num = 10001; // Default starting invoice number

    // Read current number from file, use default if file doesn't exist
    if (in >> num)
    {
        in.close();
        return num;
    }
    return 10001; // Return default if file read fails
}

// Function to update invoice number in file
// Saves next invoice number to maintain sequence
void updateInvoiceNumberToFile(int newNumber)
{
    ofstream out("invoice_counter.txt");
    out << newNumber;    // Write new number to file
    out.close();
}

// Function to handle cart checkout process
// Creates invoice, calculates totals, saves bill to file, and clears cart
void checkoutCart(Cart *&cart)
{

    // Check if cart is empty before proceeding
    if (cart == nullptr)
    {
        cout << "\n Cart is empty! Cannot checkout!" << endl;
        return;
    }

    // Get unique invoice number from persistent storage
    int invoiceNumber = getInvoiceNumberFromFile();  // Persistent invoice number

    // Initialize calculation variables
    double subtotal = 0.0, tax = 0.0, total = 0.0;

    // Get current date and time for invoice
    time_t now = time(0);
    tm *ltm = localtime(&now);

    // Create unique bill filename using username and invoice number
    string billname = "bill_" + currentUsername + "_" + to_string(invoiceNumber) + ".txt";
    ofstream file(billname);

    // Display and write invoice header to both console and file
    cout << "\n\n\t\t\t==================================================\n";
    cout << "\t\t\t                SHOPPING PRO MAX                  \n";
    cout << "\t\t\t            Official Clothing Store               \n";
    cout << "\t\t\t==================================================\n";
    cout << "\t\t\t                INVOICE #" << invoiceNumber << "\n";
    cout << "\t\t\t                Customer: " << currentUsername << "\n";
    cout << "\t\t\t                Date: " << ltm->tm_mday << "-"
         << 1 + ltm->tm_mon << "-" << 1900 + ltm->tm_year << "\n";
    cout << "\t\t\t--------------------------------------------------\n";
    cout << "\t\t\tProduct\t\t Qty\t    Price\t   Total\n";
    cout << "\t\t\t--------------------------------------------------\n";

    // Write same header information to file
    file << "\n\n\t\t\t==================================================\n";
    file << "\t\t\t                SHOPPING PRO MAX                  \n";
    file << "\t\t\t            Official Clothing Store               \n";
    file << "\t\t\t==================================================\n";
    file << "\t\t\t                INVOICE #" << invoiceNumber << "\n";
    file << "\t\t\t                Customer: " << currentUsername << "\n";
    file << "\t\t\t                Date: " << ltm->tm_mday << "-"
         << 1 + ltm->tm_mon << "-" << 1900 + ltm->tm_year << "\n";
    file << "\t\t\t--------------------------------------------------\n";
    file << "\t\t\tProduct\t\t Qty\t    Price\t   Total\n";
    file << "\t\t\t--------------------------------------------------\n";

    // Iterate through cart items to calculate totals and display details
    Cart* temp = cart;
    while (temp != nullptr)
    {
        // Calculate total for current item (quantity * price)
        double itemTotal = temp->quantity * temp->productItem->price;
        subtotal += itemTotal;

        // Display item details to console
        cout << "\t\t\t" << setw(15) << left << temp->productItem->name
             << "\t  " << temp->quantity
             << "\t   " << "Rs." << temp->productItem->price
             << "\t  " << "Rs." << itemTotal << "\n";

        // Write same item details to file
        file << "\t\t\t" << setw(15) << left << temp->productItem->name
             << "\t  " << temp->quantity
             << "\t   " << "Rs." << temp->productItem->price
             << "\t  " << "Rs." << itemTotal << "\n";

        // Move to next item in cart
        temp = temp->next;
    }

    // Calculate tax (17% of subtotal) and grand total
    tax = subtotal * 0.17;
    total = subtotal + tax;

    // Display final totals to console
    cout << "\t\t\t--------------------------------------------------\n";
    cout << "\t\t\t\t\t\tSubtotal:        Rs. " << fixed << setprecision(2) << subtotal << "\n";
    cout << "\t\t\t\t\t\tSales Tax (17%):  Rs. " << tax << "\n";
    cout << "\t\t\t\t\t\tGrand Total:     Rs. " << total << "\n";
    cout << "\t\t\t==================================================\n";
    cout << "\t\t\t          Thank you for shopping with us!           \n";
    cout << "\t\t\t==================================================\n";

    // Write final totals to file
    file << "\t\t\t--------------------------------------------------\n";
    file << "\t\t\t\t\t\tSubtotal:        Rs. " << fixed << setprecision(2) << subtotal << "\n";
    file << "\t\t\t\t\t\tSales Tax (17%):  Rs. " << tax << "\n";
    file << "\t\t\t\t\t\tGrand Total:     Rs. " << total << "\n";
    file << "\t\t\t==================================================\n";
    file << "\t\t\t          Thank you for shopping with us!           \n";
    file << "\t\t\t==================================================\n";

    // Close the bill file
    file.close();

    // Update invoice number for next checkout
    updateInvoiceNumberToFile(invoiceNumber + 1);

    // Clear the cart after successful checkout
    clearCart(cart);
    cout << "\n Checkout completed!"<< endl;
}

// Function to display order history for current user
// Searches for and displays all previous bill files for the logged-in user
void displayOrderHistory()
{

    // Create search pattern for user's bill files
    string pattern = "bill_" + currentUsername + "_";
    bool found = false;

    // Search through reasonable range of invoice numbers to find user's bills
    for (int i = 10001; i <= 15000; i++)  // reasonable range for invoice numbers
    {
        // Construct filename based on pattern and invoice number
        string filename = pattern + to_string(i) + ".txt";
        ifstream file(filename);

        // If file exists, display its contents
        if (file.is_open())
        {
            found = true;
            string line;
            // Read and display each line of the bill file
            while (getline(file, line))
            {
                cout << line << "\n";
            }
            file.close();
            // Add separator between different bills
            cout << "\n" << string(90, '*') << "\n";
        }
    }

    // Display message if no previous orders found
    if (!found)
    {
        cout << "\n No previous orders found for " << currentUsername << ".\n";
    }
}

// Function to remove a specific item from wishlist by ID
// Updates both memory and file storage
void removeFromWishlist(Wishlist*& wishlist, int id)
{
    // Check if wishlist is empty
    if (wishlist == nullptr)
    {
        cout << "\n wishlist is empty. No items to delete." << endl;
        return;
    }

    // Special case: if first item matches the ID to remove
    if (wishlist->item->id == id)
    {
        Wishlist* temp = wishlist;
        wishlist = wishlist->next;  // Update head pointer
        delete temp;                // Free memory
        writeWishlistToFile(wishlist);  // Update file
        cout << "\n Product removed from wishlist successfully!\n";
        return;
    }

    // Search for item to remove in the rest of the list
    Wishlist *current = wishlist;
    Wishlist *previous = nullptr;

    // Traverse list to find item with matching ID
    while (current != nullptr && current->item->id != id)
    {
        previous = current;
        current = current->next;
    }

    // If item not found
    if (current == nullptr)
    {
        cout << "\n Product with ID " << id << " not found in wishlist.\n";
        return;
    }

    // Remove item from linked list
    previous->next = current->next;
    writeWishlistToFile(wishlist);  // Update file
    delete current;                 // Free memory

    cout << "\n Product removed from wishlist successfully!\n";
}

// Main user menu function - handles all user interactions
// Displays menu options and processes user choices
void userMenu(product* root, Cart*& cart)
{
    int choice;
    Wishlist* wishlist = nullptr; // Initialize wishlist for current session

    while (true)
    {
        // Display all available user options
        cout << "\n             USER MENU\n";
        cout << "--------------------------------------\n";
        cout << "  1.  View All Products\n";
        cout << "  2.  Search Product\n";
        cout << "  3.  Add Product to Cart\n";
        cout << "  4.  Add Product to Wishlist\n";
        cout << "  5.  View Cart\n";
        cout << "  6.  Edit Cart\n";
        cout << "  7.  Remove Product from Cart\n";
        cout << "  8.  Undo Last Cart Action\n";
        cout << "  9.  Clear Cart\n";
        cout << " 10.  Checkout\n";
        cout << " 11.  View Wishlist\n";
        cout << " 12.  Remove from Wishlist\n";
        cout << " 13.  View Order History\n";
        cout << " 14.  Logout\n";
        cout << "--------------------------------------\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cout << endl;



        // Process user choice using switch statement
        switch (choice)
        {
        case 1: // View all products
        {
            system("cls");
            cout << "\n             ALL PRODUCTS\n";
            cout << "--------------------------------------\n";
            cout << left << setw(5) << "ID" << setw(15) << "Category" << setw(25) << "Name" << "Price" << endl;
            cout << string(60, '-') << endl;
            inOrder(root); // Display products in sorted order
            break;
        }
        case 2: // Search products
        {
            system("cls");
            cout << "\n            SEARCH MENU\n";
            cout << "--------------------------------------\n";
            cout << " 1. Search by Name\n 2. Search by Category\n 3. Search by ID" << endl;
            int searchChoice;
            cin >> searchChoice;

            if (searchChoice == 1)// Search by name
            {
                cout << "\n Enter name to be searched: ";
                string name;
                cin >> name;
                bool found = searchByName(root, name);
                if (!found)
                {
                    cout << "\n No product with name '" << name << "' found.\n";
                }
            }
            else if (searchChoice == 2)  // Search by category
            {
                cout << "\n Enter category to be searched: ";
                string category;
                cin >> category;
                bool found = searchByCategory(root, category);
                if (!found)
                {
                    cout << "\n No product in category '" << category << "' found.\n";
                }
            }
            else if (searchChoice == 3)  // Search by ID
            {
                cout << "\n Enter ID to be searched: ";
                int id;
                cin >> id;
                product* found = searchById(root, id);
                if (!found)
                {
                    cout << "\n Product with ID " << id << " not found.\n";
                }
            }
            else
            {
                cout << "\n Incorrect option entered. Try again..." << endl;
            }
            break;
        }
        case 3:  // Add product to cart
        {
            system("cls");
            cout << "\n            ADD TO CART\n";
            cout << "--------------------------------------\n";

            int id, quantity;
            cout << "\n Enter product ID to add to cart: ";
            cin >> id;

            // Find product by ID
            product* found = searchById(root, id);
            if (found)
            {
                cout << "\n Enter quantity: ";
                cin >> quantity;

                // Validate quantity
                if (quantity > 0)
                {
                    addToCart(cart, found, quantity);
                }
                else
                {
                    cout << "\n Invalid quantity!\n";
                }
            }
            else
            {
                cout << "\n Product with ID " << id << " not found.\n";
            }
            break;
        }
        case 4:  // Add product to wishlist
        {
            system("cls");
            cout << "\n            ADD TO WISHLIST\n";
            cout << "--------------------------------------\n";
            int pId;

            cout << "\n Enter product id to add to wishlist: ";
            cin >> pId;

            product* found = searchById(root, pId); // Find product by ID
            if (found!=nullptr)
            {
                addToWishlist(wishlist, found);
            }
            else
            {
                cout << "\n Product not found!" << endl;
            }
            break;
        }
        case 5:
            // View cart contents
            system("cls");
            cout << "\n            CART CONTENT\n";
            cout << "--------------------------------------\n";
            displayCart(cart);
            break;
        case 6:
            // Edit cart (modify quantities)
            system("cls");
            cout << "\n            EDIT CART\n";
            cout << "--------------------------------------\n";
            displayCart(cart);
            editCart(cart);
            break;
        case 7:
            // Remove specific product from cart
        {
            int id;
            system("cls");
            cout << "\n            REMOVE FROM CART\n";
            cout << "--------------------------------------\n";
            cout << "\n Enter product ID to remove from cart: ";
            cin >> id;
            deleteFromCart(cart, id);
            break;
        }
        case 8: // Undo last cart action
            system("cls");
            cout << "\n            UNDO CART\n";
            cout << "--------------------------------------\n";
            undoCart(cart);
            displayCart(cart);
            break;
        case 9:
            // Clear entire cart
            system("cls");
            cout << "\n           CLEAR CART\n";
            cout << "--------------------------------------\n";
            clearCart(cart);
            break;
        case 10:
            // Proceed to checkout
            system("cls");
            cout << "\n            CHECKOUT\n";
            cout << "--------------------------------------\n";
            checkoutCart(cart);
            break;
        case 11:
            // View wishlist
            system("cls");
            cout << "\n            WISHLIST\n";
            cout << "--------------------------------------\n";
            displayWishlistFromFile();
            break;
        case 12:
        {
            system("cls");
            cout << "\n            REMOVE FROM WISHLIST\n";
            cout << "--------------------------------------\n";
            // Remove item from wishlist
            int id;
            cout << "\n Enter product ID to remove from wishlist: ";
            cin >> id;
            removeFromWishlist(wishlist, id);
            break;
        }
        case 13:  // View order history
            system("cls");
            cout << "\n            ORDER HISTORY\n";
            displayOrderHistory();
            break;
        case 14:
            // Logout
            loggedIn = false;
            currentUsername = "";
            currentUserRole = "";
            cout << "\n Logged out successfully!\n";
            return;
        default:
            cout << "\n Invalid choice. Please try again.\n";
        }
    }
}
// Function to write the entire wishlist to file
void writeWishlistToFile(Wishlist* wishlist)
{
    ofstream outFile("wishlist.txt"); // overwrite each time
    if (!outFile.is_open())
    {
        cout << "\n Error opening wishlist file!" << endl;
        return;
    }

    outFile << left << setw(5) << "ID" << setw(15) << "Name" << setw(15) << "Category" << "Price\n";
    Wishlist* temp = wishlist;  // Use temporary pointer
    while (temp != nullptr)
    {
        outFile << left << setw(5) << temp->item->id
                << setw(15) << temp->item->name
                << setw(15) << temp->item->category
                << temp->item->price << "\n";
        temp = temp->next;
    }
    outFile.close();
}

// Admin menu function - handles administrator operations
// Provides admin-specific functionality like adding products and viewing users
void adminMenu()
{
    int choice;
    product* root = nullptr;
    root = loadFromFile("items.txt", root); // Load once

    while (true)
    {
        system("cls");
        cout << "\n            ADMIN MENU\n";
        cout << "--------------------------------------\n";
        cout << " 1. View All Products\n";
        cout << " 2. Add New Product\n";
        cout << " 3. Delete Product by ID\n";  // New Option
        cout << " 4. Delete User\n";  // New Option
        cout << " 5. View All Users\n";
        cout << " 6. Logout\n";
        cout << "\n Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1://Display Products
            system("cls");
            cout << "\n            ALL PRODUCTS\n";
            cout << "--------------------------------------\n";
            cout << left << setw(5) << "ID" << setw(15) << "Category" << setw(25) << "Name" << "Price" << endl;
            cout << string(60, '-') << endl;
            inOrder(root);
            cout << "\n Press Enter to continue...";
            cin.ignore();
            cin.get();

            break;

        case 2://Add product
        {
            system("cls");
              cout << "\n            ADD PRODUCTS\n";
            cout << "--------------------------------------\n";
            int id, price;
            string name, category;
            cout << "\n Enter product ID: ";
            cin >> id;
            cout << "\n Enter product name: ";
            cin >> name;
            cout << "\n Enter product category: ";
            cin >> category;
            cout << "\n Enter product price: ";
            cin >> price;

            root = insertion(root, name, category, price, id);
            updateItemsFile(root, "items.txt"); // Save updated tree
            cout << "\n Product added successfully!\n";
            cout << "\n Press Enter to continue...";
            cin.ignore();
            cin.get();

            break;
        }

        case 3://Delete product
        {
            system("cls");
               cout << "\n            DELETE PRODUCTS\n";
            cout << "--------------------------------------\n";
            int id;
            cout << "\n Enter ID of product to delete: ";
            cin >> id;

            root = deleteProductById(root, id, "items.txt"); // From earlier response
            cout << "\n Press Enter to continue...";
            cin.ignore();
            cin.get();

            break;
        }


        case 4://delete user
        {
            system("cls"); // Clear screen before taking input
               cout << "\n            DELETE USER\n";
            cout << "--------------------------------------\n";

            User* head = NULL;

            // Load existing users
            loadUsersFromFile(head, "users.txt");

            // Prompt for deletion details
            string name, key;
            cout << "Enter Username of user to delete: ";
            cin >> name;
            cout << "Enter password of user to confirm: ";
            cin >> key;

            deleteUser(head, name, key);
            saveUsersToFile(head, "users.txt");

            cout << "\n Users Left:\n";
            displayUsers(head);

            system("pause"); // Optional: lets user read the message before menu shows
            break;
        }

        case 5://display users
        {
            system("cls");
               cout << "\n           DISPLAY USERS\n";
            cout << "--------------------------------------\n";
            User* head = NULL;
            loadUsersFromFile(head, "users.txt");
            displayUsers(head);
            cout << "\n Press Enter to continue...";
            cin.ignore();
            cin.get();

            break;
        }

        case 6:
            loggedIn = false;
            currentUsername = "";
            currentUserRole = "";
            cout << "\n Logged out successfully!\n";
            return;

        default:
            cout << "\n Invalid choice. Please try again.\n";
            cout << "\n Press Enter to continue...";
            cin.ignore();
            cin.get();

        }
    }
}

// Add item to wishlist (and update file)
void addToWishlist(Wishlist*& wishlist, product* item)
{
    Wishlist* newNode = new Wishlist{item, wishlist};
    wishlist = newNode;

    // Rewrite entire wishlist to file
    writeWishlistToFile(wishlist);

    cout << "\n Product added to wishlist and saved to file.\n";
}

// Read and display wishlist from file
void displayWishlistFromFile()
{
    ifstream inFile("wishlist.txt");
    if (!inFile.is_open())
    {
        cout << "\n Wishlist file not found.\n";
        return;
    }

    string line;
    // Read and print each line (item)

    while (getline(inFile, line))
    {
        cout << line << "\n";
    }

    inFile.close();
}


// Searches the AVL tree by product name and displays matched product(s)
bool searchByName(product* root, string name)
{
    if (!root) return false;

    // Recursively search in left subtree
    bool left = searchByName(root->left, name);
    bool foundHere = false;

    // Check current node
    if (root->name == name)
    {
        cout << "\n Name: " << root->name << endl
             << " ID: " << root->id << endl
             << " Category: " << root->category << endl
             << " Price: " << root->price << endl;
        foundHere = true;
    }

    // Recursively search in right subtree
    bool right = searchByName(root->right, name);

    // Return true if found in left, current, or right
    return left || foundHere || right;
}

// Search product by ID in AVL tree
product* searchById(product* root, int id)
{
    if (!root) return nullptr;

    if (root->id == id) // If found, display and return
    {



        cout << "\n Name: " << root->name << endl
             << " ID: " << root->id << endl
             << " Category: " << root->category << endl
             << " Price: " << root->price << endl;
        return root;
    }

    // Traverse to left or right subtree based on ID
    if (id < root->id)
        return searchById(root->left, id);
    else
        return searchById(root->right, id);
}

// Search products by category and display all matching
bool searchByCategory(product* root, string category)
{
    if (!root) return false;

    bool left = searchByCategory(root->left, category);
    bool foundHere = false;

    if (root->category == category)
    {
        cout << "\n Name: " << root->name << endl
             << " ID: " << root->id << endl
             << " Category: " << root->category << endl
             << " Price: " << root->price << endl;
        foundHere = true;
    }

    bool right = searchByCategory(root->right, category);

    return left || foundHere || right;
}

// Reads and displays contents of a file (used for generic file reading)
void file(string filename)
{
    ifstream in(filename);
    string line;

    while (getline(in, line))
    {
        cout << line << endl;
    }

    in.close(); // Close after reading
}


// Main function: Entry point of the program
int main()
{
    User* head = NULL; // Linked list head for users
    loadUsersFromFile(head, "users.txt"); // Load users into memory

    product* root = nullptr; // Root of AVL tree for products
    Cart* cart = nullptr;    // Pointer to user's cart (linked list or struct)


    cout << "\n\n\n\n\n";
    cout << "                            \t\t *****************************                             \n";
    cout << "                            \t\t *                           *                             \n";
    cout << "                            \t\t *   SHOPPING                *                             \n";
    cout << "                            \t\t *         MANAGEMENT        *                             \n";
    cout << "                            \t\t *                 SYSTEM    *                             \n";
    cout << "                            \t\t *                           *                             \n";
    cout << "                            \t\t *****************************                             \n";
    cout << "\n\n";
    cout << "                              \t\t        Developed by:-\n\n";
    cout << "                            \t\t  ----------------------------\n";
    cout << "                           \t\t *         Tahira Hafeez      *\n";
    cout << "                           \t\t *         Sabiha Usman       *\n";
    cout << "                           \t\t *         Haleema Jawad      *\n";
    cout << "                           \t\t  ----------------------------\n";
    cout << "\n";
    cout << " \n                         \t\t Press Enter to launch the system...";
    cin.get();
    cin.ignore();



    int choice;
    while (true)
    {
        system("cls");
        cout << "\n            MAIN MENU\n";
        cout << "--------------------------------------\n";

        if (!loggedIn) // If no user is logged in, show login/register menu
        {
            cout << "\n 1. Register\n";
            cout << "\n 2. Login\n";
            cout << "\n 3. Exit\n";
            cout << "\n Enter your choice: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                registerUser(head); // Register a new user
                break;
            case 2:
                login(head); // Attempt login
                if (loggedIn)
                {
                    // If login successful, load items from file into AVL tree
                    root = loadFromFile("items.txt", root);
                    if (currentUserRole == "user")
                    {
                        system("cls");
                        userMenu(root, cart); // Go to user menu
                    }
                    else
                    {
                        system("cls");
                        adminMenu(); // Go to admin menu
                    }
                }
                break;
            case 3:
                cout << "\n Exiting program. Goodbye!\n";
                deleteUsers(head); // Free memory
                return 0;
            default:
                cout << "\n Invalid choice. Please try again.\n";
            }
        }
        else // If already logged in, direct to appropriate menu
        {
            if (currentUserRole == "user")
            {
                userMenu(root, cart);
            }
            else
            {
                adminMenu();
            }
        }
    }

    return 0; // End of program
}
