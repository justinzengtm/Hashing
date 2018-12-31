/*
 * �ַ���ɢ�б� - ����̽�� 
 * date:2018/12/20
 *author:justin
 */ 
  
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define MaxTableSize 10 /*ɢ�б����󳤶�*/
typedef char ElemType; /*�ؼ��ʵ���������Ϊ�ַ�����*/
typedef int Index; /*ɢ�б�ĵ�ַ����Ϊ����*/
typedef int Position; /*�������ڵ�λ��������ɢ�б�ĵ�ַ����һ��*/

/*ɢ�б��е�Ԫ���״̬����,LegitimateΪ�кϷ�Ԫ��;EmptyΪ�յ�Ԫ��;DeletedΪ����ɾ��Ԫ��*/
typedef enum {
	Legitimate, Empty, Deleted
} EntryType; 

/*ɢ�б�Ԫ��Ĵ洢�ṹ*/
typedef struct HashEntry Cell;
struct HashEntry {
	ElemType Data[15]; /*��Ԫ���ŵ�Ԫ��*/
	EntryType Info; /*��Ԫ���״̬*/
};

/*ɢ�б�Ĵ洢�ṹ*/
typedef struct TableNode *HashTable;
struct TableNode {
	Cell *Cells; /*���ɢ�б����ݵ�����*/
	int TableSize; /*ɢ�б����󳤶�*/
};

/*��������*/
int NextPrime(int N); /*���ش���N�Ҳ�����MaxTableSize����С����*/
HashTable CreateTable(int TableSize); /*��������ʼ��ɢ�б�*/
void BuildHashTable(HashTable H); /*����ɢ�б�*/
void PrintHashTable(HashTable H); /*���ɢ�б�*/
int Hash(ElemType Data[], int TableSize); /*ɢ�к���*/ 
Position SquareFind(HashTable H, ElemType Data[]); /*ƽ��̽�����*/
bool Insert(HashTable H, ElemType Data[]); /*ɢ�б�Ĳ������*/
bool Delete(HashTable H); /*ɢ�б��ɾ������*/
Position UserSquareFind(HashTable H, char Data[]); /*�û�ʹ��ƽ��̽�ⷨ����*/

int main(int argc, char const *argv[]) 
{
	char data[15];
	bool deleteResult;
	Position findPos;
	
	HashTable H=CreateTable(MaxTableSize); /*��ʼ��һ���յ�ɢ�б�*/ 
	//printf("H->TableSize=%d\n", H->TableSize);
	BuildHashTable(H); /*����ɢ�б�*/
	PrintHashTable(H);
	
	printf("\n������Ҫ���ҵ�Ԫ��:");
	gets(data);
	findPos=UserSquareFind(H, data);
	if (findPos!=-1) {
		printf("����Ԫ��:");
		puts(data);
		printf("ɢ�е�ַ:%d\n", findPos);
	}
	printf("\n������Ҫ�����Ԫ��:");
	gets(data);
	Insert(H, data);
	
	deleteResult=Delete(H);
	if (deleteResult) {
		printf("ɾ���ɹ�!");
		PrintHashTable(H); 
	}
	
	return 0;
}

/*���ش���N�Ҳ�����MaxTableSize����С����*/
int NextPrime(int N) 
{
	int i, p;
	
	p=(N%2)? N+2: N+1; /*��ô���N����С����*/
	
	//�ҳ���p��ʼ����һ������ 
	while (p<=MaxTableSize) {
		for (i=p-1; i>2; i--) {
			//���p��������,break; 
			if (!(p%i)) {
				break;
			}
		}
		//���������
		if (i==2) {
			break;
		} else {
			//̽����һ�������Ƿ������� 
			p+=2;
		}
	}	
}

/*����ʼ��ɢ�б�*/
HashTable CreateTable(int TableSize) 
{
	int i=0;
	HashTable H;
	
	H=(HashTable)malloc(sizeof(struct TableNode));
	H->TableSize=NextPrime(TableSize); /*ʹ��ɢ�б����󳤶�������*/ 
	H->Cells=(Cell*)malloc(H->TableSize*sizeof(Cell));
	
	/*��ʼ��ÿһ����Ԫ���״̬Ϊ��*/
	for (i=0; i<H->TableSize; i++) {
		H->Cells[i].Info=Empty;
	}
	
	return H;
}

/*����ɢ�б�*/
void BuildHashTable(HashTable H) 
{	
	printf("\n�봴��ɢ�б�:\n"); 
	while (1) {
		char i[15];
		gets(i);
		if (strcmp(i, "finish")!=0) {
			Insert(H, i);
		} else {
			break;
		}
	} 
	
	return;
} 

