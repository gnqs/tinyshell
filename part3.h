//���������ߣ����α� 2021010757

//������ƻ���2021�����ҵ
#define LINE_ID_SIZE 6//cat�������������
//���help�ĵ�

#define COLOR_RESET "\033[0m"		//
#define COLOR_RED   "\033[91;1m"	//
#define COLOR_GREEN "\033[92;1m"	//
#define COLOR_BLUE  "\033[94;1m"	//

void _doTee(int argc, char* argv[]);
void _doCat(int argc, char* argv[]);
void _doCp(int argc, char* argv[]);
void _doCd(int argc, char* argv[]);
void _doPwd(int argc, char* argv[]);

inline void doTee(int argc, char* argv[]) { _doTee(argc - 1, argv + 1); return; }
inline void doCat(int argc, char* argv[]) { _doCat(argc - 1, argv + 1); return; }
inline void doCp (int argc, char* argv[]) { _doCp (argc - 1, argv + 1); return; }
inline void doCd (int argc, char* argv[]) { _doCd (argc - 1, argv + 1); return; }
inline void doPwd(int argc, char* argv[]) { _doPwd(argc - 1, argv + 1); return; }
//��������������
void write_linehead(bool if_n, bool if_b, bool if_blank, char* &Destination, int* lineID);	//����cat()�����ÿ�����ݵ�ͷ��
void strLink(char* Destination, char* Source);												//��һ���ַ���׷�ӵ���һ������
void strremove(char* str, int begin, int length);											//ȥ���ַ��� str �Ĵ� begin ��� length �����ִ�
void strLinToWin(char* str);																//��linuxʽ·��ת��Ϊwindowsʽ·��
void puterror(int errorID, char* errorinfo, char* functioninfo);							//���������Ϣ
bool strequal(char* str1, char* str2);														//�ж����ַ����Ƿ����
int  path_analysis(char* path, char* Destination);											//·����������
int  para_translate(char* str, bool* isAble, bool* isUsed);									//������������
int  is_color_str(char* str);
void remove_color_str(char *str);
//�����Ķ���

//Function para_translate
#define CASE_STR	-2
#define ERROR_PARA	-1	
#define CASE_H		 0	
#define CASE_A		 1	
#define CASE_B		 2	
#define CASE_E		 3	
#define CASE_N		 4	
#define CASE_S		 5
//With order help,a,b,E,n,s
char paras[6][8] = { "--help" ,"-a","-b","-E","-n","-s" };
//Function path_analysis()
#define PATH_DIR	0
#define PATH_STDIO	1
#define PATH_BEYOND 2
char dir_stdio[] = "-";
char slash    [] = "/";
//Function puterror()
#define PATH_NOT_FOUND	 0
#define PATH_BEYOND_ROOT 1
#define PARA_TOO_LONG	 2
#define PARA_TOO_SHORT	 3
#define	PARA_NOT_FOUND	 4
#define PARA_HELP		 5 
//debug()
bool showtype = false;
//���庯����ʵ��

