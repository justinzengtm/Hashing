#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define MaxTableSize 20 /*ɢ�б����󳤶�*/
#define KeyLength 20 /*����ؼ����ַ�������󳤶�*/
//typedef char ElemType[KeyLength+1]; /*�ؼ����������ַ�����ʽ*/
typedef int Index; /*ɢ�е�ַ������*/

/*������Ĵ洢�ṹ*/
typedef struct ListNode *PtrlNode;
struct ListNode {
	char Data[KeyLength+1]; /*�����е����ݽ��*/
	PtrlNode Next; /*ָ����һ������ָ��*/
};
typedef PtrlNode Position; /*ɢ�е�ַ����*/
typedef PtrlNode List; /*����*/ 

/*ɢ�б�Ĵ洢�ṹ*/
 typedef struct TableNode *HashTable; /*ɢ�б������*/
 struct TableNode {
 	int TableSize; /*ɢ�б����󳤶�*/
 	List Heads; /*�������ͷ��������*/
 };

/*��������*/
/*���ش���N�Ҳ�����MaxTableSize����С����*/
int NextPrime(int N); 
/*ɢ�б�ĳ�ʼ������*/
HashTable CreateTable(int TableSize);
/*ɢ�к���*/
int Hash(char Data[], int TableSize);
/*�������ӷ��Ĳ��Һ���*/
Position ScFind(HashTable H, char Key[]);
/*�������ӷ��Ĳ��뺯��*/
bool ScInsert(HashTable H, char Key[]);
/*����ɢ�б�*/
void BuildHashTable(HashTable H);
/*���ɢ�б�*/
void PrintHashTable(HashTable H);
/*�ͷ�ɢ�б�*/
void DestroyTable(HashTable H);
/*����������ɾ������*/
bool ScDelete(HashTable H);
/*�û�ʹ�ò��Һ���*/
void UserScFind(HashTable H);
 /*�û�ʹ�ò��뺯��*/
void UserScInsert(HashTable H);
 
