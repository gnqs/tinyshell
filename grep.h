#pragma once

struct lines {
    int hxh;//行序号
    char word[205];//每一行的内容的char形式
    char wordx[205];
    string s;//每一行的内容的string形式
    char g[200];
    bool keshuchu = false;
};
lines line[205];//行的信息
bool xing = false;//检测模式串中是否有通配符*
bool dian = false;//检测模式串中是否有通配符.
int sx = -1;//第一个*的位置
int wx = -1;//最后一个*的位置
int xingshu = 0;//*的数量
int shou = -1;//子串首部
int wei = -1;//子串尾部
int bz = 0;//标志
int s = 0;//选项总数，用于确定模式串的位置
int zhyh = 0;//文档最后一行的位置
int ppcg = 0;//匹配成功的行数
int Bhang = -1;
int Ahang = -1;
//检测是否有某个选项
bool h = false;
bool H = false;
bool ijc = false;
bool n = false;
bool c = false;
bool A = false;
bool B = false;
void stolower(char *zfc1,char *zfc2) {
    for (int k = 0;k <= strlen(zfc2) - 1;k ++) {
        if (zfc2[k] >= 'A' && zfc2[k] <= 'Z') {
            zfc1[k] = zfc2[k] + 32;
        }
        else {
            zfc1[k] = zfc2[k];
        }
    }
}
//BF算法用于有通配符.的情况
int BF(char *s, string t) {
    int i = 0;
    int j = 0;
    while (s[i] != '\0') {
        while (t[j] != '\0' && s[i + j] != '\0') {
            if (t[j] == '.') {
                j++;
                continue;
            }
            if (t[j] != s[i + j]) {
                break;
            }
            j++;
        }
        if (t[j] == '\0') {
            return i;
        }
        i++;
        j = 0;
    }
    return -1;
}
//构建get_next数组以实现KMP
void GetNext(string p, int next[]) {
    int pLen = p.size();
    next[0] = -1;
    int k = -1;
    int j = 0;
    while (j < pLen - 1) {
        if (k == -1 || p[j] == p[k]) {
            k++;
            j++;
            next[j] = k;
        }
        else {
            k = next[k];
        }
    }
}
//KMP算法用于不含通配符的匹配以提高匹配效率
int KMP(char* s, string p) {
    int i = 0;
    int j = 0;
    int next[500];
    GetNext(p, next);
    int sLen = strlen(s);
    int pLen = p.size();
    while (i < sLen && j < pLen) {
        if (j == -1 || s[i] == p[j]) {
            i++;
            j++;
        }
        else {
            j = next[j];
        }
    }
    if (j == pLen) {
        return i - j;
    }
    else {
        return -1;
    }
}
//有通配符时的初步匹配
bool Match(string s, string p) {
    int n = s.size();
    int m = p.size();
    int i = 0;
    int j = 0;
    int iStart = -1;
    int jStart = -1;
    while (i < n) {
        if (j < m && (s[i] == p[j] || p[j] == '.')) {
            i++;
            j++;
        }
        else if (j < m && p[j] == '*') {
            iStart = i;
            jStart = j++;
        }
        else if (iStart >= 0) {
            i = ++iStart;
            j = jStart + 1;
        }
        else return false;
    }
    while (j < m && p[j] == '*') {
        j++;
    }
    return j == m;
}