void _doTee(int argc, char* argv[])
{
	//With order help,a,b,E,n,s
	bool isAble[] = { 1,1,0,0,0,0 };
	bool isUsed[] = { 0,0,0,0,0,0 };
	char name[] = "tee";

	for (int i = 0; i < argc; ++i)
	{
		int paraID = para_translate(argv[i], isAble, isUsed);
		switch (paraID)
		{
			//Begin switch
		case CASE_A:
		{
			isUsed[CASE_A] = true;
			break;
		}
		case CASE_H:
		{
			if (i == 0 && argc == 1) {
				printf(HELP_TEE);
				break;
			}
			else {
				puterror(PARA_HELP, NULL, name);
				return;
			}
		}
		case CASE_STR:
		{
			char fdir[MAXLINE];//���յ�·��
			char _dir[MAXLINE];//����ϵͳ�еľ���·��
			strcpy(fdir, gTerm.root);		//����ϵͳ��Ŀ¼
			int returnvalue =
				path_analysis(argv[i], _dir);	//ƴ���������·��

				//������Ŀ¼ʱ�Ĵ�����
			if (returnvalue == PATH_BEYOND) {
				puterror(PATH_BEYOND_ROOT, argv[i], name);
				return;
			}
			//��׼������������
			if (returnvalue == PATH_STDIO) {
				strcpy(gTerm.strout, gTerm.strin);
				return;
			}
			//�ļ�·�������
			if (returnvalue == PATH_DIR) {
				strLink(fdir, slash);
				strLink(fdir, _dir);		// link strings
				strLinToWin(fdir);			// translate to Windows style

				FILE* filepin = freopen(fdir, isUsed[CASE_A] ? "a" : "w", stdout); //���Դ��ļ�
				if (filepin == NULL) {
					//��ʧ�ܣ�ת�������
					puterror(PATH_NOT_FOUND, _dir, name);
					freopen("CON", "w", stdout);
					return;
				}
				else {
					//�ɹ���
					char tempfile[MAXFILE];
					strcpy(tempfile, gTerm.strin);
					remove_color_str(tempfile);
					printf("%s", tempfile);
					freopen("CON", "w", stdout);
					break;
				}
			}
		}
		case ERROR_PARA:
		{
			puterror(PARA_NOT_FOUND, argv[i], name);
			return;
		}
		default:
		{
			std::cerr << "Default in switch of " << name << "\n";
			break;
		}
		//End of switch
		}
	}
	if (isUsed[CASE_A]) { strLink(gTerm.strout, gTerm.strin); }
	else { strcpy(gTerm.strout, gTerm.strin); }

	return;
}

