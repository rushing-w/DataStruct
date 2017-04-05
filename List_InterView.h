#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<assert.h>

typedef int DataType;
typedef struct Node
{
	DataType data;
	struct Node* next;   // 下一个结点的地址
}Node, *PNode;

// 对链表进行初始化
void InitList(PNode* pHead)
{
	assert(pHead);
	*pHead = NULL;
}

// 在链表中开辟新结点
PNode BuyNode(DataType data)
{
	PNode tmp = (PNode)malloc(sizeof(Node));
	if (tmp != NULL)
	{
		tmp->data = data;
		tmp->next = NULL;
	}
	return tmp;
}

// 对链表进行尾插
void PushBack(PNode* pHead, DataType data)
{
	PNode New = NULL;
	PNode Cur = NULL;
	assert(pHead);
	if (*pHead == NULL)
	{
		*pHead = BuyNode(data);
	}
	else
	{
		New = BuyNode(data);
		Cur = *pHead;
		while (Cur->next)
		{
			Cur = Cur->next;
		}
		Cur->next = New;
	}
}

// 对链表进行尾删
void PopBack(PNode* pHead)
{
	assert(pHead);
	if ((*pHead) == NULL)
	{
		return;
	}
	else if ((*pHead)->next == NULL)
	{
		free(pHead);
		(*pHead) = NULL;
	}
	else
	{
		PNode Cur = *pHead;
		while (Cur)
		{
			if (Cur->next->next == NULL)
			{
				PNode Pre = Cur->next;
				free(Pre);
				Pre = NULL;
				Pre = Cur;
				Pre->next = NULL;
				break;
			}
			Cur = Cur->next;
		}
	}
}

//void PopBack(PNode* pHead)
//{
//	assert(pHead != NULL);
//	PNode Cur = *pHead;
//	PNode DelNode = NULL;
//	if (Cur->next == NULL)
//	{
//		*pHead = NULL;
//	}
//	while (Cur->next->next != NULL)
//	{
//		Cur = Cur->next;
//	}
//
//	//Cur->next = NULL;
//
//	DelNode = Cur->next;
//	free(DelNode);
//	//free(*(Cur->next));
//
//	DelNode = NULL;
//	Cur->next = NULL;
//}

// 对链表进行头插
void PushFront(PNode* pHead, DataType data)
{
	PNode New = NULL;
	PNode Cur = NULL;
	assert(pHead);
	if ((*pHead) == NULL)
	{
		*pHead = BuyNode(data);
	}
	else
	{
		New = BuyNode(data);
		Cur = (*pHead);
		New->next = (*pHead);
		(*pHead) = New;
	}
}

// 对链表进行头删
void PopFront(PNode* pHead)
{
	assert(pHead);
	if ((*pHead) == NULL)
	{
		return;
	}
	else if ((*pHead)->next == NULL)
	{
		free(pHead);
		(*pHead) = NULL;
	}
	else
	{
		PNode Cur = *pHead;
		PNode Pre = (*pHead)->next;
		*pHead = Pre;
		free(Cur);
		Cur = NULL;
	}
}

// 查找数据为data的结点
PNode Find(PNode pHead, DataType data)
{
	PNode Cur = pHead;
	while (Cur)
	{
		if (Cur->data == data)
		{
			return Cur;
		}
		Cur = Cur->next;
	}
	return NULL;
}

// 对链表进行插入操作
void Insert(PNode pos, DataType data)
{
	assert(pos);
	PNode New = BuyNode(data);
	if (New != NULL)
	{
		New->next = pos->next;
		pos->next = New;
	}
	else
	{
		return;
	}
}

// 打印链表
void PrintList(PNode pHead)
{
	PNode Cur = pHead;
	while (Cur != NULL)
	{
		printf("%d ", Cur->data);
		Cur = Cur->next;
	}
	printf("NULL\n");
}

// 删除链表Pos位置的元素
void Erase(PNode* pHead, PNode pos)
{
	PNode Cur = *pHead;
	PNode Pre = NULL;
	assert(pHead);
	assert(pos);
	if (pos == *pHead)
	{
		PopFront(pHead);
	}
	else
	{
		while (Cur)
		{
			if (Cur->next == NULL)
			{
				PopBack(&pos);
				break;
			}
			else if (Cur->next == pos)
			{
				Pre = Cur->next;
				Cur->next = Pre->next;
				free(Pre);
				Pre = NULL;
				break;
			}
			Cur = Cur->next;
		}
	}
}

// 删除链表中第一个值为data的元素
void Remove(PNode* pHead, DataType data)
{
	PNode Cur = *pHead;
	PNode Pre = NULL;
	assert(pHead);
	if ((*pHead) == NULL)
	{
		return;
	}
	else
	{
		while (Cur)
		{
			Pre = Cur->next;
			if (Pre->data == data)
			{
				Cur->next = Pre->next;
				free(Pre);
				Pre = NULL;
				break;
			}
			Cur = Cur->next;
		}
	}
}

