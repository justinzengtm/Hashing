/*
 *��ͻ���� - ƽ��̽�ⷨ
 *date:2018/12/28
 *author:justinzeng
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define MaxTableSize 10 /*ɢ�б����󳤶�*/
typedef int ElemType; /*�ؼ��ʵ���������Ϊ����*/
typedef int Index; /*ɢ�б�ĵ�ַ����Ϊ����*/
typedef int Position; /*���ݵ�λ��������ɢ�б�ĵ�ַ����һ��*/ 

/*ɢ�б��е�Ԫ���״̬*/
typedef enum {
	Legitimate, Empty, Deleted
} EntryType;

/*ɢ�б�Ԫ������ݽṹ*/
typedef struct HashEntry Cell;
struct HashEntry {
	ElemType Data; /*��Ԫ���ŵ�Ԫ��*/
	EntryType Info; /*��Ԫ���״̬*/
};

/*ɢ�б�Ĵ洢�ṹ*/
typedef struct TableNode *HashTable;
struct TableNode {
	Cell *Cells; /*���ɢ�б������*/
	int TableSize; /*ɢ�б����󳤶�*/
}; 

/*��������*/
int NextPrime(int N); /*���ش���N�Ҳ�����MaxTableSize����С����*/
HashTable CreateTable(int TableSize); /*��ʼ��ɢ�б�*/
void BulidHashTable(HashTable H); /*����ɢ�б�*/
void PrintHashTable(HashTable H); /*���ɢ�б�*/
int Hash(ElemType Data, int TableSize); /*ɢ�к���*/
Position SquareFind(HashTable H, ElemType Data); /*ƽ��̽�ⷨ*/
bool Insert(HashTable H, ElemType Data); /*ɢ�б�Ĳ������*/
bool Delete(HashTable H); /*ɢ�б��ɾ������*/
Position UserSquareFind(HashTable H, ElemType Data); /*�û�ʹ��ƽ��̽�ⷨ����*/

int main(int argc, char const *argv[]) 
{
	int i, data, findPos;
	bool result;
	
	HashTable H=CreateTable(MaxTableSize); /*��ʼ��һ��ɢ�б�*/
	printf("H->TableSize=%d\n", H->TableSize);
	BulidHashTable(H); /*����ɢ�б�*/
	PrintHashTable(H); /*���ɢ�б�*/
	
	printf("\n������Ҫ���ҵ�Ԫ�أ�");
	scanf("%d", &data);
	findPos=UserSquareFind(H, data);
	if (data!=-1) {
		printf("����Ԫ��:%d ɢ�е�ַ%d", data, findPos);
	}
	printf("\n\n������Ҫ�����Ԫ�أ�");
	scanf("%d", &data);
	Insert(H, data);
	
	result=Delete(H); /*ɾ��һ��Ԫ��*/
	if (result) {
		printf("ɾ���ɹ�!");
		PrintHashTable(H); 
	}
	
	return 0;
}
/*�ش���N�Ҳ�����MaxTableSize����С����*/ 
int NextPrime(int N) 
{
	int i, p;
	
	p=(N%2)? N+2: N+1; /*��ô���N����С����*/
	
	/*Ȼ��������С������ʼѰ����һ������*/
	while (p<=MaxTableSize) {
		for (i=p-1; i>2; i--) {
			if (!(p%i)) {
				break;
			}
		}
		/*���������*/
		if (i==2) {
			break;
		} else {
			p+=2; /*Ȼ����һ�������Ƿ�������*/
		}
	}
}

/*��ʼ��ɢ�б�*/
HashTable CreateTable(int TableSize) 
{
	int i=0; 
	HashTable H;
	
	H=(HashTable)malloc(sizeof(struct TableNode));
	/*ʹ��ɢ�б����󳤶�������*/
	H->TableSize=NextPrime(TableSize); 
	H->Cells=(Cell*)malloc(H->TableSize*sizeof(Cell));
	
	/*��ʼ��ɢ�б���ÿһ����Ԫ���״̬Ϊ��*/
	for (i=0; i<=H->TableSize; i++) {
		H->Cells[i].Info=Empty;
	} 
	
	return H;
}

/*����ɢ�б�*/
void BulidHashTable(HashTable H) 
{
	int i=0; 
	
	printf("\n�봴��ɢ�б�:");
	while (i!=-1) {
		scanf("%d", &i);
		Insert(H, i);
	}
	
	return;
}

/*���ɢ�б�*/
void PrintHashTable(HashTable H) 
{
	int i=0;
	float count=0;
	
	printf("\nɢ�б�Ϊ:\n"); 
	for(i=0; i<=H->TableSize; i++) {
		if (H->Cells[i].Info==Legitimate) {
			printf("S[%d]��", i);
			printf("%d", H->Cells[i].Data);
			count++;
		} else if (H->Cells[i].Info==Empty) {
			printf("S[%d]��", i);
		} else {
			printf("S[%d]����Ԫ��״̬��Deleted", i);
		}
		printf("\n");
	}
	printf("װ������=%f\n", count/H->TableSize);
	
	return;
}

