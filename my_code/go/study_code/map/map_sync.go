package main

import (
	"sync"
	"time"
)

func main () {
	var lock sync.RWMutex
	m := make(map[string]int)

	go func() {
		for {
			println("write")
			lock.Lock()
			m["a"] += 1
			lock.Unlock()
			time.Sleep(time.Microsecond)
		}
	}()

	go func() {
		for {
			println("read")
			lock.Lock()
			_ = m["b"] 
			lock.Unlock()
			time.Sleep(time.Microsecond)
		}
	}()

	select{}
}

