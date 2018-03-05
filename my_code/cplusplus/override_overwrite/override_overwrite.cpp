#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

class Parent  
{  
	public:  
		void F()  
		{  
			printf("Parent.F()\n");  
		}
		virtual void G()  
		{
			printf("Parent.G()\n");  
		}

		virtual void Fun(int a,float b) const
		{
			int c = a + int(b);
			printf("Parent.Fun()\n");  
		}

		int Add(int x, int y)  
		{
			return x + y;  
		}
		//重载(overload)Add函数  
		float Add(float x, float y)  
		{
			return x + y;  
		}
};

class ChildOne:Parent
{
	public:
	//重写(overwrite)父类函数
	void F()
	{
		printf("ChildOne.F()\n");
	}

	//覆写(override)父类虚函数,主要实现多态
	void G()
	{
		printf("ChildOne.G()\n");
	}

	void Fun()
	{
		printf("ChildOne.Fun()\n");
	}
};

int main(int argc, char** argv)
{
	ChildOne childOne;
	Parent* p = (Parent*)&childOne;

	// 调用ChildOne.F()
	childOne.F();
	
	// 调用Parent.F()
	p->F();

	//实现多态
	p->G();

	childOne.Fun();

	// Parent->Fun(int,float) overwrited by childOne.Fun()
	// so this sentence will cause compile error 	
	// childOne.Fun(1,1.2);

	Parent* p2 = new Parent();
	
	//重载(overload)
	printf("%d\n",p2->Add(1, 2));
	printf("%f\n",p2->Add(3.4f, 4.5f));
	delete p2;

	return 0;
}
