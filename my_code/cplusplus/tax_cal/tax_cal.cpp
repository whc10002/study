#include <vector>
#include <string>
#include <map>
#include <iostream>

int g_old_reduce = 3500;
typedef struct Tax_Rate
{
	int begin;
	int end;
	float ratio;
}Tax_Rate;

Tax_Rate g_old_tax_rate[] = 
{
	{0, 1500, 0.03},
	{1500, 4500, 0.10},
	{4500, 9000, 0.20},
	{9000, 35000, 0.25},
	{35000, 55000, 0.30},
	{55000, 80000, 0.35},
	{80000, INT_MAX, 0.45}	
};

int g_new_reduce = 5000;
Tax_Rate g_new_tax_rate[] = 
{
	{0, 3000, 0.03},
	{3000, 12000, 0.10},
	{12000, 25000, 0.20},
	{25000, 35000, 0.25},
	{35000, 55000, 0.30},
	{55000, 80000, 0.35},
	{80000, INT_MAX, 0.45}	
};

using namespace std;

int calculate_tax(int income, const Tax_Rate* tax_rate, int count, int reduce)
{
	int tax = 0;
	if (income <= reduce)
		return tax;
	int current = income - reduce;
	for (int i = 0; i < count; i++)
	{
		if (current <= tax_rate[i].end)
		{
			tax += current * tax_rate[i].ratio;
			break;
		}
		else
		{
			tax += tax_rate[i].ratio * tax_rate[i].end;
			current -= tax_rate[i].end;
		}

	}
	return tax;
}

int main(int argc, char**argv)
{
	int old_tax = calculate_tax(33358, g_old_tax_rate, sizeof(g_old_tax_rate)/sizeof(Tax_Rate), g_old_reduce);
	std::cout << "tax:" << old_tax << std::endl;
	int new_tax = calculate_tax(33358, g_new_tax_rate, sizeof(g_new_tax_rate)/sizeof(Tax_Rate), g_new_reduce);
	std::cout << "tax:" << new_tax << std::endl;
	return 0;
}
