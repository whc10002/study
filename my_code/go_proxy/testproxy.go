package main

import (
	"context"
	"errors"
	"fmt"
	// "io/ioutil"
	"net"
	"net/http"
	"net/url"
	"time"
)

func myDialContext(ctx context.Context, network, address string) (net.Conn, error) {
	return (&net.Dialer{Timeout: 3 * time.Second}).DialContext(ctx, network, "stghlsproxyvip1:9080")
}

func myDialTLS(network, address string) (net.Conn, error) {
	return (&net.Dialer{Timeout: 3 * time.Second}).Dial(network, "stghlsproxyvip1:9080")
}

func myCheckRedirect(req *http.Request, via []*http.Request) error {
	if len(via) >= 10 {
		return errors.New("stopped after 10 redirects")
	}
	setURLOpaqueForProxy(req.URL)
	return nil
}

func hackProxy(req *http.Request) (*url.URL, error) {
	if req != nil && req.URL != nil {
		setURLOpaqueForProxy(req.URL)
	}
	return nil, nil
}

func setURLOpaqueForProxy(url *url.URL) {
	path := url.EscapedPath()
	if path == "" {
		path = "/"
	}

	fmt.Printf("path:\r\n%s\r\n", path)
	url.Opaque = "//" + url.Host + path
	fmt.Printf("Opaque:\r\n%s\r\n", url.Opaque)

	fmt.Printf("send request: url:\r\n%s\r\n", url.String())
	fmt.Printf("send request: uri:\r\n%s\r\n", url.RequestURI())
}

func sendRequest(client *http.Client, url string) {
	req, _ := http.NewRequest(http.MethodGet, url, nil)
	req.Close = true

	// setURLOpaqueForProxy(req.URL)

	if resp, err := client.Do(req); err != nil {
		fmt.Println(err)
	} else {
		fmt.Println(resp.Status)
		// body, _ := ioutil.ReadAll(resp.Body)
		// fmt.Println(string(body))
	}
}

func main() {
	proxyURL, _ := url.Parse("http://127.0.0.1:9080")
	clt := &http.Client{
		Timeout: 3 * time.Second,
		Transport: &http.Transport{
			Proxy: http.ProxyURL(proxyURL),
			//Proxy:             hackProxy,
			DialContext:       myDialContext,
			DialTLS:           myDialTLS,
			DisableKeepAlives: true,
		},
		CheckRedirect: myCheckRedirect,
	}

	sendRequest(clt, "https://adclick.g.doubleclick.net/pcs/click?xai=AKAOjssFRNrVoHUvJ2qsNMY3tK_z2sYifxabNMG5_T3GEZZuKNEuVHPehTWl6qpDCbqIACdq_SWX6cWrjNwkvC3-t0c&sig=Cg0ArKJSzF5IZGcvvUZjEAE&urlfix=1&adurl=https://buildon.aws/%3Fsc_channel%3Dba%26sc_segment%3Dchi%26sc_campaign%3Dbrandawareness_2017%26sc_publisher%3Dcadreon%26sc_medium%3Dnews_video%26sc_detail%3D15%26sc_content%3Dbuild_on_15%26sc_country%3Dus%26sc_geo%3Dnamer%26sc_outcome%3Daware%26trk%3Dba_boa_dv_cadreon_chi")

	// sendRequest(clt, "https://buildon.aws/?sc_channel=ba&sc_segment=chi&sc_campaign=brandawareness_2017&sc_publisher=cadreon&sc_medium=news_video&sc_detail=15&sc_content=build_on_15&sc_country=us&sc_geo=namer&sc_outcome=aware&trk=ba_boa_dv_cadreon_chi")

	// sendRequest(clt, "http://www.amazon.com")
	// sendRequest(clt, "https://www.amazon.com")

	// sendRequest(clt, "http://www.baidu.com")
	// sendRequest(clt, "https://www.baidu.com")

	// sendRequest(clt, "http://adclick.g.doubleclick.net/pcs/click?xai=AKAOjstjOyGjV3xPExCwf2fVydLmFtYU3PQx5zYUbe73Ts0EGX3T1XPuYDUrNeG13M8WMKjbejk-Z7v5Qb9tx1MxdySpD8U12s97L2wEBXWiq2lp&sig=Cg0ArKJSzCBhMY7eqMGOEAE&urlfix=1&adurl=http://www.marshallsonline.com/store-finder%3Fcid%3Dola:FOX:FY18SummerFEP:FOX_SummerFY18_Bonejangles_15s")
}
