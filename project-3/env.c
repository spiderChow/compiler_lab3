#include "env.h"

E_enventry E_FunEntry(Ty_tyList formals, Ty_ty result)
{E_enventry entry = checked_malloc(sizeof(*entry));
 entry->kind=E_funEntry;
 entry->u.fun.formals=formals;
 entry->u.fun.result=result;
 return entry;
}

E_enventry E_VarEntry(Ty_ty ty)
{E_enventry entry = checked_malloc(sizeof(*entry));
 entry->kind=E_varEntry;
 entry->u.var.ty=ty;
 return entry;

}

S_table E_base_tenv(void){
	S_table t = S_empty();
	S_enter(t, S_Symbol("int"), Ty_Int());
	S_enter(t, S_Symbol("sting"), Ty_String());
	return t;
}

S_table E_base_venv(void){
	S_table t = S_empty();
	S_enter(t,S_Symbol("print"),E_funEntry(Ty_TyList(Ty_String(),NULL),Ty_void()));
	S_enter(t,S_Symbol("flush"),E_funEntry(Ty_TyList(Ty_Void(),NULL),Ty_void()));
	S_enter(t,S_Symbol("getchar"),E_funEntry(Ty_TyList(Ty_Void(),NULL),Ty_String()));
	S_enter(t,S_Symbol("ord"),E_funEntry(Ty_TyList(Ty_String(),NULL),Ty_Int()));
	S_enter(t,S_Symbol("chr"),E_funEntry(Ty_TyList(Ty_Int(),NULL),Ty_String()));
	S_enter(t,S_Symbol("size"),E_funEntry(Ty_TyList(Ty_String(),NULL),Ty_Int()));
	Ty_tyList int_last = Ty_TyList(Ty_Int(),NULL);
	Ty_tyList int_int_last = Ty_TyList(Ty_Int(),int_last);
	S_enter(t,S_Symbol("substring"),E_funEntry(Ty_TyList(Ty_String(),int_int_last),Ty_String()));
	Ty_tyList string_last = Ty_TyList(Ty_String(),NULL); 
	S_enter(t,S_Symbol("concat"),E_funEntry(Ty_TyList(Ty_String(),string_last),Ty_String()));
	S_enter(t,S_Symbol("not"),E_funEntry(Ty_TyList(Ty_Int(),NULL),Ty_Int()));
	S_enter(t,S_Symbol("exit"),E_funEntry(Ty_TyList(Ty_Int(),NULL),Ty_void()));
	return t;
}