void _doCat(int argc, char* argv[])
{
	//With order help,a,b,E,n,s
	bool isAble[] = { 1,0,1,1,1,1 };
	bool isUsed[] = { 0,0,0,0,0,0 };
	char name[] = "cat";
	bool if_source = false;

	char tempfile[MAXFILE] = {};
	char*tail = tempfile;

	for (int i = 0; i < argc; ++i)
	{
		int paraID = para_translate(argv[i], isAble, isUsed);
		switch (paraID)
		{
			//Begin switch
		case CASE_H:
		{
			if (i == 0 && argc == 1) {
				printf(HELP_CAT);
			}
			else {
				puterror(PARA_HELP, NULL, name);
			}
			return;
		}
		break;
		case CASE_B:
		{
			isUsed[CASE_B] = true;
		}
		break;
		case CASE_E:
		{
			isUsed[CASE_E] = true;
		}
		break;
		case CASE_N:
		{
			isUsed[CASE_N] = true;
		}
		break;
		case CASE_S:
		{
			isUsed[CASE_S] = true;
		}
		break;
		case CASE_STR:
		{
			char fdir[MAXLINE];//���յ�·��
			char _dir[MAXLINE];//����ϵͳ�еľ���·��
			strcpy(fdir, gTerm.root);		//����ϵͳ��Ŀ¼
			int returnvalue =
				path_analysis(argv[i], _dir);	//ƴ���������·��
				//�������������ʱ�ļ�֮��׷���ı�������
			if (returnvalue == PATH_BEYOND) {
				puterror(PATH_BEYOND_ROOT, argv[i], name);
				return;
			}
			if (returnvalue == PATH_STDIO) {
				strLink(tempfile, gTerm.strin);
				tail += strlen(gTerm.strin);
				if (strlen(gTerm.strin)) {
					*(tail) = '\n';
					++tail;
				}
				break;
			}
			if (returnvalue == PATH_DIR) {
				if_source = true;

				strLink(fdir, slash);
				strLink(fdir, _dir);		// link strings
				strLinToWin(fdir);			// translate to Windows style
				FILE* filepin = freopen(fdir, "r", stdin);
				if (filepin == NULL) {
					puterror(PATH_NOT_FOUND, argv[i], name);
					freopen("CON", "r", stdin);
					return;
				}
				else {
					char tempc;
					while ((tempc = getchar()) != EOF) {
						*tail = tempc;
						++tail;
					}
					freopen("CON", "r", stdin);
					if (*(tail - 1) != '\n') { //����ļ�ĩβû��'\n'���ͼ���һ������Ϊ�ļ�֮��ķָ�
						*tail = '\n';
						++tail;
					}
				}
			}
			//end of case STR
		}
		break;
		case ERROR_PARA:
		{
			puterror(PARA_NOT_FOUND, argv[i], name);
		}
		break;
		default:
		{
			std::cerr << "Default in switch of " << name << "\n";
		}
		break;
		//End of switch
		}
	}
	*tail = '\0';

	//�����ļ�鲿��
	if (!if_source) {
		puterror(PARA_TOO_SHORT, NULL, name);
		return;
	}
	if (isUsed[CASE_N] && isUsed[CASE_B]) {
		std::cerr << COLOR_RED << "Error: cat: There is a contradiction between Parameter \"-n\" and  \"-b\" \n" << COLOR_RESET;
		std::cerr << "Use \"" << "cat" << " --help\" for more information\n";
		return;
	}

	//��ʼ�����ı����������׼���
	int i = 0;
	char* strout_pin = gTerm.strout;
	memset(gTerm.strout, '\0', sizeof(gTerm.strout));

	bool if_prev_enter = true;	//ǰһ���ַ��Ƿ��ǻ��з�
	bool if_prev_blank = false; //ǰһ���Ƿ�Ϊ�հ���
	int  ID_of_thisline = 1;	//��ǰ�еı��
	while (tempfile + i < tail)
	{
		switch (tempfile[i])
		{
		case '\n'://�����е�ĩβ
		{
			if (if_prev_enter)
			{													//��һ���ǿհ���
				if ((if_prev_blank) && (isUsed[CASE_S])) {
					break;
				}
				else {
					write_linehead(isUsed[CASE_N], isUsed[CASE_B], true, strout_pin, &ID_of_thisline);
					if (isUsed[CASE_E])
					{
						*strout_pin = '$'; ++strout_pin;
					}//������� -E
					*strout_pin = tempfile[i];
					++strout_pin;
					if_prev_blank = true;
				}
			}
			else { //��һ�в��ǿհ��У��������'\n'
				if (isUsed[CASE_E]) {
					*strout_pin = '$';
					++strout_pin;
				}//������� -E
				*strout_pin = tempfile[i];
				++strout_pin;
			}
			//������Σ�if_prev_enter��Ϊtrue
			if_prev_enter = true;
			//The end of case '\n'
			break;
		}
		default:  //�еĿ�ͷ���м�
		{
			if (if_prev_enter) {						//�����ı��еĿ�ͷ
				write_linehead(isUsed[CASE_N], isUsed[CASE_B], false, strout_pin, &ID_of_thisline);
				*strout_pin = tempfile[i];
				++strout_pin;
				//����״̬
				if_prev_blank = false;
				if_prev_enter = false;
			}
			else {										//�ⲻ���ı��еĿ�ͷ
				*strout_pin = tempfile[i];
				++strout_pin;
			}
		}
		//End of switch
		}
		//�ƶ�����һ��λ��
		++i;
	}
	//Ϊstrout��β
	/*
	if ((gTerm.strout[0] != '\0') && *(strout_pin - 1) == '\n')
	{
		*(strout_pin - 1) = '\0';//�ַ����ǿ���ĩβ�л��з���ȥ�����з�
	}
	*/
	*strout_pin = '\0';

	return;
}

