#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define MaxTableSize 20 /*散列表的最大长度*/
#define KeyLength 20 /*定义关键词字符串的最大长度*/
//typedef char ElemType[KeyLength+1]; /*关键词类型用字符串形式*/
typedef int Index; /*散列地址的类型*/

/*单链表的存储结构*/
typedef struct ListNode *PtrlNode;
struct ListNode {
	char Data[KeyLength+1]; /*链表中的数据结点*/
	PtrlNode Next; /*指向下一个结点的指针*/
};
typedef PtrlNode Position; /*散列地址类型*/
typedef PtrlNode List; /*链表*/ 

/*散列表的存储结构*/
 typedef struct TableNode *HashTable; /*散列表的类型*/
 struct TableNode {
 	int TableSize; /*散列表的最大长度*/
 	List Heads; /*存放链表头结点的数组*/
 };

/*函数声明*/
/*返回大于N且不超过MaxTableSize的最小素数*/
int NextPrime(int N); 
/*散列表的初始化函数*/
HashTable CreateTable(int TableSize);
/*散列函数*/
int Hash(char Data[], int TableSize);
/*分离链接法的查找函数*/
Position ScFind(HashTable H, char Key[]);
/*分离链接法的插入函数*/
bool ScInsert(HashTable H, char Key[]);
/*构建散列表*/
void BuildHashTable(HashTable H);
/*输出散列表*/
void PrintHashTable(HashTable H);
/*释放散列表*/
void DestroyTable(HashTable H);
/*分离链表法的删除函数*/
bool ScDelete(HashTable H);
/*用户使用查找函数*/
void UserScFind(HashTable H);
 /*用户使用插入函数*/
void UserScInsert(HashTable H);
 
int main(int argc, char const *argv[]) 
{
	bool deleteResult; 
	
	HashTable H=CreateTable(MaxTableSize); /*初始化一个空的散列表*/ 
	printf("H->TableSize=%d\n", H->TableSize);
	BuildHashTable(H); /*构建散列表*/
	PrintHashTable(H); /*构建完散列表后输出看看*/ 
	UserScFind(H); /*用户使用查找接口*/
	UserScInsert(H); /*用户使用插入接口*/
	ScDelete(H); /*删除操作*/
 
	DestroyTable(H);
	
	return 0; 
}

/*返回大于N且不超过MaxTableSize的最小素数*/
int NextPrime(int N) 
{
	int i, p;
	
	p=(N%2)? N+2: N+1; /*获得大于N的最小奇数*/
	
	//找出从p开始的下一个素数 
	while (p<=MaxTableSize) {
		for (i=p-1; i>2; i--) {
			//如果p不是素数,break; 
			if (!(p%i)) {
				break;
			}
		}
		//如果是素数
		if (i==2) {
			break;
		} else {
			//探测下一个奇数是否是素数 
			p+=2;
		}
	}	
}

/*散列表的初始化函数*/
HashTable CreateTable(int TableSize) 
{
	HashTable H;
	int i=0;
	
	H=(HashTable)malloc(sizeof(struct TableNode));
	/*使散列表的最大长度是素数*/
	H->TableSize=NextPrime(TableSize);
	 
	/*初始化数组中每个单元格一个单链表的头结点*/
	H->Heads=(List)malloc(H->TableSize*sizeof(struct ListNode)); 
	/*初始化每一个数组中头结点*/
	for (i=0; i<H->TableSize; i++) {
		H->Heads[i].Data[0]='\0';
		H->Heads[i].Next=NULL;  
	}
	 
	return H; 
} 

/*构建散列表*/
void BuildHashTable(HashTable H) 
{	
	printf("\n请创建散列表:\n"); 
	while (1) {
		char i[KeyLength+1];
		gets(i);
		if (strcmp(i, "finish")!=0) {
			ScInsert(H, i);
		} else {
			break;
		}
	} 
	
	return;
} 

/*输出散列表*/
void PrintHashTable(HashTable H) 
{
	int i=0;
	float count=0; 
	Position P;
	P=(Position)malloc(sizeof(struct ListNode));
	
	printf("\n散列表为:\n");
	for (i=0; i<H->TableSize; i++) {
		P=H->Heads[i].Next;
		printf("S[%d]：", i);
		while (P) {
			printf("%s ", P->Data);;
			count++;
			P=P->Next;
		} 
	 	printf("\n");
	}
	printf("装填因子=%f\n", count/H->TableSize);
	
	return;
} 

/*散列函数*/
int Hash(char Data[], int TableSize) 
{
	int Pos=Data[0]-'a'; //字符串首字母转换成数字 
	
	return (int)(Pos%TableSize);
}

