#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <list>
#include <algorithm>
using namespace std;
#define max(a,b) (((a)>(b)) ? (a):(b))

typedef struct node {
	int key; //나이
	char sex; //이름, 성별(f,m)
	char name[3];
	struct node* left, *right;
}NODE;

//AVL구현
NODE* rotate_LL(NODE* parent) { //LL회전(오른쪽으로 회전한다)
	NODE* child = parent->left;
	parent->left = child->right; //부모의 왼쪽 노드=자식 오른쪽 노드(부모에게는 NULL값이 왼/오에 달림)
	child->right = parent; //부모를 자식의 오른쪽 노드로(자식 오른쪽에다가 부모 트리 연결)
	return child;
}

NODE* rotate_RR(NODE* parent) { //RR회전(왼쪽으로 회전한다)
	NODE* child = parent->right;
	parent->right = child->left;
	child->left = parent;
	return child;
}

NODE* rotate_LR(NODE* parent) { //LR회전(오른쪽-왼쪽으로 회전한다)
	NODE* child = parent->left;
	parent->left = rotate_RR(child); //자식 노드 RR회전해서 균형맞춤
	return rotate_LL(parent); //부모를 LL회전해서 자식의 오른쪽으로
}

NODE* rotate_RL(NODE* parent) { //RL회전(왼쪽-오른쪽으로 회전한다)
	NODE* child = parent->right;
	parent->right = rotate_LL(child);
	return rotate_RR(parent);
}

int get_height(NODE* node) { //노드를 기준으로 트리의 높이 구하기
	int height = 0;
	if (node != NULL)
		//NULL이 아니면 루트 1개는 무조건 있음
		//max(왼쪽 높이, 오른쪽 높이)
		height = 1 + max(get_height(node->left), get_height(node->right));
	return height;
}

int get_balance(NODE* node) { //균형이 되는 높이를 구함
	if (node == NULL) return 0;
	return get_height(node->left) - get_height(node->right); //왼쪽 서브트리 높이 - 오른쪽 서브트리 높이 
}

NODE* balance_tree(NODE** node) { //트리의 균형 맞춤
	int height = get_balance(*node); //왼쪽 높이-오른쪽 높이
	if (height > 1) { //왼쪽이 더 크다면 왼쪽 서브트리 균형을 맞춤
		if (get_balance((*node)->left) > 0) { //왼쪽 노드가 더 크면
			//printf("LL\n");
			*node = rotate_LL(*node); //오른쪽으로 돌림
		}
		else {
			//printf("LR\n");
			*node = rotate_LR(*node); //아니면 오른쪽-왼쪽으로
		}
	}
	else if (height < -1) { //오른쪽 서브트리 균형을 맞춤
		if (get_balance((*node)->right) < 0) { //오른쪽 노드가 더 크면
			//printf("RR\n");
			*node = rotate_RR(*node); //왼쪽으로 돌림
		}
		else {
			//printf("RL\n");
			*node = rotate_RL(*node); //아니면 왼쪽-오른쪽으로
		}
	}
	//0이면 균형이 맞는 것이므로 패스
	return *node;
}

int cnt = 0;
NODE* search(NODE* root, int key) {
	if (root == NULL) return NULL;
	cnt++;
	printf("%d->", root->key);

	if (root->key == key) return root; //key가 자기 자신인 경우 찾았으므로 리턴
	else if (root->key < key) return search(root->right, key); //현재키<찾으려는 키면 오른쪽 트리로(큰수로)
	else return search(root->left, key); //아니면 왼쪽 트리로
}

NODE* insert(NODE** node, int key, char name[], char sex) {
	if (*node == NULL) {
		*node = (NODE*)malloc(sizeof(NODE));
		(*node)->key = key;
		for (int i = 0; i < 4; i++) (*node)->name[i] = name[i];
		(*node)->sex = sex;
		(*node)->left = (*node)->right = NULL;
	}
	else if (key < (*node)->key) {
		(*node)->left = insert(&(*node)->left, key, name, sex); //찾는키<노드키면 왼쪽
		(*node) = balance_tree(node); //균형 맞춤
	}
	else if (key > (*node)->key) {
		(*node)->right = insert(&(*node)->right, key, name, sex); //반대면 오르쪽
		(*node) = balance_tree(node); //균형 맞춤
	}
	else {
		printf("중복된 데이터로 삽입 실패");
	}
	return *node; //노드 포인터 반환
}


void inorder(NODE* root) {
	if (root != NULL) {
		inorder(root->left);

		printf("%d세 이름: ", root->key);
		for (int i = 0; i < 3; i++) printf("%c", root->name[i]);
		;		if (root->sex == 'f') printf(", 여자\n");
		else printf(", 남자\n");

		inorder(root->right);
	}
}

int main() {
	srand((unsigned)time(NULL));

	char rand_name[7][3]; //랜덤 이름 생성
	for (int i = 0; i < 7; i++) {
		int j;
		for (j = 0; j < 3; j++) {
			rand_name[i][j] = 'a' + rand() % 26;
		}
		rand_name[i][j] = 0;
	}

	NODE* root = NULL;
	root = insert(&root, 50, rand_name[0], 'f');
	insert(&root, 30, rand_name[1], 'm');
	insert(&root, 20, rand_name[2], 'm');
	insert(&root, 40, rand_name[3], 'f');
	insert(&root, 70, rand_name[4], 'm');
	insert(&root, 60, rand_name[5], 'f');
	insert(&root, 80, rand_name[6], 'f');

	printf("모든 사람 오름차순 출력\n");
	inorder(root);


	while (1) {
		int findAge = 0;
		printf("\n찾고 싶은 사람의 나이는?(종료는 0) ");
		scanf("%d", &findAge);
		if (findAge == 0) exit(1);

		cnt = 0;
		NODE* res = search(root, findAge);
		if (res == NULL) printf("\n그런 나이의 사람은 없습니다");
		else {
			printf("\n%d세 이름: ", res->key);
			for (int i = 0; i < 3; i++) printf("%c", root->name[i]);
			if (res->sex == 'f') printf(", 여자\n");
			else printf(", 남자\n");
			printf("\n총 %d번만에 찾았습니다\n", cnt);
		}
	}
}