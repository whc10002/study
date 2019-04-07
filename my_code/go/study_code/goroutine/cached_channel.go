package main

import (
)

func main () {
	done := make (chan struct{})

	// data cache
	data := []chan int {
		make(chan int, 3),
	}

	go func () {
		defer close(done)

		for i := 0; i < 10; i++ {
			select {
			// create data
			case data[len(data)-1] <- i:
				println("create data:", i)
			// when current channel is full, create new cached channel
			default:
				data = append(data, make(chan int, 3))
				data[len(data)-1] <- i
				println("create channel:", i)
			}
		}
	}()

	<-done

	for i := 0; i < len(data); i++ {
		c := data[i]
		close(c)
		for x := range c {
			println(x)
		}
	}
}