void doGrep(int argc, char *argv[]) {
    xing = false;
    dian = false;
    sx = -1;
    wx = -1;
    xingshu = 0;
    shou = -1;
    wei = -1;
    bz = 0;
    s = 0;
    zhyh = 0;
    ppcg = 0;
    Bhang = -1;
    Ahang = -1;
    h = false;
    H = false;
    ijc = false;
    n = false;
    c = false;
    A = false;
    B = false;
    if (argc == 1) {
        cerr << "Usage: grep [OPTION]... PATTERNS [FILE]...\n";
        cerr << "Try 'grep --help' for more information.\n";
        return;
    }
    //判断指令选项是否为help,把help拷贝到strout里
    if (strcmp(argv[1], "--help") == 0) {
        strcat(gTerm.strout, "Usage: grep [OPTION]... PATTERNS [FILE]...\n");
        strcat(gTerm.strout, "Search for PATTERNS in each FILE.\n");
        strcat(gTerm.strout, "Example: grep -i 'hello world' menu.h main.c\n");
        strcat(gTerm.strout, "PATTERNS can contain multiple patterns separated by newlines.\n");
        strcat(gTerm.strout, "Pattern selection and interpretation:\n");
        strcat(gTerm.strout, "-c：计算符合样式的行数\n");
        strcat(gTerm.strout, "-h：在显示符合样式的那一行之前，不标示该行所属的文件名称\n");
        strcat(gTerm.strout, "-H：在显示符合样式的那一行之前，标示该行所属的文件名称\n");
        strcat(gTerm.strout, "-i：忽略字符大小写的差别\n");
        strcat(gTerm.strout, "-n：在显示符合样式的那一行之前，标示出该行的编号\n");
        strcat(gTerm.strout, "-A[行数]：除了显示符合范本样式的那一列之外，同时显示该行后的[行数]行内容\n");
        strcat(gTerm.strout, "-B[行数]：除了显示符合范本样式的那一行之外，同时显示该行前的[行数]行内容\n");
    }
        //如果指令选项非help，则依次判断选项是否存在剩余的七个选项
    else {
        for (int i = 1; i <= argc - 1; i++) {
            if (strcmp(argv[i], "-h") == 0) {
                h = true;
                s++;
            }
            if (strcmp(argv[i], "-H") == 0) {
                H = true;
                s++;
            }
            if (strcmp(argv[i], "-i") == 0) {
                ijc = true;
                s++;
            }
            if (strcmp(argv[i], "-n") == 0) {
                n = true;
                s++;
            }
            if (strcmp(argv[i], "-c") == 0) {
                c = true;
                s++;
            }
            if (argv[i][0] == '-' && argv[i][1] == 'A') {
                char a[5];
                for (int j = 2; argv[i][j] != '\0'; j++) {
                    a[j - 2] = argv[i][j];
                }
                Ahang = atoi(a);
                if (Ahang == 0) {
                    cerr << "grep:" << argv[i - 1] << ":Wrong option or can not find the file\n";
                    cerr << "Usage: grep [OPTION]... PATTERNS [FILE]...\n";
                    cerr << "Try 'grep --help' for more information.\n";
                    return;
                }
                A = true;
                s++;
            }
            if (argv[i][0] == '-' && argv[i][1] == 'B') {
                char b[5];
                for (int j = 2; argv[i][j] != '\0'; j++) {
                    b[j - 2] = argv[i][j];
                }
                Bhang = atoi(b);
                if (Bhang == 0) {
                    cerr << "grep:" << argv[i - 1] << ":Wrong option or can not find the file\n";
                    cerr << "Usage: grep [OPTION]... PATTERNS [FILE]...\n";
                    cerr << "Try 'grep --help' for more information.\n";
                    return;
                }
                B = true;
                s++;
            }
        }
    }
    string msc;
    msc += argv[s + 1];
    for (int i = 0; i <= msc.size() - 1; i++) {
        if (msc[i] == '*') {
            xing = true;
            xingshu++;
        }
        if (msc[i] == '.') {
            dian = true;
        }
    }
    for (int z = s + 2; z <= argc - 1; z++) {
        if (strcmp(argv[z], "-") == 0) {
            cin.getline(line[0].word, 200, '\n');
            line[0].s = line[0].word;
            line[0].hxh = 1;
            zhyh = 2;
        } else {
            string l;
            l = gTerm.root;
            l += gTerm.wdir;
            l += '/';
            l += argv[z];
            char file_a[1000] ;
            strcpy(file_a, l.c_str());
			ifstream fin;
			fin.open(file_a) ; 
            if (fin.good()) {
                for (int i = 0; i <= 200; i++) {
                    if (!fin.eof()) {
                        line[i].hxh = i + 1;
                        fin.getline(line[i].word, 200, '\n');
                        line[i].word[200] = '\0';
                        line[i].s = line[i].word;
                        zhyh = i;
                    } else {
                        fin.close();
                        break;
                    }
                }
            } else {
                cerr << "grep:" << argv[z] << ":Wrong option or can not find the file\n";
                cerr << "Usage: grep [OPTION]... PATTERNS [FILE]...\n";
                cerr << "Try 'grep --help' for more information.\n";
                return;
            }
        }

        if (strcmp(argv[s + 1], "*") != 0) {
            //既没有*也没有.
            if (!xing && !dian) {
                if (!c) {
                    for (int i = 0; i <= zhyh - 2; i++) {
                        if (ijc) {
                            stolower(line[i].wordx, line[i].word);
                            stolower(argv[s + 1], argv[s + 1]);
                            bz = KMP(line[i].wordx, argv[s + 1]);
                        } else {
                            bz = KMP(line[i].word, argv[s + 1]);
                        }
                        if (B) {
                            if (bz != -1) {
                                for (int j = 0; (j <= Bhang) && (i - j >= 0); j++) {
                                    line[i - j].keshuchu = true;
                                }
                            }
                        }
                        if (A) {
                            if (bz != -1) {
                                for (int j = 0; (j <= Ahang) && (i + j <= zhyh - 2); j++) {
                                    line[i + j].keshuchu = true;
                                }
                            }
                        }
                        if (!A && !B && bz != -1) {
                            line[i].keshuchu = true;
                        }
                    }
                    for (int i = 0; i <= zhyh - 2; i++) {
                        if (line[i].keshuchu) {
                            if (ijc) {
                                stolower(line[i].wordx, line[i].word);
                                stolower(argv[s + 1], argv[s + 1]);
                                bz = KMP(line[i].wordx, argv[s + 1]);
                            } else {
                                bz = KMP(line[i].word, argv[s + 1]);
                            }
                            if (bz != -1) {
                                if ((((s + 2) != (argc - 1)) || (H)) && (!h)) {
                                    strcat(gTerm.strout, "\033[35;1m");
                                    if (strcmp(argv[z], "-") == 0) {
                                        strcat(gTerm.strout, "(standard input)");
                                    } else {
                                        strcat(gTerm.strout, argv[z]);
                                    }
                                    strcat(gTerm.strout, "\033[94;1m");
                                    strcat(gTerm.strout, ":");
                                }
                                if (n) {
                                    strcat(gTerm.strout, "\033[92;1m");
                                    sprintf(line[i].g, "%d", line[i].hxh);
                                    strcat(gTerm.strout, line[i].g);
                                    strcat(gTerm.strout, "\033[94;1m:");
                                }
                                strcat(gTerm.strout, "\033[0m");
                                for (int j = 0; j <= bz - 1; j++) {
                                    gTerm.strout[strlen(gTerm.strout)] = line[i].word[j];
                                }
                                strcat(gTerm.strout, "\033[91;1m");
                                for (int j = bz; j <= bz + strlen(argv[s + 1]) - 1; j++) {
                                    gTerm.strout[strlen(gTerm.strout)] = line[i].word[j];
                                }
                                strcat(gTerm.strout, "\033[0m");
                                for (int j = bz + strlen(argv[s + 1]); j <= strlen(line[i].word) - 1; j++) {
                                    gTerm.strout[strlen(gTerm.strout)] = line[i].word[j];
                                }
                                strcat(gTerm.strout, "\033[0m");
                                strcat(gTerm.strout, "\n");
                            } else {
                                if ((((s + 2) != (argc - 1)) || (H)) && (!h)) {
                                    strcat(gTerm.strout, "\033[35;1m");
                                    if (strcmp(argv[z], "-") == 0) {
                                        strcat(gTerm.strout, "(standard input)");
                                    } else {
                                        strcat(gTerm.strout, argv[z]);
                                    }
                                    strcat(gTerm.strout, "\033[94;1m");
                                    strcat(gTerm.strout, ":");
                                }
                                if (n) {
                                    strcat(gTerm.strout, "\033[92;1m");
                                    sprintf(line[i].g, "%d", line[i].hxh);
                                    strcat(gTerm.strout, line[i].g);
                                    strcat(gTerm.strout, "\033[94;1m-");
                                }
                                strcat(gTerm.strout, "\033[0m");
                                strcat(gTerm.strout, line[i].word);
                                strcat(gTerm.strout, "\033[0m");
                                strcat(gTerm.strout, "\n");
                            }
                        }
                    }
                } else {
                    for (int i = 0; i <= zhyh - 2; i++) {
                        if (ijc) {
                            stolower(line[i].wordx, line[i].word);
                            stolower(argv[s + 1], argv[s + 1]);
                            bz = BF(line[i].wordx, argv[s + 1]);
                        } else {
                            bz = BF(line[i].word, argv[s + 1]);
                        }
                        if (bz != -1) {
                            ppcg++;
                        }
                    }
                    if ((((s + 2) != (argc - 1)) || (H)) && (!h)) {
                        strcat(gTerm.strout, "\033[35;1m");
                        if (strcmp(argv[z], "-") == 0) {
                            strcat(gTerm.strout, "(standard input)");
                        } else {
                            strcat(gTerm.strout, argv[z]);
                        }
                        strcat(gTerm.strout, "\033[94;1m");
                        strcat(gTerm.strout, ":");
                    }
                    strcat(gTerm.strout, "\033[0m");
                    char f[10];
                    sprintf(f, "%d", ppcg);
                    strcat(gTerm.strout, f);
                    strcat(gTerm.strout, "\n");
                }
            }
            //没有*有.
            if (!xing && dian) {
                if (!c) {
                    for (int i = 0; i <= zhyh - 2; i++) {
                        if (ijc) {
                            stolower(line[i].wordx, line[i].word);
                            stolower(argv[s + 1], argv[s + 1]);
                            bz = BF(line[i].wordx, argv[s + 1]);
                        } else {
                            bz = BF(line[i].word, argv[s + 1]);
                        }
                        if (A) {
                            if (bz != -1) {
                                for (int j = 0; (j <= Ahang) && (i + j <= zhyh - 2); j++) {
                                    line[i + j].keshuchu = true;
                                }
                            }
                        }
                        if (B) {
                            if (bz != -1) {
                                for (int j = 0; (j <= Bhang) && (i - j >= 0); j++) {
                                    line[i - j].keshuchu = true;
                                }
                            }
                        }
                        if (!A && !B && bz != -1) {
                            line[i].keshuchu = true;
                        }
                    }
                    for (int i = 0; i <= zhyh - 2; i++) {
                        if (line[i].keshuchu) {
                            if (ijc) {
                                stolower(line[i].wordx, line[i].word);
                                stolower(argv[s + 1], argv[s + 1]);
                                bz = BF(line[i].wordx, argv[s + 1]);
                            } else {
                                bz = BF(line[i].word, argv[s + 1]);
                            }
                            if (bz != -1) {
                                if ((((s + 2) != (argc - 1)) || (H)) && (!h)) {
                                    strcat(gTerm.strout, "\033[35;1m");
                                    if (strcmp(argv[z], "-") == 0) {
                                        strcat(gTerm.strout, "(standard input)");
                                    } else {
                                        strcat(gTerm.strout, argv[z]);
                                    }
                                    strcat(gTerm.strout, "\033[94;1m");
                                    strcat(gTerm.strout, ":");
                                }
                                if (n) {
                                    strcat(gTerm.strout, "\033[92;1m");
                                    sprintf(line[i].g, "%d", line[i].hxh);
                                    strcat(gTerm.strout, line[i].g);
                                    strcat(gTerm.strout, "\033[94;1m:");
                                }
                                strcat(gTerm.strout, "\033[0m");
                                for (int j = 0; j <= bz - 1; j++) {
                                    gTerm.strout[strlen(gTerm.strout)] = line[i].word[j];
                                }
                                strcat(gTerm.strout, "\033[91;1m");
                                for (int j = bz; j <= bz + strlen(argv[s + 1]) - 1; j++) {
                                    gTerm.strout[strlen(gTerm.strout)] = line[i].word[j];
                                }
                                strcat(gTerm.strout, "\033[0m");
                                for (int j = bz + strlen(argv[s + 1]); j <= strlen(line[i].word) - 1; j++) {
                                    gTerm.strout[strlen(gTerm.strout)] = line[i].word[j];
                                }
                                strcat(gTerm.strout, "\033[0m");
                                strcat(gTerm.strout, "\n");
                            } else {
                                if ((((s + 2) != (argc - 1)) || (H)) && (!h)) {
                                    strcat(gTerm.strout, "\033[35;1m");
                                    if (strcmp(argv[z], "-") == 0) {
                                        strcat(gTerm.strout, "(standard input)");
                                    } else {
                                        strcat(gTerm.strout, argv[z]);
                                    }
                                    strcat(gTerm.strout, argv[z]);
                                    strcat(gTerm.strout, "\033[94;1m");
                                    strcat(gTerm.strout, ":");
                                }
                                if (n) {
                                    strcat(gTerm.strout, "\033[92;1m");
                                    sprintf(line[i].g, "%d", line[i].hxh);
                                    strcat(gTerm.strout, line[i].g);
                                    strcat(gTerm.strout, "\033[94;1m-");
                                }
                                strcat(gTerm.strout, "\033[0m");
                                strcat(gTerm.strout, line[i].word);
                                strcat(gTerm.strout, "\033[0m");
                                strcat(gTerm.strout, "\n");
                            }
                        }
                    }
                } else {
                    for (int i = 0; i <= zhyh - 2; i++) {
                        if (ijc) {
                            stolower(line[i].wordx, line[i].word);
                            stolower(argv[s + 1], argv[s + 1]);
                            bz = BF(line[i].wordx, argv[s + 1]);
                        } else {
                            bz = BF(line[i].word, argv[s + 1]);
                        }
                        if (bz != -1) {
                            ppcg++;
                        }
                    }
                    if ((((s + 2) != (argc - 1)) || (H)) && (!h)) {
                        strcat(gTerm.strout, "\033[35;1m");
                        if (strcmp(argv[z], "-") == 0) {
                            strcat(gTerm.strout, "(standard input)");
                        } else {
                            strcat(gTerm.strout, argv[z]);
                        }
                        strcat(gTerm.strout, "\033[94;1m");
                        strcat(gTerm.strout, ":");
                    }
                    strcat(gTerm.strout, "\033[0m");
                    char f[10];
                    sprintf(f, "%d", ppcg);
                    strcat(gTerm.strout, f);
                    strcat(gTerm.strout, "\n");
                }
            }
            //有*没有.或者既有*又有.
            if ((xing && !dian) || (xing && dian)) {
                if (!c) {
                    for (int l = 0; l <= zhyh - 2; l++) {
                        string d;
                        if (ijc) {
                            stolower(line[l].wordx, line[l].word);
                            stolower(argv[s + 1], argv[s + 1]);
                            for (int i = 0; i <= strlen(argv[s + 1]) - 1; i++) {
                                if (argv[s + 1][i] == '*') {
                                    sx = i;
                                    break;
                                }
                            }
                            for (int i = strlen(argv[s + 1]) - 1; i >= 0; i--) {
                                if (argv[s + 1][i] == '*') {
                                    wx = i;
                                    break;
                                }
                            }
                            string a;
                            string b;
                            if (argv[s + 1][0] != '*') {
                                for (int i = 0; i <= sx - 1; i++) {
                                    a.push_back(argv[s + 1][i]);
                                }
                            } else {
                                for (int i = 1; i <= sx - 1; i++) {
                                    a.push_back(argv[s + 1][i]);
                                }
                            }
                            if (argv[s + 1][strlen(argv[s + 1]) - 1] != '*') {
                                for (int i = wx + 1; i <= strlen(argv[s + 1]) - 1; i++) {
                                    b.push_back(argv[s + 1][i]);
                                }
                            } else {
                                for (int i = wx + 1; i <= strlen(argv[s + 1]) - 2; i++) {
                                    b.push_back(argv[s + 1][i]);
                                }
                            }
                            shou = BF(line[l].wordx, a);
                            for (int i = 0; i <= 20; i++) {
                                line[i].s = line[i].wordx;
                            }
                            reverse(line[l].s.begin(), line[l].s.end());
                            reverse(b.begin(), b.end());
                            for (int i = 0; i <= 190; i++) {
                                line[l].wordx[i] = line[l].s[i];
                            }
                            wei = line[l].s.size() - BF(line[l].wordx, b) - 1;
                            reverse(line[l].s.begin(), line[l].s.end());
                            for (int i = 0; i <= 190; i++) {
                                line[l].wordx[i] = line[l].s[i];
                            }
                            for (int i = shou; i <= wei; i++) {
                                d.push_back(line[l].s[i]);
                            }
                        } else {
                            for (int i = 0; i <= strlen(argv[s + 1]) - 1; i++) {
                                if (argv[s + 1][i] == '*') {
                                    sx = i;
                                    break;
                                }
                            }
                            for (int i = strlen(argv[s + 1]) - 1; i >= 0; i--) {
                                if (argv[s + 1][i] == '*') {
                                    wx = i;
                                    break;
                                }
                            }
                            string a;
                            string b;
                            if (argv[s + 1][0] != '*') {
                                for (int i = 0; i <= sx - 1; i++) {
                                    a.push_back(argv[s + 1][i]);
                                }
                            } else {
                                for (int i = 1; i <= sx - 1; i++) {
                                    a.push_back(argv[s + 1][i]);
                                }
                            }
                            if (argv[s + 1][strlen(argv[s + 1]) - 1] != '*') {
                                for (int i = wx + 1; i <= strlen(argv[s + 1]) - 1; i++) {
                                    b.push_back(argv[s + 1][i]);
                                }
                            } else {
                                for (int i = wx + 1; i <= strlen(argv[s + 1]) - 2; i++) {
                                    b.push_back(argv[s + 1][i]);
                                }
                            }
                            shou = BF(line[l].word, a);
                            reverse(line[l].s.begin(), line[l].s.end());
                            reverse(b.begin(), b.end());
                            for (int i = 0; i <= 190; i++) {
                                line[l].word[i] = line[l].s[i];
                            }
                            wei = line[l].s.size() - BF(line[l].word, b) - 1;
                            reverse(line[l].s.begin(), line[l].s.end());
                            for (int i = 0; i <= 190; i++) {
                                line[l].word[i] = line[l].s[i];
                            }
                            for (int i = shou; i <= wei; i++) {
                                d.push_back(line[l].s[i]);
                            }
                        }
                        if (A) {
                            if (Match(d, argv[s + 1])) {
                                for (int j = 0; (j <= Ahang) && (l + j <= zhyh - 2); j++) {
                                    line[l + j].keshuchu = true;
                                }
                            }
                        }
                        if (B) {
                            if (Match(d, argv[s + 1])) {
                                for (int j = 0; (j <= Bhang) && (l - j >= 0); j++) {
                                    line[l - j].keshuchu = true;
                                }
                            }
                        }
                        if (!A && !B && Match(d, argv[s + 1])) {
                            line[l].keshuchu = true;
                        }
                    }
                    for (int l = 0; l <= zhyh - 2; l++) {
                        if (line[l].keshuchu) {
                            string d;
                            if (ijc) {
                                stolower(line[l].wordx, line[l].word);
                                stolower(argv[s + 1], argv[s + 1]);
                                for (int i = 0; i <= strlen(argv[s + 1]) - 1; i++) {
                                    if (argv[s + 1][i] == '*') {
                                        sx = i;
                                        break;
                                    }
                                }
                                for (int i = strlen(argv[s + 1]) - 1; i >= 0; i--) {
                                    if (argv[s + 1][i] == '*') {
                                        wx = i;
                                        break;
                                    }
                                }
                                string a;
                                string b;
                                if (argv[s + 1][0] != '*') {
                                    for (int i = 0; i <= sx - 1; i++) {
                                        a.push_back(argv[s + 1][i]);
                                    }
                                } else {
                                    for (int i = 1; i <= sx - 1; i++) {
                                        a.push_back(argv[s + 1][i]);
                                    }
                                }
                                if (argv[s + 1][strlen(argv[s + 1]) - 1] != '*') {
                                    for (int i = wx + 1; i <= strlen(argv[s + 1]) - 1; i++) {
                                        b.push_back(argv[s + 1][i]);
                                    }
                                } else {
                                    for (int i = wx + 1; i <= strlen(argv[s + 1]) - 2; i++) {
                                        b.push_back(argv[s + 1][i]);
                                    }
                                }
                                shou = BF(line[l].wordx, a);
                                for (int i = 0; i <= 20; i++) {
                                    line[i].s = line[i].wordx;
                                }
                                reverse(line[l].s.begin(), line[l].s.end());
                                reverse(b.begin(), b.end());
                                for (int i = 0; i <= 190; i++) {
                                    line[l].wordx[i] = line[l].s[i];
                                }
                                wei = line[l].s.size() - BF(line[l].wordx, b) - 1;
                                reverse(line[l].s.begin(), line[l].s.end());
                                for (int i = 0; i <= 190; i++) {
                                    line[l].wordx[i] = line[l].s[i];
                                }
                                for (int i = shou; i <= wei; i++) {
                                    d.push_back(line[l].s[i]);
                                }
                            } else {
                                for (int i = 0; i <= strlen(argv[s + 1]) - 1; i++) {
                                    if (argv[s + 1][i] == '*') {
                                        sx = i;
                                        break;
                                    }
                                }
                                for (int i = strlen(argv[s + 1]) - 1; i >= 0; i--) {
                                    if (argv[s + 1][i] == '*') {
                                        wx = i;
                                        break;
                                    }
                                }
                                string a;
                                string b;
                                if (argv[s + 1][0] != '*') {
                                    for (int i = 0; i <= sx - 1; i++) {
                                        a.push_back(argv[s + 1][i]);
                                    }
                                } else {
                                    for (int i = 1; i <= sx - 1; i++) {
                                        a.push_back(argv[s + 1][i]);
                                    }
                                }
                                if (argv[s + 1][strlen(argv[s + 1]) - 1] != '*') {
                                    for (int i = wx + 1; i <= strlen(argv[s + 1]) - 1; i++) {
                                        b.push_back(argv[s + 1][i]);
                                    }
                                } else {
                                    for (int i = wx + 1; i <= strlen(argv[s + 1]) - 2; i++) {
                                        b.push_back(argv[s + 1][i]);
                                    }
                                }
                                shou = BF(line[l].word, a);
                                reverse(line[l].s.begin(), line[l].s.end());
                                reverse(b.begin(), b.end());
                                for (int i = 0; i <= 190; i++) {
                                    line[l].word[i] = line[l].s[i];
                                }
                                wei = line[l].s.size() - BF(line[l].word, b) - 1;
                                reverse(line[l].s.begin(), line[l].s.end());
                                for (int i = 0; i <= 190; i++) {
                                    line[l].word[i] = line[l].s[i];
                                }
                                for (int i = shou; i <= wei; i++) {
                                    d.push_back(line[l].s[i]);
                                }
                            }
                            if (Match(d, argv[s + 1])) {
                                if ((((s + 2) != (argc - 1)) || (H)) && (!h)) {
                                    strcat(gTerm.strout, "\033[35;1m");
                                    if (strcmp(argv[z], "-") == 0) {
                                        strcat(gTerm.strout, "(standard input)");
                                    } else {
                                        strcat(gTerm.strout, argv[z]);
                                    }
                                    strcat(gTerm.strout, "\033[94;1m");
                                    strcat(gTerm.strout, ":");
                                }
                                if (n) {
                                    strcat(gTerm.strout, "\033[92;1m");
                                    sprintf(line[l].g, "%d", line[l].hxh);
                                    strcat(gTerm.strout, line[l].g);
                                    strcat(gTerm.strout, "\033[94;1m:");
                                }
                                if (argv[s + 1][0] != '*' && argv[s + 1][strlen(argv[s + 1]) - 1] != '*') {
                                    strcat(gTerm.strout, "\033[0m");
                                    for (int i = 0; i <= shou - 1; i++) {
                                        gTerm.strout[strlen(gTerm.strout)] = line[l].word[i];
                                    }
                                    strcat(gTerm.strout, "\033[91;1m");
                                    for (int i = shou; i <= wei; i++) {
                                        gTerm.strout[strlen(gTerm.strout)] = line[l].word[i];
                                    }
                                    strcat(gTerm.strout, "\033[0m");
                                    for (int i = wei + 1; i <= line[l].s.size() - 1; i++) {
                                        gTerm.strout[strlen(gTerm.strout)] = line[l].word[i];
                                    }
                                    strcat(gTerm.strout, "\033[0m");
                                    strcat(gTerm.strout, "\n");
                                }
                                if (argv[s + 1][0] == '*' && argv[s + 1][strlen(argv[s + 1]) - 1] != '*' &&
                                    xingshu != 1) {
                                    strcat(gTerm.strout, "\033[91;1m");
                                    for (int i = 0; i <= wei; i++) {
                                        gTerm.strout[strlen(gTerm.strout)] = line[l].word[i];
                                    }
                                    strcat(gTerm.strout, "\033[0m");
                                    for (int i = wei + 1; i <= line[l].s.size() - 1; i++) {
                                        gTerm.strout[strlen(gTerm.strout)] = line[l].word[i];
                                    }
                                    strcat(gTerm.strout, "\033[0m");
                                    strcat(gTerm.strout, "\n");
                                }
                                if (argv[s + 1][0] != '*' && argv[s + 1][strlen(argv[s + 1]) - 1] == '*' &&
                                    xingshu != 1) {
                                    strcat(gTerm.strout, "\033[0m");
                                    for (int i = 0; i <= shou - 1; i++) {
                                        gTerm.strout[strlen(gTerm.strout)] = line[l].word[i];
                                    }
                                    strcat(gTerm.strout, "\033[91;1m");
                                    for (int i = shou; i <= line[l].s.size() - 1; i++) {
                                        gTerm.strout[strlen(gTerm.strout)] = line[l].word[i];
                                    }
                                    strcat(gTerm.strout, "\033[0m");
                                    strcat(gTerm.strout, "\n");
                                }
                                if (argv[s + 1][0] == '*' && argv[s + 1][strlen(argv[s + 1]) - 1] == '*') {
                                    strcat(gTerm.strout, "\033[91;1m");
                                    for (int i = 0; i <= line[l].s.size() - 1; i++) {
                                        gTerm.strout[strlen(gTerm.strout)] = line[l].word[i];
                                    }
                                    strcat(gTerm.strout, "\033[0m");
                                    strcat(gTerm.strout, "\n");
                                }
                                if (argv[s + 1][0] == '*' && argv[s + 1][strlen(argv[s + 1]) - 1] != '*' &&
                                    xingshu == 1) {
                                    string d;
                                    for (int i = 1; i <= strlen(argv[s + 1]) - 1; i++) {
                                        d.push_back(argv[s + 1][i]);
                                    }
                                    reverse(line[l].s.begin(), line[l].s.end());
                                    reverse(d.begin(), d.end());
                                    for (int i = 0; i <= 20; i++) {
                                        line[l].wordx[i] = line[l].s[i];
                                    }
                                    int q = BF(line[l].wordx, d);
                                    reverse(line[l].s.begin(), line[l].s.end());
                                    for (int i = 0; i <= 20; i++) {
                                        line[l].wordx[i] = line[l].s[i];
                                    }
                                    if (q != -1) {
                                        strcat(gTerm.strout, "\033[91;1m");
                                        for (int i = 0; i <= line[l].s.size() - q - 1; i++) {
                                            gTerm.strout[strlen(gTerm.strout)] = line[l].word[i];
                                        }
                                        strcat(gTerm.strout, "\033[0m");
                                        for (int i = line[l].s.size() - q; i <= line[l].s.size() - 1; i++) {
                                            gTerm.strout[strlen(gTerm.strout)] = line[l].word[i];
                                        }
                                        strcat(gTerm.strout, "\033[0m");
                                        strcat(gTerm.strout, "\n");
                                    }
                                }
                                if (argv[s + 1][0] != '*' && argv[s + 1][strlen(argv[s + 1]) - 1] == '*' &&
                                    xingshu == 1) {
                                    string d;
                                    for (int i = 0; i <= strlen(argv[s + 1]) - 2; i++) {
                                        d.push_back(argv[s + 1][i]);
                                    }
                                    int q = BF(line[l].wordx, d);
                                    if (q != -1) {
                                        strcat(gTerm.strout, "\033[0m");
                                        for (int i = 0; i <= q - 1; i++) {
                                            gTerm.strout[strlen(gTerm.strout)] = line[l].word[i];
                                        }
                                        strcat(gTerm.strout, "\033[91;1m");
                                        for (int i = q; i <= line[l].s.size() - 1; i++) {
                                            gTerm.strout[strlen(gTerm.strout)] = line[l].word[i];
                                        }
                                        strcat(gTerm.strout, "\033[0m");
                                        strcat(gTerm.strout, "\n");
                                    }
                                }
                            } else {
                                if ((((s + 2) != (argc - 1)) || (H)) && (!h)) {
                                    strcat(gTerm.strout, "\033[35;1m");
                                    if (strcmp(argv[z], "-") == 0) {
                                        strcat(gTerm.strout, "(standard input)");
                                    } else {
                                        strcat(gTerm.strout, argv[z]);
                                    }
                                    strcat(gTerm.strout, "\033[94;1m");
                                    strcat(gTerm.strout, ":");
                                }
                                if (n) {
                                    strcat(gTerm.strout, "\033[92;1m");
                                    sprintf(line[l].g, "%d", line[l].hxh);
                                    strcat(gTerm.strout, line[l].g);
                                    strcat(gTerm.strout, "\033[94;1m-");
                                }
                                strcat(gTerm.strout, "\033[0m");
                                strcat(gTerm.strout, line[l].word);
                                strcat(gTerm.strout, "\033[0m");
                                strcat(gTerm.strout, "\n");
                            }
                        }
                    }
                } else {
                    for (int l = 0; l <= zhyh - 2; l++) {
                        string d;
                        if (ijc) {
                            stolower(line[l].wordx, line[l].word);
                            stolower(argv[s + 1], argv[s + 1]);
                            for (int i = 0; i <= strlen(argv[s + 1]) - 1; i++) {
                                if (argv[s + 1][i] == '*') {
                                    sx = i;
                                    break;
                                }
                            }
                            for (int i = strlen(argv[s + 1]) - 1; i >= 0; i--) {
                                if (argv[s + 1][i] == '*') {
                                    wx = i;
                                    break;
                                }
                            }
                            string a;
                            string b;
                            if (argv[s + 1][0] != '*') {
                                for (int i = 0; i <= sx - 1; i++) {
                                    a.push_back(argv[s + 1][i]);
                                }
                            } else {
                                for (int i = 1; i <= sx - 1; i++) {
                                    a.push_back(argv[s + 1][i]);
                                }
                            }
                            if (argv[s + 1][strlen(argv[s + 1]) - 1] != '*') {
                                for (int i = wx + 1; i <= strlen(argv[s + 1]) - 1; i++) {
                                    b.push_back(argv[s + 1][i]);
                                }
                            } else {
                                for (int i = wx + 1; i <= strlen(argv[s + 1]) - 2; i++) {
                                    b.push_back(argv[s + 1][i]);
                                }
                            }
                            shou = BF(line[l].wordx, a);
                            for (int i = 0; i <= 20; i++) {
                                line[i].s = line[i].wordx;
                            }
                            reverse(line[l].s.begin(), line[l].s.end());
                            reverse(b.begin(), b.end());
                            for (int i = 0; i <= 190; i++) {
                                line[l].wordx[i] = line[l].s[i];
                            }
                            wei = line[l].s.size() - BF(line[l].wordx, b) - 1;
                            reverse(line[l].s.begin(), line[l].s.end());
                            for (int i = 0; i <= 190; i++) {
                                line[l].wordx[i] = line[l].s[i];
                            }
                            for (int i = shou; i <= wei; i++) {
                                d.push_back(line[l].s[i]);
                            }
                        } else {
                            for (int i = 0; i <= strlen(argv[s + 1]) - 1; i++) {
                                if (argv[s + 1][i] == '*') {
                                    sx = i;
                                    break;
                                }
                            }
                            for (int i = strlen(argv[s + 1]) - 1; i >= 0; i--) {
                                if (argv[s + 1][i] == '*') {
                                    wx = i;
                                    break;
                                }
                            }
                            string a;
                            string b;
                            if (argv[s + 1][0] != '*') {
                                for (int i = 0; i <= sx - 1; i++) {
                                    a.push_back(argv[s + 1][i]);
                                }
                            } else {
                                for (int i = 1; i <= sx - 1; i++) {
                                    a.push_back(argv[s + 1][i]);
                                }
                            }
                            if (argv[s + 1][strlen(argv[s + 1]) - 1] != '*') {
                                for (int i = wx + 1; i <= strlen(argv[s + 1]) - 1; i++) {
                                    b.push_back(argv[s + 1][i]);
                                }
                            } else {
                                for (int i = wx + 1; i <= strlen(argv[s + 1]) - 2; i++) {
                                    b.push_back(argv[s + 1][i]);
                                }
                            }
                            shou = BF(line[l].word, a);
                            reverse(line[l].s.begin(), line[l].s.end());
                            reverse(b.begin(), b.end());
                            for (int i = 0; i <= 190; i++) {
                                line[l].word[i] = line[l].s[i];
                            }
                            wei = line[l].s.size() - BF(line[l].word, b) - 1;
                            reverse(line[l].s.begin(), line[l].s.end());
                            for (int i = 0; i <= 190; i++) {
                                line[l].word[i] = line[l].s[i];
                            }
                            for (int i = shou; i <= wei; i++) {
                                d.push_back(line[l].s[i]);
                            }
                        }
                        if (Match(d, argv[s + 1])) {
                            ppcg++;
                        }
                    }
                    if ((((s + 2) != (argc - 1)) || (H)) && (!h)) {
                        strcat(gTerm.strout, "\033[35;1m");
                        if (strcmp(argv[z], "-") == 0) {
                            strcat(gTerm.strout, "(standard input)");
                        } else {
                            strcat(gTerm.strout, argv[z]);
                        }
                        strcat(gTerm.strout, "\033[94;1m");
                        strcat(gTerm.strout, ":");
                    }
                    strcat(gTerm.strout, "\033[0m");
                    char f[10];
                    sprintf(f, "%d", ppcg);
                    strcat(gTerm.strout, f);
                    strcat(gTerm.strout, "\n");
                }
            }
            //模式串为*时，直接输出整行
            if (strcmp(argv[s + 1], "*") == 0) {
                if (!c) {
                    for (int i = 0; i <= zhyh - 2; i++) {
                        if ((((s + 2) != (argc - 1)) || (H)) && (!h)) {
                            strcat(gTerm.strout, "\033[35;1m");
                            if (strcmp(argv[z], "-") == 0) {
                                strcat(gTerm.strout, "(standard input)");
                            } else {
                                strcat(gTerm.strout, argv[z]);
                            }
                            strcat(gTerm.strout, "\033[94;1m");
                            strcat(gTerm.strout, ":");
                        }
                        if (n) {
                            strcat(gTerm.strout, "\033[92;1m");
                            sprintf(line[i].g, "%d", line[i].hxh);
                            strcat(gTerm.strout, line[i].g);
                            strcat(gTerm.strout, "\033[94;1m:");
                        }
                        strcat(gTerm.strout, "\033[91;1m");
                        strcat(gTerm.strout, line[i].word);
                        strcat(gTerm.strout, "\033[0m");
                        strcat(gTerm.strout, "\n");
                    }
                } else {
                    if ((((s + 2) != (argc - 1)) || (H)) && (!h)) {
                        strcat(gTerm.strout, "\033[35;1m");
                        if (strcmp(argv[z], "-") == 0) {
                            strcat(gTerm.strout, "(standard input)");
                        } else {
                            strcat(gTerm.strout, argv[z]);
                        }
                        strcat(gTerm.strout, "\033[94;1m");
                        strcat(gTerm.strout, ":");
                    }
                    strcat(gTerm.strout, "\033[0m");
                    char f[10];
                    sprintf(f, "%d", zhyh - 1);
                    strcat(gTerm.strout, f);
                    strcat(gTerm.strout, "\n");
                }
            }
            for (int i = 0; i <= 200; i++) {
                line[i].keshuchu = false;
            }
        }
    }
}

