
#include "semant.h"
void         transDec (S_table venv, S_table tenv, A_dec d){
	switch (d->kind){
		case A_varDec:{
			//struct {S_symbol var; S_symbol typ; A_exp init; bool escape;} var;
			if (strcmp(S_name(d->u.var.typ),"")!=0){
				//var x := exp
				struct expty exp_ty = transExp (venv, tenv, d->u.var.init);
				Ty_ty pre_type =  S_look(tenv,d->u.var.typ);
				if (exp_ty.ty->kind==pre_type->kind){
					S_enter(venv,d->u.var.var,E_varEntry(exp_ty.ty));
				}else{
					EM_error(d->pos, "type not the same!");
				}
			}else{
				//var x : type-id := exp
				struct expty exp_ty = transExp (venv, tenv, d->u.var.init);
				S_enter(venv,d->u.var.var,E_varEntry(exp_ty.ty));
			}
			break;
		}
		case A_typeDec:{
			//A_nametyList type;
			A_nametyList list;
			for (list=d->u.type;list;list=list->tail){
				S_enter(tenv,list->head->name,list->head->ty);

			}
        	break;
		}
		case A_functionDec:{
			//A_fundecList function;
			A_fundec f = d->u.function->head;
			Ty_ty resultTy = S_look(tenv,f->result);
			Ty_tyList formalTys = makeFormalTyList(tenv,f->params);
			S_enter(venv,f->name,E_FunEntry(formalTys,resultTy));
			S_beginScope(venv);
			{A_fieldList l; Ty_tyList t:
				for(l=f->params,t=formalTys;l;l=l->tail,t=t->tail){
					S_enter(venv,l->head->name,E_VarEntry(t->head));
				}

			}
			//????
			transExp(venv,tenv,);
			S_endScope(venv);
			break;
		}
	}

}
struct expty transExp (S_table venv, S_table tenv, A_exp a){
	switch(a->kind){
		case A_opExp:{
			A_oper oper = a->u.op.oper;
			struct expty left = transExp(venv,tenv,a->u.op.left);
			struct expty right = transExp(venv,tenv,a->u.op.right);
			if (oper==A_plusOp){
				if (left.ty->kind!=Ty_int){
					EM_error(a->u.op.left->pos, "Integer required");
				}
				if (right.ty->kind!=Ty_int){
					EM_error(a->u.op.right->pos, "Integer required");
				}
				return expTy(NULL, Ty_Int());
			}          

	     
		}
		case A_letExp:{
			//struct {A_decList decs; A_exp body;} let;
			struct expty exp;
			A_decList d;
			S_beginScope(venv);
			S_beginScope(tenv);
			for (d = a->u.let.decs; d; d=d->tail){
				transDec(venv,tenv,d->head); //expand the scope
			}
			exp = transExp(venv,tenv,a->u.let.body);
			S_endScope(tenv);
			S_endScope(venv);
			return exp;

		}
		case A_varExp:{

		}
		case A_nilExp:{

		}
		case A_intExp:{

		}

		case A_stringExp:{

		}
		case A_callExp:{

		}
		case A_recordExp:{
			
		}case A_seqExp:{

		}
		case A_assignExp:{

		}
		case A_ifExp:{
			
		}case A_whileExp:{

		}
		case A_forExp:{

		}
		case A_breakExp:{
			
		}
		case A_arrayExp:{
			
		}
	}
	assert(0);
}
struct expty transVar (S_table venv, S_table tenv, A_var v){
	/*struct A_var_
       {enum {A_simpleVar, A_fieldVar, A_subscriptVar} kind;
        A_pos pos;
		union {S_symbol simple;
	       		struct {A_var var; S_symbol sym;} field;
	       		struct {A_var var; A_exp exp;} subscript;
	    } u;
      };*/
	switch(v->kind){
		case A_SimpleVar:{
			// id
			//A_SimpleVar(A_pos pos, S_symbol sym);
			E_enventry x = S_look(venv,v->u.simple);
			if (x && x->kind==E_varEntry)
				return expTy(NULL, actual_ty(x->u.var.ty));
			else{
				EM_error(v->pos,"undefined variable %s", S_name(v->u.simple));
				return expTy(NULL,Ty_Int());
			}
		}
		case A_fieldVar:{
			// lvalue.id
			struct expty exp_ty = transVar(venv,tenv,v->u.field.var);
			if (exp_ty.ty->kind!=Ty_record){
				EM_error(v->u.field.var->pos, "Record required");
				return expTy(NULL, Ty_Int());
			}
			E_enventry x = S_look(venv,v->u.field.sym);// how to enter the record's scope?
			if (x && x->kind==E_varEntry)
				return expTy(NULL, actual_ty(x->u.var.ty));
			else{
				EM_error(v->pos,"undefined variable %s", S_name(v->u.field.sym));
				return expTy(NULL,Ty_Int());
			}
		}
		case A_subscriptVar:{
			// lvalue[exp]
			struct expty exp_ty = transVar(venv,tenv,v->u.subscript.var);
			if (exp_ty.ty->kind!=Ty_array){
				EM_error(v->u.subscript.var->pos, "Subscript required");
				return expTy(NULL, Ty_Int());
			}
			struct expty exp_ty = transExp(venv,tenv,v->u.subscript.exp);// how to enter the record's scope?
			if (exp_ty.ty!=Ty_Int){
				EM_error(v->u.op.left->pos, "Integer required");
				return expTy(NULL, Ty_Int());
			}else{
				//?????
			}
		}
	}
}

