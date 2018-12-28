/*
 * 散列表 - 线性探测 
 * date:2018/12/28
 *author:justin
 */ 
  
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define MaxTableSize 20 /*散列表的最大长度*/
typedef int ElemType; /*关键词的数据类型为整型*/
typedef int Index; /*散列表的地址类型为整型*/
typedef int Position; /*数据所在的位置类型与散列表的地址类型一致*/

/*散列表中单元格的状态类型,Legitimate为有合法元素;Empty为空单元格;Deleted为有已删除元素*/
typedef enum {
	Legitimate, Empty, Deleted
} EntryType; 

/*散列表单元格的存储结构*/
typedef struct HashEntry Cell;
struct HashEntry {
	ElemType Data; /*单元格存放的元素*/
	EntryType Info; /*单元格的状态*/
};

/*散列表的存储结构*/
typedef struct TableNode *HashTable;
struct TableNode {
	Cell *Cells; /*存放散列表数据的数组*/
	int TableSize; /*散列表的最大长度*/
};

/*函数声明*/
int NextPrime(int N); /*返回大于N且不超过MaxTableSize的最小素数*/
HashTable CreateTable(int TableSize); /*创建并初始化散列表*/
void BuildHashTable(HashTable H); /*构建散列表*/
void PrintHashTable(HashTable H); /*输出散列表*/
int Hash(ElemType Data, int TableSize); /*散列函数*/ 
Position LinearFind(HashTable H, ElemType Data); /*线性探测查找*/
bool Insert(HashTable H, ElemType Data); /*散列表的插入操作*/
void Find(HashTable H); /*用户使用线性探测查找*/

int main(int argc, char const *argv[]) 
{
	int i, data;
	HashTable H=CreateTable(MaxTableSize); /*初始化一个空的散列表*/ 
	printf("H->TableSize=%d\n", H->TableSize);
	BuildHashTable(H); /*构建散列表*/
	printf("\n散列表为:\n");
	PrintHashTable(H);
	
	Find(H); /*用户使用的查找函数接口*/
	printf("\n请输入要插入的元素:");
	scanf("%d", &data);
	Insert(H, data);
	 
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

/*创建始化散列表*/
HashTable CreateTable(int TableSize) 
{
	int i=0;
	HashTable H;
	
	H=(HashTable)malloc(sizeof(struct TableNode));
	H->TableSize=NextPrime(TableSize); /*使得散列表的最大长度是素数*/ 
	H->Cells=(Cell*)malloc(H->TableSize*sizeof(Cell));
	
	/*初始化每一个单元格的状态为空*/
	for (i=0; i<H->TableSize; i++) {
		H->Cells[i].Info=Empty;
	}
	
	return H;
}

/*构建散列表*/
void BuildHashTable(HashTable H) 
{
	int i=0;
	
	printf("\n请创建散列表:"); 
	while (i!=-1) {
		scanf("%d", &i);
		Insert(H, i);
	}
	
	return;
} 

/*输出散列表*/
void PrintHashTable(HashTable H) 
{
	int i=0;
	float count=0; 
	
	for (i=0; i<=H->TableSize; i++) {
		if (H->Cells[i].Info==Legitimate) {
			printf("S[%d]：", i);
			printf("%d\t", H->Cells[i].Data);
			count++;
		} else if (H->Cells[i].Info==Empty) {
			printf("S[%d]：", i);
		}
		printf("\n");
	}
	printf("装填因子=%f\n", count/H->TableSize);
	
	return;
} 

/*散列函数*/
int Hash(ElemType Data, int TableSize) 
{
	return (int)(Data%TableSize);
}

/*线性探测查找*/
Position LinearFind(HashTable H, ElemType Data) 
{
	Position currentPos, newPos; /*一个指示初始插入位置,一个指示新位置*/
	int collisionNum=0; /*记录发生冲突的次数*/
	if (Data!=-1) {
		/*根据散列函数找到该元素应该放的初始位置*/
		newPos=currentPos=Hash(Data, H->TableSize); 
		/*如果该位置的单元格非空,并且不是要找的元素时,即发生了冲突*/
		while (H->Cells[newPos].Info!=Empty && H->Cells[newPos].Data!=Data) {
			/*冲突次数+1*/
			collisionNum++;
			/*线性探测,增量+1*/
			newPos=currentPos+collisionNum; /*!!!!*/
			/*特殊情况判断看位置下标是否越界,是就调整成正确位置*/
			if (newPos>=H->TableSize) {
				newPos=newPos%H->TableSize;
			} 
		}
		printf("元素%d插入操作:\t散列地址=%d\t实际地址=%d\t冲突次数=%d\n", Data,currentPos, newPos, collisionNum);
	}
	
	return newPos; /*此时的newPos位置是Data的位置,或者是一个空单元格的位置*/
} 

/*散列表的插入操作*/
bool Insert(HashTable H, ElemType Data) 
{
	Position Pos=LinearFind(H, Data); /*探测Data是否已存在散列表中*/
	
	if (Data!=-1 && H->Cells[Pos].Info!=Legitimate) {
		/*如果这个单元格没有被占用,说明Data可以插入在这个Pos位置*/ 
		/*插入*/
		H->Cells[Pos].Data=Data;
		H->Cells[Pos].Info=Legitimate;
		return true;
	} else if (Data==-1) {
		return;
	} else {
		printf("该元素已存在于散列表中.\n");
		return false;
	}
}

/*用户使用线性探测查找*/
void Find(HashTable H) 
{
	Position currentPos, newPos; /*一个指示初始位置,一个指示新位置*/
	int collisionNum=0; /*记录发生冲突的次数*/
	int Data=0;
	
	printf("\n请输入要查找的元素:");
	scanf("%d", &Data); 
	if (Data!=-1) {
		/*根据散列函数找到该元素应该放的初始位置*/
		newPos=currentPos=Hash(Data, H->TableSize); 
		/*如果该位置的单元格非空,并且不是要找的元素时,即发生了冲突*/
		while (H->Cells[newPos].Info!=Empty && H->Cells[newPos].Data!=Data) {
			/*冲突次数+1*/
			collisionNum++;
			/*线性探测,增量+1*/
			newPos=currentPos+collisionNum; /*!!!!*/
			/*特殊情况判断看位置下标是否越界,是就调整成正确位置*/
			if (newPos>=H->TableSize) {
				newPos=newPos%H->TableSize;
			} 
		}
	}
	
	printf("查找结果：元素%d散列地址为=%d\n", Data, newPos);
	
	return; 
}
