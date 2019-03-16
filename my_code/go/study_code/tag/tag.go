package main

import(
	"reflect"
	"fmt"
)

type user struct{
	name string `姓名`
	sex byte `性别`
}

func main() {
	u := user {"Tome", 1}
	v := reflect.ValueOf(u)
	t := v.Type()

	for i, n := 0, t.NumField(); i < n; i++ {
		fmt.Printf("%s: %v\n", t.Field(i).Tag, v.Field(i))
	}
}
