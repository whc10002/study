#include <string>
#include <iostream>

using namespace std;

class base
{
	public:
		base(int bv):bval(bv){};
		virtual void base_f(){cout<<"base2::f()"<<endl;}
		virtual void base_g(){cout<<"base2::g()"<<endl;}
	private:
		int bval;
};

class derived: public base
{
	public:
		derived(int bv1, int dv):base(bv1), dval(dv){};
		virtual void base_f(){cout<<"derived::f()"<<endl;}
		virtual void derived_h(){cout<<"derived::h()"<<endl;}
	private:
		int dval;
};

typedef void (*FUN)();

int main(int argc,char** argv)
{
	base b(10);
	FUN fun;
	int **pvtab = (int**)&b;
	cout<<"[0]:base->vptr"<<endl;
	for(int i=0; i<2; i++)
	{
		fun = (FUN)pvtab[0][i];
		cout<<" "<<i<<" ";
		fun();
	}
	cout<<" 2 "<<pvtab[0][2]<<endl;
	cout<<"[1]:bval "<<(long)pvtab[1]<<endl;
	
	/*derived d(10, 100);
	pvtab = (int**)&d;
	cout<<"[0]:derived->vptr "<<endl;
	for(int i=0; i<3; i++)
	{
		fun = (FUN)pvtab[0][i];
		cout<<" "<<i<<" ";
		fun();
	}
	cout<<" 3 "<<pvtab[0][3]<<endl;
	cout<<"[1]:bval "<<(int)pvtab[1]<<endl;
	cout<<"[2]:dval "<<(int)pvtab[2]<<endl;
	*/
	return 0;
}
