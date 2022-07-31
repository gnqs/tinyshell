#pragma once
struct Opt {
	int op, x, y;
	Opt() {}
	Opt(int _op, int _x, int _y) {
		op = _op, x = _x, y = _y;
	}
};
struct Range {
	int l1, r1, l2, r2, op;
	Range() {}
	Range(int _l1, int _r1, int _l2, int _r2, int _op) {
		l1 = _l1, r1 = _r1, l2 = _l2, r2 = _r2, op = _op;
	}
};

istrstream strin(gTerm.strin, MAXFILE);

void GetHelp() {
	strcat(gTerm.strout, "Usage: diff [OPTION]... FILES\nCompare FILES line by line.\n");
	strcat(gTerm.strout, "-q                   report only when files differ\n");
	strcat(gTerm.strout, "-i                   ignore case differences in file contents\n");
	strcat(gTerm.strout, "-b                   ignore changes in the amount of white space\n");
	strcat(gTerm.strout, "-w                   ignore all white space\n");
	strcat(gTerm.strout, "-B                   ignore changes where lines are all blank\n");
	strcat(gTerm.strout, "-I[RE]               ignore changes where all lines match RE (without brackets)\n");
}

bool is_upper(char a) {
	return 'A' <= a && a <= 'Z';
}
// convert upper-case letters to lower-case letters (-i)
char convert(char x) {
	if (is_upper(x)) return 'a' + (x - 'A');
	return x;
}
string tostr(int x) {
	string str = "";
	while (x) {
		str += ('0' + x % 10);
		x /= 10;
	}
	reverse(str.begin(), str.end());
	return str;
} 
// check if two strings are equal
bool CMP(string A, string B, bool fgw, bool fgi, bool fgb) {
	string a, b;
	bool fg_space = false;
	for (int i = 0; i < (int)A.size(); i++) {
		if (A[i] == ' ') {
			if (fgw || fg_space) continue;
			if (fgb) fg_space = true;
		}
		else if (fgb) {
			fg_space = false;
		}
		if (fgi) {
			a.push_back(convert(A[i]));
		}
		else {
			a.push_back(A[i]);
		}
	}
	fg_space = false;
	for (int i = 0; i < (int)B.size(); i++) {
		if (B[i] == ' ') {
			if (fgw || fg_space) continue;
			if (fgb) fg_space = true;
		}
		else if (fgb) {
			fg_space = false;
		}
		if (fgi) {
			b.push_back(convert(B[i]));
		}
		else {
			b.push_back(B[i]);
		}
	}
	return a == b;
}

