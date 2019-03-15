package main

func main () {
	exit := make(chan struct{})

	go func() {
		println("hello, world!")

		// 'struct{}' is a type, 'struct{}{}' is a object of 'struct{}'
		exit <- struct{}{}
	} ()

	<-exit
	println("end.")
}
