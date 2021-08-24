#include <stdio.h>
#include <stdlib.h>
struct Node {
	int val = 0;
	Node* pre = NULL;
	Node* next = NULL;
};
struct stack {
	Node* top = NULL;//记录下当前栈顶
	int size = 0;
	Node* node = NULL;
};
//在栈中嵌套链表，并使用size 变量记录下栈的大小
stack* inintStack() {
	stack* sta = (stack*)malloc(sizeof(stack));
	sta->node = (Node*)malloc(sizeof(Node));
	sta->size = 0;
	sta->top = NULL;
	return sta;
}
int isEmpty(stack* sta) {
	return sta->size == 0;
}
int getSize(stack* sta) {
	return sta->size;
}
int getTop(stack* sta) {
	return sta->size == 0 ? -1 : sta->top->val;
}
void push(stack* sta, int val) {
	sta->size++;
	//创建新节点
	sta->node->next = (Node*)malloc(sizeof(Node));
	sta->node->next->val = val;
	//连接
	sta->node->next->pre = sta->node;
	sta->node = sta->node->next;
	//下一节点置空，更新top
	sta->node->next = NULL;
	sta->top = sta->node;
}

void show(stack* sta) {
	Node* cur = sta->node;
	int size = sta->size;
	printf("stack: ");
	if (size == 0)
		printf("NULL");
	while (size-- > 0) {
		printf("%d\t", cur->val);
		cur = cur->pre;
	}
	printf("\n");
}
int pop(stack* sta) {
	if (sta->size == 0)
		return -1;
	sta->size--;
	sta->node = sta->node->pre;
	free(sta->node->next);
	sta->top = sta->node;
	return 1;
}
int main() {
	//测试的一些操作
	stack* sta = inintStack();
	show(sta);
	push(sta, 1); push(sta, 2); push(sta, 3);show(sta);
	pop(sta); show(sta); 
	pop(sta); show(sta);
	push(sta, 4); show(sta);
	pop(sta); show(sta);
	pop(sta); show(sta);
	return 0;
}

