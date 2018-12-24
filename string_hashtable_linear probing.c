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

#define MaxTableSize 26 /*ɢ�б����󳤶�*/
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
Position LinearFind(HashTable H, ElemType Data[]); /*����̽�����*/
bool Insert(HashTable H, ElemType Data[]); /*ɢ�б�Ĳ������*/
void Find(HashTable H); /*�û�ʹ������̽�����*/

int main(int argc, char const *argv[]) 
{
	char data[15];
	HashTable H=CreateTable(MaxTableSize); /*��ʼ��һ���յ�ɢ�б�*/ 
	//printf("H->TableSize=%d\n", H->TableSize);
	BuildHashTable(H); /*����ɢ�б�*/
	printf("\nɢ�б�Ϊ:\n");
	PrintHashTable(H);
	
	Find(H); /*�û�ʹ�õĲ��Һ����ӿ�*/
	printf("\n������Ҫ�����Ԫ��:");
	gets(data);
	Insert(H, data);
	 
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
	printf("�봴��ɢ�б�:\n"); 
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
	
	for (i=0; i<H->TableSize; i++) {
		if (H->Cells[i].Info==Legitimate) {
			printf("S[%d]��", i);
			puts(H->Cells[i].Data);
			count++;
		} else if (H->Cells[i].Info==Empty) {
			printf("S[%d]��", i);
			printf("\n");
		}
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

/*����̽�����*/
Position LinearFind(HashTable H, ElemType Data[]) 
{
	Position currentPos, newPos; /*һ��ָʾ��ʼ����λ��,һ��ָʾ��λ��*/
	int collisionNum=0; /*��¼������ͻ�Ĵ���*/
	if (strcmp(Data, "-1")!=0) {
		/*����ɢ�к����ҵ���Ԫ��Ӧ�÷ŵĳ�ʼλ��*/
		newPos=currentPos=Hash(Data, H->TableSize); 
		/*�����λ�õĵ�Ԫ��ǿ�,���Ҳ���Ҫ�ҵ�Ԫ��ʱ,�������˳�ͻ*/
		while (H->Cells[newPos].Info!=Empty && strcmp(H->Cells[newPos].Data, Data)!=0) {
			/*��ͻ����+1*/
			collisionNum++;
			/*����̽��,����+1*/
			newPos=currentPos+collisionNum; /*!!!!*/
			/*��������жϿ�λ���±��Ƿ�Խ��,�Ǿ͵�������ȷλ��*/
			if (newPos>=H->TableSize) {
				newPos=newPos%H->TableSize;
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
	Position Pos=LinearFind(H, Data); /*̽��Data�Ƿ��Ѵ���ɢ�б���*/
	
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

/*�û�ʹ������̽�����*/
void Find(HashTable H) 
{
	Position currentPos, newPos; /*һ��ָʾ��ʼλ��,һ��ָʾ��λ��*/
	int collisionNum=0; /*��¼������ͻ�Ĵ���*/
	char Data[15];
	
	printf("\n������Ҫ���ҵ�Ԫ��:");
	gets(Data); //�û�����Ҫ���ҵ��ַ��� 
	if (strcmp(Data, "finish")!=0) {
		/*����ɢ�к����ҵ���Ԫ��Ӧ�÷ŵĳ�ʼλ��*/
		newPos=currentPos=Hash(Data, H->TableSize); 
		/*�����λ�õĵ�Ԫ��ǿ�,���Ҳ���Ҫ�ҵ�Ԫ��ʱ,�������˳�ͻ*/
		while (H->Cells[newPos].Info!=Empty && strcmp(H->Cells[newPos].Data, Data)!=0) {
			/*��ͻ����+1*/
			collisionNum++;
			/*����̽��,����+1*/
			newPos=currentPos+collisionNum; /*!!!!*/
			/*��������жϿ�λ���±��Ƿ�Խ��,�Ǿ͵�������ȷλ��*/
			if (newPos>=H->TableSize) {
				newPos=newPos%H->TableSize;
			} 
		}
	}
	
	printf("���ҽ����");
	printf("Ԫ�ش���! ɢ�е�ַΪ=%d\n", newPos);
	
	return; 
}
