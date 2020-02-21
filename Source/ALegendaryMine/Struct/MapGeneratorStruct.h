// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Engine/Classes/Engine/NetSerialization.h"

#include "MapGeneratorStruct.generated.h"

#define ARR_SIZE 1000

USTRUCT(Atomic)
struct FRoomData
{
	GENERATED_USTRUCT_BODY()

		int X = 0;
	int Y = 0;

	int SX = 0;
	int SY = 0;

	int RoomNumber = 0;

	//방 종류, 방 위치, 방 회전
};

USTRUCT(Atomic)
struct FRoadData
{
	GENERATED_USTRUCT_BODY()

		int X = 0;
	int Y = 0;

	int V1 = 0;
	FVector V1R = FVector();

	int V2 = 0;
	FVector V2R = FVector();
};

USTRUCT(Atomic)
struct FArrayStack
{
	GENERATED_USTRUCT_BODY()

		int StackArr[ARR_SIZE];
	int TopIndex;
};

USTRUCT(Atomic)
struct FListNode
{
	GENERATED_USTRUCT_BODY()

		int data = 0;

	struct FListNode * next;
};

USTRUCT(Atomic)
struct FList
{
	GENERATED_USTRUCT_BODY()

		FListNode * Head;
	FListNode * Cur;
	FListNode * Before;

	int NumOfData = 0;
	int(*comp)(int d1, int d2);
};


USTRUCT(Atomic)
struct FGraphEdge
{
	GENERATED_USTRUCT_BODY()

		int Vertex1 = 0; //첫 번째 정점
	int Vertex2 = 0; //두 번째 정점
	int Weight = 0; //가중치
};

typedef int PriorityComp(FGraphEdge d1, FGraphEdge d2);

USTRUCT(Atomic)
struct FHeapElem
{
	GENERATED_USTRUCT_BODY()

		int pr;
	FGraphEdge data;
};

USTRUCT(Atomic)
struct FPQueue
{
	GENERATED_USTRUCT_BODY()

		PriorityComp * Comp;
	int NumOfData;
	FGraphEdge HeapArr[ARR_SIZE];
};

USTRUCT(Atomic)
struct FALGraph
{
	GENERATED_USTRUCT_BODY()

		int NumV = 0;
	int NumE = 0;

	FList * AdjList; //간선 정보
	int * VisitInfo; //새로 추가된 멤버
	FPQueue Queue; //간선 가중치 정보 저장
};

/*배열기반 스택 함수들*/
void StackInit(FArrayStack * _pstack);
bool SIsEmpty(FArrayStack * _pstack);

void SPush(FArrayStack * _pstack, int _data);
int SPop(FArrayStack * _pstack);
int SPeek(FArrayStack * _pstack);
/*배열기반 스택 함수들*/

/*연결리스트 함수들*/
void ListInit(FList * _plist);

void FInsert(FList * _plist, int _data);
void SInsert(FList * _plist, int _data);
void LInsert(FList * _plist, int _data);

bool LFirst(FList * _plist, int * _data);
bool LNext(FList * _plist, int * _data);

int LRemove(FList * _plist);
int LCount(FList * _plist);

void SetSortRule(FList * _plist, int(*comp)(int d1, int d2));
/*연결리스트 함수들*/

/*우선순위 큐 함수들*/
int GetParentIDX(int _idx);
int GetLChildIDX(int _idx);
int GetRChildIDX(int _idx);
int GetHiPriChildIDX(FPQueue * _ppq, int _idx);

void PQueueInit(FPQueue * _ppq, PriorityComp _pc);
bool PQIsEmpty(FPQueue * _ppq);
void PEnqueue(FPQueue * _ppq, FGraphEdge _data);
FGraphEdge PDequeue(FPQueue * _ppq);
/*우선순위 큐 함수들*/

/*그래프 함수들*/
void GraphInit(FALGraph * _pg, int _nv);
void GraphDestory(FALGraph * _pg);
void AddEdge(FALGraph * _pg, int _fromV, int _toV, int _weight);
void RemoveEdge(FALGraph * _pg, int _fromV, int _toV);
void RemoveWayEdge(FALGraph * _pg, int _fromV, int _toV);
void RecoverEdge(FALGraph * _pg, int _fromV, int _toV, int _weight);
bool IsConnvertex(FALGraph * _pg, int _v1, int _v2);
void ShowGraphEdgeInfo(FALGraph * _pg);
bool VisitVertex(FALGraph * _pg, int _visitV);
void DFShowGraphVertex(FALGraph * _pg, int _startV);
void ConKruskalMST(FALGraph * _pg);
void ShowGraphEdgeWeightInfo(FALGraph * _pg);
/*그래프 함수들*/