/*ɢ�к���*/
int Hash(ElemType Data, int TableSize) 
{
	return (int)(Data%TableSize);
}

/*ƽ��̽�ⷨ*/
Position SquareFind(HashTable H, ElemType Data) 
{
	/*ǰ��ָʾ��ʼ����λ��,����ָʾʵ�ʲ���λ��*/
	Position currentPos, newPos;
	int collisionNum=0; /*��¼������ͻ�Ĵ���*/
	int numCount=0;
	
	if (Data!=-1) {
		/*���ȸ���ɢ�к������������λ��*/
		newPos=currentPos=Hash(Data, H->TableSize);
		/*�õ���ʼ����λ�ú�,�жϸ�λ���Ƿ�Ϊ�ջ����Ƿ�����ͻ*/
		while (H->Cells[newPos].Info!=Empty && H->Cells[newPos].Data!=Data) {
			collisionNum++;
			/*����������β���,Ҳ����������ƽ��̽��*/
			if (++numCount%2) {
				/*12 -12 22 -22 32 -32 42 -42*/
				/*1   2  3   4  5   6  7   8*/ 
				newPos=currentPos+(numCount+1)*(numCount+1)/4;
				/*�жϲ���λ���Ƿ���Խ��,�о͵���*/
				if (newPos>=H->TableSize) {
					newPos=newPos%H->TableSize;
				} 
			} else {
				/*�����ż���β���,Ҳ���Ǹ�����ƽ��̽��*/
				newPos=currentPos-numCount*numCount/4;
				/*�±�Խ���ж�*/
				while (newPos<0) {
					newPos+=H->TableSize;
				}
			}
		} 
		printf("Ԫ��%d�������:\tɢ�е�ַ=%d\tʵ�ʵ�ַ=%d\t��ͻ����=%d\n", Data,currentPos, newPos, collisionNum);
	} 
	
	return newPos;
}

/*ɢ�б�Ĳ������*/
bool Insert(HashTable H, ElemType Data) 
{
	/*����ǰ��̽�����λ��*/
	Position Pos=SquareFind(H, Data);
	
	if (Data!=-1 && H->Cells[Pos].Info!=Legitimate) {
		/*��������Ԫ��û�б�ռ��,˵��Data���Բ��뵽���Posλ��*/
		/*����*/
		H->Cells[Pos].Data=Data;
		H->Cells[Pos].Info=Legitimate;
		return true; 
	} else if (Data==-1) {
		return false;
	}
}

/*ɢ�б��ɾ������*/
bool Delete(HashTable H) 
{
	ElemType Data;
	
	printf("\n������Ҫɾ����Ԫ�أ�");
	scanf("%d", &Data);
	/*ɾ��ǰ����Ԫ���Ƿ����*/
	Position Pos=UserSquareFind(H, Data);
	
	if (Data!=-1 && H->Cells[Pos].Info!=Empty && H->Cells[Pos].Data==Data) {
		/*��������Ԫ��Ԫ�ز����ҵ���Ҫɾ����Ԫ��*/
		/*�޸������Ԫ���״̬Ϊ"ɾ��"*/
		H->Cells[Pos].Info=Deleted;
		return true;
	} else {
		return false;
	}
}

/*�û�ʹ��ƽ��̽�ⷨ����*/
Position UserSquareFind(HashTable H, ElemType Data) 
{
	/*ǰ��ָʾ��ʼ����λ��,����ָʾʵ�ʲ���λ��*/
	Position currentPos, newPos;
	int collisionNum=0; /*��¼������ͻ�Ĵ���*/
	int numCount=0;
	
	if (Data!=-1) {
		/*���ȸ���ɢ�к������������λ��*/
		newPos=currentPos=Hash(Data, H->TableSize);
		/*�õ���ʼ����λ�ú�,�жϸ�λ���Ƿ�Ϊ�ջ����Ƿ�����ͻ*/
		while (H->Cells[newPos].Info!=Empty && H->Cells[newPos].Data!=Data) {
			collisionNum++;
			/*����������β���,Ҳ����������ƽ��̽��*/
			if (++numCount%2) {
				/*12 -12 22 -22 32 -32 42 -42*/
				/*1   2  3   4  5   6  7   8*/ 
				newPos=currentPos+(numCount+1)*(numCount+1)/4;
				/*�жϲ���λ���Ƿ���Խ��,�о͵���*/
				if (newPos>=H->TableSize) {
					newPos=newPos%H->TableSize;
				} 
			} else {
				/*�����ż���β���,Ҳ���Ǹ�����ƽ��̽��*/
				newPos=currentPos-numCount*numCount/4;
				/*�±�Խ���ж�*/
				while (newPos<0) {
					newPos+=H->TableSize;
				}
			}
		} 
	} 
	
	return newPos;
}
