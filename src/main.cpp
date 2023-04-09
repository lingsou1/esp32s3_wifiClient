/*
接线说明:无

程序说明:此程序用于演示如何使用esp32s3作为互联网客户端向www.example.com网站服务器发送http请求，
         并且将网站服务器响应的信息输出在屏幕中 。

注意事项:无

函数示例:无

作者:灵首

时间:2023_3_18

*/
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClient.h>



WiFiMulti wifi_multi;  //建立WiFiMulti 的对象,对象名称是 wifi_multi



/*
# brief 连接WiFi的函数
# param 无
# retval 无
*/
void wifi_multi_con(void){
  int i=0;
  while(wifi_multi.run() != WL_CONNECTED){
    delay(1000);
    i++;
    Serial.print(i);
  }

}



/*
# brief 写入自己要连接的WiFi名称及密码,之后会自动连接信号最强的WiFi
# param 无
# retval  无
*/
void wifi_multi_init(void){
  wifi_multi.addAP("LINGSOU123","123455678");
  wifi_multi.addAP("LINGSOU12","123455678");
  wifi_multi.addAP("LINGSOU1","123455678");
  wifi_multi.addAP("LINGSOU234","12345678");   //通过 wifi_multi.addAP() 添加了多个WiFi的信息,当连接时会在这些WiFi中自动搜索最强信号的WiFi连接
}


/*
# brief 通过WiFiClient库向指定网址建立连接并发出请求,
        同时获取响应信息,并将响应信息通过串口打印出来
# param  const char* host:需要建立连接的网站的网址
# param   const int httpPort:对应的端口号
# retval  无,但是会通过串口打印出请求的相关的内容
*/
void wifiClientRequest(const char* host,const int httpPort){
  WiFiClient client;

  // 建立字符串，用于HTTP请求
  String httpRequest =  String("GET /") + " HTTP/1.1\r\n" +
                        "Host: " + host + "\r\n" +
                        "Connection: close\r\n" +
                        "\r\n";

  //输出连接的网址
  Serial.print("connecting to :");
  Serial.print(host);


  //连接网络服务器
  if(client.connect(host,httpPort)){
    Serial.print("success\n");    //成功后输出success

    client.print(httpRequest);    //向服务器发送HTTP请求
    Serial.print("sending request:");   //串口输出HTTP请求信息
    Serial.print(httpRequest);
    Serial.print("\n");


    //串口输出网络服务器响应信息
    Serial.print("web serve response:\n");
    while(client.connected() || client.available()){
      if(client.available()){
        String line = client.readStringUntil('\n');
        Serial.print(line);
      }
    }

    client.stop();    //断开与服务器的连接
    Serial.print("disconnected from:");
    Serial.print(host);
  }
  else{
    Serial.print("connect failed!!!\n");
    client.stop();

  }

}

void setup() {
  //连接串口
  Serial.begin(9600);
  Serial.print("serial is OK\n");

  //wifi 连接设置
  wifi_multi_init();
  wifi_multi_con();
  Serial.print("wifi connected!!!\n");
  
  //连接网站,输出信息
  wifiClientRequest("www.example.com",80);
  wifiClientRequest("www.baidu.com",80);


}

void loop() {

}