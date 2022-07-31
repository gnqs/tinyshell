/* run this program using the console pauser or add your own getch, system("pause") or input loop */
//�������� tinyshell part4
//���ߣ� ��ŵ�� 
//ʱ�䣺
//���ܣ� ��������ܣ�echoָ�����ָ�� 
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
/*���������� */
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


 /*�����ȫ�ֱ���*/ 
bool judge_n = true ;
struct Terminal {                      
    char user[MAXLINE];      // �û���
    char mach[MAXLINE];      // �������
    char root[MAXLINE];      // ��Ŀ¼
    char wdir[MAXLINE];      // ����Ŀ¼
    char strin[MAXFILE];     // �ض����׼����
    char strout[MAXFILE];    // �ض����׼���
};
Terminal gTerm;
Terminal *p = &gTerm;
char cmdline[MAXLINE];       //�����������

#include"info.h"
#include"diff.h"
#include"grep.h"
#include"part3.h"

 /*������*/ 
int main(int argc,char **argv){                                      
	setup(true);               //��װ�ź� ,��ʼ���û����������������Ŀ¼ 
    shell_loop();          //����shellѭ��
    return 0;
}
/*��ʼ���û����������������Ŀ¼�����������е��ã�*/
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

/*�ظ�������������ʾ��ִ�У����������е��ã�*/ 
void shell_loop(void){                                                
    while(1){                                                      //˫��whileʹ���������ʱҲ�ܼ��� 
        while(1){                                                    
            cout<<"\033[92;1m"<<p->user<<"@"<<p->mach
            <<"\033[0m"<<":"<<"\033[94;1m"<<p->wdir<<"\033[0m"<<"$"<<" ";
            /*��������У���׼����,��׼���*/
            memset(cmdline , '\0' , MAXLINE);                      
            memset(gTerm.strout , '\0' , MAXFILE);
            memset(gTerm.strin , '\0' , MAXFILE);  
            cin.getline(cmdline , MAXLINE);            //�������������� 
            if(cmdline[0] == '\0')
                break;
            parse_command_and_fuction_call();      // ��������,��������Ϊ�����в�������������Ӧ���� 
        }
    }
}
                
/*������������ֳ������в���,ʶ�������������Ӧ����*/ 
void parse_command_and_fuction_call(void){                                    
    char *line = cmdline;
    int cate ; 
    while(*line != '\0'){
        /*���붯̬�������ڴ�Ž������Ĳ���*/ 
        char **argv1 = new char* [50];             
        for(int i=0 ; i<50 ; i++){
            argv1[i] = new char [MAXLINE];
            for(int j=0 ; j<MAXLINE ; j++){
                argv1[i][j] = '\0'; 
            } 
        }
        int i = 0;                                   //i�������������������                         
        /*ÿ��һ����������������ÿ��������*/
        while (*line!='\0' && *line!='|'){           
            int len1 = 0 ;
            while (*line == ' ')                   //�����ո� 
                line ++;                       
            while((*line!='\0') && (*line!=' ') && (*line!='|')){     //�Կո�Ϊ���¼ÿ������ 
                argv1[i][len1] = *line;                          
                len1++;
                line ++;
            }
            while (*line == ' ')                            //�����ո� 
                line ++;

            if(*line != '\0' && *line !='|')
                i++ ;
        }
        int a = i+1;
        judge_n = true ;
        /*ִ��������ú���*/
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
        /*Ϊ��һ������׼��������׼������Ƶ���׼���������ձ�׼���*/
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

/*�ж�ָ�����һ��intֵ������ʹ��switch
����parse_command_and_fuction_call�е��ã�*/
int judge(char b[MAXLINE]){                                                    
    
    if(b[0]=='d'&&b[1]=='i'&&b[2]=='f'&&b[3]=='f'&&b[4]=='\0')        //�����һλ�Ƿ�Ϊ\0 ��ֹ���� ��ȷָ��+�����ַ� ��pwdf 
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
    
        cerr<<"command '"<<b<<"' not found \n";                      //δ��⵽���ֱ�ӽ�������ʾ������Ļ��                                      
        return 0;
    
}

void doEcho(int argc, char * argv[]){
    char help[100]="Echo the STRING(s) to standard output. \n-n    do not output the trailing newline";      //�������� 
    int start = 1;                   //start���ڼ�¼���ĸ�������ʼ�����׼��� 
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

