/* Modified for Rising Sun: explicit storage for the original string prefix.
 * Descriptive annotation names; see docs/Lua.md. */
/*
** $Id: lundump.c,v 1.33 2000/10/31 16:57:23 lhf Exp $
** load bytecodes from files
** See Copyright Notice in lua.h
*/

#include <stdio.h>
#include <string.h>

#include "lfunc.h"
#include "lmem.h"
#include "lopcodes.h"
#include "lstring.h"
#include "lundump.h"

/* Target storage annotations; names describe original string literals. */
static const char end_of_file_error[] __attribute__((section(".rodata.prefix"), aligned(4))) = "unexpected end of file in `%.99s'";
static const char allocation_label[] __attribute__((section(".rodata.prefix"), aligned(4))) = "lua";
static const char bad_code_error[] __attribute__((section(".rodata.prefix"), aligned(4))) = "bad code in `%.99s'";
static const char chunk_signature[] __attribute__((section(".rodata.prefix"), aligned(4))) = "Lua";
static const char bad_signature_error[] __attribute__((section(".rodata.prefix"), aligned(4))) = "bad signature in `%.99s'";
static const char vm_mismatch_error[] __attribute__((section(".rodata.prefix"), aligned(4))) = "virtual machine mismatch in `%.99s':\n  %.20s is %d but read %d";
static const char new_version_error[] __attribute__((section(".rodata.prefix"), aligned(4))) = "`%.99s' too new:\n  read version %d.%d; expected at most %d.%d";
static const char old_version_error[] __attribute__((section(".rodata.prefix"), aligned(4))) = "`%.99s' too old:\n  read version %d.%d; expected at least %d.%d";
static const char int_size_label[] __attribute__((section(".rodata.prefix"), aligned(4))) = "sizeof(int)";
static const char size_t_size_label[] __attribute__((section(".rodata.prefix"), aligned(4))) = "sizeof(size_t)";
static const char instruction_size_label[] __attribute__((section(".rodata.prefix"), aligned(4))) = "sizeof(Instruction)";
static const char instruction_bits_label[] __attribute__((section(".rodata.prefix"), aligned(4))) = "SIZE_INSTRUCTION";
static const char opcode_bits_label[] __attribute__((section(".rodata.prefix"), aligned(4))) = "SIZE_OP";
static const char b_bits_label[] __attribute__((section(".rodata.prefix"), aligned(4))) = "SIZE_B";
static const char number_size_label[] __attribute__((section(".rodata.prefix"), aligned(4))) = "sizeof(Number)";
static const char number_format_error[] __attribute__((section(".rodata.prefix"), aligned(4))) = "unknown number format in `%.99s':\n  read %.16g; expected %.16g";

#undef luaM_malloc
#define luaM_malloc(L, t) DWI_alloc(allocation_label, (t), 1024)

#define	LoadByte		ezgetc

static const char* ZNAME (ZIO* Z)
{
 const char* s=zname(Z);
 return (*s=='@') ? s+1 : s;
}

static void unexpectedEOZ (lua_State* L, ZIO* Z)
{
 luaO_verror(L,end_of_file_error,ZNAME(Z));
}

static int ezgetc (lua_State* L, ZIO* Z)
{
 int c=zgetc(Z);
 if (c==EOZ) unexpectedEOZ(L,Z);
 return c;
}

static void ezread (lua_State* L, ZIO* Z, void* b, int n)
{
 int r=zread(Z,b,n);
 if (r!=0) unexpectedEOZ(L,Z);
}

static void LoadBlock (lua_State* L, void* b, size_t size, ZIO* Z, int swap)
{
 if (swap)
 {
  char *p=(char *) b+size-1;
  int n=size;
  while (n--) *p--=(char)ezgetc(L,Z);
 }
 else
  ezread(L,Z,b,size);
}

static void LoadVector (lua_State* L, void* b, int m, size_t size, ZIO* Z, int swap)
{
 if (swap)
 {
  char *q=(char *) b;
  while (m--)
  {
   char *p=q+size-1;
   int n=size;
   while (n--) *p--=(char)ezgetc(L,Z);
   q+=size;
  }
 }
 else
  ezread(L,Z,b,m*size);
}

static int LoadInt (lua_State* L, ZIO* Z, int swap)
{
 int x;
 LoadBlock(L,&x,sizeof(x),Z,swap);
 return x;
}

static size_t LoadSize (lua_State* L, ZIO* Z, int swap)
{
 size_t x;
 LoadBlock(L,&x,sizeof(x),Z,swap);
 return x;
}

static Number LoadNumber (lua_State* L, ZIO* Z, int swap)
{
 Number x;
 LoadBlock(L,&x,sizeof(x),Z,swap);
 return x;
}

static TString* LoadString (lua_State* L, ZIO* Z, int swap)
{
 size_t size=LoadSize(L,Z,swap);
 if (size==0)
  return NULL;
 else
 {
  char* s=luaO_openspace(L,size);
  LoadBlock(L,s,size,Z,0);
  return luaS_newlstr(L,s,size-1);	/* remove trailing '\0' */
 }
}

