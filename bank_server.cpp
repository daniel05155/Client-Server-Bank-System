#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <ctype.h>
#include <unistd.h>           // write(),read(),lssek()...
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>            // 讀寫檔案
#include <pthread.h>
#include <ctime>
#include <time.h>
using namespace std;

constexpr unsigned int SERVER_PORT = 50544;
constexpr unsigned int buffer_size = 128;
constexpr unsigned int MSG_REPLY_LENGTH = 20;
constexpr unsigned int NUM_THREADS= 5;

pthread_mutex_t lock;

struct account{
     
     string name;
     string password;
     int number;
     int balance; 
     
}info;

// error
// struct account detail;
// detail.password = "123456";


const char *pass = "123456";

//  紀錄服務項目
const char *service_buffer;

void *handle_connection(void *socket_arg);
void  Login(void *socket);
void  SaveMoney(void *socket);
void  Withdraw(void *socket);
void  Check_balance(void *socket);
void  trans_detail(void);

 

int main(int argc, char *argv[])
{
     char msg_buffer2[buffer_size];
     void *ret;                          // receive the return value of thread
     int new_socket[NUM_THREADS] ;       // accept()return的值
     pthread_t new_thread[NUM_THREADS];  
     int i = 0,num = 0;
     

     // create server's socket
     int sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
     {
          cerr << "open socket error" << endl;
          return 1;
     }

     // 要再查setsockopt
     int optval = 1;
     setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));


     struct sockaddr_in server_info, client_info;
     bzero((char *) &server_info, sizeof(server_info));
     server_info.sin_family = AF_INET;                 // 使用IPv4
     server_info.sin_addr.s_addr = INADDR_ANY;  
     server_info.sin_port = htons(SERVER_PORT);        // 設定port

     // 把server的port和ID等資訊綁在socket
     if (bind(sockfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0)
     {
          std::cerr << "bind error" << std::endl;
          return 2;
     }
     
      // 計算接收client端的struct容器大小
     socklen_t client_info_size = sizeof(client_info);

     while(1)
     {    
          // sock(監聽sockfd這個socket , 一次最多5人連到server) 
          if( listen(sockfd, 5) == -1 )
          {
               cerr <<"Can Not Listening"<< endl;
               return 3;
          };     

          // accept(): 會接收client的連線資訊 
          // 且 return 一個socket number, 之後就以這socket跟client通訊  
          new_socket[i] = accept(sockfd, (struct sockaddr *) &client_info, &client_info_size) ;
          
          if(new_socket[i]<0)
          {    
               cerr << "accept() is failed!" <<endl;
               return 4;
          }

         
          cout << "server: got connection from =  " 
	     << inet_ntoa( client_info.sin_addr)
	     << " and port = " << ntohs( client_info.sin_port) << endl;
 
          int status = pthread_create(&new_thread[num], NULL, handle_connection, &new_socket[i]);
          
          if(status!= 0 )
          {
               cerr << " pthread_create() is failed ! " << endl;
               return 5;
          }
          
          i++;
          num++;
     }

cout<<"Exit System"<<endl;

// close(new_socket[i]);

close(sockfd);


return 0; 
}





//===============處理thread===================
void *handle_connection(void *socket_arg)
{
     // Receive the identifier of client's socket
     int client_socket = *(int *)socket_arg; 

     printf("It is from pthread ID - %1lu\n",pthread_self());

     // 通知client連線成功    
     char reply[] = "You are connected!";
     unsigned long int size =  sizeof(reply)/sizeof(reply[0]);
     write( client_socket ,reply,size);

     char send_buffer[buffer_size];
     char receive_buffer[buffer_size];
     receive_buffer[buffer_size] = {0};
     send_buffer[buffer_size] = {0};


     // Got the request  
     while(1)
   {
        memset(receive_buffer,0,buffer_size);
     
     cout << endl;
     cout << "============ waiting next request ==============" << endl;
     
     if (read(client_socket, &receive_buffer[0] , buffer_size )< 0)
     {
          cerr << "read from socket error" << endl;
          return NULL ;
     }   
     service_buffer = &receive_buffer[0];
     cout << "Got the message :" << receive_buffer << endl;

     // strcmp():字串相等 回傳0
     if(strcmp(receive_buffer,"EXIT")!=0){

          if(strcmp(receive_buffer,"LOGIN")==0)
               Login(&client_socket);
          else if(strcmp(receive_buffer,"SAVE")==0)
               SaveMoney(&client_socket);

          else if(strcmp(receive_buffer,"WITHDRAW")==0)
               Withdraw(&client_socket);
         
          else if(strcmp(receive_buffer,"BALANCE")==0) 
               Check_balance(&client_socket);
         
     }    
     else{
          cout << "Client will end the transaction!" <<endl;
          break; 
     }
   }
     
   return NULL;
}


