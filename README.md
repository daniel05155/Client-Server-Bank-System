# Client-Server-Bank-System
## 簡介
- 在Linux System，實作一個簡易版的Client-Server Bank System，來源IP Address均為127.0.0.1(Localhost)。
- 欲完成的功能:
  - 登入、存提款、查詢餘額、結束交易(登出)
  - 確保共用資料(Shared Data)的正確性
  - 可接收多位使用者的連線與請求  

以下為目前已實現的部份:

| 功能                 | 內容                                                       |
| -------------------- | ---------------------------------------------------------- |
| 登入、存款、提款、查詢餘額、登出    | 提供一般性帳戶的功能。尚未完成轉帳功能。            |
| 多執行緒 (Multithread) | 透過Socket的設定及運用Pthread來建立執行緒，達到多位用戶同時使用同個帳戶，實現平行化程式。|
| 不同人對於同個帳戶，同個時間進行存款與提款，但不會有資料不正確的現象。| 使用 Pthread 的 mutex，實作出critical section 的要求，避免帳戶金額不正確。|
| 結束交易 | 可以透過 Ctrl-C 發出 Linux Signal的中斷訊號，結束 Client 的執行。 | 

呈現上述提及Client-Server Bank System的功能
1.	Multithread Programming (以三位user使用帳戶為例，分別為程式碼及執行結果)
![](https://i.imgur.com/0mAIcaC.png)
![](https://i.imgur.com/9tJn4G6.png)

2. 實作 Critical Section 的要求:
![](https://i.imgur.com/IHxPz74.png)

3. 軟體中斷 — Signal:	
![](https://i.imgur.com/ErtV5tQ.png)


目前實現的作業流程(登入->存款->提款->查詢餘額->登出)
![](https://i.imgur.com/OvR86zu.png)


