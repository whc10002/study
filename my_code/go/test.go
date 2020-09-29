package main
import (
    "bufio"
    "fmt"
    "net/http"
    "bytes"
)
func main() {
	postData := []byte("http")
	client := &http.Client{}
	req, err := http.NewRequest("POST", "https://platform.rtbiq.com/requests/freewheel", bytes.NewReader(postData))
	req.Header.Add("User-Agent", "myClient")
	resp, err := client.Do(req)
	defer resp.Body.Close()
	//resp, err := http.Post("https://platform.rtbiq.com/requests/freewheel","",nil)
    if err != nil {
        panic(err)
    }
    fmt.Println("Response status:", resp.Status)
    scanner := bufio.NewScanner(resp.Body)
    for i := 0; scanner.Scan() && i < 5; i++ {
        fmt.Println(scanner.Text())
    }
    if err := scanner.Err(); err != nil {
        panic(err)
    }
}
