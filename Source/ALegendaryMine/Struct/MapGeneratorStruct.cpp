// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGeneratorStruct.h"

int WhoIsPrecede(int _data1, int _data2);
int PQWeightComp(FGraphEdge d1, FGraphEdge d2);

int WhoIsPrecede(int _data1, int _data2)
{
	if (_data1 < _data2)
		return 0;
	else
		return 1;
}
int PQWeightComp(FGraphEdge d1, FGraphEdge d2)
{
	return d1.Weight - d2.Weight;
}

/*�迭��� ���� �Լ���*/
void MapGeneratorStruct::StackInit(FArrayStack * _pstack)
{
	_pstack->TopIndex = -1;
}

bool MapGeneratorStruct::SIsEmpty(FArrayStack * _pstack)
{
	if (_pstack->TopIndex == -1)
		return true;
	else
		return false;
}

void MapGeneratorStruct::SPush(FArrayStack * _pstack, int _data)
{
	_pstack->TopIndex += 1;
	_pstack->StackArr[_pstack->TopIndex] = _data;
}

int MapGeneratorStruct::SPop(FArrayStack * _pstack)
{
	int rIdx;

	if (SIsEmpty(_pstack))
		exit(-1);

	rIdx = _pstack->TopIndex;
	_pstack->TopIndex -= 1;

	return _pstack->StackArr[rIdx];
}

int MapGeneratorStruct::SPeek(FArrayStack * _pstack)
{
	if (SIsEmpty(_pstack))
		exit(-1);

	return _pstack->StackArr[_pstack->TopIndex];
}
/*�迭��� ���� �Լ���*/

/*���Ḯ��Ʈ �Լ���*/
void MapGeneratorStruct::ListInit(FList * _plist)
{
	_plist->Head = new FListNode;
	_plist->Head->next = nullptr;
	_plist->comp = nullptr;
	_plist->NumOfData = 0;
}

void MapGeneratorStruct::FInsert(FList * _plist, int _data)
{
	FListNode * newNode = new FListNode;
	newNode->data = _data;

	newNode->next = _plist->Head->next;
	_plist->Head->next = newNode;

	(_plist->NumOfData)++;
}

void MapGeneratorStruct::SInsert(FList * _plist, int _data)
{
	FListNode * newNode = new FListNode;
	FListNode * pred = _plist->Head;

	newNode->data = _data;

	while (pred->next != nullptr && _plist->comp(_data, pred->next->data) != 0)
	{
		pred = pred->next;
	}

	newNode->next = pred->next;
	pred->next = newNode;

	(_plist->NumOfData)++;
}

void MapGeneratorStruct::LInsert(FList * _plist, int _data)
{
	if (_plist->comp == nullptr)
		FInsert(_plist, _data);
	else
		SInsert(_plist, _data);
}

bool MapGeneratorStruct::LFirst(FList * _plist, int * _data)
{
	if (_plist->Head->next == nullptr)
		return false;

	_plist->Before = _plist->Head;
	_plist->Cur = _plist->Head->next;

	*_data = _plist->Cur->data;
	return true;
}

bool MapGeneratorStruct::LNext(FList * _plist, int * _data)
{
	if (_plist->Cur->next == nullptr)
		return false;

	_plist->Before = _plist->Cur;
	_plist->Cur = _plist->Cur->next;

	*_data = _plist->Cur->data;
	return true;
}

int MapGeneratorStruct::LRemove(FList * _plist)
{
	FListNode * rpos = _plist->Cur;
	int rdata = rpos->data;

	_plist->Before->next = _plist->Cur->next;
	_plist->Cur = _plist->Before;

	delete(rpos);
	(_plist->NumOfData)--;

	return rdata;
}

int MapGeneratorStruct::LCount(FList * _plist)
{
	return _plist->NumOfData;
}

void MapGeneratorStruct::SetSortRule(FList * _plist, int(*comp)(int d1, int d2))
{
	_plist->comp = comp;
}
/*���Ḯ��Ʈ �Լ���*/

/*�켱���� ť �Լ���*/
int MapGeneratorStruct::GetParentIDX(int _idx)
{
	return _idx / 2;
}

int MapGeneratorStruct::GetLChildIDX(int _idx)
{
	return _idx / 2;
}

int MapGeneratorStruct::GetRChildIDX(int _idx)
{
	return _idx * 2;
}

