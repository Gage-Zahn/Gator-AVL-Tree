/*
Author: Gage Zahn
Project 1 for COP3530: Gator AVL Project

Some logic and inspiration from: "4 - Balanced Trees.pdf" for COP3530
*/

#include <iostream>
#include <string>
#include <vector>
#include <stack>
using namespace std;

//~~~~~~~~~~~~~~~~AVL~~~~~~~~~~~~~~~~~~
class AVL {
private:
	class Node {
	public:
		Node(string name, int id);
		string name;
		unsigned int id;
		unsigned int height;
		Node* left;
		Node* right;
	};

	//Rotations
	Node* rotateL(Node* node);
	Node* rotateR(Node* node);
	Node* rotateLR(Node* node);
	Node* rotateRL(Node* node);
	
	//Insert
	Node* insert(Node* root, string name, unsigned int id);
	
	//Search
	Node* search(Node* root, unsigned int id);
	Node* search(Node* root, string name, bool& found);

	//Print
	void printInorder(Node* node, vector<Node*>& output);
	void printPreorder(Node* node, vector<Node*>& output);
	void printPostorder(Node* node, vector<Node*>& output);
	
	//Tree Data
	unsigned int height(Node* node);
	Node* rebalance(Node* root);
	
	//Remove
	Node* remove(Node* root, unsigned int id);
	void removePostOrder(Node* root);
	
	//Data
	Node* root;
public:
	AVL();
	~AVL();
	
	//Insert
	void insert(string name, unsigned int id);
	
	//Search
	void search(unsigned int id);
	void search(string name);
	
	//Print
	void printInorder();
	void printPreorder();
	void printPostorder();
	void printLevelCount();
	
	//Remove
	void removeInorder(unsigned int n);
	void remove(unsigned int id);
	
};

//--------------AVL Functions--------------
AVL::AVL() {
	root = nullptr;
}

AVL::~AVL() {
	removePostOrder(this->root);
}


//*****Insert and Rebalance*****
void AVL::insert(string name, unsigned int id) { //Public Insert
	if (this->root == nullptr) {
		this->root = new Node(name, id);
		cout << "successful" << endl;
	}
	else
		this->insert(this->root, name, id);		
}

AVL::Node* AVL::insert(Node* root, string name, unsigned int id) { //Private Insert
	if (root == nullptr) {
		cout << "successful" << endl;
		return new Node(name, id);	
	}
	else if (id < root->id)
		root->left = this->insert(root->left,name ,id);
	else if (id != root->id)
		root->right = this->insert(root->right,name, id);
	else
		cout << "unsuccessful" << endl;
		
	//Height Adjustments
	root->height = max(height(root->left), height(root->right)) + 1;
	
	//Rebalancing
	return this->rebalance(root);
}

AVL::Node* AVL::rebalance(Node* root) {
	//Rebalancing
	if ((int)height(root->left) - (int)height(root->right) < -1) { //Right
		//R L rotation
		if ((int)height(root->right->left) - (int)height(root->right->right) > 0) { //Right Left
			root = rotateRL(root);
			root->height++;
			root->left->height -= 2;
			root->right->height--;
			return root;
		}
		//L rotation
		else if (root->right->right != nullptr) { //Right Right
			root = rotateL(root);
			root->left->height -= 2;
			return root;
		}
	}
	else if ((int)height(root->left) - (int)height(root->right) > 1) { //Left
		//L R rotation
		if ((int)height(root->left->left) - (int)height(root->left->right) < 0) { //Left Right
			root = rotateLR(root);
			root->height++;
			root->right->height -= 2;
			root->left->height--;
			return root;
		}
		//R rotation
		else if (root->left->left != nullptr) { //Left Left
			root = rotateR(root);
			root->right->height -= 2;
			return root;
		}
	}

	return root;
}

//*****Rotations*****
AVL::Node* AVL::rotateL(Node* node) { //Right Right Case
	Node* grandchild = node->right->left;
	Node* newParent = node->right;
	newParent->left = node;
	node->right = grandchild;
	if (node->id == this->root->id) //Reassign root
		this->root = newParent;
	return newParent;
}

AVL::Node* AVL::rotateR(Node* node) { // Left Left Case
	Node* grandchild = node->left->right;
	Node* newParent = node->left;
	newParent->right = node;
	node->left = grandchild;
	if (node->id == this->root->id) //Reassign root
		this->root = newParent;
	return newParent;
}
AVL::Node* AVL::rotateLR(Node* node) { //Left Right Case
	Node* grandchild = node->left->right;
	Node* greatGrandchild = node->left->right->left;
	node->left->right = greatGrandchild;
	grandchild->left = node->left;
	node->left = grandchild;
	return rotateR(node);
}
AVL::Node* AVL::rotateRL(Node* node) { //Right Left Case
	Node* grandchild = node->right->left;
	Node* greatGrandchild = node->right->left->right;
	node->right->left = greatGrandchild;
	grandchild->right = node->right;
	node->right = grandchild;
	return rotateL(node);
}

