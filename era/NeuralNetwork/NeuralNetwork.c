
#include <malloc.h>

#include "NeuralNetwork.h"
/*#include "Neuron.h"*/




nero_us32int neroKind[]=
{
 NeuronNode_ForImage,    //当一个概念节点的类型为此时表示一个图像对象
NeuronNode_ForLine,    //当一个概念节点的类型为此时表示一个线条对象

NeuronNode_ForChCharacter,    //当一个概念节点的类型为此时表示一个汉字
NeuronNode_ForChWord ,    //当一个概念节点的类型为此时表示一个中文词语
 NeuronNode_ForChSentence,    //当一个概念节点的类型为此时表示一个中文句子
};



/*ActNero只是用来保存基础数据，它对应于一个单纯的神经元*/


/*
将指定位置的位设置为特定的值：
1111
	&
0101



msg：
低位			高位
1-------8  9-----16 17-----24  25----32
1111 1111 1111 1111 1111 1111 1111 1111 
1-8位表示该节点种类




*/



 struct ActivationNeuron
{
nero_us32int msg;/*记录该nero的种类，性质等信息*/
nero_s32int x;/*取值范围-2147483648 ~ 2147483647*/
nero_s32int y;
nero_s32int z;
struct NerveFiber_  * inputListHead;
struct NerveFiber_   * outputListHead;
};
/*神经纤维---用来连接各个神经元*/
  struct NerveFiber_
 {
struct ActivationNeuron   *obj;
struct NerveFiber_ * next;
 };
 
 
 
/*typedef struct ActivationNeuron  NeuronObject;*/




ActNero NeroPool[MaxNeroNum];
nero_us32int nextAvailableNeroInPool;//它指向NeroPool中当前可用的（即使未加入网络的nero）
NeuronObject *GodNero;/*所有神经元理论上都最终与这个相通*/


static inline void setActNeroKind(ActNero *nero,nero_us32int kind)
{
	
	if(nero ==NULL || kind <NeuronNode_ForNone || kind >=NeuronNode_Max)
		return ;
	nero->msg =nero->msg & 0xffffff00;//低8位清零
	nero->msg =nero->msg | kind;//低8位清零
}
static inline NerveFiber * addNerveFiber(ActNero *  n,nero_s32int type)
{
/*
#define NerveFiber_Input 1 
#define Relationship_Output  2
*/	NerveFiber  *tmp,*p;
	if(n ==NULL ||  type <NerveFiber_Input || type >NerveFiber_Output)
	{
		NeroErrorMsg;
		return NULL;
	}	
	
	if (type == NerveFiber_Input)
	{
		p= n->inputListHead;
	}
	else if(type == NerveFiber_Output)
	{
		p= n->outputListHead;
	}
	else
	{
		NeroErrorMsg;
		return NULL;
	}	

	while(p != NULL  && p->next != NULL)
	{
		p=p->next;
		
	}
	tmp=(NerveFiber *)malloc(sizeof(NerveFiber));
	tmp->next=NULL;
	if (p == NULL)
	{
		p=tmp;
	}
	else	
		p->next=tmp;
	return  tmp;

}
void donother()
{


}
nero_s32int CreateActNeroNet()
{
	
	nero_s32int res;
	nero_s32int i;
	NeuronObject *BaseNeuronObject;
	/*do some init*/
	initNeroPool();
	
	/*首先一个网络你是否导入了数据必须有一些基本的构建*/

	/*显然它必须生成一些基本的神经对象，就像面向对象语言中万物。之母一样*/
	GodNero=(NeuronObject *)getNeuronObject();
	res=initActNero(GodNero,NeuronNode_ForGodNero,NULL,NULL);
	if(res == NeroError)
	{
		NeroErrorMsg;
		return res;
	}
		/*现在生成其他基类*/
	for(i=0;i<sizeof(neroKind)/sizeof(nero_us32int);i++)	
	{
		
		BaseNeuronObject=(NeuronObject *)getNeuronObject();
		res=initActNero(BaseNeuronObject,neroKind[i],NULL,NULL);
		if(res == NeroError)
		{
			NeroErrorMsg;
			return res;
		}	
		/*将其他基类加入网络，他们与GodNero是相互联系的关系*/	
		addNeuronChild(GodNero,BaseNeuronObject,Relationship_bothTother);
	}

		

	return NeroOK;
}


nero_s32int PointingToObject(NeuronObject *lower,NeuronObject *higher)
{
	/*很明显各个对象之间的连接关系需要额外的空间来存储
	你有俩种选择：使用神经元来保存，---浪费空间
		：    使用额外的结构，----似乎会麻烦
	这里还是觉得该用第二种方案
	*/
	if(lower ==NULL || higher ==NULL )
	{
		NeroErrorMsg;
		return NeroError;
	}	
	NerveFiber * newfiber=addNerveFiber(lower,NerveFiber_Output);
	if (newfiber ==NULL)
	{
		NeroErrorMsg;
		return NeroError;
	}
	newfiber->obj=higher;
	
	
	
	return NeroOK;
}
nero_s32int addNeuronChild(NeuronObject *father,NeuronObject *child,nero_s32int Relationship)
{
	if(child ==NULL || father ==NULL || Relationship <Relationship_FatherToChild || Relationship >Relationship_bothTother)
	{
		NeroErrorMsg;
		return NeroError;
	}
	switch(Relationship)
	{
	case Relationship_FatherToChild:
		PointingToObject(father,child);
		break;
		
	case Relationship_ChildToFather:
		PointingToObject(child,father);
		break;
		
	case Relationship_bothTother:
		PointingToObject(father,child);
		PointingToObject(child,father);
		break;
	default:break;

	}
	
	return NeroOK;
}


nero_s32int initNeroPool()
{

	nextAvailableNeroInPool=0;
	return NeroOK;
}

nero_s32int initActNero(ActNero * nero,nero_us32int kind,NerveFiber *inputListHead,NerveFiber *outputListHead)
{
	if(nero ==NULL || kind <NeuronNode_ForNone || kind >=NeuronNode_Max)
		return NeroError;
	setActNeroKind( nero,kind);
	nero->inputListHead=inputListHead;
	nero->outputListHead=outputListHead;




	return NeroOK;
}

nero_s32int initActNeroNet()
{







	return NeroOK;
}

NeuronObject * getNeuronObject()
{
/*	nero_us32int nextAvailableNeroInPool,cur ;*/

	if(nextAvailableNeroInPool <0 || nextAvailableNeroInPool >=MaxNeroNum)
		return NULL;
/*	cur=nextAvailableNeroInPool;*/
/*	nextAvailableNeroInPool++;*/
	return (&(NeroPool[nextAvailableNeroInPool++]));

}


/*NeuronObject * getNeuronObject(nero_us32int kind,NeuronObject *inputListHead,NeuronObject *inputListHead)*/
/*{*/

/*	*/



/*}*/