void _doCp(int argc, char* argv[])
{
	//With order help,a,b,E,n,s
	bool isAble[] = { 1,0,0,0,1,0 };
	bool isUsed[] = { 0,0,0,0,0,0 };
	char name[] = "cp";

	bool if_sour = false;
	bool if_dest = false;
	bool if_sour_stdio = false;
	bool if_dest_stdio = false;

	int sourcekey = 0;
	int destinationkey = 0;

	for (int i = 0; i < argc; ++i)
	{
		int paraID = para_translate(argv[i], isAble, isUsed);
		switch (paraID)
		{
			//Begin switch
		case CASE_H:
		{
			if (i == 0 && argc == 1) {
				printf(HELP_CP);
			}
			else {
				puterror(PARA_HELP, NULL, name);
			}
			return;
		}
		case CASE_N:
		{
			isUsed[CASE_N] = true;
			break;
		}
		case CASE_STR:
		{
			if (!if_sour) {
				sourcekey = i;
				if_sour = true;
			}
			else {
				if (!if_dest) {
					destinationkey = i;
					if_dest = true;
				}
				else {
					puterror(PARA_TOO_LONG, NULL, name);
				}
			}
			break;
		}
		case ERROR_PARA:
		{
			puterror(PARA_NOT_FOUND, argv[i], name);
			break;
		}
		default:
		{
			std::cerr << "Default in switch of " << name << "\n";
			break;
		}
		//End of switch
		}
	}

	//��ʼ������
	if (!if_sour) {
		puterror(PARA_TOO_SHORT, NULL, name);
		return;
	}
	if (!if_dest) {
		puterror(PARA_TOO_SHORT, NULL, name);
		return;
	}
	//��ʼ�����������
	//begin to IO
	char fdir_source[MAXLINE] = {};
	char _dir_source[MAXLINE] = {};
	char fdir_destin[MAXLINE] = {};
	char _dir_destin[MAXLINE] = {};
	//����Դ�ļ�·��
	if (true)
	{
		strcpy(fdir_source, gTerm.root);
		int returnvalue =
			path_analysis(argv[sourcekey], _dir_source);

		if (returnvalue == PATH_BEYOND) {
			puterror(PATH_BEYOND_ROOT, argv[sourcekey], name);
			return;
		}
		if (returnvalue == PATH_STDIO) {
			if_sour_stdio = true;
		}
		if (returnvalue == PATH_DIR) {
			strLink(fdir_source, slash);
			strLink(fdir_source, _dir_source);
			strLinToWin(fdir_source);			// translate to Windows style
		}
	}
	//����Ŀ���ļ�·��
	if (true)
	{
		strcpy(fdir_destin, gTerm.root);
		int returnvalue =
			path_analysis(argv[destinationkey], _dir_destin);

		if (returnvalue == PATH_BEYOND) {
			puterror(PATH_BEYOND_ROOT, argv[destinationkey], name);
			return;
		}
		if (returnvalue == PATH_STDIO) {
			if_dest_stdio = true;
		}
		if (returnvalue == PATH_DIR) {
			strLink(fdir_destin, slash);
			strLink(fdir_destin, _dir_destin);
			strLinToWin(fdir_destin);			// translate to Windows style
		}
	}
	if (!if_dest_stdio)
	{
		//��д�ж�
		if (isUsed[CASE_N]) {
			FILE* testpin = freopen(fdir_destin, "r", stdin);
			if (testpin != NULL) {
				std::cerr << "The destination file already exists. The operation is cancelled\n";
				freopen("CON", "r", stdin);
				return;
			}
			freopen("CON", "r", stdin);
		}
	}
	//���Դ�ļ�·��
	if (!if_sour_stdio)
	{
		FILE* sourpin = freopen(fdir_source, "r", stdin);
		if (sourpin == NULL) {
			puterror(PATH_NOT_FOUND, _dir_source, name);
			freopen("CON", "r", stdin);
			return;
		}
	}
	//���Ŀ���ļ�·��
	if (!if_dest_stdio)
	{
		FILE* destpin = freopen(fdir_destin, "w", stdout);
		if (destpin == NULL) {
			puterror(PATH_NOT_FOUND, _dir_destin, name);
			freopen("CON", "w", stdout);
			return;
		}
	}
	//��������
	char tempfile[MAXFILE];
	if (if_sour_stdio) {
		strcpy(tempfile, gTerm.strin);
	}
	else {
		char tempchar;
		int i = 0;
		while ((tempchar = getchar()) != EOF) {
			tempfile[i] = tempchar;
			++i;
		}
		tempfile[i] = '\0';
	}

	if (if_dest_stdio) {
		strcpy(gTerm.strout, tempfile);
	}
	else {
		remove_color_str(tempfile);
		printf("%s", tempfile);
	}

	freopen("CON", "r", stdin);
	freopen("CON", "w", stdout);
	return;
}