// 删除链表中所有值为Data的元素
void RemoveAll(PNode* pHead, DataType data)
{
	PNode Cur = *pHead;
	PNode Pre = NULL;
	assert(pHead);
	if ((*pHead) == NULL)
	{
		return;
	}
	else
	{
		while (Cur)
		{
			Pre = Cur->next;
			if (Pre->data == data)
			{
				Cur->next = Pre->next;
				free(Pre);
				Pre = NULL;
			}
			Cur = Cur->next;
			if (Cur->next == NULL)
			{
				break;
			}
		}
	}
}

// 销毁链表
void Destroy(PNode* pHead)
{
	PNode Cur = *pHead;
	PNode Pre = NULL;
	assert(pHead);
	while (Cur)
	{
		Pre = Cur;
		Cur = Cur->next;
		free(Pre);
		Pre = NULL;
	}
}

// 链表是否为空
int Empty(PNode pHead)
{
	assert(pHead);
	if (pHead == NULL)
		return 0;
	else
		return 1;
}

// 求链表中结点的个数
int Size(PNode pHead)
{
	PNode Cur = pHead;
	int count = 0;
	assert(pHead);
	while (Cur)
	{
		count++;
		Cur = Cur->next;
	}
	return count;
}

// 找到链表的最后一个结点
PNode Back(PNode pHead)
{
	PNode Cur = pHead;
	assert(pHead);
	while (Cur)
	{
		if (Cur->next == NULL)
		{
			return Cur;
		}
		Cur = Cur->next;
	}
	return NULL;
}

// 逆序打印链表（递归）
void PrintListFromT2H(PNode pHead)
{
	if (pHead)
	{
		PrintListFromT2H(pHead->next);
		printf("%d ", pHead->data);
	}
}

// 不采用遍历方式删除无头单链表的非尾结点
void DeleteNotTail(PNode pos)
{
	assert(pos);
	PNode Cur = NULL;
	Cur = pos->next;
	pos->data = Cur->data;
	pos->next = Cur->next;
	free(Cur);
}

// 不采用遍历方式插入无头单链表的非头结点
void InsertNotHead(PNode pHead, PNode pos, DataType data)
{
	assert(pHead);
	assert(pos);
	PNode tmp = BuyNode(pos->data);
	tmp->next = pos->next;
	pos->next = tmp;
	pos->data = data;
}

// 实现约瑟夫环
int JosephCircle(PNode* pHead, int M)
{
	PNode Cur = *pHead;
	PNode Pre = NULL;
	int a = 0;
	assert(pHead);
	while (Cur)//通过循环建立一个环
	{
		if (Cur->next == NULL)
		{
			Cur->next = *pHead;
			break;
		}
		Cur = Cur->next;
	}
	Cur = *pHead;//从头结点重新开始
	while (1)
	{
		a = M;
		if (Cur == Cur->next)
		{
			break;
		}
		while (--a)
		{
			Cur = Cur->next;
		}
		Pre = Cur->next;
		Cur->data = Pre->data;
		Cur->next = Pre->next;
		free(Pre);
		Pre = NULL;
	}
	return Cur->data;
}

// 找到最中间的结点
PNode FindMidNode(PNode pHead)
{
	PNode Fast = pHead;
	PNode Slow = pHead;
	while (Fast&&Fast->next)
	{
		Fast = Fast->next->next;
		Slow = Slow->next;
	}
	return Slow;
}

// 找到倒数第k个结点
PNode FindLastKNode(PNode pHead, int k)
{
	PNode Fast = pHead;
	PNode Slow = pHead;
	while (Fast && k--)
	{
		Fast = Fast->next;
	}
	if (k > 0)
	{
		return NULL;
	}
	while (Fast)
	{
		Fast = Fast->next;
		Slow = Slow->next;
	}
	return Slow;
}

// 逆置/反转单链表
void ReverseList(PNode* pHead)
{
	PNode Pre = *pHead;
	PNode Cur = NULL;
	PNode Next = NULL;
	assert(pHead);
	if (*pHead == NULL || (*pHead)->next == NULL)
	{
		return;
	}
	Cur = Pre->next;
	Next = Cur->next;
	while (Next)
	{
		Cur->next = Pre;
		Pre = Cur;
		Cur = Next;
		Next = Next->next;
	}
	Cur->next = Pre;
	(*pHead)->next = NULL;
	*pHead = Cur;
}

// 冒泡排序
void Bubblesort(PNode* pHead)
{
	PNode Cur = *pHead;
	assert(pHead);
	if ((*pHead) == NULL || (*pHead)->next == NULL)
	{
		return;
	}
	while (Cur)
	{
		PNode Pre = Cur->next;
		while (Pre)
		{
			if (Cur->data > Pre->data)
			{
				DataType tmp = Cur->data;
				Cur->data = Pre->data;
				Pre->data = tmp;
			}
			Pre = Pre->next;
		}
		Cur = Cur->next;
	}
}