int MapGeneratorStruct::GetHiPriChildIDX(FPQueue * _ppq, int _idx)
{
	if (GetLChildIDX(_idx) > _ppq->NumOfData)
		return 0;
	else if (GetLChildIDX(_idx) == _ppq->NumOfData)
		return GetLChildIDX(_idx);
	else
	{
		if (_ppq->Comp(_ppq->HeapArr[GetLChildIDX(_idx)],
			_ppq->HeapArr[GetRChildIDX(_idx)]) < 0)
			return GetRChildIDX(_idx);
		else
			return GetLChildIDX(_idx);
	}
}

void MapGeneratorStruct::PQueueInit(FPQueue * _ppq, PriorityComp _pc)
{
	_ppq->NumOfData = 0;
	_ppq->Comp = _pc;
}

bool MapGeneratorStruct::PQIsEmpty(FPQueue * _ppq)
{
	if (_ppq->NumOfData == 0)
		return true;
	else
		return false;
}

void MapGeneratorStruct::PEnqueue(FPQueue * _ppq, FGraphEdge _data)
{
	int idx = _ppq->NumOfData + 1;

	while (idx != 1)
	{
		if (_ppq->Comp(_data, _ppq->HeapArr[GetParentIDX(idx)]) > 0)
		{
			_ppq->HeapArr[idx] = _ppq->HeapArr[GetParentIDX(idx)];
			idx = GetParentIDX(idx);
		}
		else
		{
			break;
		}
	}

	_ppq->HeapArr[idx] = _data;
	_ppq->NumOfData += 1;
}

FGraphEdge MapGeneratorStruct::PDequeue(FPQueue * _ppq)
{
	FGraphEdge retData = _ppq->HeapArr[1];
	FGraphEdge lastElem = _ppq->HeapArr[_ppq->NumOfData];

	int ParentIdx = 1;
	int ChildIdx;

	while (true)
	{
		ChildIdx = GetHiPriChildIDX(_ppq, ParentIdx);

		if (_ppq->Comp(lastElem, _ppq->HeapArr[ChildIdx]) >= 0)
			break;

		_ppq->HeapArr[ParentIdx] = _ppq->HeapArr[ChildIdx];
		ParentIdx = ChildIdx;
	}

	_ppq->HeapArr[ParentIdx] = lastElem;
	_ppq->NumOfData -= 1;

	return retData;
}
/*�켱���� ť �Լ���*/

/*�׷��� �Լ���*/
void MapGeneratorStruct::GraphInit(FALGraph * _pg, int _nv)
{
	int i;

	_pg->AdjList = new FList;
	_pg->NumV = _nv;
	_pg->NumE = 0;

	for (i = 0; i < _nv; i++)
	{
		ListInit(&(_pg->AdjList[i]));

		SetSortRule(&(_pg->AdjList[i]), WhoIsPrecede);
	}

	PQueueInit(&(_pg->Queue), PQWeightComp);
}

void MapGeneratorStruct::GraphDestory(FALGraph * _pg)
{
	if (_pg->AdjList != nullptr)
		delete(_pg->AdjList);
}

void MapGeneratorStruct::AddEdge(FALGraph * _pg, int _fromV, int _toV, int _weight)
{
	FGraphEdge edge = { _fromV, _toV, _weight };

	LInsert(&(_pg->AdjList[_fromV]), _toV);
	LInsert(&(_pg->AdjList[_toV]), _fromV);
	_pg->NumE += 1;

	PEnqueue(&(_pg->Queue), edge);
}

void MapGeneratorStruct::RemoveEdge(FALGraph * _pg, int _fromV, int _toV)
{
	RemoveWayEdge(_pg, _fromV, _toV);
	RemoveWayEdge(_pg, _toV, _fromV);
	(_pg->NumE)--;
}

void MapGeneratorStruct::RemoveWayEdge(FALGraph * _pg, int _fromV, int _toV)
{
	int Edge;

	if (LFirst(&(_pg->AdjList[_fromV]), &Edge))
	{
		if (Edge == _toV)
		{
			LRemove(&(_pg->AdjList[_fromV]));
			return;
		}

		while (LNext(&(_pg->AdjList[_fromV]), &Edge))
		{
			if (Edge == _toV)
			{
				LRemove(&(_pg->AdjList[_fromV]));
				return;
			}
		}
	}
}

void MapGeneratorStruct::RecoverEdge(FALGraph * _pg, int _fromV, int _toV, int _weight)
{
	LInsert(&(_pg->AdjList[_fromV]), _toV);
	LInsert(&(_pg->AdjList[_toV]), _fromV);
	(_pg->NumE)++;
}

