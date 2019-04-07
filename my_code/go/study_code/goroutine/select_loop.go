package main

import (
	//"sync"
	"time"
	"fmt"
)

func main() {
	done := make(chan struct{})
	d := make(chan int)

	go func() {
		defer close(done)

		for {
			select {
			case x, ok := <-d:
				if !ok {
					return
				}
				fmt.Println("data:", x)
			// avoid select blocking
			default:
			}

			fmt.Println(time.Now())
			time.Sleep(time.Second)
		}
	}()

	time.Sleep(time.Second * 5)
	d <- 100
	close(d)

	<-done
}