// main function
void doDiff(int argc, char * argv[]) {
	bool flagb = false, flagB = false, flagi = false, flagq = false, flagw = false, flagI = false;
	// reading inputs and checking if the inputs are legal
	if (strcmp(argv[0], "diff")) {
		cerr << "Error: Wrong Function!\n";
		return;
	}

	if (argc == 1) {
		cerr << "diff: missing operand after 'diff'\n";
		return;
	}

	if (!strcmp(argv[1], "--help")) {
		GetHelp();
		return;
	}

	vector<string> I;
	string All_Options = "iBqwIb";
	bool nxtflagI = false, fg_A = false, fg_B = false;
	char FileA[MAXLINE];
	char FileB[MAXLINE];
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-B")) {
			flagB = true;
			continue;
		}
		if (!strcmp(argv[i], "-i")) {
			flagi = true;
			continue;
		}
		if (!strcmp(argv[i], "-q")) {
			flagq = true;
			continue;
		}
		if (!strcmp(argv[i], "-w")) {
			flagw = true;
			continue;
		}
		if (!strcmp(argv[i], "-b")) {
			flagb = true;
			continue;
		}
		if (argv[i][0] == '-' && argv[i][1] == 'I') {
			flagI = true;
			if (strlen(argv[i]) == 2) {
				nxtflagI = true;
			}
			else {
				string tmp;
				for (int j = 2; j < strlen(argv[i]); j++) {
					tmp.push_back(argv[i][j]);
				}
				I.push_back(tmp);
			}
			continue;
		}
		if (nxtflagI) {
			string tmp;
			for (int j = 0; j < strlen(argv[i]); j++) {
				tmp.push_back(argv[i][j]);
			}
			I.push_back(tmp);
			nxtflagI = false;
			continue;
		}
		if (argv[i][0] == '-' && strlen(argv[i]) != 1) {
			if (All_Options.find(argv[i][1]) == string::npos) {
				cerr << "diff: invalid option -- '" << argv[i][1] << "'" << endl;
			}
			else {
				cerr << "diff: invalid option -- '" << argv[i][2] << "'" << endl;
			}
			cerr << "diff: Try 'diff --help' for more information.\n";
			return;
		}
		else {
			if (!fg_A) {
				//FileA = argv[i];
				strcpy(FileA, argv[i]);
				fg_A = true;
			}
			else if (!fg_B) {
				//FileB = argv[i];
				strcpy(FileB, argv[i]);
				fg_B = true;
			}
			else {
				cerr << "diff: extra operand '" << argv[i] << "'" << endl;
				cerr << "diff: Try 'diff --help' for more information.\n";
				return;
			}
		}
	}

	if (!fg_A) {
		cerr << "diff: missing operand after '" << argv[argc - 1] << "'\n";
		return;
	}
	else if (!fg_B) {
		cerr << "diff: missing operand after '" << FileA << "'\n";
		return;
	}

	fstream _file;
	string tmp_str;
	vector<string> A, B;
	A.push_back("");
	B.push_back("");
	char slash[2];
	slash[0] = '/';
	slash[1] = '\0';
	if (strcmp(FileA, "-")) {
		char path[MAXFILE] = "";
		strcat(path, gTerm.root);
		if (FileA[0] != '/') {
			strcat(path, gTerm.wdir);
			if (strcmp(gTerm.wdir, "/")) strcat(path, slash);
		}
		strcat(path, FileA);
		_file.open(path, ios::in);
		if (!_file) {
			//cerr << "diff: " << path << ": No such file or directory";
			cerr << "diff: " << FileA << ": No such file or directory\n";
			return;
		}
		_file.close();
		ifstream Ain(path);
		while (!Ain.eof()) {
			getline(Ain, tmp_str);
			A.push_back(tmp_str);
		}
		Ain.close();
	}
	else {
		while (getline(strin, tmp_str)) {
			A.push_back(tmp_str);
		}
	}
	if (strcmp(FileB, "-")) {
		char path[MAXFILE] = "";
		strcat(path, gTerm.root);
		if (FileB[0] != '/') {
			strcat(path, gTerm.wdir);
			if (strcmp(gTerm.wdir, "/")) strcat(path, slash);
		}
		strcat(path, FileB);
		_file.open(path, ios::in);
		if (!_file) {
			cerr << "diff: " << FileB << ": No such file or directory\n";
			return;
		}
		_file.close();
		ifstream Bin(path);
		while (!Bin.eof()) {
			getline(Bin, tmp_str);
			B.push_back(tmp_str);
		}
		Bin.close();
	}
	else {
		while (getline(strin, tmp_str)) {
			B.push_back(tmp_str);
		}
	}
	int len1 = (int)A.size() - 1;
	int len2 = (int)B.size() - 1;
	int f[110][110];
	Opt pre[110][110];
	f[0][0] = 0;
	for (int i = 1; i <= len2; i++) {
		f[0][i] = i;
		pre[0][i] = Opt(2, 0, 0);
	}
	for (int i = 1; i <= len1; i++) {
		f[i][0] = i;
		pre[i][0] = Opt(1, 0, 0);
	}
	bool fg_empty = false;
	for (int i = 1; i <= len1; i++) {
		if (flagB && A[i] == "") fg_empty = true;
		for (int j = 1; j <= len2; j++) {
			if (fg_empty) {
				f[i][j] = f[i - 1][j];
				pre[i][j] = Opt(-1, i - 1, j);
				continue;
			}
			if (flagB && B[j] == "") {
				f[i][j] = f[i][j - 1];
				pre[i][j] = Opt(-1, i, j - 1);
				continue;
			}
			if (CMP(A[i], B[j], flagw, flagi, flagb)) {
				f[i][j] = f[i - 1][j - 1];
				pre[i][j] = Opt(-1, i - 1, j - 1);
			}
			else {
				f[i][j] = f[i - 1][j - 1] + 1;
				pre[i][j] = Opt(0, i - 1, j - 1);
				if (f[i - 1][j] + 1 <= f[i][j]) {
					f[i][j] = f[i - 1][j] + 1;
					pre[i][j] = Opt(1, i - 1, j);
				}
				if (f[i][j - 1] + 1 <= f[i][j]) {
					f[i][j] = f[i][j - 1] + 1;
					pre[i][j] = Opt(2, i, j - 1);
				}
			}
		}
		fg_empty = false;
	}

	if (!f[len1][len2]) {
		strcat(gTerm.strout, "No difference\n");
		return;
	}
	if (flagq) {
		if (f[len1][len2]) {
			strcat(gTerm.strout, "File ");
			strcat(gTerm.strout, FileA);
			strcat(gTerm.strout, " and ");
			strcat(gTerm.strout, FileB);
			strcat(gTerm.strout, " differ\n");
		}
		return;
	}

	stack<Opt> S;
	int nowx = len1, nowy = len2;
	S.push(Opt(-1, len1, len2));
	while (nowx || nowy) {
		Opt tmp = pre[nowx][nowy];
		int x = tmp.x, y = tmp.y, op = tmp.op;
		S.push(tmp);
		nowx = x, nowy = y;
	}
	int stat = -1, l1 = -1, r1 = -1, l2 = -1, r2 = -1;
	nowx = 0, nowy = 0;
	vector<Range> Q;
	while (!S.empty()) {
		Opt now = S.top();
		S.pop();
		int x = now.x, y = now.y, op = now.op;
		if (op != stat) {
			if (stat == 0) {
				r1 = x, r2 = y;
				l1++, l2++;
			}
			else if (stat == 2) {
				r2 = y; l2++;
			}
			else if (stat == 1) {
				r1 = x; l1++;
			}
			if (stat != -1) Q.push_back(Range(l1, r1, l2, r2, stat));
			l1 = r1 = x, l2 = r2 = y;
		}
		stat = op;
	}
	if (flagI) {
		for (int r = 0; r < (int)I.size(); r++) {
			bool legal = true;
			for (int i = 1; i < (int)Q.size(); i++) {
				Range now = Q[i];
				int l1 = now.l1, r1 = now.r1, l2 = now.l2, r2 = now.r2, op = now.op;
				if (op == 0) {
					for (int j = l1; j <= r1; j++) {
						if (A[j].find(I[r]) == string::npos) {
							legal = false;
							break;
						}
					}
					for (int j = l2; j <= r2; j++) {
						if (B[j].find(I[r]) == string::npos) {
							legal = false;
							break;
						}
					}
				}
				else if (op == 1) {
					for (int j = l1; j <= r1; j++) {
						if (A[j].find(I[r]) == string::npos) {
							legal = false;
							break;
						}
					}
				}
				else if (op == 2) {
					for (int j = l2; j <= r2; j++) {
						if (B[j].find(I[r]) == string::npos) {
							legal = false;
							break;
						}
					}
				}
				if (!legal) break;
			}
			if (legal) {
				strcat(gTerm.strout, "No difference\n");
				return;
			}
		}
	}
	int tot = 0;
	for (int i = 1; i < (int)Q.size(); i++) {
		Range now = Q[i];
		if (Q[i].l1 == Q[i - 1].r1 + 1 && (Q[i].op != 2 || Q[i - 1].op != 2)) {
			Q[tot].op = 0;
			Q[tot].r2 = r2;
			Q[tot].r1 = r1;
		}
		else {
			Q[++tot] = Q[i];
		}
	}
	for (int i = 0; i <= tot; i++) {
		Range now = Q[i];
		int l1 = now.l1, r1 = now.r1, l2 = now.l2, r2 = now.r2, op = now.op;
		if (op == 0) {
			if (l1 == r1) {
				//strout << l1;
				strcat(gTerm.strout, tostr(l1).data());
			}
			else {
				//strout << l1 << "," << r1;
				strcat(gTerm.strout, tostr(l1).data());
				strcat(gTerm.strout, ",");
				strcat(gTerm.strout, tostr(r1).data());
			}
			//strout << "c";
			strcat(gTerm.strout, "c");
			if (l2 == r2) {
				//strout << l2;
				strcat(gTerm.strout, tostr(l2).data());
			}
			else {
				//strout << l2 << "," << r2;
				strcat(gTerm.strout, tostr(l2).data());
				strcat(gTerm.strout, ",");
				strcat(gTerm.strout, tostr(r2).data());
			}
			//strout << endl;
			strcat(gTerm.strout, "\n");
			for (int j = l1; j <= r1; j++) {
				//strout << "< " << A[j] << endl;
				strcat(gTerm.strout, "< ");
				strcat(gTerm.strout, A[j].data());
				strcat(gTerm.strout, "\n");
			}
			//strout << "---" << endl;
			strcat(gTerm.strout, "---\n");
			for (int j = l2; j <= r2; j++) {
				//strout << "> " << B[j] << endl;
				strcat(gTerm.strout, "> ");
				strcat(gTerm.strout, B[j].data());
			}
		}
		else if (op == 2) {
			//strout << l1 << "a";
			strcat(gTerm.strout, tostr(l1).data());
			strcat(gTerm.strout, "a");
			if (l2 == r2) {
				//strout << l2;
				strcat(gTerm.strout, tostr(l2).data());
			}
			else {
				//strout << l2 << "," << r2;
				strcat(gTerm.strout, tostr(l2).data());
				strcat(gTerm.strout, ",");
				strcat(gTerm.strout, tostr(r2).data());
			}
			//strout << endl;
			strcat(gTerm.strout, "\n");
			for (int j = l2; j <= r2; j++) {
				//strout << "> " << B[j] << endl;
				strcat(gTerm.strout, "> ");
				strcat(gTerm.strout, B[j].data());
				strcat(gTerm.strout, "\n");
			}
		}
		else if (op == 1) {
			if (l1 == r1) {
				//strout << l1;
				strcat(gTerm.strout, tostr(l1).data());
			}
			else {
				//strout << l1 << "," << r1;
				strcat(gTerm.strout, tostr(l1).data());
				strcat(gTerm.strout, ",");
				strcat(gTerm.strout, tostr(r1).data());
			}
			//strout << "d" << l2 << endl;
			strcat(gTerm.strout, "d");
			strcat(gTerm.strout, tostr(l2).data());
			strcat(gTerm.strout, "\n");
			for (int j = l1; j <= r1; j++) {
				//strout << "< " << A[j] << endl;
				strcat(gTerm.strout, "< ");
				strcat(gTerm.strout, A[j].data());
				strcat(gTerm.strout, "\n");
			}
		}
	}
}

