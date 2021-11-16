#include <iostream>
#include <vector>
using namespace std;

// 애플리케이션 정보를 저장하는 Node 클래스
class Node {
public:
	// 애플리케이션 정보들
	int ID;
	string name;
	int capacity;
	int price;

	char color;	 // red 또는 black

	// node 간의 관계를 나타냄.
	Node* left;
	Node* right;
	Node* parent;

	// 애플리케이션이 새로 생성될 때
	Node(int ID, string name, int capacity, int price) {
		this->ID = ID;  // 입력된 ID로 애플리케이션의 ID 설정
		this->name = name;  // 입력된 이름으로 애플리케이션의 이름 설정
		this->capacity = capacity;  // 입력된 용량으로 애플리케이션의 용량 설정
		this->price = price;  // 입력된 가격으로 애플리케이션의 가격 설정
		this->color = 'r';  // node가 삽입될 때는 red로 삽입됨.
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

// red-black tree를 나타내는 클래스
class Tree {
private:
	Node* root = NULL;
	// vector<Node*> node_list;
public:
	// 애플리케이션 등록
	Node* insertNode(int ID, string name, int capacity, int price) {
		// tree가 비어있다면
		// 바로 새로운 node를 추가하고 그 node를 return
		if (root == NULL) {
			Node* node = new Node(ID, name, capacity, price);  // 새로운 애플리케이션 추가
			// node_list.push_back(node);
			root = node;
			insertColor(node);  // node의 color 설정
			return node;
		}
		// tree가 비어있지 않다면
		else {
			Node* nownode = root;

			// binary search tree의 '자신보다 작은 데이터는 왼쪽 자식에, 자신보다 큰 데이터는 오른쪽 자식에 있다.'의 특징 이용
			// 새로 추가되는 node의 자리를 찾아주는 반복문
			while (nownode != NULL) {
				// 입력된 ID가 작다면 왼쪽으로 
				if (ID < nownode->ID) {
					Node* leftnode = nownode->left;
					if (leftnode == NULL) break;
					nownode = leftnode;
				}
				// 입력된 ID가 크다면 오른쪽으로
				else if (ID > nownode->ID) {
					Node* rightnode = nownode->right;
					if (rightnode == NULL) break;
					nownode = rightnode;
				}
				// 입력된 ID가 같다면 새로운 node가 추가되면 안됨. 해당 ID를 가지는 node를 찾고 return
				else {
					Node* existnode = findNode(ID);
					return existnode;
				}
			}
			Node* node = new Node(ID, name, capacity, price);  // while문을 성공적으로 수행했다면 새로운 어플리케이션 추가

			// 새로운 node와 while문을 통해 구한 node의 부모자식 관계 연결
			if (ID < nownode->ID) nownode->left = node;
			else nownode->right = node;
			node->parent = nownode;
			// node_list.push_back(node);
			insertColor(node);  // node의 color 설정

			return node;
		}
	}
	// node의 color 설정 (double red일 경우 고쳐줌)
	void insertColor(Node* node) {
		// double red 일 경우
		if ((node->parent != NULL) && (node->parent->color == 'r')) {
			Node* uncle = NULL;
			Node* parent = node->parent;
			Node* grandparent = node->parent->parent;
			if ((grandparent->left != NULL) || (grandparent->right != NULL)) {
				if (parent == grandparent->left) {
					uncle = grandparent->right;
					// parent와 uncle 모두 red일 경우 recoloring 수행
					if ((uncle != NULL) && (uncle->color == 'r')) {
						parent->color = 'b';
						uncle->color = 'b';
						grandparent->color = 'r';
						insertColor(grandparent);  // recoloring은 수행 후에도 double red가 해결되지 않을 수도 있으므로 재귀
					}
					// parent는 red uncle은 black일 경우 restructuring 수행
					else {
						// if((node == parent->right) && (parent == grandparent->left))와 같음
						if (node == parent->right) {
							leftRotation(parent);  // restructuring을 수행해주기 위해 grandparent, parent, node 모두 왼쪽에 있도록
							node = node->left;  // 위의 함수를 수행하면, node와 parent의 위치가 변경되므로 node->left 수행
						}
						// restructuring
						node->parent->color = 'b';
						node->parent->parent->color = 'r';
						rightRotation(node->parent->parent);
					}
				}
				// left, right만 변경되었고, 위의 과정과 동일
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
		root->color = 'b';  // root는 black이므로 마지막에 처리해줌
	}
	// restucturing을 수행할 때 필요한 함수
	// tree의 node를 왼쪽으로 회전시키는 것처럼 각 node의 부모 자식 관계를 변경해줌
	void leftRotation(Node* node) {
		Node* tmp = node->right;

		node->right = tmp->left;
		if (tmp->left != NULL)
			tmp->left->parent = node;
		tmp->parent = node->parent;

		// root일 경우 예외처리
		if (node->parent == NULL)
			root = tmp;
		else if (node == node->parent->left)
			node->parent->left = tmp;
		else
			node->parent->right = tmp;
		node->parent = tmp;
		tmp->left = node;
	}
	// restucturing을 수행할 때 필요한 함수
	// tree의 node를 오른쪽으로 회전시키는 것처럼 각 node의 부모 자식 관계를 변경해줌
	void rightRotation(Node* node) {
		Node* tmp = node->left;

		node->left = tmp->right;
		if (tmp->right != NULL)
			tmp->right->parent = node;
		tmp->parent = node->parent;

		// root일 경우 예외처리
		if (node->parent == NULL)
			root = tmp;
		else if (node == node->parent->right)
			node->parent->right = tmp;
		else
			node->parent->left = tmp;
		node->parent = tmp;
		tmp->right = node;
	}
	// 애플리케이션 검색
	void searchNode(int ID) {
		Node* nownode = findNode(ID);
	
		if(nownode != NULL)
			cout << depth(nownode) << " " << nownode->name << " " << nownode->capacity << " " << nownode->price << "\n";
		else
			cout << "NULL" << "\n";
	}
	// 애플리케이션 업데이트
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
	// 애플리케이션 할인
	void discountNode(int x, int y, int P) {
		inorderTraversal(root, x, y, P);
	}
	// 중위순회를 통해 모든 node를 순회하면서 범위에 해당하는 애플리케이션 할인율 적용
	void inorderTraversal(Node* node, int x, int y, int P) {
		if (node != NULL) {
			inorderTraversal(node->left, x, y, P);
			if ((node->ID >= x) && (node->ID <= y))
				node->price = (node->price * (100 - P) / 100);
			inorderTraversal(node->right, x, y, P);
		}
	}
	// 입력된 ID를 가진 node를 찾는 함수
	Node* findNode(int ID) {
		Node* nownode = root;

		// binary search tree의 '자신보다 작은 데이터는 왼쪽 자식에, 자신보다 큰 데이터는 오른쪽 자식에 있다.'의 특징 이용
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
	// node의 depth를 구하는 함수
	int depth(Node* node) {
		if (node->parent == NULL) {
			return 0;
		}
		return depth(node->parent) + 1; // node의 parent가 존재한다면 depth를 1 늘리고 재귀 수행
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
		// 애플리케이션 등록
		if (N == 'I') {
			cin >> ID >> name >> capacity >> price;
			Node* node = tree.insertNode(ID, name, capacity, price);
			cout << tree.depth(node) << "\n";
		}
		// 애플리케이션 검색
		else if (N == 'F') {
			cin >> ID;
			tree.searchNode(ID);
		}
		// 애플리케이션 업데이트
		else if (N == 'R') {
			cin >> ID >> name >> capacity >> price;
			tree.updateNode(ID, name, capacity, price);
		}
		// 애플리케이션 할인
		else if (N == 'D') {
			int x, y, P;
			cin >> x >> y >> P;
			tree.discountNode(x, y, P);
		}
	}
	return 0;
}