void _doCd(int argc, char* argv[])
{
	//With order help,a,b,E,n,s
	bool isAble[] = { 1,0,0,0,0,0 };
	bool isUsed[] = { 0,0,0,0,0,0 };
	char name[] = "cd";

	if (argc == 0) {
		puterror(PARA_TOO_SHORT, NULL, name);
		return;
	}
	if (argc == 1) {
		int paraID = para_translate(argv[0], isAble, isUsed);
		switch (paraID)
		{
			//Begin switch
		case CASE_H:
		{
			printf(HELP_CD);
			return;
		}
		case CASE_STR:
		{
			//��⵽·��
			char tempwdir[MAXLINE] = "";
			int returnvalue =
				path_analysis(argv[0], tempwdir);
			if (returnvalue == PATH_BEYOND) {
				puterror(PATH_BEYOND_ROOT, argv[0], name);
				return;
			}
			if (returnvalue == PATH_STDIO) {
				puterror(PATH_NOT_FOUND, argv[0], name);
			}
			if (returnvalue == PATH_DIR) {
				strcpy(gTerm.wdir + 1, tempwdir);
				return;
			}
		}
		case ERROR_PARA:
		{
			puterror(PARA_NOT_FOUND, argv[0], name);
			break;
		}
		default:
		{
			std::cerr << "Default in switch of " << name << "\n";
			break;
		}
		//End of switch
		}
	}
	puterror(PARA_TOO_LONG, NULL, name);
	return;
}

void _doPwd(int argc, char* argv[])
{
	//With order help,a,b,E,n,s
	bool isAble[] = { 1,0,0,0,0,0 };
	bool isUsed[] = { 0,0,0,0,0,0 };
	char name[] = "pwd";

	if (argc == 0) {
		printf("/%s\n", gTerm.wdir + 1);
		return;
	}
	if (argc == 1) {
		int paraID = para_translate(argv[0], isAble, isUsed);
		switch (paraID)
		{
			//Begin switch
		case CASE_H:
		{
			printf(HELP_PWD);
			return;
		}
		case ERROR_PARA:
		{
			puterror(PARA_NOT_FOUND, argv[0], name);
			break;
		}
		default:
		{
			std::cerr << "Default in switch of " << name << "\n";
			break;
		}
		//End of switch
		}
		puterror(PARA_TOO_LONG, NULL, name);
		return;
	}
}

//����������ʵ��

void write_linehead(bool if_n, bool if_b, bool if_blank, char* &Dest_pin, int* ID)
{
	if ((if_n) || (if_b))
	{//��head�����+
		if (  (if_n)      // n = true 
			||(!if_blank) // b = true & not a blank line 
			)
		{//Ӧ�������������line head��ʱ��
			int lineID = *(ID);
			char head_ID_part[16];
			for (int i = LINE_ID_SIZE - 1; i >= 0; --i) {
				head_ID_part[i] = lineID ? char('0' + (lineID % 10)) : ' ';
				lineID /= 10;
			}
			head_ID_part[LINE_ID_SIZE] = '\0';
			strLink(Dest_pin, head_ID_part);
			Dest_pin += LINE_ID_SIZE;

			//����������ո�
			*Dest_pin = ' ';
			++Dest_pin;
			*Dest_pin = ' ';
			++Dest_pin;

			++(*ID);
			return;
		}
		else
		{
			for (int i = 0; i < LINE_ID_SIZE; ++i) {
				*Dest_pin = ' ';
				++Dest_pin;
			}

			//����������ո�
			*Dest_pin = ' ';
			++Dest_pin;
			*Dest_pin = ' ';
			++Dest_pin;

			return;
		}
	}
	return;
}

void strLink(char* Destination, char* Source)
{
	int i = 0;
	int j = 0;
	while (Destination[i] != '\0')
		++i;
	while (Source[j] != '\0') {
		Destination[i + j] = Source[j];
		++j;
	}
	Destination[i + j] = '\0';
	return;
}

void strremove(char* str, int begin, int length)
{
	int j = begin + length;
	while (str[j] != '\0') {
		str[j - length] = str[j];
		++j;
	}
	str[j - length] = str[j];
	return;
}

void strLinToWin(char* str)
{
	for (int i = 0; str[i] != '\0'; ++i) {
		if (str[i] == '/')
			str[i] = '\\';
	}
	return;
}

