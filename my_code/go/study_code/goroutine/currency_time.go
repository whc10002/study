package main

import (
	"time"
	"fmt"
	"os"
)

func main () {
	go func () {
		for {
			select {
			case <-time.After(time.Second * 5):
				fmt.Println("timeout...")
				os.Exit(0)
			}
		}
	}()

	go func() {
		tick := time.Tick(time.Second)

		for {
			select {
			case <-tick:
				fmt.Println(time.Now())
			}
		}
	}()

	// directly use nil channel to block process
	<-(chan struct{})(nil)
}
