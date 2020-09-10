#define _CRT_SECURE_NO_WARNINGS

/*
 * PL/0 编译器
 *
 * 使用方法：
 * 运行后输入PL/0源程序文件?
 * 回答是否输出虚拟机代码
 * 回答是否输出名字表
 * fa.tmp输出虚拟机代码
 * fa1.tmp输出源文件及其各行对应的首地址
 * fa2.tmp输出结果
 * fas.tmp输出名字表
 */

#include "pl0.h"
#include "string.h"

 //运行前初始化，对保留字表 (word)、保留字表中每一个保留字对应的 symbol 类型 ( wsym )、
 //部分符号对应的 symbol 类型表 ( ssym )、类 PCODE 指令助记符表 ( mnemonic )、
 //声明开始集合 ( declbegsys )、表达式开始集合 ( statbegsys )、
 //项开始符号集合 ( facbegsys ) 以及一些全局变量的初始化
void init()
{
	int i;
	// ASCII 范围(0–31 控制字符, 32–126 分配给了能在键盘上找到的字符
	//数字 127 代表 DELETE 命令, 后 128 个是扩展 ASCII 打印字符) 因此共 256 个
	/* 设置单字符符号 */
	for (i = 0; i <= 255; i++)
	{
		ssym[i] = nul;
	}
	ssym['+'] = plus;
	ssym['-'] = minus;
	ssym['*'] = times;
	ssym['/'] = slash;
	ssym['('] = lparen;
	ssym[')'] = rparen;
	ssym['='] = eql;
	ssym[','] = comma;
	ssym['.'] = period;
	ssym['#'] = neq;
	ssym[';'] = semicolon;

	/* 设置保留字名字,按照字母顺序，便于折半查找 */
	strcpy(&(word[0][0]), "begin");
	strcpy(&(word[1][0]), "call");
	strcpy(&(word[2][0]), "const");
	strcpy(&(word[3][0]), "do");
	strcpy(&(word[4][0]), "end");
	strcpy(&(word[5][0]), "if");
	strcpy(&(word[6][0]), "odd");
	strcpy(&(word[7][0]), "procedure");
	strcpy(&(word[8][0]), "read");
	strcpy(&(word[9][0]), "then");
	strcpy(&(word[10][0]), "var");
	strcpy(&(word[11][0]), "while");
	strcpy(&(word[12][0]), "write");

	/* 设置保留字符号 */
	wsym[0] = beginsym;
	wsym[1] = callsym;
	wsym[2] = constsym;
	wsym[3] = dosym;
	wsym[4] = endsym;
	wsym[5] = ifsym;
	wsym[6] = oddsym;
	wsym[7] = procsym;
	wsym[8] = readsym;
	wsym[9] = thensym;
	wsym[10] = varsym;
	wsym[11] = whilesym;
	wsym[12] = writesym;

	/* 设置指令名称 */
	strcpy(&(mnemonic[lit][0]), "lit");
	strcpy(&(mnemonic[opr][0]), "opr");
	strcpy(&(mnemonic[lod][0]), "lod");
	strcpy(&(mnemonic[sto][0]), "sto");
	strcpy(&(mnemonic[cal][0]), "cal");
	strcpy(&(mnemonic[inte][0]), "int");
	strcpy(&(mnemonic[jmp][0]), "jmp");
	strcpy(&(mnemonic[jpc][0]), "jpc");

	/* 设置符号集 */
	for (i = 0; i < symnum; i++)
	{
		declbegsys[i] = false;
		statbegsys[i] = false;
		facbegsys[i] = false;
	}

	/* 设置声明开始符号集 */
	declbegsys[constsym] = true;
	declbegsys[varsym] = true;
	declbegsys[procsym] = true;

	/* 设置语句开始符号集 */
	statbegsys[beginsym] = true;
	statbegsys[callsym] = true;
	statbegsys[ifsym] = true;
	statbegsys[whilesym] = true;
	statbegsys[readsym] = true;
	statbegsys[writesym] = true;
	statbegsys[ident] = true;

	/* 设置因子开始符号集 */
	facbegsys[ident] = true;
	facbegsys[number] = true;
	facbegsys[lparen] = true;
}

//主函数，主要命令行询问
int main()
{
	bool nxtlev[symnum];

	printf("Input pl/0 file?   ");
	scanf("%s", fname);     /* 输入文件名 */

	fin = fopen(fname, "r");

	if (fin)
	{
		printf("List object code?(Y/N)");   /* 是否输出虚拟机代码 */
		scanf("%s", fname);
		listswitch = (fname[0] == 'y' || fname[0] == 'Y');

		printf("List symbol table?(Y/N)");  /* 是否输出名字表 */
		scanf("%s", fname);
		tableswitch = (fname[0] == 'y' || fname[0] == 'Y');

		fa1 = fopen("fa1.tmp", "w");
		fprintf(fa1, "Input pl/0 file?   ");
		fprintf(fa1, "%s\n", fname);

		init();     /* 初始化 */

		err = 0;
		cc = cx = ll = 0;
		ch = ' ';

		//看能否取出第一个单词
		if (-1 != getsym())
		{
			fa = fopen("fa.tmp", "w");
			fas = fopen("fas.tmp", "w");
			addset(nxtlev, declbegsys, statbegsys, symnum);
			nxtlev[period] = true;

			if (-1 == block(0, 0, nxtlev))   /* 调用编译程序 */
			{
				fclose(fa);
				fclose(fa1);
				fclose(fas);
				fclose(fin);
				printf("\n");
				return 0;
			}
			fclose(fa);
			fclose(fa1);
			fclose(fas);
			//当前单词是否为.
			if (sym != period)
			{
				error(9);
			}
			//源程序没有错误
			if (err == 0)
			{
				interpret();    /* 调用解释执行程序 */
			}
			else
			{
				printf("Errors in pl/0 program");
			}
		}

		fclose(fin);
	}
	else
	{
		printf("Can't open file!\n");
	}

	printf("\n");
	getchar();
	getchar();
	getchar();
	return 0;
}


#pragma region 词法分析


/*
* 漏掉换行符，读取一个字符。
*
* 读一行，存入line缓冲区，line被getsym取空后再读一行
*
* 每次从line缓冲区读出一个字符放在全局变量ch里面,输出代码行号
*
* 被函数getsym调用。
*/
int getch()
{
	//读完一行
	if (cc == ll)
	{
		if (feof(fin))  // 文件结束
		{
			printf("program incomplete");
			return -1;
		}
		ll = 0;
		cc = 0;
		printf("%d ", cx);
		fprintf(fa1, "%d ", cx);
		ch = ' ';
		//如果不是换行符
		while (ch != 10)
		{
			//读入一个字符到ch
			if (EOF == fscanf(fin, "%c", &ch))
			{
				line[ll] = 0;
				break;
			}
			printf("%c", ch);
			fprintf(fa1, "%c", ch);
			//往后再读一个字符
			line[ll] = ch;
			ll++;
		}
		printf("\n");
		fprintf(fa1, "\n");
	}
	ch = line[cc];
	cc++;
	return 0;
}

//词法分析，获取一个符号
//从源文件中读出若干有效字符，组成一个 token 串，识别它的类型为保留字/标识
//符/数字或是其它符号。如果是保留字，把 sym 置成相应的保留字类型，如果是标
//识符，把 sym 置成 ident 表示是标识符，于此同时，id 变量中存放的即为保留
//字字符串或标识符名字。如果是数字，把 sym 置为 number,同时 num 变量中存
//放该数字的值。如果是其它的操作符，则直接把 sym 置成相应类型。经过本过程后
//ch 变量中存放的是下一个即将被识别的字符

