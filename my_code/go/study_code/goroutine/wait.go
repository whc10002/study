package main

import (
	"time"
)

func main() {
	chanel := make(chan struct{})
	go func () {
		time.Sleep(time.Second)
		println("--goroutine--")
		close(chanel)
	}()

	println("main ...")
	<-chanel
	println("main exit")
}
