#include <iostream>
#include <chrono>
#include <thread>

#include "dual_buffer.h"

typedef struct Node
{
	int node_value_;
	bool status_ = false;
}Node;

typedef dual_buffer<Node> dual_node;
void update_thread(dual_node *dual_node)
{
	for (int i = 0; i < 100000000; i++)
	{
		Node *node = new Node();
		node->node_value_ = i;
		node->status_ = true;

		dual_node->load(node);
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
}

void read_thread(dual_node *dual_node)
{
	for (int i = 0; i < 10000000000; i++)
	{
		const auto *node = dual_node->get();

		if (node)
		{
			std::cout << "node_value:" << node->node_value_ << ";node_status" << node->status_ << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
	}
}
int main(int argc, char* argv[])
{
	dual_node dual_node;
	std::thread t1(update_thread, &dual_node);
	std::thread t2(read_thread, &dual_node);

	t1.join();
	t2.join();
	return 0;
}
