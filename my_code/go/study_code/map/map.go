package main

import (
	"fmt"
)

func main() {
	m := make(map[string]int)
	m["a"] = 1
	m["b"] = 2

	m2 := map[int]struct {
		x int
	} {
		1: {x: 100},
		2: {x: 200},
	}

	fmt.Println(m, m2)

	m3 := map[string]int{
		"a" : 1,
		"b" : 2,
	}

	// if value is simple type, you can directly modify the value
	m3["c"] = 10
	m3["a"] = 11

	if v, ok := m3["d"]; !ok {
		m3["d"] = 4
	} else {
		println(v)
	}
	delete(m3, "d")

	m4 := map[int]struct {
		name string
		age byte
	} {
		1 : {"caohongwei", 32},
		2 : {"zhangqian", 31},
		3 : {"caoyuran", 5},
	}
	// if value type is struct or arrary, you can't directly modify the value
	// m4[1].age += 1
	u := m4[1]
	u.age += 1
	m4[1] = u

	for k,v := range m4 {
		println(k, v.name, v.age)
	}
}