int main(int argc, char const *argv[]) 
{
	bool deleteResult; 
	
	HashTable H=CreateTable(MaxTableSize); /*��ʼ��һ���յ�ɢ�б�*/ 
	printf("H->TableSize=%d\n", H->TableSize);
	BuildHashTable(H); /*����ɢ�б�*/
	PrintHashTable(H); /*������ɢ�б���������*/ 
	UserScFind(H); /*�û�ʹ�ò��ҽӿ�*/
	UserScInsert(H); /*�û�ʹ�ò���ӿ�*/
	ScDelete(H); /*ɾ������*/
 
	DestroyTable(H);
	
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

/*ɢ�б�ĳ�ʼ������*/
HashTable CreateTable(int TableSize) 
{
	HashTable H;
	int i=0;
	
	H=(HashTable)malloc(sizeof(struct TableNode));
	/*ʹɢ�б����󳤶�������*/
	H->TableSize=NextPrime(TableSize);
	 
	/*��ʼ��������ÿ����Ԫ��һ���������ͷ���*/
	H->Heads=(List)malloc(H->TableSize*sizeof(struct ListNode)); 
	/*��ʼ��ÿһ��������ͷ���*/
	for (i=0; i<H->TableSize; i++) {
		H->Heads[i].Data[0]='\0';
		H->Heads[i].Next=NULL;  
	}
	 
	return H; 
} 

/*����ɢ�б�*/
void BuildHashTable(HashTable H) 
{	
	printf("\n�봴��ɢ�б�:\n"); 
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

/*���ɢ�б�*/
void PrintHashTable(HashTable H) 
{
	int i=0;
	float count=0; 
	Position P;
	P=(Position)malloc(sizeof(struct ListNode));
	
	printf("\nɢ�б�Ϊ:\n");
	for (i=0; i<H->TableSize; i++) {
		P=H->Heads[i].Next;
		printf("S[%d]��", i);
		while (P) {
			printf("%s ", P->Data);;
			count++;
			P=P->Next;
		} 
	 	printf("\n");
	}
	printf("װ������=%f\n", count/H->TableSize);
	
	return;
} 

/*ɢ�к���*/
int Hash(char Data[], int TableSize) 
{
	int Pos=Data[0]-'a'; //�ַ�������ĸת�������� 
	
	return (int)(Pos%TableSize);
}

/*�������ӷ��Ĳ��Һ���*/
Position ScFind(HashTable H, char Key[]) 
{
	Position P;
	Index Pos; /*ɢ�е�ַ*/
	
	Pos=Hash(Key, H->TableSize); /*��ó�ʼɢ�е�ַ*/
	//printf("Ԫ�ز������:\t��ʼɢ�е�ַ=%d\n", Pos);
	P=H->Heads[Pos].Next; /*�Ӽ������ɢ�е�ַ������ͷ����Next��ʼ����*/
	/*����*/
	while (P && strcmp(P->Data, Key)) {
		P=P->Next;
	} 
	return P; /*��ʱPָ���ҵ��Ľ��*/
}

/*�������ӷ��Ĳ��뺯��*/
bool ScInsert(HashTable H, char Key[]) 
{
	Position P, NewCell;
	Index Pos;
	
	P=ScFind(H, Key); /*��ɢ�б��в���Ҫ�����Ԫ��*/
	/*������Ԫ����ɢ�б���û��*/
	if (!P) {
		NewCell=(Position)malloc(sizeof(struct ListNode)); /*����ý��*/
		strcpy(NewCell->Data, Key); /*���ݸ��ƽ����*/
		Pos=Hash(Key, H->TableSize); /*��øò���Ԫ�صĲ����ַ*/ 
		/*��NewCell���뵽H->Heads[Pos]����ĵ�һ�����*/
		NewCell->Next=H->Heads[Pos].Next;
		H->Heads[Pos].Next=NewCell;
		return true;
	} else {
		/*������Ԫ���Ѿ�����*/
		printf("�ùؼ����Ѵ�����ɢ�б���.\n");
		return false;
	} 
}

/*����������ɾ������*/
bool ScDelete(HashTable H) 
{
	Position P, Q;
	Index Pos;
	char Key[KeyLength+1];
	
	printf("\n������Ҫ��ɾ����Ԫ��:");
	gets(Key);
	
	Pos=Hash(Key, H->TableSize); /*���Ҫɾ����Ԫ�س�ʼɢ�е�ַ*/
	P=H->Heads[Pos].Next; /*�Ӽ������ɢ�е�ַ������ͷ����Next��ʼ����*/
	
	/*��Ϊɾ��������ԭ��,�ȿ�����������Ҫɾ����Ԫ����ͷ���*/
	if (P && strcmp(P->Data,Key)==0) {
		H->Heads[Pos].Next=P->Next;
		free(P);
		printf("ɾ���ɹ�!\n");
		PrintHashTable(H); /*ɾ����������һ������*/
		return true;
	} else {
		/*����*/
		while (P->Next && strcmp(P->Next->Data, Key)) {
			P=P->Next;
		} 
		/*���ɢ�б���û�����Ԫ��*/
		if (!P->Next) {
			printf("Ҫɾ����Ԫ�ز�������ɢ�б���.\n");
			return false;
		} else {
			Q=P->Next; /*��ʱQ��Ҫɾ����Ԫ�صĽ��*/
			P->Next=Q->Next;
			free(Q);
			printf("ɾ���ɹ�!\n");
			PrintHashTable(H); /*ɾ����������һ������*/
			return true;
		}
	}

}

/*�ͷ�ɢ�б�*/
void DestroyTable(HashTable H) 
{
	int i;
	Position P, Tmp;
	
	/*�ͷ�ɢ�б��е�ÿһ����Ԫ���������*/
	for (i=0; i<H->TableSize; i++) {
		P=H->Heads[i].Next;
		while (P) {
			Tmp=P->Next;
			free(P);
			P=Tmp;
		}
	}
	free(H->Heads); /*�ͷŴ��ͷ��������*/
	free(H); /*�ͷ�ɢ�б�*/
	
	return; 
} 

/*�û�ʹ�ò��Һ���*/
void UserScFind(HashTable H) 
{
	Position P;
	Index Pos; /*ɢ�е�ַ*/
	char Key[KeyLength+1];
	
	printf("\n������Ҫ���ҵ�Ԫ�أ�");
	gets(Key);
	Pos=Hash(Key, H->TableSize); /*��ó�ʼɢ�е�ַ*/
	P=H->Heads[Pos].Next; /*�Ӽ������ɢ�е�ַ������ͷ����Next��ʼ����*/
	/*����*/
	while (P && strcmp(P->Data, Key)) {
		P=P->Next;
	} 
//	printf("Ԫ��%s���ҳɹ�! ɢ�е�ַΪ:%d\n", P->Data);
	if (P) {
		printf("Ԫ��%s���ҳɹ�! ɢ�е�ַΪ:%d\n", P->Data, Pos);
	} else {
		printf("û�����Ԫ��.\n");
	}
	
	return;
}

/*�û�ʹ�ò��뺯��*/
void UserScInsert(HashTable H) 
{
	Position P, NewCell;
	Index Pos;
	char Key[KeyLength+1];
	
	printf("\n������Ҫ�����Ԫ��:");
	gets(Key);
	P=ScFind(H, Key); /*��ɢ�б��в���Ҫ�����Ԫ��*/
	/*������Ԫ����ɢ�б���û��*/
	if (!P) {
		NewCell=(Position)malloc(sizeof(struct ListNode)); /*����ý��*/
		strcpy(NewCell->Data, Key); /*���ݸ��ƽ����*/
		Pos=Hash(Key, H->TableSize); /*��øò���Ԫ�صĲ����ַ*/ 
		/*��NewCell���뵽H->Heads[Pos]����ĵ�һ�����*/
		NewCell->Next=H->Heads[Pos].Next;
		H->Heads[Pos].Next=NewCell;
		printf("����ɹ���ɢ�е�ַΪ:%d\n", Pos);
	} else {
		/*������Ԫ���Ѿ�����*/
		printf("�ùؼ����Ѵ�����ɢ�б���.\n");
	} 
	
	return;
}