int getsym()
{
	int i, j, k;

	/* the original version lacks "\r", thanks to foolevery */
	while (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')  /* 忽略空格、换行、回车和TAB */
	{
		getchdo;
	}

	//名字或保留字（以A..z开头）
	if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z')
	{
		k = 0;
		do {
			if (k < al)
			{
				a[k] = ch;
				k++;
			}
			getchdo;
		} while (ch >= 'a' && ch <= 'z' || ch >= '0' && ch <= '9' || ch >= 'A' && ch <= 'Z');
		a[k] = 0;
		strcpy(id, a);
		i = 0;
		j = norw - 1;
		do {    /* 二分查找，搜索当前符号是否为保留字 */
			k = (i + j) / 2;
			if (strcmp(id, word[k]) <= 0)
			{
				j = k - 1;
			}
			if (strcmp(id, word[k]) >= 0)
			{
				i = k + 1;
			}
		} while (i <= j);
		if (i - 1 > j)
		{
			sym = wsym[k]; // 找到则标记保留字
		}
		else
		{
			sym = ident; // 否则标记为标识符
		}
	}
	// 为数字：以0..9开头 
	else if (ch >= '0' && ch <= '9')
	{
		k = 0;
		num = 0;
		sym = number;
		do {
			num = 10 * num + ch - '0';
			k++;
			getchdo;
		} while (ch >= '0' && ch <= '9'); /* 获取数字的值 */
		k--;
		if (k > nmax)
		{
			error(30);
		}
	}
	//占两个字符的运算符及界符
	else
	{
		if (ch == ':')      /* 检测赋值符号 */
		{
			getchdo;
			if (ch == '=')
			{
				sym = becomes;
				getchdo;
			}
			else
			{
				sym = nul;  /* 不能识别的符号 */
			}
		}
		else if (ch == '<')      /* 检测小于或小于等于符号 */
		{
			getchdo;
			if (ch == '=')
			{
				sym = leq;
				getchdo;
			}
			else
			{
				sym = lss;
			}
		}
		else if (ch == '>')        /* 检测大于或大于等于符号 */
		{
			getchdo;
			if (ch == '=')
			{
				sym = geq;
				getchdo;
			}
			else
			{
				sym = gtr;
			}
		}
		else            /* 当符号不满足上述条件时，全部按照单字符符号处理 */
		{
			sym = ssym[ch];
			if (sym != period)
			{
				getchdo;
			}
		}
	}
	return 0;
}

#pragma endregion

#pragma region 语法分析

/*
* 编译程序主模块
*
* lev:    当前分程序所在层
* tx:     名字表当前尾指针
* fsys:   当前模块后跟符号集
*/
int block(int lev, int tx, bool* fsys)
{
	int i;

	int dx;                 /* 名字分配到的相对地址 */
	int tx0;                /* 保留初始tx */
	int cx0;                /* 保留初始cx */
	bool nxtlev[symnum];    /* 在下级函数的参数中，符号集合均为值参，但由于使用数组实现，
							传递进来的是指针，为防止下级函数改变上级函数的集合，开辟新的空间
							传递给下级函数*/

	dx = 3;
	tx0 = tx;               /* 记录本层符号的初始位置 */
	table[tx].adr = cx;

	gendo(jmp, 0, 0);

	if (lev > levmax)
	{
		error(32);
	}

	do {

		if (sym == constsym)    /* 收到常量声明符号，开始处理常量声明 */
		{
			getsymdo;
			constdeclarationdo(&tx, lev, &dx);  /* dx的值会被constdeclaration改变，使用指针 */
			while (sym == comma)
			{
				getsymdo;
				constdeclarationdo(&tx, lev, &dx);
			}
			if (sym == semicolon)
			{
				getsymdo;
			}
			else
			{
				error(5);   /*漏掉了逗号或者分号*/
			}
		}

		if (sym == varsym)      /* 收到变量声明符号，开始处理变量声明 */
		{
			getsymdo;

			vardeclarationdo(&tx, lev, &dx);
			while (sym == comma)
			{
				getsymdo;
				vardeclarationdo(&tx, lev, &dx);
			}
			if (sym == semicolon)
			{
				getsymdo;
			}
			else
			{
				error(5);
			}
		}

		while (sym == procsym) /* 收到过程声明符号，开始处理过程声明 */
		{
			getsymdo;

			if (sym == ident)
			{
				enter(procedur, &tx, lev, &dx); /* 记录过程名字 */
				getsymdo;
			}
			else
			{
				error(4);   /* procedure后应为标识符 */
			}

			if (sym == semicolon)
			{
				getsymdo;
			}
			else
			{
				error(5);   /* 漏掉了分号 */
			}

			memcpy(nxtlev, fsys, sizeof(bool) * symnum);
			nxtlev[semicolon] = true;
			if (-1 == block(lev + 1, tx, nxtlev))
			{
				return -1;  /* 递归调用 */
			}

			if (sym == semicolon)
			{
				getsymdo;
				memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
				nxtlev[ident] = true;
				nxtlev[procsym] = true;
				testdo(nxtlev, fsys, 6);
			}
			else
			{
				error(5);   /* 漏掉了分号 */
			}
		}
		memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
		nxtlev[ident] = true;
		testdo(nxtlev, declbegsys, 7);
	} while (inset(sym, declbegsys));   /* 直到没有声明符号 */

	code[table[tx0].adr].a = cx;    /* 开始生成当前过程代码 */
	table[tx0].adr = cx;            /* 当前过程代码地址 */
	table[tx0].size = dx;           /* 声明部分中每增加一条声明都会给dx增加1，声明部分已经结束，dx就是当前过程数据的size */
	cx0 = cx;
	gendo(inte, 0, dx);             /* 生成分配内存代码 */

	if (tableswitch)        /* 输出符号表 */
	{
		i = tx;
		while (table[i].kind != procedur && i > 0) i--;
		if (i != tx && i != 0) printf("The Table at procedure %s:\n", table[i].name);
		else  printf("The Table at main program:\n");

		if (tx0 + 1 > tx)
		{
			printf("    NULL\n");
		}
		else {
			printf("%-8s %-8s %-8s %-8s %-8s %-8s \n", "number", "kind", "name", "val/lev", "addr", "size");
			for (i = tx0 + 1; i <= tx; i++)
			{
				switch (table[i].kind)
				{
				case constant:
					printf("%-8d const    %-8s ", i, table[i].name);
					printf("val=%-4d\n", table[i].val);
					fprintf(fas, "%-8d const    %-8s ", i, table[i].name);
					fprintf(fas, "val=%-4d\n", table[i].val);
					break;
				case variable:
					printf("%-8d var      %-8s ", i, table[i].name);
					printf("lev=%-4d addr=%-3d\n", table[i].level, table[i].adr);
					fprintf(fas, "%-8d var      %-8s ", i, table[i].name);
					fprintf(fas, "lev=%-4d addr=%-3d\n", table[i].level, table[i].adr);
					break;
				case procedur:
					printf("%-8d proc     %-8s ", i, table[i].name);
					printf("lev=%-4d addr=%-3d size=%-3d\n", table[i].level, table[i].adr, table[i].size);
					fprintf(fas, "%-8d proc     %-8s ", i, table[i].name);
					fprintf(fas, "lev=%-4d addr=%-3d size=%-3d\n", table[i].level, table[i].adr, table[i].size);
					break;
				}
			}
		}

		printf("\n");
	}

	/* 语句后跟符号为分号或end */
	memcpy(nxtlev, fsys, sizeof(bool) * symnum);  /* 每个后跟符号集和都包含上层后跟符号集和，以便补救 */
	nxtlev[semicolon] = true;
	nxtlev[endsym] = true;
	statementdo(nxtlev, &tx, lev);
	gendo(opr, 0, 0);                       /* 每个过程出口都要使用的释放数据段指令 */
	memset(nxtlev, 0, sizeof(bool) * symnum); /*分程序没有补救集合 */
	testdo(fsys, nxtlev, 8);                /* 检测后跟符号正确性 */
	listcode(cx0);                          /* 输出代码 */
	return 0;
}
/*
* 语句处理
*/
int statement(bool* fsys, int* ptx, int lev)
{
	int i, cx1, cx2;
	bool nxtlev[symnum];

	if (sym == ident)   /* 准备按照赋值语句处理 */
	{
		i = position(id, *ptx);
		if (i == 0)
		{
			error(11);  /* 变量未找到 */
		}
		else
		{
			if (table[i].kind != variable)
			{
				error(12);  /* 赋值语句格式错误 */
				i = 0;
			}
			else
			{
				getsymdo;
				if (sym == becomes)
				{
					getsymdo;
				}
				else
				{
					error(13);  /* 没有检测到赋值符号 */
				}
				memcpy(nxtlev, fsys, sizeof(bool) * symnum);
				expressiondo(nxtlev, ptx, lev); /* 处理赋值符号右侧表达式 */
				if (i != 0)
				{
					/* expression将执行一系列指令，但最终结果将会保存在栈顶，执行sto命令完成赋值 */
					gendo(sto, lev - table[i].level, table[i].adr);
				}
			}
		}//if (i == 0)
	}
	else if (sym == readsym) /* 准备按照read语句处理 */
	{
		getsymdo;
		if (sym != lparen)
		{
			error(34);  /* 格式错误，应是左括号 */
		}
		else
		{
			do {
				getsymdo;
				if (sym == ident)
				{
					i = position(id, *ptx); /* 查找要读的变量 */
				}
				else
				{
					i = 0;
				}

				if (i == 0)
				{
					error(35);  /* read()中应是声明过的变量名 */
				}
				else if (table[i].kind != variable)
				{
					error(35);	/* read()参数表的标识符不是变量 */
				}
				else
				{
					gendo(opr, 0, 16);  /* 生成输入指令，读取值到栈顶 */
					gendo(sto, lev - table[i].level, table[i].adr);   /* 储存到变量 */
				}
				getsymdo;

			} while (sym == comma); /* 一条read语句可读多个变量 */
		}
		if (sym != rparen)
		{
			error(33);  /* 格式错误，应是右括号 */
			while (!inset(sym, fsys))   /* 出错补救，直到收到上层函数的后跟符号 */
			{
				getsymdo;
			}
		}
		else
		{
			getsymdo;
		}
	}
	else if (sym == writesym)    /* 准备按照write语句处理，与read类似 */
	{
		getsymdo;
		if (sym == lparen)
		{
			do {
				getsymdo;
				memcpy(nxtlev, fsys, sizeof(bool) * symnum);
				nxtlev[rparen] = true;
				nxtlev[comma] = true;       /* write的后跟符号为) or , */
				expressiondo(nxtlev, ptx, lev); /* 调用表达式处理，此处与read不同，read为给变量赋值 */
				gendo(opr, 0, 14);  /* 生成输出指令，输出栈顶的值 */
			} while (sym == comma);
			if (sym != rparen)
			{
				error(33);  /* write()中应为完整表达式 */
			}
			else
			{
				getsymdo;
			}
		}
		gendo(opr, 0, 15);  /* 输出换行 */
	}
	else if (sym == callsym) /* 准备按照call语句处理 */
	{
		getsymdo;
		if (sym != ident)
		{
			error(14);  /* call后应为标识符 */
		}
		else
		{
			i = position(id, *ptx);
			if (i == 0)
			{
				error(11);  /* 过程未找到 */
			}
			else
			{
				if (table[i].kind == procedur)
				{
					gendo(cal, lev - table[i].level, table[i].adr);   /* 生成call指令 */
				}
				else
				{
					error(15);  /* call后标识符应为过程 */
				}
			}
			getsymdo;
		}
	}
	else if (sym == ifsym)   /* 准备按照if语句处理 */
	{
		getsymdo;
		memcpy(nxtlev, fsys, sizeof(bool) * symnum);
		nxtlev[thensym] = true;
		nxtlev[dosym] = true;   /* 后跟符号为then或do */
		conditiondo(nxtlev, ptx, lev); /* 调用条件处理（逻辑运算）函数 */
		if (sym == thensym)
		{
			getsymdo;
		}
		else
		{
			error(16);  /* 缺少then */
		}
		cx1 = cx;   /* 保存当前指令地址 */
		gendo(jpc, 0, 0);   /* 生成条件跳转指令，跳转地址未知，暂时写0 */
		statementdo(fsys, ptx, lev);    /* 处理then后的语句 */
		code[cx1].a = cx;   /* 经statement处理后，cx为then后语句执行完的位置，它正是前面未定的跳转地址 */
	}
	else if (sym == beginsym)    /* 准备按照复合语句处理 */
	{
		getsymdo;
		memcpy(nxtlev, fsys, sizeof(bool) * symnum);
		nxtlev[semicolon] = true;
		nxtlev[endsym] = true;  /* 后跟符号为分号或end */
		/* 循环调用语句处理函数，直到下一个符号不是语句开始符号或收到end */
		statementdo(nxtlev, ptx, lev);

		while (inset(sym, statbegsys) || sym == semicolon)
		{
			if (sym == semicolon)
			{
				getsymdo;
			}
			else
			{
				error(10);  /* 缺少分号 */
			}
			statementdo(nxtlev, ptx, lev);
		}
		if (sym == endsym)
		{
			getsymdo;
		}
		else
		{
			error(17);  /* 缺少end或分号 */
		}
	}
	else if (sym == whilesym)    /* 准备按照while语句处理 */
	{
		cx1 = cx;   /* 保存判断条件操作的位置 */
		getsymdo;
		memcpy(nxtlev, fsys, sizeof(bool) * symnum);
		nxtlev[dosym] = true;   /* 后跟符号为do */
		conditiondo(nxtlev, ptx, lev);  /* 调用条件处理 */
		cx2 = cx;   /* 保存jpc指令的位置 */
		gendo(jpc, 0, 0);   /* 生成条件跳转，但跳出循环的地址未知 */
		if (sym == dosym)
		{
			getsymdo;
		}
		else
		{
			error(18);  /* 缺少do */
		}
		statementdo(fsys, ptx, lev);    /* 循环体 */
		gendo(jmp, 0, cx1); /* 回头重新判断条件 */
		code[cx2].a = cx;   /* 反填跳出循环的地址，与if类似 */
	}
	else
	{
		memset(nxtlev, 0, sizeof(bool) * symnum); /* 语句结束无补救集合 */
		testdo(fsys, nxtlev, 19);   /* 检测语句结束的正确性 */
	}

	return 0;
}
/*
* 常量声明处理
*/
int constdeclaration(int* ptx, int lev, int* pdx)
{
	if (sym == ident)
	{
		getsymdo;
		if (sym == eql || sym == becomes)
		{
			if (sym == becomes)
			{
				error(1);   /* 把=写成了:= */
			}
			getsymdo;
			if (sym == number)
			{
				enter(constant, ptx, lev, pdx);
				getsymdo;
			}
			else
			{
				error(2);   /* 常量说明=后应是数字 */
			}
		}
		else
		{
			error(3);   /* 常量说明标识后应是= */
		}
	}
	else
	{
		error(4);   /* const后应是标识 */
	}
	return 0;
}

/*
* 变量声明处理
ptx为符号表尾位置
lev为当前层
pdx为为在当前层的偏移量
*/
int vardeclaration(int* ptx, int lev, int* pdx)
{
	if (sym == ident)
	{
		enter(variable, ptx, lev, pdx); // 填写名字表
		getsymdo;
	}
	else
	{
		error(4);   /* var后应是标识 */
	}
	return 0;
}
/*
* 表达式处理
*/
int expression(bool* fsys, int* ptx, int lev)
{
	enum symbol addop;  /* 用于保存正负号 */
	bool nxtlev[symnum];

	if (sym == plus || sym == minus) /* 开头的正负号，此时当前表达式被看作一个正的或负的项 */
	{
		addop = sym;    /* 保存开头的正负号 */
		getsymdo;
		memcpy(nxtlev, fsys, sizeof(bool) * symnum);
		nxtlev[plus] = true;
		nxtlev[minus] = true;
		termdo(nxtlev, ptx, lev);   /* 处理项 */
		if (addop == minus)
		{
			gendo(opr, 0, 1); /* 如果开头为负号生成取负指令 */
		}
	}
	else    /* 此时表达式被看作项的加减 */
	{
		memcpy(nxtlev, fsys, sizeof(bool) * symnum);
		nxtlev[plus] = true;
		nxtlev[minus] = true;
		termdo(nxtlev, ptx, lev);   /* 处理项 */
	}
	while (sym == plus || sym == minus)
	{
		addop = sym;
		getsymdo;
		memcpy(nxtlev, fsys, sizeof(bool) * symnum);
		nxtlev[plus] = true;
		nxtlev[minus] = true;
		termdo(nxtlev, ptx, lev);   /* 处理项 */
		if (addop == plus)
		{
			gendo(opr, 0, 2);   /* 生成加法指令 */
		}
		else
		{
			gendo(opr, 0, 3);   /* 生成减法指令 */
		}
	}
	return 0;
}

/*
* 项处理
*/
int term(bool* fsys, int* ptx, int lev)
{
	enum symbol mulop;  /* 用于保存乘除法符号 */
	bool nxtlev[symnum];

	memcpy(nxtlev, fsys, sizeof(bool) * symnum);
	nxtlev[times] = true;
	nxtlev[slash] = true;
	factordo(nxtlev, ptx, lev); /* 处理因子 */
	while (sym == times || sym == slash)
	{
		mulop = sym;
		getsymdo;
		factordo(nxtlev, ptx, lev);
		if (mulop == times)
		{
			gendo(opr, 0, 4);   /* 生成乘法指令 */
		}
		else
		{
			gendo(opr, 0, 5);   /* 生成除法指令 */
		}
	}
	return 0;
}

/*
* 因子处理
*/
int factor(bool* fsys, int* ptx, int lev)
{
	int i;
	bool nxtlev[symnum];
	testdo(facbegsys, fsys, 24);    /* 检测因子的开始符号 */
	/* while(inset(sym, facbegsys)) */  /* 循环直到不是因子开始符号 */
	if (inset(sym, facbegsys))    /* BUG: 原来的方法var1(var2+var3)会被错误识别为因子 */
	{
		if (sym == ident)    /* 因子为常量或变量 */
		{
			i = position(id, *ptx); /* 查找名字 */
			if (i == 0)
			{
				error(11);  /* 标识符未声明 */
			}
			else
			{
				switch (table[i].kind)
				{
				case constant:  /* 名字为常量 */
					gendo(lit, 0, table[i].val);    /* 直接把常量的值入栈 */
					break;
				case variable:  /* 名字为变量 */
					gendo(lod, lev - table[i].level, table[i].adr);   /* 找到变量地址并将其值入栈 */
					break;
				case procedur:  /* 名字为过程 */
					error(21);  /* 不能为过程 */
					break;
				}
			}
			getsymdo;
		}
		else if (sym == number)   /* 因子为数 */
		{
			if (num > amax)
			{
				error(31);
				num = 0;
			}
			gendo(lit, 0, num);
			getsymdo;
		}
		else
		{
			if (sym == lparen)  /* 因子为表达式 */
			{
				getsymdo;
				memcpy(nxtlev, fsys, sizeof(bool) * symnum);
				nxtlev[rparen] = true;
				expressiondo(nxtlev, ptx, lev);
				if (sym == rparen)
				{
					getsymdo;
				}
				else
				{
					error(33);  /* 缺少右括号 */
				}
			}
			testdo(fsys, facbegsys, 23);    /* 因子后有非法符号 */
		}
	}
	return 0;
}

/*
* 条件处理
*/
int condition(bool* fsys, int* ptx, int lev)
{
	enum symbol relop;
	bool nxtlev[symnum];

	if (sym == oddsym)   /* 准备按照odd运算处理 */
	{
		getsymdo;
		expressiondo(fsys, ptx, lev);
		gendo(opr, 0, 6);   /* 生成odd指令 */
	}
	else
	{
		/* 逻辑表达式处理 */
		memcpy(nxtlev, fsys, sizeof(bool) * symnum);
		nxtlev[eql] = true;
		nxtlev[neq] = true;
		nxtlev[lss] = true;
		nxtlev[leq] = true;
		nxtlev[gtr] = true;
		nxtlev[geq] = true;
		expressiondo(nxtlev, ptx, lev);
		if (sym != eql && sym != neq && sym != lss && sym != leq && sym != gtr && sym != geq)
		{
			error(20);
		}
		else
		{
			relop = sym;
			getsymdo;
			expressiondo(fsys, ptx, lev);
			switch (relop)
			{
			case eql:
				gendo(opr, 0, 8);
				break;
			case neq:
				gendo(opr, 0, 9);
				break;
			case lss:
				gendo(opr, 0, 10);
				break;
			case geq:
				gendo(opr, 0, 11);
				break;
			case gtr:
				gendo(opr, 0, 12);
				break;
			case leq:
				gendo(opr, 0, 13);
				break;
			}
		}
	}
	return 0;
}

#pragma endregion

#pragma region 中间代码生成
/*
* 生成虚拟机代码
*
* x: instruction.f;
* y: instruction.l;
* z: instruction.a;
*/
int gen(enum fct x, int y, int z)
{
	if (cx >= cxmax)
	{
		printf("程序过长！");
		return -1;
	}
	code[cx].f = x;
	code[cx].l = y;
	code[cx].a = z;
	cx++;
	return 0;
}


/*
* 输出目标代码清单
*/
void listcode(int cx0)
{
	int i;
	if (listswitch)
	{
		for (i = cx0; i < cx; i++)
		{
			printf("%d %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
			fprintf(fa, "%d %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
		}
	}
}

#pragma endregion

#pragma region 解释器

//格式化返回一个MIPS指令
void WriteMpisAsm(const char* ins, const char* op1=NULL, const char* op2 = NULL, const  char* op3 = NULL) {
	if (mips == NULL) {
		printf("文件都没打开！\n");
		return;
	}
	if (op1 == NULL) {
		fprintf(mips, "\t%s\n", ins);
	}
	else if (op2 == NULL)
	{
		fprintf(mips, "\t%s %s\n", ins, op1);
	}
	else if(op3==NULL)
	{
		fprintf(mips, "\t%s %s, %s\n", ins, op1, op2);
	}
	else
	{
		fprintf(mips, "\t%s %s, %s, %s\n", ins,op1,op2,op3);
	}
}

void Label(const char* lab,int num=-1) {
	if(num==-1)fprintf(mips, "%s:\n", lab);
	else fprintf(mips, "%s%d:\n", lab,num);
}

/* 求出定义该过程的过程基址
*l为层次差，b为上一过程（调用该过程的过程）基址，s为栈
从这改成生成mips汇编代码
调用前需在t4中存入层次差l的值
该函数在t4寄存器中返回需要的值
*/
void base()
{
	Label("base");

	WriteMpisAsm("move", "$t1", "$t9");//t1=t9(base),b1=b,t1即b1
	WriteMpisAsm("la", "$t0", "stack");    //取栈顶的地址

	Label("baseWhile");
	WriteMpisAsm("bgt", "$t4", "$0", "baseGo");//l>0?,是则跳转basego
	WriteMpisAsm("j", "baseEnd");//否则返回值

	Label("baseGo");
	WriteMpisAsm("sll", "$t2", "$t1", "2");//t2=t1<<2,即乘以4
	WriteMpisAsm("add", "$t0", "$t0", "$t2");//得到s[b1]的地址
	WriteMpisAsm("lw", "$t1", "($t0)");//t1=s[b1],b1 = s[b1]

	WriteMpisAsm("addi", "$t4", "$t4", "-1");    //l--
	WriteMpisAsm("j", "baseWhile");//继续循环

	Label("baseEnd");
	WriteMpisAsm("move", "$t4", "$t1");//return b1
	WriteMpisAsm("jr", "$31");//返回调用处

}

/*
* 解释程序,仅执行一遍
t7寄存器用于存放栈顶指针，该指针-1为最近存放的数的地址
t8用于存放PC，该pc为pcode的PC
t9用于存放pcode栈的基地址
他们分别与C程序中的t,p,b对应
*/
void interpret()
{
	struct instruction i;   /* 存放当前指令 */
	int j;
	char t1[20], t2[20];
	bool baseFlag = false;
	FILE* pcode;

	pcode= fopen("Pcode.pcode", "w");
	mips = fopen("mpis32.s", "w");

	fprintf(mips, ".data\nstack:\t.space\t %d\n\n.text\nmain:\n",stacksize*4);

	printf("start pl0\n");

	WriteMpisAsm("li", "$t7", "0");
	WriteMpisAsm("li", "$t8", "1");
	WriteMpisAsm("li", "$t9", "0");
	WriteMpisAsm("la", "$t0", "stack");//加载栈基地址
	WriteMpisAsm("sw", "$0", "($t0)");
	WriteMpisAsm("sw", "$0", "4($t0)");
	WriteMpisAsm("sw", "$0", "8($t0)");

	printf("%d\n", cx);

	for(j=0;j<cx;j++) {
		fprintf(pcode, "%d %s %d %d\n", j, mnemonic[code[j].f], code[j].l, code[j].a);
		i = code[j];    /* 读当前指令 */
		
		printf("%d\n", j);
		WriteMpisAsm("addi", "$t8", "$t8", "1");//p++
		Label("ins", j);
		switch (i.f)
		{
		case lit:   /* 将a的值取到栈顶 */
			sprintf(immediate, "%d", i.a);
			WriteMpisAsm("li", "$t2", immediate);//t2存放a的值

			WriteMpisAsm("la", "$t0", "stack");//加载栈基地址
			WriteMpisAsm("move", "$t1", "$t7");//t1=t7
			WriteMpisAsm("sll", "$t1", "$t1", "2");//t1=t1<<2,即乘以4
			WriteMpisAsm("add", "$t0", "$t0", "$t1");//得到栈top的地址

			WriteMpisAsm("sw", "$t2", "($t0)");//存入栈
			WriteMpisAsm("addi", "$t7", "$t7", "1");//栈顶++
			break;
		case opr:   /* 数学、逻辑运算 */
			switch (i.a)
			{
			case 0://过程返回
				WriteMpisAsm("add", "$t7", "$t9", "$0");//将栈顶指针重置，即top=base
				WriteMpisAsm("la", "$t0", "stack");//加载栈基地址
				WriteMpisAsm("move", "$t1", "$t7");//t1=t7
				WriteMpisAsm("sll", "$t1", "$t1", "2");//t1=t1<<2,即乘以4
				WriteMpisAsm("add", "$t0", "$t0", "$t1");//得到栈top的地址
				WriteMpisAsm("lw", "$t8", "8($t0)");//将栈顶的数s[t + 2]取出，放入寄存器t8(PC)
				WriteMpisAsm("lw", "$t9", "4($t0)");//将栈顶的数s[t + 1]取出，放入寄存器t9(base)

				WriteMpisAsm("beq", "$t8", "$0", "mainEnd");//t8==0(p==0)?,是则整个程序结束
				WriteMpisAsm("jr", "$s4");//返回调用处
				
				break;
			case 1://求相反数
				WriteMpisAsm("la", "$t0", "stack");//加载栈基地址
				WriteMpisAsm("addi", "$t1", "$t7", "-1");//栈顶地址-1,即栈最上面的那个数的地址
				WriteMpisAsm("sll", "$t1", "$t1", "2");//t1=t1<<2,即乘以4
				WriteMpisAsm("add", "$t0", "$t0", "$t1");//得到栈最上面的那个数的地址

				WriteMpisAsm("lw", "$t1", "($t0)");//将栈顶的数a取出，放入寄存器t1
				WriteMpisAsm("sub", "$t1", "$0", "$t1");//a=-a
				WriteMpisAsm("sw", "$t1", "($t0)");//将a存回
				break;
			case 2://次栈顶值与栈顶相加，存入次栈顶
				WriteMpisAsm("addi", "$t7", "$t7", "-1");//栈顶指针-1,
				WriteMpisAsm("la", "$t0", "stack");//加载栈基地址
				WriteMpisAsm("addi", "$t1", "$t7", "-1");//栈顶地址-1,即栈最上面的那个数的地址
				WriteMpisAsm("sll", "$t1", "$t1", "2");//t1=t1<<2,即乘以4
				WriteMpisAsm("add", "$t0", "$t0", "$t1");//得到次栈顶那个数的地址
				WriteMpisAsm("lw", "$t1", "4($t0)", NULL);//将栈顶的数a取出，放入寄存器t1
				WriteMpisAsm("lw", "$t2", "($t0)", NULL);//将次栈顶的数b取出，放入寄存器t2
				WriteMpisAsm("add", "$t2", "$t2", "$t1");//b=b+a
				WriteMpisAsm("sw", "$t2", "($t0)", NULL);//将b存回
				break;
			case 3://次栈顶值-栈顶，存入次栈顶
				WriteMpisAsm("addi", "$t7", "$t7", "-1");//栈顶指针-1,
				WriteMpisAsm("la", "$t0", "stack", NULL);//加载栈基地址
				WriteMpisAsm("addi", "$t1", "$t7", "-1");//栈顶地址-1,即栈最上面的那个数的地址
				WriteMpisAsm("sll", "$t1", "$t1", "2");//t1=t1<<2,即乘以4
				WriteMpisAsm("add", "$t0", "$t0", "$t1");//得到次栈顶的那个数的地址
				WriteMpisAsm("lw", "$t1", "4($t0)", NULL);//将栈顶的数a取出，放入寄存器t1
				WriteMpisAsm("lw", "$t2", "($t0)", NULL);//将次栈顶的数b取出，放入寄存器t2
				WriteMpisAsm("sub", "$t2", "$t2", "$t1");//b=b-a
				WriteMpisAsm("sw", "$t2", "($t0)", NULL);//将b存回
				break;
			case 4://次栈顶值*栈顶，存入次栈顶
				WriteMpisAsm("addi", "$t7", "$t7", "-1");//栈顶指针-1,
				WriteMpisAsm("la", "$t0", "stack", NULL);//加载栈基地址
				WriteMpisAsm("addi", "$t1", "$t7", "-1");//栈顶地址-1,即栈最上面的那个数的地址
				WriteMpisAsm("sll", "$t1", "$t1", "2");//t1=t1<<2,即乘以4
				WriteMpisAsm("add", "$t0", "$t0", "$t1");//得到次栈顶那个数的地址
				WriteMpisAsm("lw", "$t1", "4($t0)", NULL);//将栈顶的数a取出，放入寄存器t1
				WriteMpisAsm("lw", "$t2", "($t0)", NULL);//将次栈顶的数b取出，放入寄存器t2
				WriteMpisAsm("mul", "$t2", "$t2", "$t1");//b=b*a
				WriteMpisAsm("sw", "$t2", "($t0)", NULL);//将b存回
				break;
			case 5://次栈顶值/栈顶，存入次栈顶
				WriteMpisAsm("addi", "$t7", "$t7", "-1");//栈顶指针-1,
				WriteMpisAsm("la", "$t0", "stack", NULL);//加载栈基地址
				WriteMpisAsm("addi", "$t1", "$t7", "-1");//栈顶地址-1,即栈最上面的那个数的地址
				WriteMpisAsm("sll", "$t1", "$t1", "2");//t1=t1<<2,即乘以4
				WriteMpisAsm("add", "$t0", "$t0", "$t1");//得到次栈顶那个数的地址
				WriteMpisAsm("lw", "$t1", "4($t0)", NULL);//将栈顶的数a取出，放入寄存器t1
				WriteMpisAsm("lw", "$t2", "($t0)", NULL);//将次栈顶的数b取出，放入寄存器t2
				WriteMpisAsm("div", "$t2", "$t1", NULL);//b=b/a，结果放在lo寄存器
				WriteMpisAsm("mflo", "$t2", NULL, NULL);//t2=lo
				WriteMpisAsm("sw", "$t2", "($t0)", NULL);//将b存回
				break;
			case 6://栈顶元素求mod2，存入栈顶
				WriteMpisAsm("la", "$t0", "stack", NULL);//加载栈基地址
				WriteMpisAsm("addi", "$t1", "$t7", "-1");//栈顶地址-1,即栈最上面的那个数的地址
				WriteMpisAsm("sll", "$t1", "$t1", "2");//t1=t1<<2,即乘以4
				WriteMpisAsm("add", "$t0", "$t0", "$t1");//得到栈顶那个数的地址
				WriteMpisAsm("lw", "$t1", "($t0)", NULL);//将栈顶的数a取出，放入寄存器t1
				WriteMpisAsm("li", "$t2", "2", NULL);//t2=2
				WriteMpisAsm("div", "$t1", "$t2", NULL);//a=amod2，结果放在hi寄存器
				WriteMpisAsm("mfhi", "$t1", NULL, NULL);//t1=hi
				WriteMpisAsm("sw", "$t1", "($t0)", NULL);//将a存回
				break;
			case 8://次栈顶值==栈顶？，存入次栈顶
				sprintf(t1, "%s%d", "beqget", j);
				sprintf(t2, "%s%d", "beqEnd", j);
				WriteMpisAsm("addi", "$t7", "$t7", "-1");//栈顶指针-1,
				WriteMpisAsm("la", "$t0", "stack", NULL);//加载栈基地址
				WriteMpisAsm("addi", "$t1", "$t7", "-1");//栈顶地址-1,即栈最上面的那个数的地址
				WriteMpisAsm("sll", "$t1", "$t1", "2");//t1=t1<<2,即乘以4
				WriteMpisAsm("add", "$t0", "$t0", "$t1");//得到次栈顶那个数的地址
				WriteMpisAsm("lw", "$t1", "4($t0)", NULL);//将栈顶的数a取出，放入寄存器t1
				WriteMpisAsm("lw", "$t2", "($t0)", NULL);//将次栈顶的数b取出，放入寄存器t2
				WriteMpisAsm("beq", "$t1", "$t2", t1);//b==a?,是则跳转get1
				WriteMpisAsm("li", "$t1", "0", NULL);//b!=a
				WriteMpisAsm("j", t2, NULL, NULL);//到这就存回去
				Label(t1);
				WriteMpisAsm("li", "$t1", "1", NULL);//b==a
				Label(t2);
				WriteMpisAsm("sw", "$t1", "($t0)", NULL);//将bool值存回
				break;
			case 9://次栈顶值！=栈顶？，存入次栈顶
				sprintf(t1, "%s%d", "bneget", j);
				sprintf(t2, "%s%d", "bneEnd", j);
				WriteMpisAsm("addi", "$t7", "$t7", "-1");//栈顶指针-1,
				WriteMpisAsm("la", "$t0", "stack", NULL);//加载栈基地址
				WriteMpisAsm("addi", "$t1", "$t7", "-1");//栈顶地址-1,即栈最上面的那个数的地址
				WriteMpisAsm("sll", "$t1", "$t1", "2");//t1=t1<<2,即乘以4
				WriteMpisAsm("add", "$t0", "$t0", "$t1");//得到次栈顶那个数的地址
				WriteMpisAsm("lw", "$t1", "4($t0)", NULL);//将栈顶的数a取出，放入寄存器t1
				WriteMpisAsm("lw", "$t2", "($t0)", NULL);//将次栈顶的数b取出，放入寄存器t2
				WriteMpisAsm("bne", "$t1", "$t2", t1);//b!=a?,是则跳转get1
				WriteMpisAsm("li", "$t1", "0", NULL);//b==a
				WriteMpisAsm("j", t2, NULL, NULL);//到这就存回去
				Label(t1);
				WriteMpisAsm("li", "$t1", "1", NULL);//b!=a
				Label(t2);
				WriteMpisAsm("sw", "$t1", "($t0)", NULL);//将bool值存回
				break;
			case 10://次栈顶值<栈顶？，存入次栈顶
				WriteMpisAsm("addi", "$t7", "$t7", "-1");//栈顶指针-1,
				WriteMpisAsm("la", "$t0", "stack", NULL);//加载栈基地址
				WriteMpisAsm("addi", "$t1", "$t7", "-1");//栈顶地址-1,即栈最上面的那个数的地址
				WriteMpisAsm("sll", "$t1", "$t1", "2");//t1=t1<<2,即乘以4
				WriteMpisAsm("add", "$t0", "$t0", "$t1");//得到次栈顶那个数的地址
				WriteMpisAsm("lw", "$t1", "4($t0)", NULL);//将栈顶的数a取出，放入寄存器t1
				WriteMpisAsm("lw", "$t2", "($t0)", NULL);//将次栈顶的数b取出，放入寄存器t2
				WriteMpisAsm("slt", "$t1", "$t2", "$t1");//b<a?,结果放入t1
				WriteMpisAsm("sw", "$t1", "($t0)", NULL);//将bool值存回

				break;
			case 11://次栈顶值>=栈顶？，存入次栈顶
				sprintf(t1, "%s%d", "bgeget", j);
				sprintf(t2, "%s%d", "bgeEnd", j);
				WriteMpisAsm("addi", "$t7", "$t7", "-1");//栈顶指针-1,
				WriteMpisAsm("la", "$t0", "stack", NULL);//加载栈基地址
				WriteMpisAsm("addi", "$t1", "$t7", "-1");//栈顶地址-1,即栈最上面的那个数的地址
				WriteMpisAsm("sll", "$t1", "$t1", "2");//t1=t1<<2,即乘以4
				WriteMpisAsm("add", "$t0", "$t0", "$t1");//得到次栈顶那个数的地址
				WriteMpisAsm("lw", "$t1", "4($t0)", NULL);//将栈顶的数a取出，放入寄存器t1
				WriteMpisAsm("lw", "$t2", "($t0)", NULL);//将次栈顶的数b取出，放入寄存器t2
				WriteMpisAsm("bge", "$t2", "$t1", t1);//b>=a?,是则跳转get1
				WriteMpisAsm("li", "$t1", "0", NULL);//b<a
				WriteMpisAsm("j", t2, NULL, NULL);//到这就存回去
				Label(t1);
				WriteMpisAsm("li", "$t1", "1", NULL);//b>=a
				Label(t2);
				WriteMpisAsm("sw", "$t1", "($t0)", NULL);//将bool值存回

				break;
			case 12://次栈顶值>栈顶？，存入次栈顶
				sprintf(t1, "%s%d", "bgtget", j);
				sprintf(t2, "%s%d", "bgtEnd", j);
				WriteMpisAsm("addi", "$t7", "$t7", "-1");//栈顶指针-1,
				WriteMpisAsm("la", "$t0", "stack", NULL);//加载栈基地址
				WriteMpisAsm("addi", "$t1", "$t7", "-1");//栈顶地址-1,即栈最上面的那个数的地址
				WriteMpisAsm("sll", "$t1", "$t1", "2");//t1=t1<<2,即乘以4
				WriteMpisAsm("add", "$t0", "$t0", "$t1");//得到次栈顶那个数的地址
				WriteMpisAsm("lw", "$t1", "4($t0)", NULL);//将栈顶的数a取出，放入寄存器t1
				WriteMpisAsm("lw", "$t2", "($t0)", NULL);//将次栈顶的数b取出，放入寄存器t2
				WriteMpisAsm("bgt", "$t2", "$t1", t1);//b>a?,是则跳转get1
				WriteMpisAsm("li", "$t1", "0", NULL);//b<=a
				WriteMpisAsm("j", t2, NULL, NULL);//到这就存回去
				Label(t1);
				WriteMpisAsm("li", "$t1", "1", NULL);//b>a
				Label(t2);
				WriteMpisAsm("sw", "$t1", "($t0)", NULL);//将bool值存回
				break;
			case 13://次栈顶值<=栈顶？，存入次栈顶
				sprintf(t1, "%s%d", "bleget", j);
				sprintf(t2, "%s%d", "bleEnd", j);
				WriteMpisAsm("addi", "$t7", "$t7", "-1");//栈顶指针-1,
				WriteMpisAsm("la", "$t0", "stack", NULL);//加载栈基地址
				WriteMpisAsm("addi", "$t1", "$t7", "-1");//栈顶地址-1,即栈最上面的那个数的地址
				WriteMpisAsm("sll", "$t1", "$t1", "2");//t1=t1<<2,即乘以4
				WriteMpisAsm("add", "$t0", "$t0", "$t1");//得到次栈顶那个数的地址
				WriteMpisAsm("lw", "$t1", "4($t0)", NULL);//将栈顶的数a取出，放入寄存器t1
				WriteMpisAsm("lw", "$t2", "($t0)", NULL);//将次栈顶的数b取出，放入寄存器t2
				WriteMpisAsm("ble", "$t2", "$t1", t1);//b<=a?,是则跳转get1
				WriteMpisAsm("li", "$t1", "0", NULL);//b>a
				WriteMpisAsm("j", t2, NULL, NULL);//到这就存回去
				Label(t1);
				WriteMpisAsm("li", "$t1", "1", NULL);//b<=a
				Label(t2);
				WriteMpisAsm("sw", "$t1", "($t0)", NULL);//将bool值存回
				break;
			case 14://输出栈顶值，栈顶指针--
				WriteMpisAsm("li", "$v0", "1");

				WriteMpisAsm("la", "$t0", "stack");//加载栈基地址
				WriteMpisAsm("addi", "$t1", "$t7","-1");//t1=t7-1
				WriteMpisAsm("sll", "$t1", "$t1", "2");//t1=t1<<2,即乘以4
				WriteMpisAsm("add", "$t0", "$t0", "$t1");//得到栈顶那个数的地址

				WriteMpisAsm("lw", "$a0", "($t0)");
				WriteMpisAsm("syscall");
				WriteMpisAsm("addi", "$t7", "$t7", "-1");

				break;
			case 15://输出\n
				sprintf(immediate, "%d", '\n');
				WriteMpisAsm("li", "$v0", "11");
				WriteMpisAsm("addi", "$a0", "$zero", immediate);
				WriteMpisAsm("syscall");

				break;
			case 16://输出?，并从控制台读入一个数字到栈顶
				//			    输出问号
				sprintf(immediate, "%d", '?');
				WriteMpisAsm("li", "$v0", "11");
				WriteMpisAsm("addi", "$a0", "$zero", immediate);
				WriteMpisAsm("syscall");
				//			    输入数字，放入v0
				WriteMpisAsm("li", "$v0", "5");
				WriteMpisAsm("syscall");
				//              放入栈顶
				WriteMpisAsm("la", "$t0", "stack");//加载栈基地址
				WriteMpisAsm("move", "$t1", "$t7");//t1=t7
				WriteMpisAsm("sll", "$t1", "$t1", "2");//t1=t1<<2,即乘以4
				WriteMpisAsm("add", "$t0", "$t0", "$t1");//得到栈top的地址

				WriteMpisAsm("sw", "$v0", "($t0)");//存入
				WriteMpisAsm("addi", "$t7", "$t7", "1");

				break;
			}
			break;
		case lod:   /* 取相对当前过程层差为l，偏移量为A的单元的值到栈顶 */
			baseFlag = true;
			sprintf(immediate, "%d", i.l);
			WriteMpisAsm("li", "$t4", immediate);//t4=l
			WriteMpisAsm("jal", "base");//调用base函数，返回值存入t4

			sprintf(immediate, "%d", i.a);
			WriteMpisAsm("li", "$t1", immediate);//t1=i.a
			WriteMpisAsm("add", "$t1", "$t1", "$t4");//base(i.l, s, b) + i.a

			WriteMpisAsm("la", "$t0", "stack");//加载栈基地址
			WriteMpisAsm("sll", "$t1", "$t1", "2");//t1=t1<<2,即乘以4
			WriteMpisAsm("add", "$t0", "$t0", "$t1");//得到s[base(i.l, s, b) + i.a]地址,在t0
			WriteMpisAsm("lw", "$t2", "($t0)");//t2=s[base(i.l, s, b) + i.a]

			WriteMpisAsm("la", "$t0", "stack");//加载栈基地址
			WriteMpisAsm("move", "$t1", "$t7");//t1=t7
			WriteMpisAsm("sll", "$t1", "$t1", "2");//t1=t1<<2,即乘以4
			WriteMpisAsm("add", "$t0", "$t0", "$t1");//得到s[t]的地址

			WriteMpisAsm("sw", "$t2", "($t0)");//存入s[t]
			WriteMpisAsm("addi", "$t7", "$t7", "1");
			break;
		case sto:   /* 栈顶的值存到相对当前过程层差为l，偏移量为A的单元 */
			baseFlag = true;
			WriteMpisAsm("addi", "$t7", "$t7", "-1");//t--

			WriteMpisAsm("la", "$t0", "stack");//加载栈基地址
			WriteMpisAsm("move", "$t1", "$t7");//t1=t7
			WriteMpisAsm("sll", "$t1", "$t1", "2");//t1=t1<<2,即乘以4
			WriteMpisAsm("add", "$t0", "$t0", "$t1");//得到s[t]的地址
			WriteMpisAsm("lw", "$s0", "($t0)");//s0=s[t]

			sprintf(immediate, "%d", i.l);
			WriteMpisAsm("li", "$t4", immediate);//t4=l
			WriteMpisAsm("jal", "base");//调用base函数，返回值存入t4

			sprintf(immediate, "%d", i.a);
			WriteMpisAsm("li", "$t1", immediate);//t1=i.a
			WriteMpisAsm("add", "$t1", "$t1", "$t4");//base(i.l, s, b) + i.a

			WriteMpisAsm("sll", "$t1", "$t1", "2");//t1=t1<<2,即乘以4
			WriteMpisAsm("la", "$t0", "stack");//加载栈基地址
			WriteMpisAsm("add", "$t0", "$t0", "$t1");//得到s[base(i.l, s, b) + i.a]地址,在t0
			
			WriteMpisAsm("sw", "$s0", "($t0)");//s0存入s[base(i.l, s, b) + i.a]
			break;
		case cal:   /* 调用子过程 */
			baseFlag = true;
			sprintf(immediate, "%d", i.l);
			WriteMpisAsm("li", "$t4", immediate);//t4=l
			WriteMpisAsm("jal", "base");//调用base函数，返回值存入t4,t4=base(i.l, s, b)

			WriteMpisAsm("la", "$t0", "stack");    //取栈顶的地址

			WriteMpisAsm("move", "$t1", "$t7");//t1=t7
			WriteMpisAsm("sll", "$t1", "$t1", "2");//t1=t1<<2,即乘以4
			WriteMpisAsm("add", "$t0", "$t0", "$t1");//得到s[t]地址,在t0

			WriteMpisAsm("sw", "$t4", "($t0)");//s[t] = base(i.l, s, b)
			
			WriteMpisAsm("sw", "$t9", "4($t0)");//s[t + 1] = b
			WriteMpisAsm("sw", "$t8", "8($t0)");//s[t + 2] = p
			WriteMpisAsm("move", "$t9", "$t7");//b = t

			sprintf(immediate, "%d", i.a);
			WriteMpisAsm("li", "$t8", immediate);//p=i.a

			sprintf(immediate, "ins%d", i.a);
			WriteMpisAsm("la", "$s1", immediate);//跳转的地址
			WriteMpisAsm("jalr", "$s4", "$s1");//跳转到i.a，原地址保存在s4

			break;
		case inte:  /* 分配内存 */
			sprintf(immediate, "%d", i.a);
			WriteMpisAsm("addi", "$t7", "$t7", immediate);
			break;
		case jmp:   /* 直接跳转 */
			sprintf(immediate, "ins%d", i.a);
			WriteMpisAsm("j", immediate);

			sprintf(immediate, "%d", i.a);
			WriteMpisAsm("li", "$t8", immediate);//p=i.a
			break;
		case jpc:   /* 条件跳转 */
			sprintf(t1, "%s%d", "nojump", j);
			WriteMpisAsm("addi", "$t7", "$t7", "-1");//t--

			WriteMpisAsm("la", "$t0", "stack");    //取栈顶的地址
			WriteMpisAsm("move", "$t1", "$t7");//t1=t7
			WriteMpisAsm("sll", "$t1", "$t1", "2");//t1=t1<<2,即乘以4
			WriteMpisAsm("add", "$t0", "$t0", "$t1");//得到s[t]的地址
			WriteMpisAsm("lw", "$t1", "($t0)");//t1=s[t]

			WriteMpisAsm("bne", "$t1", "$0", t1);     //s[t] != 0? 不相等则跳转
			sprintf(immediate, "ins%d", i.a);
			WriteMpisAsm("j", immediate);

			sprintf(immediate, "%d", i.a);
			WriteMpisAsm("li", "$t8", immediate);//p=i.a
			Label(t1);

		}
	} 

	
	if(baseFlag)
		base();

	//结束
	Label("mainEnd");
	WriteMpisAsm("li", "$v0", "10");
	WriteMpisAsm("syscall");

	fclose(mips);
	fclose(pcode);
}



#pragma endregion

#pragma region 用数组实现集合的集合运算，n是集合大小


//用数组实现集合的集合运算

int inset(int e, bool* s)
{
	return s[e];
}

//s1并s2
int addset(bool* sr, bool* s1, bool* s2, int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		sr[i] = s1[i] || s2[i];
	}
	return 0;
}
//s1-s2
int subset(bool* sr, bool* s1, bool* s2, int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		sr[i] = s1[i] && (!s2[i]);
	}
	return 0;
}
//s1交s2
int mulset(bool* sr, bool* s1, bool* s2, int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		sr[i] = s1[i] && s2[i];
	}
	return 0;
}
#pragma endregion

