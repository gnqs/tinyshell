/* run this program using the console pauser or add your own getch, system("pause") or input loop */
//程序名： tinyshell part4
//作者： 归诺祺 
//时间：
//功能： 编译器框架，echo指令，复合指令 
#define _CRT_SECURE_NO_WARNINGS
//I/O header files
#include<iostream>                                       
#include<cstdio>
#include<fstream>
#include<strstream>
//data struct
#include<vector>
#include<stack>
#include<queue>
//library
#include<cstring>
#include<string>
#include<algorithm>
#include<cstdlib>
using namespace std;
#define MAXLINE 1000
#define MAXFILE 131072
/*各函数声明 */
void doDiff(int argc, char * argv[]);                    
void doGrep(int argc, char * argv[]);
void doTee(int argc, char* argv[]);
void doCat(int argc, char* argv[]);
void doCp(int argc, char* argv[]);
void doCd(int argc, char* argv[]);
void doPwd(int argc, char* argv[]);
void debug(int argc, char* argv[]);
void doEcho(int argc, char * argv[]);

void setup(bool);
void shell_loop(void);
void parse_command_and_fuction_call(void);
int  judge(char c[MAXLINE]);


 /*定义各全局变量*/ 
bool judge_n = true ;
struct Terminal {                      
    char user[MAXLINE];      // 用户名
    char mach[MAXLINE];      // 计算机名
    char root[MAXLINE];      // 根目录
    char wdir[MAXLINE];      // 工作目录
    char strin[MAXFILE];     // 重定向标准输入
    char strout[MAXFILE];    // 重定向标准输出
};
Terminal gTerm;
Terminal *p = &gTerm;
char cmdline[MAXLINE];       //输入的命令行

#include"info.h"
#include"diff.h"
#include"grep.h"
#include"part3.h"

 /*主函数*/ 
int main(int argc,char **argv){                                      
	setup(true);               //安装信号 ,初始化用户名，计算机名，根目录 
    shell_loop();          //进入shell循环
    return 0;
}
/*初始化用户名，计算机名，根目录（在主函数中调用）*/
void setup(bool UseDefaultSet) {
	if (!UseDefaultSet) {
		cout << "Machine Name:" << " ";
		cin.getline(p->mach, MAXLINE);
		cout << "Root Directory:" << " ";
		cin.getline(p->root, MAXLINE);
		cout << "Login:" << " ";
		cin.getline(p->user, MAXLINE);
		p->wdir[0] = '/';
	}
	else
	{
		const char mach_default[] = "default_Machine";
		const char root_default[] = "C:\\Users\\moon\\Desktop\\default_version\\test";
		const char user_default[] = "default_User";
		strcpy(p->mach, mach_default);
		strcpy(p->root, root_default);
		strcpy(p->user, user_default);
		p->wdir[0] = '/';

		printf("default set loaded.\n");
	}
	return;
}

/*重复命令行输入提示并执行（在主函数中调用）*/ 
void shell_loop(void){                                                
    while(1){                                                      //双层while使输入空命令时也能继续 
        while(1){                                                    
            cout<<"\033[92;1m"<<p->user<<"@"<<p->mach
            <<"\033[0m"<<":"<<"\033[94;1m"<<p->wdir<<"\033[0m"<<"$"<<" ";
            /*清空命令行，标准输入,标准输出*/
            memset(cmdline , '\0' , MAXLINE);                      
            memset(gTerm.strout , '\0' , MAXFILE);
            memset(gTerm.strin , '\0' , MAXFILE);  
            cin.getline(cmdline , MAXLINE);            //接收输入命令行 
            if(cmdline[0] == '\0')
                break;
            parse_command_and_fuction_call();      // 解析命令,将命令拆分为命令行参数，并调用相应函数 
        }
    }
}
                
