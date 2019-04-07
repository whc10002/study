package main

import (
	"sync"
	"fmt"
)

func main() {
	var wg sync.WaitGroup
	var gs [5]struct {
		id 	int
		result	int
	}

	for i := 0; i < len(gs); i++ {
		wg.Add(1)

		go func(id int) {
			defer wg.Done()

			gs[id].id = id
			gs[id].result = (id + 1) *100
		}(i)
	}

	wg.Wait()
	fmt.Printf("%+v\n", gs)
}
