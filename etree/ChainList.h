//****************************************************************************
//  ChainList.h: class for ChainList.
//
//  Copyright (c) Boris J. Wang (e_boris2002@hotmail.com) 2002 - 2003
//  From Institute of Computing Technology, Chinese Academy of Sciences
//                                                Beijing 100871, China
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A 
//  PARTICULAR PURPOSE.
//****************************************************************************

#ifndef __ChainList__
#define __ChainList__

template <class Teg>
class CChainNode
{
protected:
	Teg * Next;
public:
	CChainNode()
	{
		Next = NULL;
	};

	virtual ~CChainNode()
	{
		DeleteNodesAfterMe();
	};

	void DeleteNodesAfterMe()
	{
		if(Next)
		{
			Next->DeleteNodesAfterMe();
			delete Next;
			Next = NULL;
		}
	};

	void Append(Teg * New)
	{ 
		ASSERT(New);
		ASSERT(New->Prev == NULL);

		New->GetTail()->Next = Next;
		Next = New;

		New->Prev = this;
	};

	inline Teg * GetNext(){return Next;}

	Teg * GetTail()
	{CChainNode * n;
		n = this;
		while(n->Next)
		{
			n = n->Next;
		}
		ASSERT(n);
		return n;
	}

	DWORD GetListLength()
	{CChainNode * n;
	 DWORD i;
		n = this;

		for(i=1;;i++)
		{
			if(n->Next == NULL)break;
			n = n ->Next;
		}

		return i;
	}

};

#endif