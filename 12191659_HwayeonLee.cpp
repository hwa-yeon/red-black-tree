#include <iostream>
#include <vector>
using namespace std;

// ���ø����̼� ������ �����ϴ� Node Ŭ����
class Node {
public:
	// ���ø����̼� ������
	int ID;
	string name;
	int capacity;
	int price;

	char color;	 // red �Ǵ� black

	// node ���� ���踦 ��Ÿ��.
	Node* left;
	Node* right;
	Node* parent;

	// ���ø����̼��� ���� ������ ��
	Node(int ID, string name, int capacity, int price) {
		this->ID = ID;  // �Էµ� ID�� ���ø����̼��� ID ����
		this->name = name;  // �Էµ� �̸����� ���ø����̼��� �̸� ����
		this->capacity = capacity;  // �Էµ� �뷮���� ���ø����̼��� �뷮 ����
		this->price = price;  // �Էµ� �������� ���ø����̼��� ���� ����
		this->color = 'r';  // node�� ���Ե� ���� red�� ���Ե�.
		this->left = NULL;
		this->right = NULL;
		this->parent = NULL;
	}
	/*void setName(string name) {
		this->name = name;
	}
	void setCapacity(int capacity) {
		this->capacity = capacity;
	}
	void setPrice(int price) {
		this->price = price;
	}
	void setColor(string color) {
		this->color = color;
	}
	void setLeft(Node* left) {
		this->left = left;
	}
	void setRight(Node* right) {
		this->right = right;
	}
	void setParent(Node* parent) {
		this->parent = parent;
	}*/
};

