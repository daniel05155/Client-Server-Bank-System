# Client-Server-Bank-System
## 簡介
目前正在實作一個簡易版的Client-Server Bank System，來源IP Address均為127.0.0.1(Localhost)。
目標功能:登入、存提款、查詢餘額、轉帳、結束交易，且確保shared data的正確性，及同時接收多位使用者連線(Multithread)。

下列為目前實現的部份:


| 功能                 | 內容                                                       |
| -------------------- | ---------------------------------------------------------- |
| 登入、登出、存款、提款、查詢餘額     | 提供一般性帳戶的功能。尚未完成轉帳功能。            |
| 多執行緒 (Multithread) | 透過Socket的設定及運用Pthread來建立執行緒，達到多位用戶同時使用同個帳戶，實現平行化程式。|
| 不同人對於同個帳戶，同個時間進行存款與提款，但不會有資料不正確的現象。| 使用 Pthread 的 mutex，實作出critical section 的要求，避免帳戶金額不正確。|
| 結束交易 | 可以透過 Ctrl-C 發出 Linux Signal的中斷訊號，結束 Client 的執行。 | 
