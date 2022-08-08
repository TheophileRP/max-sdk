// see https://github.com/Cycling74/max-sdk/blob/main/source/basics/simplemax/simplemax.c
// Jeremy Bernstein

#include "ext.h"         // standard Max include, always required
#include "ext_obex.h"    // required for new style Max object
#include "ext_dictobj.h"
#include "../include/testInclude.h"

////////////////////////// object struct
typedef struct _Tsend {
	t_object ob;    // the object itself (must be first)
    t_symbol *name;
    t_atom *input_message;
} t_Tsend;

///////////////////////// function prototypes
//// standard set
void* Tsend_new(t_symbol* s, long argc, t_atom* argv);
void  Tsend_free(t_Tsend* x);
void  Tsend_assist(t_Tsend* x, void* b, long m, long a, char* s);
void  Tsend_anything(t_Tsend *x, t_symbol *s, long argc, t_atom *argv);
void  Tsend_dictionary_funall(t_dictionary_entry *entry, void *arg);
void Tsend_bang(t_Tsend *x);
void Tsend_int(t_Tsend *x, t_symbol *s, long n);
void Tsend_float(t_Tsend *x, t_symbol *s, float f);
void Tsend_sym(t_Tsend *x, t_symbol *s, t_symbol *m);

//////////////////////// global class pointer variable
void* Tsend_class;
static t_dictionary *global_receive_dictionary = NULL;
t_symbol *ps_anything;

void ext_main(void* r) {
	t_class* c;
    

	c = class_new("Tsend", (method)Tsend_new, (method)Tsend_free, (long)sizeof(t_Tsend), 0L /* leave NULL!! */, A_GIMME, 0);

	/* you CAN'T call this from the patcher */
	class_addmethod(c, (method)Tsend_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)Tsend_bang, "bang", 0);
    class_addmethod(c, (method)Tsend_int, "int", A_LONG, 0);
    class_addmethod(c, (method)Tsend_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)Tsend_sym, "sym", A_SYM, 0);
    class_addmethod(c, (method)Tsend_anything, "anything", A_GIMME, 0);

	class_register(CLASS_BOX, c); /* CLASS_NOBOX */
	Tsend_class = c;
    
    t_symbol *ps_receive_dict = gensym("receive_dictionary");
    global_receive_dictionary = dictobj_findregistered_retain(ps_receive_dict);
    if(global_receive_dictionary == NULL){
        global_receive_dictionary = dictionary_new();
        dictobj_register(global_receive_dictionary, &ps_receive_dict);
    }
    
    ps_anything = gensym("anything");

    post("Tsend : %s %s", __TIME__, __DATE__);
}

void Tsend_assist(t_Tsend* x, void* b, long m, long a, char* s) {
	if (m == ASSIST_INLET) {    // inlet
		sprintf(s, "I am inlet %ld", a);
	}
	else {    // outlet
		sprintf(s, "I am outlet %ld", a);
	}
}

void Tsend_bang(t_Tsend *x){
    
}

void Tsend_int(t_Tsend *x, t_symbol *s, long n){
    atom_setlong(x->input_message, n);
    dictionary_funall(global_receive_dictionary, (method)Tsend_dictionary_funall, x);
}

void Tsend_float(t_Tsend *x, t_symbol *s, float f){
    atom_setfloat(x->input_message, f);
    dictionary_funall(global_receive_dictionary, (method)Tsend_dictionary_funall, x);
}

void Tsend_sym(t_Tsend *x, t_symbol *s, t_symbol *m){
    atom_setsym(x->input_message, m);
    dictionary_funall(global_receive_dictionary, (method)Tsend_dictionary_funall, x);
}

void  Tsend_anything(t_Tsend *x, t_symbol *s, long argc, t_atom *argv){
    x->input_message = argv;
    post("anything !");
    dictionary_funall(global_receive_dictionary, (method)Tsend_dictionary_funall, x);
}

void  Tsend_dictionary_funall(t_dictionary_entry *entry, void *arg){
    t_atom value;
    dictionary_entry_getvalue(entry, &value);
    t_Tsend *x = (t_Tsend *)arg;
    
    if(x->name == object_method(atom_getobj(&value), gensym("getname"))){
        post("%d", atom_gettype(x->input_message));
        object_method_direct(void, (t_object *, t_symbol *, long, t_atom *), atom_getobj(&value), ps_anything, ps_anything, 1, x->input_message);
    }
}

void Tsend_free(t_Tsend* x) {
	;
}

void* Tsend_new(t_symbol* s, long argc, t_atom* argv) {
    t_Tsend* x = NULL;
    
    x = (t_Tsend*)object_alloc(Tsend_class);
    
    if(x){
        if(argc && argv){
            x->name = atom_getsym(argv);
        }else{
            x->name = gensym("");
        }
        
        x->input_message = NULL;
    }
    return (x);
}