void puterror(int errorID, char* errorinfo, char* functioninfo)
{
	/*Format:
			cerr << COLOR_RED << functioninfo << ':' << errorinfo << ':' << "\n" << COLOR_RESET;
			break;
	*/
	char nullstr[] = "";
	if (errorinfo == NULL)
	{
		errorinfo = nullstr;
	}
	std::cerr << COLOR_RED << functioninfo << "  :" << errorinfo << "  :";
	switch (errorID) {
	case PATH_BEYOND_ROOT: {
		std::cerr << "Error: Can not return to one level above the root directory\n" << COLOR_RESET;
		break;
	}
	case PATH_NOT_FOUND: {
		std::cerr << "Error: No find such file or directory\n" << COLOR_RESET;
		break;
	}
	case PARA_NOT_FOUND: {
		std::cerr << "Error: No parameters matching were found\n" << COLOR_RESET;
		break;
	}
	case PARA_TOO_LONG: {
		std::cerr << "Error: Too many paraators were entered\n" << COLOR_RESET;
		break;
	}
	case PARA_TOO_SHORT: {
		std::cerr << "Error: Not enough parameters were entered\n" << COLOR_RESET;
		break;
	}
	case PARA_HELP: {
		std::cerr << "Error: Can not use parameter --help together with other parameters\n" << COLOR_RESET;
		break;
	}
	default: break;
	}
	std::cerr << "Use \"" << functioninfo << " --help\" for more information\n";
	return;
}

bool strequal(char* str1, char* str2) {
	int l1 = strlen(str1);
	int l2 = strlen(str2);
	if (l1 != l2)
		return false;
	for (int i = 0; i < l1; ++i) {
		if (str1[i] != str2[i])
			return false;
	}
	return true;
}

int path_analysis(char* path, char* Destination)
{
	//�����"-",���ر�׼�������
	//Root::	F:/GreatHomework
	//Wdir::	test0-1
	//Path::	-
	//Output	PATH_STDIO
	if (strequal(path, dir_stdio)) {
		return PATH_STDIO;
	}
	if (path[0] == '/')
	{
		//����Ǿ���·��,ԭ������(ע�⿪ͷ��'/'������)
		//Root::	F:/GreatHomework
		//Wdir::	test0-1
		//Path::	/test1-1/test1-4
		//Output	tets1-1/test1-4
		strcpy(Destination, path + 1);
	}
	else {
		//��������·�������м����"/"������
		//Root::	F:/GreatHomework
		//Wdir::	test0-1
		//Path::	./test1-4
		//Output	test0-1/./test1-4
		strcpy(Destination, gTerm.wdir + 1);
		strLink(Destination, slash);
		strLink(Destination, path);
		//ȥ����ͷ�� '/'
		if (Destination[0] == '/')
			strremove(Destination, 0, 1);
	}
	//�ڽ�β���� '/'
	int k = strlen(Destination);
	if (Destination[k - 1] != '/') {
		Destination[k] = '/';
		Destination[k + 1] = '\0';
	}
	//��ʼ���� ./ �� ../

	//���� ./
	{
		//ֱ��ȥ�� ./
		//Input:	test0-1/./test1-4
		//Marked	test0-1/#/test1-4
		//Output	test0-1/test1-4
		int len = strlen(Destination);
		for (int i = 0; i < len - 1; ++i) {
			if (
				Destination[i] == '.'
				&&Destination[i + 1] == '/'
				&& (i == 0 || Destination[i - 1] != '.')
				)//�ҵ�"./"
			{
				Destination[i] = '#';
				//��ʱ���
			}
		}
		for (int i = len - 2; i >= 0; --i) {
			if (Destination[i] == '#')
				strremove(Destination, i, 2);
			//ȥ�� "./"
		}
	}
	//���� ../
	{
		//ǰ��׷������һ��'/',���ʧ�ܣ��ͱ���
		//Input:	test0-1/../test0-0
		//Marked	test0-1/#./test0-0
		//Output	test0-0

		//Input:	test0-1/../../test0-0
		//Marked	test0-1/#./#./test0-0
		//			#./test0-0
		//Output	PATH_BEYOND

		int len = strlen(Destination);
		for (int i = 0; i < len - 2; ++i) {
			if (
				Destination[i] == '.'
				&&Destination[i + 1] == '.'
				&&Destination[i + 2] == '/'
				)//�ҵ� "../"
			{
				Destination[i] = '#';
			}
		}
		for (int i = 0; Destination[i] != '\0' && Destination[0] != '\0'; ++i) {
			if (Destination[i] == '#') {	//�������ʱ
				if (i == 0) {
					//�������Ŀ¼��һ��ʱ������PATH_BRYOND��������
					Destination[0] = '\0';
					return PATH_BEYOND;
				}
				else {
					int j = i - 2;
					while (j >= 0 && Destination[j] != '/') {
						--j;
					}
					strremove(Destination, j + 1, i - j - 1 + 3);
					//�ҵ�ɾ���������㣬����������ɾ��
					i = 0;
					//���´�ͷ��ʼɨ��
				}
			}
		}
	}
	//��ȥĩβ�� '/'
	if (Destination[strlen(Destination) - 1] == '/')
		Destination[strlen(Destination) - 1] = '\0';
	//�ڿ�ͷ���� '/'
	/*int length = strlen(Destination);
	for (int i = length; i > 0; --i) {
		Destination[i] = Destination[i - 1]; //�ַ�������'\0'���������
	}
	Destination[0] = '/';*/
	//����������������� PATH_DIR
	return PATH_DIR;
}