static void LoadCode (lua_State* L, Proto* tf, ZIO* Z, int swap)
{
 int size=LoadInt(L,Z,swap);
 tf->code=luaM_newvector(L,size,Instruction);
 LoadVector(L,tf->code,size,sizeof(*tf->code),Z,swap);
 if (tf->code[size-1]!=OP_END) luaO_verror(L,bad_code_error,ZNAME(Z));
 luaF_protook(L,tf,size);
}

static void LoadLocals (lua_State* L, Proto* tf, ZIO* Z, int swap)
{
 int i,n;
 tf->nlocvars=n=LoadInt(L,Z,swap);
 tf->locvars=luaM_newvector(L,n,LocVar);
 for (i=0; i<n; i++)
 {
  tf->locvars[i].varname=LoadString(L,Z,swap);
  tf->locvars[i].startpc=LoadInt(L,Z,swap);
  tf->locvars[i].endpc=LoadInt(L,Z,swap);
 }
}

static void LoadLines (lua_State* L, Proto* tf, ZIO* Z, int swap)
{
 int n;
 tf->nlineinfo=n=LoadInt(L,Z,swap);
 tf->lineinfo=luaM_newvector(L,n,int);
 LoadVector(L,tf->lineinfo,n,sizeof(*tf->lineinfo),Z,swap);
}

static Proto* LoadFunction (lua_State* L, ZIO* Z, int swap);

static void LoadConstants (lua_State* L, Proto* tf, ZIO* Z, int swap)
{
 int i,n;
 tf->nkstr=n=LoadInt(L,Z,swap);
 tf->kstr=luaM_newvector(L,n,TString*);
 for (i=0; i<n; i++)
  tf->kstr[i]=LoadString(L,Z,swap);
 tf->nknum=n=LoadInt(L,Z,swap);
 tf->knum=luaM_newvector(L,n,Number);
 LoadVector(L,tf->knum,n,sizeof(*tf->knum),Z,swap);
 tf->nkproto=n=LoadInt(L,Z,swap);
 tf->kproto=luaM_newvector(L,n,Proto*);
 for (i=0; i<n; i++)
  tf->kproto[i]=LoadFunction(L,Z,swap);
}

static Proto* LoadFunction (lua_State* L, ZIO* Z, int swap)
{
 Proto* tf=luaF_newproto(L);
 tf->source=LoadString(L,Z,swap);
 tf->lineDefined=LoadInt(L,Z,swap);
 tf->numparams=LoadInt(L,Z,swap);
 tf->is_vararg=LoadByte(L,Z);
 tf->maxstacksize=LoadInt(L,Z,swap);
 LoadLocals(L,tf,Z,swap);
 LoadLines(L,tf,Z,swap);
 LoadConstants(L,tf,Z,swap);
 LoadCode(L,tf,Z,swap);
 return tf;
}

static void LoadSignature (lua_State* L, ZIO* Z)
{
 const char* s=chunk_signature;
 while (*s!=0 && ezgetc(L,Z)==*s)
  ++s;
 if (*s!=0) luaO_verror(L,bad_signature_error,ZNAME(Z));
}

static void TestSize (lua_State* L, int s, const char* what, ZIO* Z)
{
 int r=ezgetc(L,Z);
 if (r!=s)
  luaO_verror(L,vm_mismatch_error,ZNAME(Z),what,s,r);
}

#define TESTSIZE(s)	TestSize(L,s,#s,Z)
#define V(v)	v/16,v%16

static int LoadHeader (lua_State* L, ZIO* Z)
{
 int version,swap;
 Number f=0,tf=TEST_NUMBER;
 LoadSignature(L,Z);
 version=ezgetc(L,Z);
 if (version>VERSION)
  luaO_verror(L,new_version_error,
	ZNAME(Z),V(version),V(VERSION));
 if (version<VERSION0)			/* check last major change */
  luaO_verror(L,old_version_error,
	ZNAME(Z),V(version),V(VERSION));
 swap=(luaU_endianess()!=ezgetc(L,Z));	/* need to swap bytes? */
 TestSize(L,sizeof(int),int_size_label,Z);
 TestSize(L,sizeof(size_t),size_t_size_label,Z);
 TestSize(L,sizeof(Instruction),instruction_size_label,Z);
 TestSize(L,SIZE_INSTRUCTION,instruction_bits_label,Z);
 TestSize(L,SIZE_OP,opcode_bits_label,Z);
 TestSize(L,SIZE_B,b_bits_label,Z);
 TestSize(L,sizeof(Number),number_size_label,Z);
 f=LoadNumber(L,Z,swap);
 if ((long)f!=(long)tf)		/* disregard errors in last bit of fraction */
  luaO_verror(L,number_format_error, ZNAME(Z),f,tf);
 return swap;
}

static Proto* LoadChunk (lua_State* L, ZIO* Z)
{
 return LoadFunction(L,Z,LoadHeader(L,Z));
}

/*
** load one chunk from a file or buffer
** return main if ok and NULL at EOF
*/
Proto* luaU_undump (lua_State* L, ZIO* Z)
{
 Proto* tf=NULL;
 int c=zgetc(Z);
 if (c==ID_CHUNK)
  tf=LoadChunk(L,Z);
 c=zgetc(Z);
 if (c!=EOZ)
  luaO_verror(L,"`%.99s' apparently contains more than one chunk",ZNAME(Z));
 return tf;
}

/*
** find byte order
*/
int luaU_endianess (void)
{
 int x=1;
 return *(char*)&x;
}
