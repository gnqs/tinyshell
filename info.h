//存放命令的 --help 功能
#ifndef INFO_H
#define INFO_H

const char HELP_TEE[] = { \
"Usage: tee {OPTION}* [FILE]*									\n\
		tee --help												\n\
Copy standard input to each FILE, and also to standard output.	\n\
																\n\
- a,		append to the given FILEs, do not overwrite			\n\
--help		display this help and exit							\n\
"
};
const char HELP_CAT[] = { \
"Usage: cat {OPTION}* [FILE]*							\n\
		cat --help										\n\
Concatenate FILE(s) to standard output.					\n\
When FILE is - , read standard input.					\n\
														\n\
- b,		number nonempty output lines				\n\
- E,		display $ at end of each line				\n\
- n, 		number all output lines						\n\
- s,		suppress repeated empty output lines		\n\
--help		display this help and exit					\n\
"
};
const char HELP_CP[] = { \
"Usage: cp {OPTION} [SOURCE] [DEST]						\n\
		cp --help										\n\
Copy SOURCE to DEST, or multiple SOURCE(s) to DIRECTORY.\n\
When DEST is - , write standard output					\n\
When SOURCE is - , read standard input					\n\
														\n\
- n,		do not overwrite an existing file			\n\
--help		display this help and exit					\n\
"
};
const char HELP_CD[] = { \
"Usage: cd [dir]														 \n\
		cd --help														 \n\
Change the shell working directory to dir								 \n\
																		 \n\
--help		display this help and exit									 \n\
'..' is processed by removing the immediately previous pathname component\n\
back to a slash or the beginning of DIR.								 \n\
"
};
const char HELP_PWD[] = { \
"Usage: pwd										\n\
		pwd --help								\n\
Print the name of the current working directory.\n\
--help		display this help and exit			\n\
"
};
#endif // 