/*将输入的命令拆分成命令行参数,识别参数并调用相应函数*/ 
void parse_command_and_fuction_call(void){                                    
    char *line = cmdline;
    int cate ; 
    while(*line != '\0'){
        /*申请动态数组用于存放解析出的参数*/ 
        char **argv1 = new char* [50];             
        for(int i=0 ; i<50 ; i++){
            argv1[i] = new char [MAXLINE];
            for(int j=0 ; j<MAXLINE ; j++){
                argv1[i][j] = '\0'; 
            } 
        }
        int i = 0;                                   //i用来计数命令参数个数                         
        /*每次一个解析复合命令中每个子命令*/
        while (*line!='\0' && *line!='|'){           
            int len1 = 0 ;
            while (*line == ' ')                   //跳过空格 
                line ++;                       
            while((*line!='\0') && (*line!=' ') && (*line!='|')){     //以空格为界记录每个命令 
                argv1[i][len1] = *line;                          
                len1++;
                line ++;
            }
            while (*line == ' ')                            //跳过空格 
                line ++;

            if(*line != '\0' && *line !='|')
                i++ ;
        }
        int a = i+1;
        judge_n = true ;
        /*执行命令，调用函数*/
        cate =  judge (argv1[0]) ;
        switch(cate){                                          
            case 1:
            doDiff(a,argv1);
            break;
            case 2:
            doGrep(a,argv1);
            break;
            case 3:
            doTee (a,argv1);
            break;
            case 4:
            doCat (a,argv1);
            break;
            case 5:
            doCp  (a,argv1);
            break;
            case 6:
            doCd  (a,argv1);
            break;
            case 7:
            doPwd (a,argv1);
            break;
            case 8:
            doEcho(a,argv1);
            break;
            case 9:
            exit(0) ;
            default:
            break;
        }
        /*为下一个命令准备，将标准输出复制到标准输出，并清空标准输出*/
        if(*line == '|'){                               
            line++;
            memset(gTerm.strin , '\0' , MAXFILE);
            strcpy (gTerm.strin , gTerm.strout);
            memset(gTerm.strout , '\0',MAXFILE);
        }
        delete []argv1;   
    }
    if(gTerm.strout[0]=='\0'){
        
    }
    else{
        if(cate==8){
            cout << gTerm.strout ;
        }
        else{
            char *n = gTerm.strout;
            while(*n != '\0'){
                n ++ ;
            }
            if(*(n-1) !='\n' && *(n-2)!='\n' && *(n-3)!='\n'){
                cout << gTerm.strout << endl ;
            }
            else{
                cout << gTerm.strout ;
            }
        }
    }
}

/*判断指令，返回一个int值，便于使用switch
（在parse_command_and_fuction_call中调用）*/
int judge(char b[MAXLINE]){                                                    
    
    if(b[0]=='d'&&b[1]=='i'&&b[2]=='f'&&b[3]=='f'&&b[4]=='\0')        //检测下一位是否为\0 防止误判 正确指令+多余字符 如pwdf 
        return 1;
    else if(b[0]=='g'&&b[1]=='r'&&b[2]=='e'&&b[3]=='p'&&b[4]=='\0')
        return 2;
    else if(b[0]=='t'&&b[1]=='e'&&b[2]=='e'&&b[3]=='\0')
        return 3;
    else if(b[0]=='c'&&b[1]=='a'&&b[2]=='t'&&b[3]=='\0')
        return 4;
    else if(b[0]=='c'&&b[1]=='p'&&b[2]=='\0')
        return 5;
    else if(b[0]=='c'&&b[1]=='d'&&b[2]=='\0')
        return 6;
    else if(b[0]=='p'&&b[1]=='w'&&b[2]=='d'&&b[3]=='\0')
        return 7;
    else if(b[0]=='e'&&b[1]=='c'&&b[2]=='h'&&b[3]=='o'&&b[4]=='\0')
        return 8;
    else if(b[0]=='e'&&b[1]=='x'&&b[2]=='i'&&b[3]=='t'&&b[4]=='\0')
        return 9;
    
        cerr<<"command '"<<b<<"' not found \n";                      //未检测到命令，直接将错误提示打在屏幕上                                      
        return 0;
    
}

void doEcho(int argc, char * argv[]){
    char help[100]="Echo the STRING(s) to standard output. \n-n    do not output the trailing newline";      //帮助内容 
    int start = 1;                   //start用于记录从哪个参数开始放入标准输出 
    if(argc ==1) return ;
    if(argv[1][0]=='-'&&argv[1][1]=='-'&&argv[1][2]=='h'&&argv[1][3]=='e'&&argv[1][4]=='l'&&argv[1][5]=='p'&&argv[1][6]=='\0'){
        strcpy(gTerm.strout , help);
        char a[2] = "\n" ;
        strcat(gTerm.strout , a) ;
        return;
    }
    if(argv[1][0]=='-'&&argv[1][1]=='n'&&argv[1][2]=='\0'){
         judge_n = false;
         start = 2;
    }    
    int j=0;   
    for(int i=start ; i<argc ; i++){
        char *pot=argv[i];
        while(*pot != '\0'){
            gTerm.strout[j] = *pot;
            j++;
            pot++;
        }
        gTerm.strout[j] = ' ';
        j++;
    }
    if(judge_n){
          gTerm.strout[j] = '\n' ;
          j++;
    }
    gTerm.strout[j] = '\0';
    
}

