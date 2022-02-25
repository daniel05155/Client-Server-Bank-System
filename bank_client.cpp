#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>

using namespace std;

constexpr unsigned int SERVER_PORT = 50544;
constexpr unsigned int MAX_BUFFER = 128;

int sockfd; //接收socket number

typedef enum 
{
    LOGIN,
    SAVE,
    WITHDRAW,
    BALANCE,
    TRANSFER,
    EXIT

}service;

typedef struct
{
    string name;
    string password;
    int num  = 0;
    int balance ;
    
}info;

void login();
void save();
void withdraw();
void balance();
void transfer();
void handle_signal(int sig);



int main(int  argc,char *argv[])
{
    // 建socket, 透過TCP與IPv4進行資料傳輸
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)   
    {
        cerr << "socket error" << endl;
        return 1;
    }

// command line 輸入server ID
  struct hostent* server = gethostbyname(argv[1]);

 if (server == nullptr) 
    {
        cerr << "No such  host name" << endl;
        return 2;
    }

    /*  設定client給socket的資訊  */
    struct sockaddr_in client_info;             //  sockaddr_in  ＝ IPv4 結構
    bzero(&client_info, sizeof(client_info));   // 把struct涵蓋的bits設為0   
    client_info.sin_family = AF_INET;                //使用IPv4協定
    bcopy( (char *)server->h_addr,   (char *)& client_info.sin_addr.s_addr , server->h_length);  // Server- IP address
    client_info.sin_port = htons(SERVER_PORT);        //Server-port
  
    /*connect between  client and server 的連線 */
    if (connect(sockfd, (struct  sockaddr *) &client_info, sizeof(client_info)) < 0)
    {
        cerr << "connect error: the server does not look running" << endl;
        return 3;
    }
        string read_buffer1(MAX_BUFFER, 0);
     
         if (read(sockfd, &read_buffer1[0], MAX_BUFFER-1) < 0)
    {
       cerr << "read from socket is failed!" <<endl;
        return 4;
    }
    cout << read_buffer1 << endl;

    signal(SIGINT,handle_signal);



while(1)
{
    string read_buffer(MAX_BUFFER, 0);
    string service_buffer (MAX_BUFFER, 0);  

    service work;

    cout << endl <<"========= Welcome to Bank Client ======="<<endl;
    cout << "請輸入服務項目-> 登入: LOGIN, 存款: SAVE, 提款: WITHDRAW,"
    << "查餘額: BALANCE,轉帳:TRANSFER,結束交易: EXIT "<<endl; 
    cout << endl;


    // getline(): get a line from stream to string 
    getline(cin, service_buffer);        
    write(sockfd,service_buffer.c_str(), strlen(service_buffer.c_str()));

    if(service_buffer=="LOGIN") work = LOGIN;
    else if(service_buffer=="SAVE") work = SAVE;
    else if(service_buffer=="WITHDRAW") work = WITHDRAW;
    else if(service_buffer=="BALANCE") work = BALANCE;
    else if(service_buffer=="TRANSFER") work = TRANSFER;
    else  work = EXIT;
    
    switch(work)
    {
        case 0:
            login();
            break;        
        case 1:
            save();
            break;    
        case 2:
            withdraw();
            break; 
        case 3:
            balance();
            break;
        case 4:
            // void transfer();
            break;
        default:
            handle_signal(2);
    }
}
 

// close(sockfd);
return 0;
}

//  ===============登入帳戶===============
void login()
{
    string read_buffer(MAX_BUFFER, 0);
    string write_buffer (MAX_BUFFER, 0);

    cout << "Got it! Login! "<<endl;
    cout << "Please enter your password:";

    getline(cin, write_buffer);        // getline(): get a line from stream to string 
    if (write(sockfd, write_buffer.c_str(), strlen(write_buffer.c_str())) < 0) 
    {
        cerr << "write to socket is failed!" << endl;
    }

    if (read(sockfd, &read_buffer[0], MAX_BUFFER-1) < 0)
    {
       cerr << "read from socket is failed!" <<endl;
    }
    cout << read_buffer << endl;
}


// ===============存款===============
void save()
{
    string read_buffer(MAX_BUFFER, 0);
    string write_buffer (MAX_BUFFER, 0);

    cout << "Got it! Saving! "<<endl;
    cout << "How much money? : ";

    getline(cin, write_buffer);        // getline(): get a line from stream to string 
    if (write(sockfd, write_buffer.c_str(), strlen(write_buffer.c_str())) < 0) 
    {
        cerr << "write to socket is failed!" << endl;
    }

    if (read(sockfd, &read_buffer[0], MAX_BUFFER-1) < 0)
    {
       cerr << "read from socket is failed!" <<endl;
    }
    cout <<read_buffer << endl;
}

// ==============提款===============
void withdraw()
{
    string read_buffer(MAX_BUFFER, 0);
    string write_buffer (MAX_BUFFER, 0);

    cout << "Got it! Withdraw "<< endl;
    cout << "How much money? : ";

    getline(cin, write_buffer);       // getline(): get a line from stream to string 
    if (write(sockfd, write_buffer.c_str(), strlen(write_buffer.c_str())) < 0) 
    {
        cerr << "write to socket is failed!" << endl;   
    }

    if (read(sockfd, &read_buffer[0], MAX_BUFFER-1) < 0)
    {
       cerr << "read from socket is failed!" <<endl;
    }
    cout <<read_buffer << endl;
  

}

//==============查詢餘額===============
void balance()
{
    string read_buffer (MAX_BUFFER, 0);
    string write_buffer (MAX_BUFFER, 0);

    cout << "Got it! balance "<< endl;
    if (read(sockfd, &read_buffer[0], MAX_BUFFER-1) < 0)
    {
       cerr << "read from socket is failed!" <<endl;       
    }
    cout << read_buffer << endl;
    cout << " Need transaction detail? (Y/N) ";

    getline(cin, write_buffer);       // getline(): get a line from stream to string 
    if (write(sockfd, write_buffer.c_str(), strlen(write_buffer.c_str())) < 0) 
    {
        cerr << "write to socket is failed!" << endl;   
    }

}

//==============結束交易===============
void handle_signal(int sig)
{
    string write_buffer = "EXIT";
    cout << "結束交易中.......Close Socket" <<endl;

    write(sockfd, write_buffer.c_str(), strlen(write_buffer.c_str()));
    close(sockfd);
    cout << "Done!" << endl;
    abort();

}