// ===============登入===============
void  Login(void *socket)
{
     int socket_id = *(int *)socket ;
     string send_buffer(buffer_size,0);
     string ret_buffer(buffer_size,0);
     string  receive_buffer(buffer_size,0);

     cout << "------ Logining ------" << endl;
     if ((read(socket_id ,&receive_buffer[0],buffer_size-1) < 0))
     {
          cerr << "The function read1() from client's message is failed!" <<endl;
          return;
     }
     

     if(strcmp(receive_buffer.c_str(),pass)==0)  send_buffer = "Login Succeed! "; 
     else  send_buffer  = "Login Failed! Please try again!";  

     write(socket_id ,send_buffer.c_str(), strlen(send_buffer.c_str())); 

}

// ===============存款===============
void  SaveMoney(void *socket)
{
     int socket_id = *(int *)socket ;
     string receive_buffer(buffer_size,0);
     string send_buffer(buffer_size,0);
     string ret_buffer(buffer_size,0);

     pthread_mutex_lock(&lock);

     cout << "------ Saving ------" << endl;
     if ((read(socket_id ,&receive_buffer[0],buffer_size-1) < 0))
     {
          cerr << "The function read() from client's message is failed!" <<endl;
          return;
     }
     cout << "Client wants to save $" << receive_buffer <<endl;

     // string -> int ( using atoi() )
     info.number = stoi(receive_buffer);

     // Add to the balance
     info.balance += info.number;
     
     // int to string
     ret_buffer = to_string(info.balance);
     
     cout<<" The balance in Client's account is $" <<ret_buffer <<endl;

     send_buffer = "The balance in your account is $";
     
     send_buffer += ret_buffer;

     write(socket_id ,send_buffer.c_str(), strlen(send_buffer.c_str())); 

     pthread_mutex_unlock(&lock);
}


// ==============提款===============
void Withdraw(void *socket)
{
     int socket_id = *(int *)socket ;
    string receive_buffer(buffer_size,0);
    string send_buffer(buffer_size,0);
    string ret_buffer(buffer_size,0);

    pthread_mutex_lock(&lock);  
     cout << "------Withdrawing ------" << endl;

     if ( (read(socket_id ,&receive_buffer[0],buffer_size-1 ) < 0))
     {
          cerr <<"The function read() from client's message is failed!" <<endl;
          return ;
     }
     cout << "Client wants to withdraw $" << receive_buffer <<endl;

     // convert string to int : By atoi() 
     info.number = stoi(receive_buffer);

     // Subtract with the balance
     info.balance -= info.number;

     // int to string
     ret_buffer = to_string(info.balance);

     cout<<" The balance in Client's account is $" <<ret_buffer <<endl;

     send_buffer = "The balance in your account is $"; 
     send_buffer += ret_buffer;   
     write(socket_id ,send_buffer.c_str(), strlen(send_buffer.c_str())); 

     pthread_mutex_unlock(&lock);

}

//==============產生交易明細===============
void  trans_detail(void)
{
  
  ofstream fptr;
  fptr.open("/home/ubuntupi/Desktop/transaction.html");

  time_t curr_time;
  curr_time = time(NULL);
  char *time = ctime(&curr_time);
 
  // 交易序號
  srand( curr_time);
  int id = rand();

  
  fptr << " <!DOCTYPE html> "
         << "<table border=\"1\" cellpadding=\"1\" cellspacing=\"1\" style=\"width:600px\"><tbody>"
         << "<tr><td colspan=\"2\" style=\"text-align:center\"><span style=\"font-size:16px\">Bank Transcation Detail</span></td></tr>"
         << "<tr><td>交易時間</td><td>" << time << "</td></tr>"
         << "<tr><td>交易序號</td><td>" << id << "</td></tr>"
         << "<tr><td>服務項目</td><td>" << service_buffer << "</td></tr>"
         << "<tr><td>交易金額</td><td>" << info.number << "</td></tr>"
         << "<tr><td>帳戶餘額</td><td>" << info.balance << "</td></tbody></table>";
}


//==============查詢餘額===============
void Check_balance(void *socket)
{
     int socket_id = *(int *)socket ;
     string receive_buffer(buffer_size,0);
     string send_buffer(buffer_size,0);
     string ret_buffer(buffer_size,0);
     
     pthread_mutex_lock(&lock);

     cout << "------ Checking ------" << endl;

     // int to string
     ret_buffer = to_string(info.balance);

     cout<<" The balance in Client's account is $" <<ret_buffer <<endl;

     send_buffer = "The balance in your account is $";
     send_buffer += ret_buffer;

     write(socket_id ,send_buffer.c_str(), strlen(send_buffer.c_str())); 

     if ( (read(socket_id ,&receive_buffer[0],buffer_size-1 ) < 0))
     {
          cerr <<"The function read() from client's message is failed!" <<endl;
          return ;
     }

     if(receive_buffer=="Y") trans_detail();


     pthread_mutex_unlock(&lock);

}