/*分离链接法的查找函数*/
Position ScFind(HashTable H, char Key[]) 
{
	Position P;
	Index Pos; /*散列地址*/
	
	Pos=Hash(Key, H->TableSize); /*获得初始散列地址*/
	//printf("元素插入操作:\t初始散列地址=%d\n", Pos);
	P=H->Heads[Pos].Next; /*从计算出的散列地址的链表头结点的Next开始查找*/
	/*遍历*/
	while (P && strcmp(P->Data, Key)) {
		P=P->Next;
	} 
	return P; /*此时P指向找到的结点*/
}

/*分离链接法的插入函数*/
bool ScInsert(HashTable H, char Key[]) 
{
	Position P, NewCell;
	Index Pos;
	
	P=ScFind(H, Key); /*在散列表中查找要插入的元素*/
	/*如果这个元素在散列表中没有*/
	if (!P) {
		NewCell=(Position)malloc(sizeof(struct ListNode)); /*插入该结点*/
		strcpy(NewCell->Data, Key); /*数据复制进结点*/
		Pos=Hash(Key, H->TableSize); /*获得该插入元素的插入地址*/ 
		/*将NewCell插入到H->Heads[Pos]链表的第一个结点*/
		NewCell->Next=H->Heads[Pos].Next;
		H->Heads[Pos].Next=NewCell;
		return true;
	} else {
		/*如果这个元素已经存在*/
		printf("该关键词已存在于散列表中.\n");
		return false;
	} 
}

/*分离链表法的删除函数*/
bool ScDelete(HashTable H) 
{
	Position P, Q;
	Index Pos;
	char Key[KeyLength+1];
	
	printf("\n请输入要插删除的元素:");
	gets(Key);
	
	Pos=Hash(Key, H->TableSize); /*获得要删除的元素初始散列地址*/
	P=H->Heads[Pos].Next; /*从计算出的散列地址的链表头结点的Next开始查找*/
	
	/*因为删除操作的原因,先看特殊情况如果要删除的元素在头结点*/
	if (P && strcmp(P->Data,Key)==0) {
		H->Heads[Pos].Next=P->Next;
		free(P);
		printf("删除成功!\n");
		PrintHashTable(H); /*删除完后再输出一次链表*/
		return true;
	} else {
		/*遍历*/
		while (P->Next && strcmp(P->Next->Data, Key)) {
			P=P->Next;
		} 
		/*如果散列表中没有这个元素*/
		if (!P->Next) {
			printf("要删除的元素不存在于散列表中.\n");
			return false;
		} else {
			Q=P->Next; /*此时Q是要删除的元素的结点*/
			P->Next=Q->Next;
			free(Q);
			printf("删除成功!\n");
			PrintHashTable(H); /*删除完后再输出一次链表*/
			return true;
		}
	}

}

/*释放散列表*/
void DestroyTable(HashTable H) 
{
	int i;
	Position P, Tmp;
	
	/*释放散列表中的每一个单元格里的链表*/
	for (i=0; i<H->TableSize; i++) {
		P=H->Heads[i].Next;
		while (P) {
			Tmp=P->Next;
			free(P);
			P=Tmp;
		}
	}
	free(H->Heads); /*释放存放头结点的数组*/
	free(H); /*释放散列表*/
	
	return; 
} 

/*用户使用查找函数*/
void UserScFind(HashTable H) 
{
	Position P;
	Index Pos; /*散列地址*/
	char Key[KeyLength+1];
	
	printf("\n请输入要查找的元素：");
	gets(Key);
	Pos=Hash(Key, H->TableSize); /*获得初始散列地址*/
	P=H->Heads[Pos].Next; /*从计算出的散列地址的链表头结点的Next开始查找*/
	/*遍历*/
	while (P && strcmp(P->Data, Key)) {
		P=P->Next;
	} 
//	printf("元素%s查找成功! 散列地址为:%d\n", P->Data);
	if (P) {
		printf("元素%s查找成功! 散列地址为:%d\n", P->Data, Pos);
	} else {
		printf("没有这个元素.\n");
	}
	
	return;
}

/*用户使用插入函数*/
void UserScInsert(HashTable H) 
{
	Position P, NewCell;
	Index Pos;
	char Key[KeyLength+1];
	
	printf("\n请输入要插入的元素:");
	gets(Key);
	P=ScFind(H, Key); /*在散列表中查找要插入的元素*/
	/*如果这个元素在散列表中没有*/
	if (!P) {
		NewCell=(Position)malloc(sizeof(struct ListNode)); /*插入该结点*/
		strcpy(NewCell->Data, Key); /*数据复制进结点*/
		Pos=Hash(Key, H->TableSize); /*获得该插入元素的插入地址*/ 
		/*将NewCell插入到H->Heads[Pos]链表的第一个结点*/
		NewCell->Next=H->Heads[Pos].Next;
		H->Heads[Pos].Next=NewCell;
		printf("插入成功！散列地址为:%d\n", Pos);
	} else {
		/*如果这个元素已经存在*/
		printf("该关键词已存在于散列表中.\n");
	} 
	
	return;
}