/*���ɢ�б�*/
void PrintHashTable(HashTable H) 
{
	int i=0;
	float count=0; 
	
	printf("\nɢ�б�Ϊ:\n");
	for (i=0; i<H->TableSize; i++) {
		if (H->Cells[i].Info==Legitimate) {
			printf("S[%d]��", i);
			puts(H->Cells[i].Data);
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
int Hash(ElemType Data[], int TableSize) 
{
	int Pos=Data[0]-'a'; //�ַ�������ĸת�������� 
	
	return (int)(Pos%TableSize);
}

/*ƽ��̽�����*/
Position SquareFind(HashTable H, ElemType Data[]) 
{
	/*ǰ��ָʾ��ʼ����λ��,����ָʾʵ�ʲ���λ��*/
	Position currentPos, newPos;
	int collisionNum=0; /*��¼������ͻ�Ĵ���*/
	int numCount=0;
	
	if (strcmp(Data, "-1")!=0) {
		/*����ɢ�к����ҵ���Ԫ��Ӧ�÷ŵĳ�ʼλ��*/
		newPos=currentPos=Hash(Data, H->TableSize); 
		/*�����λ�õĵ�Ԫ��ǿ�,���Ҳ���Ҫ�ҵ�Ԫ��ʱ,�������˳�ͻ*/
		while (H->Cells[newPos].Info!=Empty && strcmp(H->Cells[newPos].Data, Data)!=0) {
			/*��ͻ����+1*/
			collisionNum++;
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
		//puts(Data);
		printf("Ԫ�ز������:\tɢ�е�ַ=%d\tʵ�ʵ�ַ=%d\t��ͻ����=%d,\n", currentPos, newPos, collisionNum);
	}	
	return newPos; /*��ʱ��newPosλ����Data��λ��,������һ���յ�Ԫ���λ��*/
} 

/*ɢ�б�Ĳ������*/
bool Insert(HashTable H, ElemType Data[]) 
{
	Position Pos=SquareFind(H, Data); /*̽��Data�Ƿ��Ѵ���ɢ�б���*/
	
	if (strcmp(Data, "finish")!=0 && H->Cells[Pos].Info!=Legitimate) {
		/*��������Ԫ��û�б�ռ��,˵��Data���Բ��������Posλ��*/ 
		/*����*/
		strcpy(H->Cells[Pos].Data, Data); /*�ַ������ƽ�ȥ*/ 
		H->Cells[Pos].Info=Legitimate;
		return true;
	} else if (strcmp(Data, "finish")==0) {
		return;
	} else {
		printf("��Ԫ���Ѵ�����ɢ�б���.\n");
		return false;
	}
}

/*ɢ�б��ɾ������*/
bool Delete(HashTable H) 
{
	char i[15];
	
	printf("\n������Ҫɾ����Ԫ�أ�");
	gets(i);
	/*ɾ��ǰ����Ԫ���Ƿ����*/
	Position Pos=UserSquareFind(H, i);
	
	if (strcmp(i, "finish")!=0 && H->Cells[Pos].Info!=Empty && strcmp(H->Cells[Pos].Data, i)==0) {
		/*��������Ԫ��Ԫ�ز����ҵ���Ҫɾ����Ԫ��*/
		/*�޸������Ԫ���״̬Ϊ"ɾ��"*/
		H->Cells[Pos].Info=Deleted;
		return true;
	} else {
		return false;
	}
}

/*�û�ʹ������̽�����*/
Position UserSquareFind(HashTable H, char Data[]) 
{
	Position currentPos, newPos; /*һ��ָʾ��ʼλ��,һ��ָʾ��λ��*/
	int collisionNum=0; /*��¼������ͻ�Ĵ���*/
	int numCount=0;

	if (strcmp(Data, "finish")!=0) {
		/*����ɢ�к����ҵ���Ԫ��Ӧ�÷ŵĳ�ʼλ��*/
		newPos=currentPos=Hash(Data, H->TableSize); 
		/*�����λ�õĵ�Ԫ��ǿ�,���Ҳ���Ҫ�ҵ�Ԫ��ʱ,�������˳�ͻ*/
		while (H->Cells[newPos].Info!=Empty && strcmp(H->Cells[newPos].Data, Data)!=0) {
			/*��ͻ����+1*/
			collisionNum++;
			/*����̽��,����+1*/
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