// red-black tree�� ��Ÿ���� Ŭ����
class Tree {
private:
	Node* root = NULL;
	// vector<Node*> node_list;
public:
	// ���ø����̼� ���
	Node* insertNode(int ID, string name, int capacity, int price) {
		// tree�� ����ִٸ�
		// �ٷ� ���ο� node�� �߰��ϰ� �� node�� return
		if (root == NULL) {
			Node* node = new Node(ID, name, capacity, price);  // ���ο� ���ø����̼� �߰�
			// node_list.push_back(node);
			root = node;
			insertColor(node);  // node�� color ����
			return node;
		}
		// tree�� ������� �ʴٸ�
		else {
			Node* nownode = root;

			// binary search tree�� '�ڽź��� ���� �����ʹ� ���� �ڽĿ�, �ڽź��� ū �����ʹ� ������ �ڽĿ� �ִ�.'�� Ư¡ �̿�
			// ���� �߰��Ǵ� node�� �ڸ��� ã���ִ� �ݺ���
			while (nownode != NULL) {
				// �Էµ� ID�� �۴ٸ� �������� 
				if (ID < nownode->ID) {
					Node* leftnode = nownode->left;
					if (leftnode == NULL) break;
					nownode = leftnode;
				}
				// �Էµ� ID�� ũ�ٸ� ����������
				else if (ID > nownode->ID) {
					Node* rightnode = nownode->right;
					if (rightnode == NULL) break;
					nownode = rightnode;
				}
				// �Էµ� ID�� ���ٸ� ���ο� node�� �߰��Ǹ� �ȵ�. �ش� ID�� ������ node�� ã�� return
				else {
					Node* existnode = findNode(ID);
					return existnode;
				}
			}
			Node* node = new Node(ID, name, capacity, price);  // while���� ���������� �����ߴٸ� ���ο� ���ø����̼� �߰�

			// ���ο� node�� while���� ���� ���� node�� �θ��ڽ� ���� ����
			if (ID < nownode->ID) nownode->left = node;
			else nownode->right = node;
			node->parent = nownode;
			// node_list.push_back(node);
			insertColor(node);  // node�� color ����

			return node;
		}
	}
	// node�� color ���� (double red�� ��� ������)
	void insertColor(Node* node) {
		// double red �� ���
		if ((node->parent != NULL) && (node->parent->color == 'r')) {
			Node* uncle = NULL;
			Node* parent = node->parent;
			Node* grandparent = node->parent->parent;
			if ((grandparent->left != NULL) || (grandparent->right != NULL)) {
				if (parent == grandparent->left) {
					uncle = grandparent->right;
					// parent�� uncle ��� red�� ��� recoloring ����
					if ((uncle != NULL) && (uncle->color == 'r')) {
						parent->color = 'b';
						uncle->color = 'b';
						grandparent->color = 'r';
						insertColor(grandparent);  // recoloring�� ���� �Ŀ��� double red�� �ذ���� ���� ���� �����Ƿ� ���
					}
					// parent�� red uncle�� black�� ��� restructuring ����
					else {
						// if((node == parent->right) && (parent == grandparent->left))�� ����
						if (node == parent->right) {
							leftRotation(parent);  // restructuring�� �������ֱ� ���� grandparent, parent, node ��� ���ʿ� �ֵ���
							node = node->left;  // ���� �Լ��� �����ϸ�, node�� parent�� ��ġ�� ����ǹǷ� node->left ����
						}
						// restructuring
						node->parent->color = 'b';
						node->parent->parent->color = 'r';
						rightRotation(node->parent->parent);
					}
				}
				// left, right�� ����Ǿ���, ���� ������ ����
				else {
					uncle = grandparent->left;
					if ((uncle != NULL) && (uncle->color == 'r')) {
						parent->color = 'b';
						uncle->color = 'b';
						grandparent->color = 'r';
						insertColor(grandparent);
					}
					else {
						if (node == parent->left) {
							rightRotation(parent);
							node = node->right;
						}
						node->parent->color = 'b';
						node->parent->parent->color = 'r';
						leftRotation(node->parent->parent);
					}
				}
			}
		}
		root->color = 'b';  // root�� black�̹Ƿ� �������� ó������
	}
	// restucturing�� ������ �� �ʿ��� �Լ�
	// tree�� node�� �������� ȸ����Ű�� ��ó�� �� node�� �θ� �ڽ� ���踦 ��������
	void leftRotation(Node* node) {
		Node* tmp = node->right;

		node->right = tmp->left;
		if (tmp->left != NULL)
			tmp->left->parent = node;
		tmp->parent = node->parent;

		// root�� ��� ����ó��
		if (node->parent == NULL)
			root = tmp;
		else if (node == node->parent->left)
			node->parent->left = tmp;
		else
			node->parent->right = tmp;
		node->parent = tmp;
		tmp->left = node;
	}
	// restucturing�� ������ �� �ʿ��� �Լ�
	// tree�� node�� ���������� ȸ����Ű�� ��ó�� �� node�� �θ� �ڽ� ���踦 ��������
	void rightRotation(Node* node) {
		Node* tmp = node->left;

		node->left = tmp->right;
		if (tmp->right != NULL)
			tmp->right->parent = node;
		tmp->parent = node->parent;

		// root�� ��� ����ó��
		if (node->parent == NULL)
			root = tmp;
		else if (node == node->parent->right)
			node->parent->right = tmp;
		else
			node->parent->left = tmp;
		node->parent = tmp;
		tmp->right = node;
	}
	// ���ø����̼� �˻�
	void searchNode(int ID) {
		Node* nownode = findNode(ID);
	
		if(nownode != NULL)
			cout << depth(nownode) << " " << nownode->name << " " << nownode->capacity << " " << nownode->price << "\n";
		else
			cout << "NULL" << "\n";
	}
	// ���ø����̼� ������Ʈ
	void updateNode(int ID, string name, int capacity, int price) {
		Node* nownode = findNode(ID);

		if (nownode != NULL) {
			nownode->name = name;
			nownode->capacity = capacity;
			nownode->price = price;
			cout << depth(nownode) << "\n";
		}
		else
			cout << "NULL" << "\n";
	}
	// ���ø����̼� ����
	void discountNode(int x, int y, int P) {
		inorderTraversal(root, x, y, P);
	}
	// ������ȸ�� ���� ��� node�� ��ȸ�ϸ鼭 ������ �ش��ϴ� ���ø����̼� ������ ����
	void inorderTraversal(Node* node, int x, int y, int P) {
		if (node != NULL) {
			inorderTraversal(node->left, x, y, P);
			if ((node->ID >= x) && (node->ID <= y))
				node->price = (node->price * (100 - P) / 100);
			inorderTraversal(node->right, x, y, P);
		}
	}
	// �Էµ� ID�� ���� node�� ã�� �Լ�
	Node* findNode(int ID) {
		Node* nownode = root;

		// binary search tree�� '�ڽź��� ���� �����ʹ� ���� �ڽĿ�, �ڽź��� ū �����ʹ� ������ �ڽĿ� �ִ�.'�� Ư¡ �̿�
		while (nownode != NULL) {
			if (ID < nownode->ID) {
				nownode = nownode->left;
			}
			else if (ID > nownode->ID) {
				nownode = nownode->right;
			}
			else {
				return nownode;
			}
		}
		return NULL;

		/*Node* nownode = NULL;

		for (int i = 0; i < node_list.size(); i++) {
			if (node_list[i]->ID == ID) {
				nownode = node_list[i];
				return nownode;
			}
		}
		return nownode;*/
	}
	// node�� depth�� ���ϴ� �Լ�
	int depth(Node* node) {
		if (node->parent == NULL) {
			return 0;
		}
		return depth(node->parent) + 1; // node�� parent�� �����Ѵٸ� depth�� 1 �ø��� ��� ����
	}
};

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	Tree tree = Tree();
	int T;
	char N;

	int ID;
	string name;
	int capacity;
	int price;

	cin >> T;
	while (T--) {
		cin >> N;
		// ���ø����̼� ���
		if (N == 'I') {
			cin >> ID >> name >> capacity >> price;
			Node* node = tree.insertNode(ID, name, capacity, price);
			cout << tree.depth(node) << "\n";
		}
		// ���ø����̼� �˻�
		else if (N == 'F') {
			cin >> ID;
			tree.searchNode(ID);
		}
		// ���ø����̼� ������Ʈ
		else if (N == 'R') {
			cin >> ID >> name >> capacity >> price;
			tree.updateNode(ID, name, capacity, price);
		}
		// ���ø����̼� ����
		else if (N == 'D') {
			int x, y, P;
			cin >> x >> y >> P;
			tree.discountNode(x, y, P);
		}
	}
	return 0;
}