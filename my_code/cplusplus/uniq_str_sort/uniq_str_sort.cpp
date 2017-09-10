#include <iostream>
#include <memory>
#include<algorithm>
#include <vector>

using namespace std;

typedef struct Node
{
	Node (float price)
	{
		price_ = price;
	}
	float price_;
}Node;

bool compare_price_desc(const unique_ptr<Node> & a, const unique_ptr<Node> & b)
{
	if (NULL == a)
	{
		return false;
	}
	if (NULL == b)
	{
		return true;
	}
	return a->price_ > b->price_;
}

int main(int argc, char * argv[])
{
	vector< unique_ptr<Node> > v_datas;


	v_datas.emplace_back(unique_ptr<Node>(new Node(1.1)));
	v_datas.emplace_back(unique_ptr<Node>(new Node(2.1)));
	v_datas.emplace_back(unique_ptr<Node>(new Node(2.0)));
	v_datas.emplace_back(unique_ptr<Node>(new Node(3.1)));
	v_datas.emplace_back(unique_ptr<Node>(new Node(3.0)));
	v_datas.emplace_back(unique_ptr<Node>(nullptr));
	v_datas.emplace_back(unique_ptr<Node>(new Node(3.4)));
	v_datas.emplace_back(unique_ptr<Node>(new Node(4.1)));

	for (auto & data: v_datas)
	{
		if (data)
		{
			cout << data->price_ << endl;
		}
		else
		{
			cout << "NULL" << endl;
		}
	}

	cout << "after sort" << endl;

	sort(v_datas.begin(), v_datas.end(), compare_price_desc);

	for (auto & data: v_datas)
	{
		if (data)
		{
			cout << data->price_ << endl;
		}
		else
		{
			cout << "NULL" << endl;
		}
	}
}
