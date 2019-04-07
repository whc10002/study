package main

import (
	"os"
	"os/signal"
	"sync"
	"syscall"
)

var exits = &struct{
	sync.RWMutex
	funcs	[]func()
	signals	chan os.Signal
}{}

func atexit(f func()) {
	exits.Lock()
	defer exits.Unlock()
	exits.funcs = append(exits.funcs, f)
}

func waitExit() {
	if exits.signals == nil {
		exits.signals = make(chan os.Signal)
		signal.Notify(exits.signals, syscall.SIGINT, syscall.SIGTERM)
	}

	exits.RLock()
	for _, f := range exits.funcs {
		// to ensure defer call as FILO
		defer f()
	}
	exits.RUnlock()

	<-exits.signals
}

func main() {
	atexit(func () {println("exit1...")})
	atexit(func () {println("exit2...")})

	waitExit()
}
