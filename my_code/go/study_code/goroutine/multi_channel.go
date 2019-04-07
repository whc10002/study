package main

import (
	"sync"
)

func main () {
	var wg sync.WaitGroup
	wg.Add(2)

	a, b := make(chan int), make(chan int)

	// receiving terminal
	go func() {
		defer wg.Done()

		for {
			var (
				name	string
				x	int
				ok	bool
			)

			// random select one channel to receive data
			select {
			case x, ok = <-a:
				name = "from channel a"
			case x, ok = <-b:
				name = "from channel b"
			}
			// if one of channel is closed, terminate receiving data
			if !ok {
				return
			}
			// output receiving data
			println(name, x)
		}
	}()

	// sending terminal
	go func() {
		defer wg.Done()
		defer close(a)
		defer close(b)

		for i := 0; i < 10; i++ {
			// random select one channel to send data
			select {
			case a <- i:
			case b <- i*10:
			}
		}
	}()

	wg.Wait()
}