//*****Search*****
void AVL::search(unsigned int id) { //Public Search by id
	Node* output = search(this->root, id);
	if(output != nullptr)
		cout << output->name << endl;
	else
		cout <<"unsuccessful" << endl;
}

AVL::Node* AVL::search(Node* root, unsigned int id) { //Private Search by id
	if (root == nullptr || root->id == id) {
		return root;
	}
	else if (id < root->id)
		return search(root->left, id);
	else if (id != root->id)
		return search(root->right, id);
	return root;
}

void AVL::search(string name) { //Public Search by name
	bool found = false;
	search(this->root, name, found);
	if (found == false)
		cout << "unsuccessful" << endl;
}

AVL::Node* AVL::search(Node* root, string name, bool& found) { //Private Search by name
	if (root == nullptr) {
		return root;
	}
	if (root->name.compare(name) == 0) {
		string val = to_string(root->id);
		for (unsigned int i = 8; i > val.length(); i--)
			cout << "0";
		cout << val << endl;
		found = true;
	}
	search(root->left, name, found);
	search(root->right, name, found);
	
	return root;
}

//*****Remove*****
void AVL::remove(unsigned int id) {  //Public remove
	this->root = remove(this->root, id);
}

AVL::Node* AVL::remove(Node* root, unsigned int id) { //Private remove (Rebalances automatically)
	if (root == nullptr) {
		cout << "unsuccessful" << endl;
		return root;
	}
	
	if (id < root->id) {
		root->left = remove(root->left, id);
		root->height = max(height(root->left), height(root->right)) + 1; //Adjust Height
		return rebalance(root);
	}
	else if (id > root->id) {
		root->right = remove(root->right, id); 
		root->height = max(height(root->left), height(root->right)) + 1; //Adjust Height
		return rebalance(root);
	}
	
	//Zero and One Child Cases
	if (root->left == nullptr) { //Right Child and Zero Child
		Node* temp = root->right;
		delete root;
		cout << "successful" << endl;
		if(temp != nullptr)
			return rebalance(temp);
		return temp;
	}
	else if (root->right == nullptr) { //Left Child
		Node* temp = root->left;
		delete root;
		cout << "successful" << endl;
		return rebalance(temp);
	}
	//Two Child Case
	else { //Inorder Successor
		Node* succParent = root;

		Node* succ = root->right;
		while (succ->left != nullptr) {
			succParent = succ;
			succ = succ->left;
		}

		if (succParent != root)
			succParent->left = succ->right;
		else
			succParent->right = succ->right;

		root->id = succ->id;
		root->name = succ->name;

		delete succ;
		cout << "successful" << endl;
		return rebalance(root);
	}
}

void AVL::removeInorder(unsigned int n) {
	stack<Node*> s;
	Node* current = this->root;
	unsigned int count = 0;
	
	//Traverse Inorder
	while (current != nullptr || s.empty() == false) {
		while (current != nullptr) {
			s.push(current);
			
			current = current->left;
		}
		
		current = s.top();
		s.pop();
		
		//Check if n is reached
		if (count == n)
			break;
		count++;

		current = current->right;
	}
	
	//Output
	if (current != nullptr)
		remove(this->root, current->id);
	else
		cout << "unsuccessful" << endl;

}

void AVL::removePostOrder(Node* node) { //Deletes all Nodes
	if (node == nullptr)
		return;
	removePostOrder(node->left);
	removePostOrder(node->right);
	delete node;
}

//*****Print*****
void AVL::printInorder() { //Public Print Inorder
	vector<Node*> output;
	printInorder(this->root, output);
	for (int i = 0; i < output.size(); i++) { //Comma separate
		cout << output[i]->name;
		if (i < output.size() - 1)
			cout << ", ";
	}
	cout << endl;
}

void AVL::printPreorder() { //Public Print Preorder
	vector<Node*> output;
	printPreorder(this->root, output);
	for (int i = 0; i < output.size(); i++) { //Comma separate
		cout << output[i]->name;
		if(i < output.size() - 1)
			cout << ", ";
	}
	cout << endl;
}

void AVL::printPostorder() { //Public Print Postorder
	vector<Node*> output;
	printPostorder(this->root, output);
	for (int i = 0; i < output.size(); i++) { //Comma separate
		cout << output[i]->name;
		if (i < output.size() - 1)
			cout << ", ";
	}
	cout << endl;
}

void AVL::printInorder(Node* node, vector<Node*>& output) { //Private Print Inorder
	if (node == nullptr)
		return;
	printInorder(node->left, output);
	output.push_back(node);
	printInorder(node->right, output);
}

