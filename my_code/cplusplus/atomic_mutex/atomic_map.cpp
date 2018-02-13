#include <atomic>
#include <map>
#include <memory>


struct Buyer_Platform_QPS_Unit
{
	long current_qps_;
	long current_time_;
};

using atomic_ptr_t = std::shared_ptr<std::atomic<Buyer_Platform_QPS_Unit>>;
typedef std::map<uint64_t, atomic_ptr_t> value_map_t;

using atomic_map_ptr_t = std::shared_ptr<std::atomic<std::map<uint64_t, Buyer_Platform_QPS_Unit>>>;

int main()
{
	value_map_t map;
	map[1] = atomic_ptr_t(new std::atomic<Buyer_Platform_QPS_Unit>({1,1}));

	//auto tmp = map[1]->load();

	atomic_map_ptr_t atomic_map; //= std::atomic<new std::map<uint64_t, Buyer_Platform_QPS_Unit>(std::make_pair(1,{1,1}))>;
	auto tmp = atomic_map->load();

	return 0;
}
