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
| Logout | The user can send the interrupt signal using Ctrl-C to end the transaction. | 



## Testing 
1.	Multithread Programming （The three users as follows）
![](https://i.imgur.com/0mAIcaC.png)
![](https://i.imgur.com/9tJn4G6.png)

2. To achieve the requests of critical section:
![](https://i.imgur.com/IHxPz74.png)

3. Software Interrupt — Signal:	
![](https://i.imgur.com/ErtV5tQ.png)


4. The completed process （Login -> Deposit -> Withdraw -> Balance-Checking -> Logout）
![](https://i.imgur.com/OvR86zu.png)


