#include <string>
#include <iostream>

using namespace std;

class base
{
	public:
		base(int bv):bval(bv){};
		virtual void base_f(){cout<<"base::f()" << endl;}
		virtual void base_g(){cout<<"base::g()" << endl;}
	private:
		int bval;
};

class derived: public base
{
	public:
		derived(int bv1, int dv):base(bv1), dval(dv){};
		virtual void derived_h(){cout<<"derived::h()"<<endl;}
	private:
		int dval;
};

typedef void (*FUN)();

int main(int argc,char** argv)
{
	base b(100);
	FUN fun;
	FUN **pvtab = (FUN**)&b;
	cout<<"[0]:base->vptr:" << pvtab <<endl;

	for(int i=0; i<2; i++)
	{
		fun = (FUN)pvtab[0][i];
		cout<<" "<<i<<" ";
		fun();
	}

	
	cout<<"[1]:bval "<<(long)pvtab[1]<<endl;
	
	derived d(10, 100);
	pvtab = (FUN**)&d;
	cout<<"[0]:derived->vptr "<<endl;
	for(int i=0; i<2; i++)
	{
		fun = (FUN)pvtab[0][i];
		cout<<" "<<i<<" ";
		fun();
	}

	int * pval = (int*)(pvtab+1);
	cout<<"[1]:bval "<<pval[0]<<endl;
	cout<<"[2]:dval "<<pval[1]<<endl;

	return 0;
}