// 合并链表
PNode MergeList(PNode pList1, PNode pList2)
{
	PNode New = NULL;
	PNode Cur = NULL;
	PNode P1 = pList1;
	PNode P2 = pList2;
	assert(pList1);
	assert(pList2);
	if (pList1 == NULL)
	{
		return pList2;
	}
	if (pList2 == NULL)
	{
		return pList1;
	}
	if (pList1 == pList2)
	{
		return pList1;
	}
	if (P1->data <= P2->data)
	{
		Cur = P1;
		P1 = P1->next;
	}
	else
	{
		Cur = P2;
		P2 = P2->next;
	}
	New = Cur;
	while (P1 && P2)
	{
		if (P1->data <= P2->data)
		{
			Cur->next = P1;
			P1 = P1->next;
		}
		else
		{
			Cur->next = P2;
			P2 = P2->next;
		}
		Cur = Cur->next;
	}
	if (P1 == NULL)
	{
		Cur->next = P2;
	}
	else
	{
		Cur->next = P1;
	}
	return New;
}

// 判断两条链表是否相交
int IsCross(PNode pHead1, PNode pHead2)
{
	PNode P1 = NULL;
	PNode P2 = NULL;
	assert(pHead1);
	assert(pHead2);
	if (pHead1 == NULL || pHead2 == NULL)
	{
		return 0;
	}
	P1 = Back(pHead1);
	P2 = Back(pHead2);
	if (P1 == P2)
	{
		return 1;
	}
	else
		return 0;
}

// 获取交点
PNode GetCrossNode(PNode pHead1, PNode pHead2)
{
	PNode P1 = pHead1;
	PNode P2 = pHead2;
	int s1 = Size(pHead1);
	int s2 = Size(pHead2);
	int i = 0;
	assert(pHead1);
	assert(pHead2);
	if (IsCross(pHead1, pHead2) == 0)
	{
		return NULL;
	}
	if (s1 - s2 >= 0)
	{
		i = s1 - s2;
		while (i)
		{
			P1 = P1->next;
			i--;
		}
	}
	else
	{
		i = s2 - s1;
		while (i)
		{
			P2 = P2->next;
			i--;
		}
	}
	while (P1 && P2)
	{
		if (P1 == P2)
		{
			break;
		}
		P1 = P1->next;
		P2 = P2->next;
	}
	return P1;
}

// 判断是否有环并返回
PNode HasCircle(PNode pHead)
{
	PNode Fast = pHead;
	PNode Slow = pHead;
	assert(pHead);
	while (Fast && Fast->next)
	{
		Fast = Fast->next->next;
		Slow = Slow->next;
		if (Fast == Slow)
		{
			return Fast;
		}
	}
	return NULL;
}

// 获取环的长度
int GetCirclrLen(PNode pMeetNode)
{
	if (pMeetNode == NULL)
	{
		return 0;
	}
	int count = 1;
	PNode Cur = pMeetNode;
	while (pMeetNode != Cur->next)
	{
		count++;
		Cur = Cur->next;
	}
	return count;
}

// 获取环的入口点-->推理的过程-->   一个指针从头结点出发，另一个从快慢指针的交点出发，
//那么这两个指针最终一定会在环的入口点相遇
PNode GetEnterNode(PNode pHead, PNode pMeetNode)
{
	PNode Cur = pHead;
	PNode Pre = pMeetNode;
	if (pHead == NULL || pMeetNode == NULL)
	{
		return NULL;
	}
	while (Cur != Pre)
	{
		Cur = Cur->next;
		Pre = Pre->next;
	}
	return Cur;
}

// 是否带环相交
int IsCrossWithCircle(PNode pHead1, PNode pHead2)
{
	PNode pMeetNode1 = HasCircle(pHead1);
	PNode pMeetNode2 = HasCircle(pHead2);
	if (pHead1 == NULL || pHead2 == NULL)
	{
		return 0;
	}
	else if (pMeetNode1 == NULL && pMeetNode2 == NULL)//两链表都不带环
	{
		if (IsCross(pHead1, pHead2) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if (pMeetNode1 != NULL && pMeetNode2 != NULL)// 两链表都带环
	{
		while (pMeetNode1->next != pMeetNode1)//在两个链表的环中，如果相交，那么必定是拥有公共环，此刻只需要遍历寻找环中的公共点
		{
			if (pMeetNode1 == pMeetNode2)
			{
				return 1;
			}
			pMeetNode1 = pMeetNode1->next;
		}
		if (pMeetNode1 == pMeetNode2)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else// 一个链表带环，一个不带环，两个链表不可能相交
	{
		return 0;
	}
}

//求两个已排序单链表中相同的数据--打印出来
void UnionSet(PNode l1, PNode l2)
{
	PNode New = NULL;
	PNode plist1 = l1;
	PNode plist2 = l2;
	while ((plist1 != NULL) && (plist2 != NULL))
	{
		if (plist1->data < plist2->data)
		{
			plist1 = plist1->next;
		}
		else if (plist1->data > plist2->data)
		{
			plist2 = plist2->next;
		}
		else
		{
			PushBack(&New, plist1->data);
			plist1 = plist1->next;
			plist2 = plist2->next;
		}
	}
	PrintList(New);
}