int para_translate(char* str, bool* isAble, bool* isUsed)
{
	bool ifsigned = false;
	int L = strlen(str);
	if (str[0] == '"'&&str[L - 1] == '"') {
		ifsigned = true;
		str[L - 1] = '\0';
		strremove(str, 0, 1);
	}
	if (!ifsigned)
	{
		if (str[0] == '-'&&str[1] == '\0') {
			return CASE_STR;
		}
		if (str[0] == '-')
		{
			for (int i = 0; i < 6; ++i)
			{
				if (isAble[i] && strequal(str, paras[i]))
				{
					if (isUsed[i]) {
						std::cerr << "Warning: " << str << ": This paramater has been entered.\n";
						return i;
					}
					else {
						return i;
					}
				}
			}
			return ERROR_PARA;
		}
		return CASE_STR;
	}
	else
		return CASE_STR;
}

void debug(int argc, char* argv[]) //������������Ե�
{
	char casestrin[] = "strin";
	char casestrout[] = "strout";
	char caseclear[] = "clear";
	char caseshowtype[] = "show";
	if (argc == 0) {
		std::cerr << "�����ǵ�����Ϣ:\n"
			<< "Username: " << gTerm.user << '\n'
			<< "Machine : " << gTerm.mach << '\n'
			<< "Root dir: " << gTerm.root << '\n'
			<< "Work dir: " << gTerm.wdir << '\n';
		std::cerr << '\n'
			<< "Stdin : \n" << gTerm.strin << '\n'
			<< "Stdout: \n" << gTerm.strout << '\n';
		return;
	}
	if (strequal(argv[0], casestrin)) {
		if (argc == 1) {
			memset(gTerm.strin, '\0', sizeof(gTerm.strin));
		}
		strcpy(gTerm.strin, argv[1]);
		return;
	}
	if (strequal(argv[0], casestrout))
	{
		if (argc == 1) {
			memset(gTerm.strout, '\0', sizeof(gTerm.strout));
		}
		strcpy(gTerm.strout, argv[1]);
		return;
	}
	if (strequal(argv[0], caseclear))
	{
		memset(gTerm.strin, '\0', sizeof(gTerm.strin));
		memset(gTerm.strout, '\0', sizeof(gTerm.strout));
		return;
	}
	if (strequal(argv[0], caseshowtype)) {
		showtype = ~showtype;
		return;
	}
}
int is_color_str(char* str) 
{
	int i = 0;
	char temp[20];
	char bluestr [] = COLOR_BLUE;
	char greenstr[] = COLOR_GREEN;
	char redstr	 [] = COLOR_RED;
	char whitestr[] = COLOR_RESET;

	for (; i < 15; ++i) {
		temp[i] = str[i];
		if (str[i] == 'm') {
			break;
		}
	}
	temp[i+1] = '\0';
	if (strequal(temp, bluestr) || strequal(temp, redstr) | strequal(temp, greenstr)){
		return 7;
	}
	if (strequal(temp, whitestr)) {
		return 4;
	}
	return 0;
}
void remove_color_str(char* str) {
	int i = 0;
	while (str[i] != '\0')
	{
		if (str[i]=='\033') {
			strremove(str, i, is_color_str(str + i));
		}else
		++i;
	}
	return;
}

