package main

import(
	"sync"
)

func main() {
	var wg sync.WaitGroup
	wg.Add(1)

	c := make(chan int)
	var send chan<- int = c
	var recv <-chan int = c 

	go func () {
		defer wg.Done()
		for x := range recv {
			println(x)
		}
	}()

	go func () {
		defer wg.Done()
		defer close(c)

		for i := 0; i < 3; i++ {
			send <- i
		}
	}()

	wg.Wait()
}
