# Client-Server-Bank-System
### System
- Ubuntu Desktop 21.10

### Programming Language
- C++ 

### Introduction 
- A simple bank system using C++ in Linux.
- This system only accepts the localhost(127.0.0.1) as source IP address 
- It only allows the same user to use it ,but the user can operate at the same time in different places. 

### The Implemented Functions:
| Name                 | Content                                                      |
| -------------------- | ---------------------------------------------------------- |
| Deposit, Withdraw , Balance-Checking     | It offers the basic functions            |
| Multithreaded System  | By socket programming and Pthread API, it can create multiple threads at the same time to handle the requests for the user. |
| The consistency of shared resources| By mutex in Pthread API, it can maintain the correctness of the synchronized data.|
| Logout | The user can send the interrupt signal using Ctr-C to end the transaction. | 


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


