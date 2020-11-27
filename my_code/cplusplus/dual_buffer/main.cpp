#include <iostream>
#include <chrono>
#include <thread>

#include "dual_buffer.h"
#include "dual_buffer_shared_ptr.h"

typedef struct Node
{
	int node_value_;
	bool status_ = false;

	~Node()
	{
		std::cout << "deconstructor" << std::endl;
	}
}Node;

typedef dual_buffer<Node> dual_node;
typedef dual_buffer_shared_ptr<Node> dual_node_shared_ptr;

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

void update_thread_shared_ptr(dual_node_shared_ptr *dual_node)
{
	for (int i = 0; i < 100000000; i++)
	{
		std::shared_ptr<Node> node(new Node);
		node->node_value_ = i;
		node->status_ = true;

		dual_node->load(node);
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
}

void read_thread_shared_ptr(dual_node_shared_ptr *dual_node)
{
	for (int i = 0; i < 10000000000; i++)
	{
		std::shared_ptr<Node> node = dual_node->get();

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
	dual_node_shared_ptr dual_node_shared_ptr;
	std::thread t1(update_thread_shared_ptr, &dual_node_shared_ptr);
	std::thread t2(read_thread_shared_ptr, &dual_node_shared_ptr);

	t1.join();
	t2.join();
	return 0;
}
