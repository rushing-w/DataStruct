#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<assert.h>

typedef int DataType;
typedef struct Node
{
	DataType data;
	struct Node* next;   // ��һ�����ĵ�ַ
}Node, *PNode;

// ��������г�ʼ��
void InitList(PNode* pHead)
{
	assert(pHead);
	*pHead = NULL;
}

// �������п����½��
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

// ���������β��
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

// ���������βɾ
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

// ���������ͷ��
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

// ���������ͷɾ
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

// ��������Ϊdata�Ľ��
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

// ��������в������
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

// ��ӡ����
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

// ɾ������Posλ�õ�Ԫ��
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

// ɾ�������е�һ��ֵΪdata��Ԫ��
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

// ɾ������������ֵΪData��Ԫ��
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

// ��������
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

// �����Ƿ�Ϊ��
int Empty(PNode pHead)
{
	assert(pHead);
	if (pHead == NULL)
		return 0;
	else
		return 1;
}

// �������н��ĸ���
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

// �ҵ���������һ�����
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

// �����ӡ�����ݹ飩
void PrintListFromT2H(PNode pHead)
{
	if (pHead)
	{
		PrintListFromT2H(pHead->next);
		printf("%d ", pHead->data);
	}
}

// �����ñ�����ʽɾ����ͷ������ķ�β���
void DeleteNotTail(PNode pos)
{
	assert(pos);
	PNode Cur = NULL;
	Cur = pos->next;
	pos->data = Cur->data;
	pos->next = Cur->next;
	free(Cur);
}

// �����ñ�����ʽ������ͷ������ķ�ͷ���
void InsertNotHead(PNode pHead, PNode pos, DataType data)
{
	assert(pHead);
	assert(pos);
	PNode tmp = BuyNode(pos->data);
	tmp->next = pos->next;
	pos->next = tmp;
	pos->data = data;
}

// ʵ��Լɪ��
int JosephCircle(PNode* pHead, int M)
{
	PNode Cur = *pHead;
	PNode Pre = NULL;
	int a = 0;
	assert(pHead);
	while (Cur)//ͨ��ѭ������һ����
	{
		if (Cur->next == NULL)
		{
			Cur->next = *pHead;
			break;
		}
		Cur = Cur->next;
	}
	Cur = *pHead;//��ͷ������¿�ʼ
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

// �ҵ����м�Ľ��
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

// �ҵ�������k�����
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

// ����/��ת������
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

// ð������
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

// �ϲ�����
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

// �ж����������Ƿ��ཻ
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

// ��ȡ����
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

// �ж��Ƿ��л�������
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

// ��ȡ���ĳ���
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

// ��ȡ������ڵ�-->����Ĺ���-->   һ��ָ���ͷ����������һ���ӿ���ָ��Ľ��������
//��ô������ָ������һ�����ڻ�����ڵ�����
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

// �Ƿ�����ཻ
int IsCrossWithCircle(PNode pHead1, PNode pHead2)
{
	PNode pMeetNode1 = HasCircle(pHead1);
	PNode pMeetNode2 = HasCircle(pHead2);
	if (pHead1 == NULL || pHead2 == NULL)
	{
		return 0;
	}
	else if (pMeetNode1 == NULL && pMeetNode2 == NULL)//������������
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
	else if (pMeetNode1 != NULL && pMeetNode2 != NULL)// ����������
	{
		while (pMeetNode1->next != pMeetNode1)//����������Ļ��У�����ཻ����ô�ض���ӵ�й��������˿�ֻ��Ҫ����Ѱ�һ��еĹ�����
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
	else// һ�����������һ�����������������������ཻ
	{
		return 0;
	}
}

//��������������������ͬ������--��ӡ����
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