void AVL::printPreorder(Node* node, vector<Node*>& output) { //Private Print Preorder
	if (node == nullptr)
		return;
	output.push_back(node);
	printPreorder(node->left, output);
	printPreorder(node->right, output);
}

void AVL::printPostorder(Node* node, vector<Node*>& output) { //Private Print Postorder
	if (node == nullptr)
		return;
	printPostorder(node->left, output);
	printPostorder(node->right, output);
	output.push_back(node);
}

void AVL::printLevelCount() {
	cout << height(this->root) << endl;
}

//*****Height*****
unsigned int AVL::height(Node* node) {
	if (node == nullptr)
		return 0;
	else
		return node->height;
}


//--------------Node Functions--------------
AVL::Node::Node(string name, int id) {
	this->name = name;
	this->id = id;
	this->height = 1;
	left = nullptr;
	right = nullptr;
}

//--------------Validation Functions--------------
bool isName(string& name) { 
	for (char i : name) {
		if (!isalpha(i) && i != ' ')
			return false;
	}
	return true;
}

bool isId(string id) {
	unsigned int count = 0;
	for (char i : id) {
		if (!isdigit(i))
			return false;
		count++;
		if (count > 8)
			return false;
	}
	if (count == 8)
		return true;
	else
		return false;
}

int main() {
	AVL tree; //Initialize Tree
	
	//Run Commands
	unsigned int commandCount;
	cin >> commandCount;
	for (unsigned int i = 0; i < commandCount; i++) {
		//Collect command
		string command;
		cin >> command;
		
		if (command.compare("insert") == 0) { //Insert
			string name;
			string id;
			
			//Collect name
			getline(cin,name,'\"');
			getline(cin, name, '\"');
			
			//Collect id
			cin >> id;

			
			if (!isName(name)) { //Validate name
				cout << "unsuccessful" << endl;
				continue;
			}
			if (!isId(id)) {  //Validate id
				cout << "unsuccessful" << endl;
				continue;
			}
			char remainder = '\n';
			cin.get(remainder);
			if (remainder != '\n') { //Validate Command End
				cout << "unsuccessful" << endl;
				continue;
			}

			tree.insert(name, stol(id));
		}
		else if (command.compare("remove") == 0) { //Remove
			string id;

			cin >> id;
			if (!isId(id)) { //Validate id
				cout << "unsuccessful" << endl;
				continue;
			}
			char remainder = '\n';
			cin.get(remainder);
			if (remainder != '\n') { //Validate Command End
				cout << "unsuccessful" << endl;
				continue;
			}
			tree.remove(stol(id));

		}
		else if (command.compare("search") == 0) { //Search
			string in;

			cin >> in;

			if (isId(in)) //Validate id
				tree.search(stol(in));
			else if (in.find('\"') == 0) { //Validate name
				in = in.substr(1, in.length() - 1);
				if (in.find('\"') == string::npos) {
					string in2;
					getline(cin, in2, '\"');
					in = in + in2;
				}
				else if(in.find('\"') == in.length()-1) {
					in = in.substr(0, in.length() - 1);
				}
				else{
					cout << "unsuccessful" << endl;
					continue;
				}
				char remainder = '\n';
				cin.get(remainder);
				if (remainder != '\n') { //Validate Command End
					cout << "unsuccessful" << endl;
					continue;
				}
				
				tree.search(in);
			} 	
			else {
				cout << "unsuccessful" << endl;
				continue;
			}		
		}
		else if (command.compare("printInorder") == 0) { //printInorder
			char remainder = '\n';
			cin.get(remainder);
			if (remainder != '\n') { //Validate Command End
				cout << "unsuccessful" << endl;
				continue;
			}
			tree.printInorder();
		}
		else if (command.compare("printPreorder") == 0) { //printPreorder
			char remainder = '\n';
			cin.get(remainder);
			if (remainder != '\n') { //Validate Command End
				cout << "unsuccessful" << endl;
				continue;
			}
			tree.printPreorder();
		}
		else if (command.compare("printPostorder") == 0) { //printPostorder
			char remainder = '\n';
			cin.get(remainder);
			if (remainder != '\n') { //Validate Command End
				cout << "unsuccessful" << endl;
				continue;
			}
			tree.printPostorder();
		}
		else if (command.compare("printLevelCount") == 0) { //printLevelCount
			char remainder = '\n';
			cin.get(remainder);
			if (remainder != '\n') { //Validate Command End
				cout << "unsuccessful" << endl;
				continue;
			}
			tree.printLevelCount();
		}
		else if (command.compare("removeInorder") == 0) { //removeInorder
			unsigned int n;
			cin >> n;
			char remainder = '\n';
			cin.get(remainder);
			if (remainder != '\n') { //Validate Command End
				cout << "unsuccessful" << endl;
				continue;
			}
			tree.removeInorder(n);
		}
		else {	//Base Case -- invalid command
			cout << "unsuccessful" << endl;
			continue;
		}
	}
	return 0;
}