#pragma region 出错处理
//出错处理，打印出错位置和错误编码
void error(int n)
{
	//打印的空格
	char space[81];
	memset(space, 32, 81);
	space[cc - 1] = 0; //出错时当前符号已经读完，所以cc-1，0表示'\0'结束

	printf("Error=>%s! %d:%s\n", space, n, err_msg[n]);
	fprintf(fa1, "****%s!%d\n", space, n);

	err++;
}

/*
* 测试当前符号是否合法，短语层恢复思想
*
* 在某一部分（如一条语句，一个表达式）将要结束时时我们希望下一个符号属于某集?
* （该部分的后跟符号），test负责这项检测，并且负责当检测不通过时的补救措施，
* 程序在需要检测时指定当前需要的符号集合和补救用的集合（如之前未完成部分的后跟
* 符号），以及检测不通过时的错误号。
*
* s1:   我们需要的符号
* s2:   如果不是我们需要的，则需要一个补救用的集合
* n:    错误号
*/
int test(bool* s1, bool* s2, int n)
{
	if (!inset(sym, s1))
	{
		error(n);
		/* 当检测不通过时，不停获取符号，直到它属于需要的集合或补救的集合 */
		while ((!inset(sym, s1)) && (!inset(sym, s2)))
		{
			getsymdo;
		}
	}
	return 0;
}

