
/* Result Sets Interface */
#ifndef SQL_CRSR
#  define SQL_CRSR
  struct sql_cursor
  {
    unsigned int curocn;
    void *ptr1;
    void *ptr2;
    unsigned int magic;
  };
  typedef struct sql_cursor sql_cursor;
  typedef struct sql_cursor SQL_CURSOR;
#endif /* SQL_CRSR */

/* Thread Safety */
typedef void * sql_context;
typedef void * SQL_CONTEXT;

/* Object support */
struct sqltvn
{
  unsigned char *tvnvsn; 
  unsigned short tvnvsnl; 
  unsigned char *tvnnm;
  unsigned short tvnnml; 
  unsigned char *tvnsnm;
  unsigned short tvnsnml;
};
typedef struct sqltvn sqltvn;

struct sqladts
{
  unsigned int adtvsn; 
  unsigned short adtmode; 
  unsigned short adtnum;  
  sqltvn adttvn[1];       
};
typedef struct sqladts sqladts;

static struct sqladts sqladt = {
  1,1,0,
};

/* Binding to PL/SQL Records */
struct sqltdss
{
  unsigned int tdsvsn; 
  unsigned short tdsnum; 
  unsigned char *tdsval[1]; 
};
typedef struct sqltdss sqltdss;
static struct sqltdss sqltds =
{
  1,
  0,
};

/* File name & Package Name */
struct sqlcxp
{
  unsigned short fillen;
           char  filnam[10];
};
static const struct sqlcxp sqlfpn =
{
    9,
    "main.pcpp"
};


static unsigned int sqlctx = 37084;


static struct sqlexd {
   unsigned long  sqlvsn;
   unsigned int   arrsiz;
   unsigned int   iters;
   unsigned int   offset;
   unsigned short selerr;
   unsigned short sqlety;
   unsigned int   occurs;
      const short *cud;
   unsigned char  *sqlest;
      const char  *stmt;
   sqladts *sqladtp;
   sqltdss *sqltdsp;
   unsigned char  **sqphsv;
   unsigned long  *sqphsl;
            int   *sqphss;
            short **sqpind;
            int   *sqpins;
   unsigned long  *sqparm;
   unsigned long  **sqparc;
   unsigned short  *sqpadto;
   unsigned short  *sqptdso;
   unsigned int   sqlcmax;
   unsigned int   sqlcmin;
   unsigned int   sqlcincr;
   unsigned int   sqlctimeout;
   unsigned int   sqlcnowait;
            int   sqfoff;
   unsigned int   sqcmod;
   unsigned int   sqfmod;
   unsigned char  *sqhstv[4];
   unsigned long  sqhstl[4];
            int   sqhsts[4];
            short *sqindv[4];
            int   sqinds[4];
   unsigned long  sqharm[4];
   unsigned long  *sqharc[4];
   unsigned short  sqadto[4];
   unsigned short  sqtdso[4];
} sqlstm = {12,4};

// Prototypes
extern "C" {
  void sqlcxt (void **, unsigned int *,
               struct sqlexd *, const struct sqlcxp *);
  void sqlcx2t(void **, unsigned int *,
               struct sqlexd *, const struct sqlcxp *);
  void sqlbuft(void **, char *);
  void sqlgs2t(void **, char *);
  void sqlorat(void **, unsigned int *, void *);
}

// Forms Interface
static const int IAPSUCC = 0;
static const int IAPFAIL = 1403;
static const int IAPFTL  = 535;
extern "C" { void sqliem(unsigned char *, signed int *); }

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{12,4274,852,8,0,
5,0,0,0,0,0,27,80,0,0,4,4,0,1,0,1,97,0,0,1,10,0,0,1,10,0,0,1,10,0,0,
36,0,0,2,0,0,30,103,0,0,0,0,0,1,0,
};


#line 1 "main.pcpp"
#include <string>
#include <iostream>

#include "arma.h"
#include "myoracomm.h"
#include "strtools.h"

using namespace std;

int connect_database(char *db_name);
int disconnect_database();


int main(int argc, char** argv) 
{
	
	int ret = -1;
	
	ret = connect_database("");
	if (ret < 0)
	{
		writelogfile(modelname,"[%s,%d]connect db  failed, sqlca.sqlcode=%d\n",__FILE__,__LINE__,ret);
		return ret;
	}
	
	
	
	CARMA armaobj();
	
	cout<<"main test"<<endl;

		
	return 0;
}



/**
 * @brief 连接数据库
 * @param in   
 *        char *db_name 数据库连接串
 * @param out   
 *        无
 * @return
 *        成功返回0，否则返回连接错误码
 * @note
 *        不传参数默认连接ALARM_DB_NAME指定的数据库
 *        传参数连接db_name指定的数据库
 * @remarks
 */
