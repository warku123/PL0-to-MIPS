#pragma once

#include <stdio.h>

#define norw 13     /* 保留字个数 */
#define txmax 100   /* 名字表容量 */
#define nmax 14     /* 数字的最大位数 */
#define al 10       /* 标识符的最大长度 */
#define amax 2047   /* 常量最大值 */
#define levmax 3    /* 最大允许过程嵌套声明层数 [0,  levmax]*/
#define cxmax 500   /* 最多的虚拟机代码数 */
#define stacksize 500    /* 解释执行时使用的栈 */

//由16个运算符及界符，13个保留字还有数字、标识符构成
enum symbol {
    nul, ident, number,
    becomes, plus, minus, times, slash,
    eql, neq, lss, leq, gtr, geq,
    lparen, rparen, comma, semicolon, period,
    beginsym, endsym, ifsym, thensym, whilesym,
    dosym, varsym, constsym, oddsym, procsym, callsym,
    readsym, writesym,
};
#define symnum 32	/* 符号数 */

/* 虚拟机代码 */
enum fct {
    lit, opr, lod,
    sto, cal, inte,
    jmp, jpc,
};
#define fctnum 8	/* 虚拟机代码数 */

/* 虚拟机代码结构 */
struct instruction
{
    enum fct f; /* 虚拟机代码指令 */
    int l;      /* 引用层与声明层的层次差 */
    int a;      /* 根据f的不同而不同 */
};

FILE* fas;  /* 输出名字表 */
FILE* fa;   /* 输出虚拟机代码 */
FILE* fa1;  /* 输出源文件及其各行对应的首地址 */
FILE* fa2;  /* 输出结果 */

FILE* mips; //输出mips汇编代码

bool listswitch;    /* 显示虚拟机代码与否 */
bool tableswitch;   /* 显示名字表与否 */
char ch;            //用于词法分析器，存放最近一次从文件中读出的字符,getch使用
enum symbol sym;    // 词法分析器输出结果之用，存放最近一次识别出来的 符号token 的类型
char id[al + 1];    // 词法分析器输出结果之用，当前ident, 多出的一个字节用于存放0
int num;            // 词法分析器输出结果之用，当前number
int cc, ll;          // getch使用的计数器，cc表示当前字符(ch)的位置，即行缓冲区的列指针,ll为行缓冲区长度
int cx;             // 虚拟机代码指针, 取值范围[0, cxmax-1],代码生成模块总在 cx 所指位置生成新的代码
char line[81];      //行缓冲区，用于从文件读出一行，供词法分析获取单词时之用
char a[al + 1];       /* 词法分析器中用于临时存放正在分析的词, 多出的一个字节用于存放0 */
struct instruction code[cxmax]; /* 存放编译得到的类 PCODE虚拟机代码的数组 */
char word[norw][al];        /* 保留字 */
enum symbol wsym[norw];     /* 保留字对应的符号值 */
enum symbol ssym[256];      /* 单字符的符号值 */
char mnemonic[fctnum][5];   //类 PCODE 指令助记符表
bool declbegsys[symnum];    /* 表示声明开始的符号集合 */
bool statbegsys[symnum];    /* 表示语句开始的符号集合 */
bool facbegsys[symnum];     /* 表示因子开始的符号集合 */

/* 符号表中的类型 */
enum object {
    constant,
    variable,
    procedur,
    array       //lkz增加部分
};
/* 符号表结构 */
struct tablestruct
{
    char name[al];      /* 名字 */
    enum object kind;   /* 类型：const, var, array or procedure */
    int val;            /* 数值，仅const使用 */
    int level;          /* 如果是变量名或过程名,存放层差、偏移地址和大小*/
    int adr;            /* 地址，仅const不使用 */
    int size;           /* 需要分配的数据区空间, 仅procedure使用 */
};

struct tablestruct table[txmax]; /* 符号表 */

FILE* fin;
FILE* fout;
char fname[al];
int err; //错误计数器

#pragma region 就是函数调用与错误处理放在一起


/* 当函数中会发生fatal error时，返回-1告知调用它的函数，最终退出程序 */
#define getsymdo                      if(-1 == getsym()) return -1
#define getchdo                       if(-1 == getch()) return -1
#define testdo(a, b, c)               if(-1 == test(a, b, c)) return -1
#define gendo(a, b, c)                if(-1 == gen(a, b, c)) return -1
#define expressiondo(a, b, c)         if(-1 == expression(a, b, c)) return -1
#define factordo(a, b, c)             if(-1 == factor(a, b, c)) return -1
#define termdo(a, b, c)               if(-1 == term(a, b, c)) return -1
#define conditiondo(a, b, c)          if(-1 == condition(a, b, c)) return -1
#define statementdo(a, b, c)          if(-1 == statement(a, b, c)) return -1
#define constdeclarationdo(a, b, c)   if(-1 == constdeclaration(a, b, c)) return -1
#define vardeclarationdo(a, b, c)     if(-1 == vardeclaration(a, b, c)) return -1
#pragma endregion
#pragma region 各函数
void error(int n);
int getsym();
int getch();
void init();
int gen(enum fct x, int y, int z);
int test(bool* s1, bool* s2, int n);
int inset(int e, bool* s);
int addset(bool* sr, bool* s1, bool* s2, int n);
int subset(bool* sr, bool* s1, bool* s2, int n);
int mulset(bool* sr, bool* s1, bool* s2, int n);
int block(int lev, int tx, bool* fsys);
void interpret();
int factor(bool* fsys, int* ptx, int lev);
int term(bool* fsys, int* ptx, int lev);
int condition(bool* fsys, int* ptx, int lev);
int expression(bool* fsys, int* ptx, int lev);
int statement(bool* fsys, int* ptx, int lev);
void listcode(int cx0);
int vardeclaration(int* ptx, int lev, int* pdx);
int constdeclaration(int* ptx, int lev, int* pdx);
int position(char* idt, int tx);
void enter(enum object k, int* ptx, int lev, int* pdx);
int base(int l, int* s, int b);
#pragma endregion


//错误信息数组
const char* err_msg[] = {
    "",//0
    "'='写成了':='",//1
    "'='后面要跟一个数字",//2
    "标识符后面要跟一个‘=’",//3
    "在const，var，procedure后面要有一个标识符",//4
    "缺少','或者';'",//5
    "过程名错误！",//6
    "需要声明",//7
    "声明后边是一个不正确的符号",//8
    "少了'.'，程序无法正常结束",//9
    "少了';'",//10
    "发现未声明的标识符！",//11
    "非法赋值",//12
    "少了':='",//13
    "call之后缺少标识符！",//14
    "call之后标识符不是过程！",//15
    "少了then",//16
    "缺少';'或者end",//17
    "少了do",//18
    "符号错误",//19
    "条件语句中未发现操作符（“#，>”等）",//20
    "不能把过程的标识符放在表达式里！",//21
    "",//22
    "符号后面不能跟着<因子>",//23
    "符号不能作为<表达式>的开始！",//24
    "",//25
    "",//26
    "",//27
    "",//28
    "",//29
    "数字过大！",//30
    "常量超过可定义的最大值！",//31
    "超过允许的最大嵌套层数，层数太多啦！",//32
    "格式错误，应是右括号')'",//33
    "格式错误，应是左括号'('",//34
    "read里不是标识符ID,或该标识符未声明",//35
};

//用于生成汇编的临时变量
char immediate[20];