bool MapGeneratorStruct::IsConnvertex(FALGraph * _pg, int _v1, int _v2)
{
	FArrayStack Stack;
	int visitV = _v1;
	int nextV;

	StackInit(&Stack);
	VisitVertex(_pg, visitV);
	SPush(&Stack, visitV);

	while (LFirst(&(_pg->AdjList[visitV]), &nextV) == true)
	{
		bool visitFlag = false;

		if (nextV == _v2)
		{
			memset(_pg->VisitInfo, 0, sizeof(int) * _pg->NumV);
			return true;
		}

		if (VisitVertex(_pg, nextV) == true)
		{
			SPush(&Stack, visitV);
			visitV = nextV;
			visitFlag = true;
		}
		else
		{
			while (LNext(&(_pg->AdjList[visitV]), &nextV) == true)
			{
				if (nextV == _v2)
				{
					memset(_pg->VisitInfo, 0, sizeof(int) * _pg->NumV);
					return true;
				}

				if (VisitVertex(_pg, nextV) == true)
				{
					SPush(&Stack, visitV);
					visitV = nextV;
					visitFlag = true;
					break;
				}
			}
		}

		if (visitFlag == false)
		{
			if (SIsEmpty(&Stack) == true)
				break;
			else
				visitV = SPop(&Stack);
		}
	}

	memset(_pg->VisitInfo, 0, sizeof(int) * _pg->NumV);
	return false;
}

void MapGeneratorStruct::ShowGraphEdgeInfo(FALGraph * _pg)
{
	int i;
	int vx;

	for (i = 0; i < _pg->NumV; i++)
	{
		FString text = FString::Chr(i + 65) + TEXT(" �� ����� ���� : ");

		if (LFirst(&(_pg->AdjList[i]), &vx))
		{
			text += FString::Chr(vx + 65) + TEXT(" ");

			while (LNext(&(_pg->AdjList[i]), &vx))
				text += FString::Chr(vx + 65) + TEXT(" ");
		}

		UE_LOG(LogTemp, Log, TEXT("%s"), *text);
	}
}

bool MapGeneratorStruct::VisitVertex(FALGraph * _pg, int _visitV)
{
	if (_pg->VisitInfo[_visitV] == 0)
	{
		_pg->VisitInfo[_visitV] = 1;

		UE_LOG(LogTemp, Log, TEXT("%c"), _visitV + 65);
		return true;
	}

	return false;
}

void MapGeneratorStruct::DFShowGraphVertex(FALGraph * _pg, int _startV)
{
	FArrayStack stack;
	int visitV = _startV;
	int nextV;

	StackInit(&stack);
	VisitVertex(_pg, visitV);
	SPush(&stack, visitV);

	while (LFirst(&(_pg->AdjList[visitV]), &nextV) == true)
	{
		bool visitFlag = false;

		if (VisitVertex(_pg, nextV) == true)
		{
			SPush(&stack, visitV);
			visitV = nextV;
			visitFlag = true;
		}
		else
		{
			while (LNext(&(_pg->AdjList[visitV]), &nextV) == true)
			{
				if (VisitVertex(_pg, nextV) == true)
				{
					SPush(&stack, visitV);
					visitV = nextV;
					visitFlag = true;
					break;
				}
			}
		}

		if (visitFlag == true)
		{
			if (SIsEmpty(&stack) == true)
				break;
			else
				visitV = SPop(&stack);
		}
	}

	memset(_pg->VisitInfo, 0, sizeof(int) * _pg->NumV);
}

void MapGeneratorStruct::ConKruskalMST(FALGraph * _pg)
{
	FGraphEdge recvEdge[100];
	FGraphEdge Edge;
	int eidx = 0;
	int i;

	while (_pg->NumE + 1 > _pg->NumV)
	{
		Edge = PDequeue(&(_pg->Queue));
		RemoveEdge(_pg, Edge.Vertex1, Edge.Vertex2);

		if (!IsConnvertex(_pg, Edge.Vertex1, Edge.Vertex2))
		{
			RecoverEdge(_pg, Edge.Vertex1, Edge.Vertex2, Edge.Weight);
			recvEdge[eidx++] = Edge;
		}
	}

	for (i = 0; i < eidx; i++)
		PEnqueue(&(_pg->Queue), recvEdge[i]);
}

void MapGeneratorStruct::ShowGraphEdgeWeightInfo(FALGraph * _pg)
{
	FPQueue copyPQ = _pg->Queue;
	FGraphEdge edge;

	while (!PQIsEmpty(&copyPQ))
	{
		edge = PDequeue(&copyPQ);

		UE_LOG(LogTemp, Log, TEXT("%c-%c weight : %d"), edge.Vertex1 + 65, edge.Vertex2 + 65, edge.Weight);
	}
}
/*�׷��� �Լ���*/

//������ ���� ũ��� ��ġ�� ���� �� �ִ� ����ü�� �����.