int connect_database(char *db_name)
{
	/* EXEC SQL begin declare section; */ 
#line 53 "main.pcpp"

		char  userid[255];
	/* EXEC SQL end declare section; */ 
#line 55 "main.pcpp"

	
	memset(userid, '\0', sizeof(userid));
	
	if (strcmp(db_name,"") == 0)
	{
		char *pdb_name = NULL;
		
		pdb_name = getenv("ALARM_DB_NAME");

		if (pdb_name != NULL)
		{
			strcpy(userid, pdb_name);
		}
		else
		{
			strcpy(userid, db_name);
		}
		
	}
	else
	{
		strcpy(userid, db_name);
	}

	/* EXEC SQL CONNECT :userid; */ 
#line 80 "main.pcpp"

{
#line 80 "main.pcpp"
 struct sqlexd sqlstm;
#line 80 "main.pcpp"
 sqlstm.sqlvsn = 12;
#line 80 "main.pcpp"
 sqlstm.arrsiz = 4;
#line 80 "main.pcpp"
 sqlstm.sqladtp = &sqladt;
#line 80 "main.pcpp"
 sqlstm.sqltdsp = &sqltds;
#line 80 "main.pcpp"
 sqlstm.iters = (unsigned int  )10;
#line 80 "main.pcpp"
 sqlstm.offset = (unsigned int  )5;
#line 80 "main.pcpp"
 sqlstm.cud = sqlcud0;
#line 80 "main.pcpp"
 sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 80 "main.pcpp"
 sqlstm.sqlety = (unsigned short)4352;
#line 80 "main.pcpp"
 sqlstm.occurs = (unsigned int  )0;
#line 80 "main.pcpp"
 sqlstm.sqhstv[0] = (unsigned char  *)userid;
#line 80 "main.pcpp"
 sqlstm.sqhstl[0] = (unsigned long )255;
#line 80 "main.pcpp"
 sqlstm.sqhsts[0] = (         int  )255;
#line 80 "main.pcpp"
 sqlstm.sqindv[0] = (         short *)0;
#line 80 "main.pcpp"
 sqlstm.sqinds[0] = (         int  )0;
#line 80 "main.pcpp"
 sqlstm.sqharm[0] = (unsigned long )0;
#line 80 "main.pcpp"
 sqlstm.sqadto[0] = (unsigned short )0;
#line 80 "main.pcpp"
 sqlstm.sqtdso[0] = (unsigned short )0;
#line 80 "main.pcpp"
 sqlstm.sqphsv = sqlstm.sqhstv;
#line 80 "main.pcpp"
 sqlstm.sqphsl = sqlstm.sqhstl;
#line 80 "main.pcpp"
 sqlstm.sqphss = sqlstm.sqhsts;
#line 80 "main.pcpp"
 sqlstm.sqpind = sqlstm.sqindv;
#line 80 "main.pcpp"
 sqlstm.sqpins = sqlstm.sqinds;
#line 80 "main.pcpp"
 sqlstm.sqparm = sqlstm.sqharm;
#line 80 "main.pcpp"
 sqlstm.sqparc = sqlstm.sqharc;
#line 80 "main.pcpp"
 sqlstm.sqpadto = sqlstm.sqadto;
#line 80 "main.pcpp"
 sqlstm.sqptdso = sqlstm.sqtdso;
#line 80 "main.pcpp"
 sqlstm.sqlcmax = (unsigned int )100;
#line 80 "main.pcpp"
 sqlstm.sqlcmin = (unsigned int )2;
#line 80 "main.pcpp"
 sqlstm.sqlcincr = (unsigned int )1;
#line 80 "main.pcpp"
 sqlstm.sqlctimeout = (unsigned int )0;
#line 80 "main.pcpp"
 sqlstm.sqlcnowait = (unsigned int )0;
#line 80 "main.pcpp"
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 80 "main.pcpp"
}

#line 80 "main.pcpp"

    if (sqlca.sqlcode != 0)
    {
		return sqlca.sqlcode;
    }
    
	return 0;
}

/**
 * @brief 断开数据库数据库
 * @param in 
 *        无
 * @param out   
 *        无
 * @return
 *        成功返回0
 * @note
 *           
 * @remarks
 */
int disconnect_database()
{
	/* EXEC SQL COMMIT WORK RELEASE; */ 
#line 103 "main.pcpp"

{
#line 103 "main.pcpp"
 struct sqlexd sqlstm;
#line 103 "main.pcpp"
 sqlstm.sqlvsn = 12;
#line 103 "main.pcpp"
 sqlstm.arrsiz = 4;
#line 103 "main.pcpp"
 sqlstm.sqladtp = &sqladt;
#line 103 "main.pcpp"
 sqlstm.sqltdsp = &sqltds;
#line 103 "main.pcpp"
 sqlstm.iters = (unsigned int  )1;
#line 103 "main.pcpp"
 sqlstm.offset = (unsigned int  )36;
#line 103 "main.pcpp"
 sqlstm.cud = sqlcud0;
#line 103 "main.pcpp"
 sqlstm.sqlest = (unsigned char  *)&sqlca;
#line 103 "main.pcpp"
 sqlstm.sqlety = (unsigned short)4352;
#line 103 "main.pcpp"
 sqlstm.occurs = (unsigned int  )0;
#line 103 "main.pcpp"
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
#line 103 "main.pcpp"
}

#line 103 "main.pcpp"

	return 0;
}