#pragma endregion

#pragma region 符号表管理

/*
* 在符号表中加入一项
*
* k:      名字种类const,var or procedure
* ptx:    名字表尾指针的指针，为了可以改变名字表尾指针的值
* lev:    名字所在的层次,，以后所有的lev都是这样
* pdx:    dx为当前应分配的变量的相对地址，分配后要增加1
为什么变量会使指针++,因为变量需要分配空间
*/
void enter(enum object k, int* ptx, int lev, int* pdx)
{
	(*ptx)++;
	strcpy(table[(*ptx)].name, id); /* 全局变量id中已存有当前名字的名字 */
	table[(*ptx)].kind = k;
	switch (k)
	{
	case constant:  /* 常量名字 */
		if (num > amax)
		{
			error(31);  /* 数越界 */
			num = 0;
		}
		table[(*ptx)].val = num;
		break;
	case variable:  /* 变量名字 */
		table[(*ptx)].level = lev;
		table[(*ptx)].adr = (*pdx);
		(*pdx)++;
		break;
	case procedur:  /*　过程名字　*/
		table[(*ptx)].level = lev;
		break;
	}
}

/*
* 查找名字的位置，从后往前，保证先看此过程局部变量再看其他的
* 找到则返回在名字表中的位置,否则返回0.
*
* idt:    要查找的名字
* tx:     当前名字表尾指针
*/
int position(char* idt, int tx)
{
	int i;
	strcpy(table[0].name, idt);
	i = tx;
	while (strcmp(table[i].name, idt) != 0)
	{
		i--;
	}
	return i;
}

#pragma endregion
